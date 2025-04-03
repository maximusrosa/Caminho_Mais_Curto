#include <iostream>
#include <vector>
#include <limits>
#include <fstream>
#include <chrono>
#include "../include/dijkstra.hpp"

int Dijkstra::insert_operations = 0;
int Dijkstra::delete_min_operations = 0;
int Dijkstra::update_operations = 0;
int MinHeap::sift_up_operations = 0;
int MinHeap::sift_down_operations = 0;

int main() {
    const string filename = "USA-road-d.NY.gr"; // Nome do arquivo de entrada
    ifstream file("../../files/" + filename);

    if (!file) {
        cerr << "Error opening file\n";
        return 1;
    }

    Graph g(file);
    file.close();

    //cout << "Graph created successfully\n";
    //graph.printGraph();

    int startVertex = 0; // considerando que os vértices dos arquivos começam em 0
    int k = 3; // Number of children for MinHeap
    //g.shortestPath(startVertex - 1, k);

    auto start = chrono::high_resolution_clock::now();

    Dijkstra::shortestPath(g, startVertex, k);

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;

    cout << "Execution time: " << elapsed.count() << " seconds" << endl;

}
