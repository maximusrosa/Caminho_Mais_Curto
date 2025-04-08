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

short int fileNumber;

void kTest(const string filename, const vector<int>& ks) {
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

    ofstream out_results("../output/best-k/resultados-" + to_string(fileNumber) + ".csv");

    // Cabeçalhos dos arquivos CSV
    out_results << "k,vertices,edges,source,target,dist,insert,delMin,update,time_ms\n";

    for (int k : ks) {
        long long total_insert = 0;  long long total_delete = 0;    long long total_update = 0;
        long long total_time = 0;

        for (int j = 0; j < NUM_PAIRS; ++j) {
            int source = rand() % n;
            int target = rand() % n;
            while (target == source) target = rand() % n;

            // Resetando contadores de operações
            Dijkstra::insert_operations = 0;  Dijkstra::delete_min_operations = 0; Dijkstra::update_operations = 0;

            auto start = high_resolution_clock::now();

            int dist_st = Dijkstra::shortestPath(g, source, target, k);

            auto end = high_resolution_clock::now();
            auto duration = duration_cast<milliseconds>(end - start).count();

            total_insert += Dijkstra::insert_operations;
            total_delete += Dijkstra::delete_min_operations;
            total_update += Dijkstra::update_operations;

            total_time += duration;

            out_results << k << ","
                        << n << ","
                        << m << ","
                        << source << ","
                        << target << ","
                        << (dist_st == INF ? "INF" : to_string(dist_st)) << ","
                        << Dijkstra::insert_operations << ","
                        << Dijkstra::delete_min_operations << ","
                        << Dijkstra::update_operations << ","
                        << duration << "\n";
        }

        cout << "k=" << k << " concluído." << endl;

    }
    
    out_results.close();
}


int main() {
    string input_dir = "../input/";
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
        
        vector<int> ks = {2, 3, 4, 8, 10, 16, 32, 64, 100}; // Valores de k a serem testados

        kTest(file, ks);
        fileNumber++;

        input_file.close();
    }



    return 0;
}
