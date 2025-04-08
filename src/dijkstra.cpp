#include "../include/dijkstra.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <limits>
#include <cstdlib>

using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Uso: " << argv[0] << " origem destino < arquivo.gr" << std::endl;
        return 1;
    }

    int origem = std::atoi(argv[1]);
    int destino = std::atoi(argv[2]);
    int k = 8; // Melhor valor de k encontrado

    Graph g(std::cin);
 
    if (origem == 0 || destino == 0) {
        throw runtime_error("Erro: Indíces começam em 1.");
    }
    
    auto dist_st = Dijkstra::shortestPath(g, origem - 1, destino - 1, k);

    cout << ((dist_st == INF) ? "INF" : to_string(dist_st)) << endl;

    return 0;
}
