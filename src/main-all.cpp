#include "../include/dijkstra.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <chrono>
#include <cmath>
#include <filesystem>
#define NUM_PAIRS 30 // Número de pares origem-destino a serem testados

using namespace std;
using namespace chrono;

int Dijkstra::insert_operations = 0;
int Dijkstra::delete_min_operations = 0;
int Dijkstra::update_operations = 0;
int MinHeap::sift_up_operations = 0;
int MinHeap::sift_down_operations = 0;
int MinHeap::insert_sifts = 0;
int MinHeap::extract_sifts = 0;
int MinHeap::update_sifts = 0;

void runFullTests(const string filename, int k, int count) {
    ifstream file(filename);

    if (!file) {
        cerr << "Error opening file\n";
        throw;
    }

    Graph g(file);
    file.close();

    cout << "Grafo criado a partir de " << filename << endl;

    ofstream out_results("../output/all/random/resultados-" + std::to_string(count) + ".csv");

    // Cabeçalhos dos arquivos CSV
    out_results << "k,vertices,edges,source,target,dist,insert,delMin,update,sift_up,sift_down,i,d,u,avgR_insert,avgR_deleteMin,avgR_update,time_ms\n";

    int n = g.getNumVertices();
    int m = g.getNumEdges();

    long long total_sift_up = 0;
    long long total_sift_down = 0;
    long long total_sifts = 0;

    long long total_insert = 0;
    long long total_delete = 0;
    long long total_update = 0;

    long long total_time = 0;

    //cout << "Iniciando testes para k=" << k << "..." << endl;

    for (int j = 0; j < NUM_PAIRS; ++j) {
        int source = rand() % n;
        int target = rand() % n;
        int dist;
        while (target == source) target = rand() % n;

        // Resetando contadores de operações
        MinHeap::sift_up_operations = 0;  MinHeap::sift_down_operations = 0;
        MinHeap::insert_sifts = 0; MinHeap::extract_sifts = 0;   MinHeap::update_sifts = 0;
        Dijkstra::insert_operations = 0;  Dijkstra::delete_min_operations = 0; Dijkstra::update_operations = 0;

        auto start = high_resolution_clock::now();
        dist = Dijkstra::shortestPath(g, source, target, k);
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(end - start).count();

        // Acumulando valores para estatísticas médias
        total_sift_up += MinHeap::sift_up_operations;
        total_sift_down += MinHeap::sift_down_operations;
        total_sifts += MinHeap::sift_up_operations + MinHeap::sift_down_operations;

        total_insert += Dijkstra::insert_operations;
        total_delete += Dijkstra::delete_min_operations;
        total_update += Dijkstra::update_operations;

        total_time += duration;

        float i = Dijkstra::insert_operations / static_cast<float>(n);
        float d = Dijkstra::delete_min_operations / static_cast<float>(n);
        float u = Dijkstra::update_operations / static_cast<float>(m);

        auto log_kn = log(k) / log(n);  
        float r_insert = MinHeap::insert_sifts / (NUM_PAIRS * log_kn);
        float r_delete = MinHeap::extract_sifts / (NUM_PAIRS * log_kn);
        float r_update = MinHeap::update_sifts / (NUM_PAIRS * log_kn);

        out_results << k << ","
                    << n << ","
                    << m << ","
                    << source << ","
                    << target << ","
                    << (dist == INF ? "INF" : std::to_string(dist)) << ","
                    << Dijkstra::insert_operations << ","
                    << Dijkstra::delete_min_operations << ","
                    << Dijkstra::update_operations << ","
                    << MinHeap::sift_up_operations << ","
                    << MinHeap::sift_down_operations << ","
                    << i << ","
                    << d << ","
                    << u << ","
                    << r_insert << ","
                    << r_delete << ","
                    << r_update << ","
                    << duration << "\n";
    }

    cout << "k=" << k << " concluído." << endl;

    
    out_results.close();
    cout << "Arquivos de resultados gerado com sucesso.\n";
}


int main() {
   // Obtendo todos os arquivos na pasta Caminho_Mais_Curto/src/input/dimacs
    string input_dir = "../input/random/";
    vector<string> files;

    for (const auto& entry : std::filesystem::directory_iterator(input_dir)) {
        if (entry.is_regular_file()) {
            files.push_back(entry.path().string());
        }
    }

    int count = 0;
    for (const auto& file : files) {
        ifstream input_file(file);

        if (!input_file) {
            cerr << "Erro ao abrir o arquivo: " << file << endl;
            continue;
        }
        
        int k = 3; // Defina o valor de k desejado

        runFullTests(file, k, count);
        count++;

        input_file.close();
    }



    return 0;
}
