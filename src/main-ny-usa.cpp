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
#define k 8 // Melhor valor de k encontrado

using namespace std;
using namespace chrono;

short int fileNumber;

void scallingTest(const string filename) {
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

    ofstream out_results("../output/ny-usa/resultados-" + to_string(fileNumber) + ".csv");

    // Cabeçalhos dos arquivos CSV
    out_results << "vertices,edges,source,target,dist,time_ms,mem_kb\n";

    long long total_time = 0;

    for (int j = 0; j < NUM_PAIRS; ++j) {
        int source = rand() % n;
        int target = rand() % n;
        while (target == source) target = rand() % n;

        auto start = high_resolution_clock::now();

        auto result = Dijkstra::shortestPath(source, target, g);
        int dist_st = result.first;
        long mem_kb = result.second;

        auto end = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(end - start).count();

        total_time += duration;

        out_results << n << ","
                    << m << ","
                    << source << ","
                    << target << ","
                    << (dist_st == INF ? "INF" : to_string(dist_st)) << ","
                    << duration << ","
                    << mem_kb << "\n";
    }

    cout << "Arquivo de resultados concluído." << endl;

    out_results.close();
}

int main() {
    string input_dir = "../input/usa/";
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

        scallingTest(file);
        fileNumber++;

        input_file.close();
    }

    return 0;
}
