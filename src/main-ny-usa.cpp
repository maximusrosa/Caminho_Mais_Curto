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

void runScallingTests(const string filename, const vector<int>& ks, int count) {
    ifstream file("../input/" + filename);

    if (!file) {
        cerr << "Error opening file\n";
        throw;
    }

    Graph g(file);
    file.close();

    cout << "Grafo criado com sucesso." << endl;

    ofstream out_stats("../output/last/estatisticas_" + std::to_string(count) + ".csv");

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
        vector<int> ks = {10};

        runScallingTests(file, ks, count);
        count++;

        input_file.close();

        cout << "Grafo lido com sucesso de: " << file << endl;
        }



    return 0;
}
