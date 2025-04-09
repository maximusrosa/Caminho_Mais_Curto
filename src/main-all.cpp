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
#define NUM_REPS 5 // Número de repetições para cada grafo
#define k 8 // Melhor valor de k encontrado

using namespace std;
using namespace chrono;

short int fileNumber;
short int iter;

void runFullTests(const string filename) {
    ifstream file(filename);

    if (!file) {
        cerr << "Error opening file\n";
        throw;
    }

    Graph g(file);
    file.close();

    cout << "Grafo criado a partir de " << filename << endl;

    int n = g.getNumVertices();
    int m = g.getNumEdges();

    ofstream out_results("../output/full/random/resultados-" + to_string(fileNumber) + ".csv");

    // Cabeçalhos dos arquivos CSV
    out_results << "iter,vertices,edges,source,target,dist,insert,delMin,update,i,d,u,avgR_insert,avgR_delMin,avgR_update,time_ms,normTime\n";

    long long total_insert = 0;  long long total_delete = 0;    long long total_update = 0;
    long long total_time = 0;

    for (int j = 0; j < NUM_PAIRS; ++j) {
        int source = rand() % n;
        int target = rand() % n;
        while (target == source) target = rand() % n;

        // Resetando contadores de operações
        Dijkstra::insert_operations = 0;  Dijkstra::delete_min_operations = 0; Dijkstra::update_operations = 0;

        auto start = high_resolution_clock::now();

        auto result = Dijkstra::shortestPath(g, source, target);
        int dist_st = result.first;
        const auto& rCount = result.second;

        auto end = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(end - start).count();

        total_insert += Dijkstra::insert_operations;
        total_delete += Dijkstra::delete_min_operations;
        total_update += Dijkstra::update_operations;

        total_time += duration;

        auto log_kn = log(n) / log(k);
        float normalizedTime = static_cast<float>(duration) / log_kn;

        float i = Dijkstra::insert_operations / static_cast<float>(n);
        float d = Dijkstra::delete_min_operations / static_cast<float>(n);
        float u = Dijkstra::update_operations / static_cast<float>(m);

        float avg_r_insert = rCount.size() > 0 ? rCount[INSERT] / Dijkstra::insert_operations : 0;
        float avg_r_delmin = rCount.size() > 1 ? rCount[DELETE_MIN] / Dijkstra::delete_min_operations : 0;
        float avg_r_update = rCount.size() > 2 ? rCount[UPDATE] / Dijkstra::update_operations : 0;

        out_results << iter << ","
                    << n << ","
                    << m << ","
                    << source << ","
                    << target << ","
                    << (dist_st == INF ? "INF" : to_string(dist_st)) << ","
                    << Dijkstra::insert_operations << ","
                    << Dijkstra::delete_min_operations << ","
                    << Dijkstra::update_operations << ","
                    << i << ","
                    << d << ","
                    << u << ","
                    << avg_r_insert << ","
                    << avg_r_delmin << ","
                    << avg_r_update << ","
                    << duration << ","
                    << normalizedTime << "\n";
    }

    out_results.close();
    cout << "Arquivos de resultados gerado com sucesso.\n";
}


int main() {
    string input_dir = "../input/random/";
    vector<string> files;

    for (const auto& entry : filesystem::directory_iterator(input_dir)) {
        if (entry.is_regular_file()) {
            files.push_back(entry.path().string());
        }
    }

    fileNumber = 0;
    for (const auto& file : files) {
        ifstream input_file(file);

        if (!input_file) {
            cerr << "Erro ao abrir o arquivo: " << file << endl;
            continue;
        }
        
        runFullTests(file);
        fileNumber++;
    

        input_file.close();
    }



    return 0;
}
