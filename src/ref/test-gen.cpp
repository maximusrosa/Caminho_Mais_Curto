// quero testar o construtor aleatório da minha classe Graph
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "../../include/graph.hpp"

using namespace std;

int main() {
    srand48(time(0)); // Seed for random numbers

    unsigned n = 15; // Number of vertices
    float p = 0.5; // Probability of edge creation

    Graph g(n, p); // Create a random graph

    cout << "Graph created with " << n << " vertices and probability " << p << endl;

    // Print the graph in DIMACS format
    g.printGraph();

    return 0;
}