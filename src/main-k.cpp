#include "../include/dijkstra.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <chrono>
#include <cmath>
#include <dirent.h>
#include <filesystem>
#include <sys/resource.h> // For getrusage
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

double log_k(double base, double value) {
    return log(value) / log(base);
}

void runBasicTests(const string filename, const vector<int>& ks, int count) {
    ifstream file("../input/" + filename);

    if (!file) {
        cerr << "Error opening file\n";
        throw;
    }

    Graph g(file);
    file.close();

    cout << "Grafo criado com sucesso." << endl;

    ofstream out_results("../output/k/resultados_por_par_" + std::to_string(count) + ".csv");

    // Cabeçalhos dos arquivos CSV
    out_results << "k,s,t,dist,insert,deleteMin,update";

    int n = g.getNumVertices();
    int m = g.getNumEdges();

    for (int k : ks) {
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
            Dijkstra::insert_operations = 0;
            Dijkstra::delete_min_operations = 0;
            Dijkstra::update_operations = 0;
            MinHeap::sift_up_operations = 0;
            MinHeap::sift_down_operations = 0;

            auto start = high_resolution_clock::now();
            dist = Dijkstra::shortestPath(g, source, target, k);
            auto end = high_resolution_clock::now();
            auto duration = duration_cast<milliseconds>(end - start).count();

            // Acumulando valores para estatísticas médias
            total_insert += Dijkstra::insert_operations;
            total_delete += Dijkstra::delete_min_operations;
            total_update += Dijkstra::update_operations;

            total_time += duration;

            out_results << k << ","
                        << source << ","
                        << target << ","
                        << (dist == INF ? "INF" : std::to_string(dist)) << ","
                        << Dijkstra::insert_operations << ","
                        << Dijkstra::delete_min_operations << ","
                        << Dijkstra::update_operations << ","
                        << duration << "\n";
        }


        cout << "k=" << k << " concluído." << endl;
    }
    
    out_results.close();
    cout << "Arquivos gerados com sucesso.\n";
}

void runFullTests(const string filename, const vector<int>& ks, int count) {
    ifstream file("../input/" + filename);

    if (!file) {
        cerr << "Error opening file\n";
        throw;
    }

    Graph g(file);
    file.close();

    cout << "Grafo criado com sucesso." << endl;

    ofstream out_results("../output/all/resultados_por_par_" + std::to_string(count) + ".csv");
    ofstream out_stats("../output/all/estatisticas_" + std::to_string(count) + ".csv");

    // Cabeçalhos dos arquivos CSV
    out_results << "k,s,t,dist,insert,deleteMin,update,sift_up,sift_down,total_sifts,r,i,d,u,time_ms\n";
    out_stats << "k,n,m,avg_insert,avg_deleteMin,avg_update,avg_time_ms,r,i,d,u\n";

    int n = g.getNumVertices();
    int m = g.getNumEdges();

    for (int k : ks) {
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
            Dijkstra::insert_operations = 0;
            Dijkstra::delete_min_operations = 0;
            Dijkstra::update_operations = 0;
            MinHeap::sift_up_operations = 0;
            MinHeap::sift_down_operations = 0;

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

            float r = (MinHeap::sift_up_operations + MinHeap::sift_down_operations) / log_k(k, n);
            float i = Dijkstra::insert_operations / static_cast<float>(n);
            float d = Dijkstra::delete_min_operations / static_cast<float>(n);
            float u = Dijkstra::update_operations / static_cast<float>(m);

            out_results << k << ","
                        << source << ","
                        << target << ","
                        << (dist == INF ? "INF" : std::to_string(dist)) << ","
                        << Dijkstra::insert_operations << ","
                        << Dijkstra::delete_min_operations << ","
                        << Dijkstra::update_operations << ","
                        << MinHeap::sift_up_operations << ","
                        << MinHeap::sift_down_operations << ","
                        << (MinHeap::sift_up_operations + MinHeap::sift_down_operations) << ","
                        << r << ","
                        << i << ","
                        << d << ","
                        << u << ","
                        << duration << "\n";
        }

        float r = total_sifts / log_k(k, n);
        float i = total_insert / static_cast<float>(n * NUM_PAIRS);
        float d = total_delete / static_cast<float>(n * NUM_PAIRS);
        float u = total_update / static_cast<float>(m * NUM_PAIRS);

        out_stats << k << ","
                  << n << ","
                  << m << ","
                  << total_insert / NUM_PAIRS << ","
                  << total_delete / NUM_PAIRS << ","
                  << total_update / NUM_PAIRS << ","
                  << total_time / NUM_PAIRS << ","
                  << r << ","
                  << i << ","
                  << d << ","
                  << u << "\n";

        cout << "k=" << k << " concluído." << endl;
    }

    out_stats.close();
    out_results.close();
    cout << "Arquivos gerados com sucesso.\n";
}

void runScallingTests(const string filename, const vector<int>& ks, int count) {
    ifstream file("../input/" + filename);

    if (!file) {
        cerr << "Error opening file\n";
        throw;
    }

    Graph g(file);
    file.close();

    cout << "Grafo criado com sucesso." << endl;

    ofstream out_stats("../output/k/estatisticas_" + std::to_string(count) + ".csv");

    // Cabeçalhos dos arquivos CSV
    out_stats << "n,m,avg_time_ms,avg_memory_kb\n";

    int n = g.getNumVertices();
    int m = g.getNumEdges();

    for (int k : ks) {
        long long total_time = 0;
        long long total_memory = 0;

        for (int j = 0; j < NUM_PAIRS; ++j) {
            int source = rand() % n;
            int target = rand() % n;
            int dist;
            while (target == source) target = rand() % n;

            struct rusage usage_before, usage_after;

            // Measure memory usage before
            getrusage(RUSAGE_SELF, &usage_before);

            auto start = high_resolution_clock::now();
            dist = Dijkstra::shortestPath(g, source, target, k);
            auto end = high_resolution_clock::now();
            auto duration = duration_cast<milliseconds>(end - start).count();

            // Measure memory usage after
            getrusage(RUSAGE_SELF, &usage_after);

            // Calculate memory difference in kilobytes
            long memory_used = usage_after.ru_maxrss - usage_before.ru_maxrss;

            // Accumulate time and memory
            total_time += duration;
            total_memory += memory_used;
        }

        out_stats << n << "," 
                  << m << "," 
                  << total_time / NUM_PAIRS << "," 
                  << total_memory / NUM_PAIRS << "\n";

        cout << "k=" << k << " concluído." << endl;
    }

    out_stats.close();
    cout << "Arquivos gerados com sucesso.\n";
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
        vector<int> ks = {16};

        runFullTests(file, ks, count);
        count++;

        input_file.close();

        cout << "Grafo lido com sucesso de: " << file << endl;
        }



    return 0;
}
