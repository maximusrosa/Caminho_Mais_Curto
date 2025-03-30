#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "../include/graph.hpp" // Include the graph header

void read_dimacs(std::istream& in, unsigned& n, unsigned& m, Graph& graph) {
    std::string line;
    std::string dummy;
    while (line.substr(0, 4) != "p sp") {
        std::getline(in, line);
    }

    // (1) Get nodes and edges
    std::stringstream linestr(line);
    linestr >> dummy >> dummy >> n >> m;
    graph = Graph(n); // Initialize the graph with 'n' vertices

    unsigned i = 0;
    while (i++ < m) {
        std::getline(in, line);
        if (line.substr(0, 2) == "a ") {
            std::stringstream arc(line);
            unsigned u, v, w;
            char ac;
            arc >> ac >> u >> v >> w;
            u--; v--; // Convert to 0-indexed (if DIMACS uses 1-indexed vertices)
            graph.addEdge(u, v, w); // Add the edge to the graph
        }
    }
}

int main() {
    // Example usage
    std::istringstream input(
        "p sp 3 2\n"
        "a 1 2 10\n"
        "a 2 3 20\n"
    );

    unsigned n, m;
    Graph graph(0); // Initialize with 0 vertices, will be resized in read_dimacs
    read_dimacs(input, n, m, graph);

    // Print the graph (for verification)
    for (unsigned u = 0; u < n; ++u) {
        for (const auto& [v, w] : graph.getNeighbors(u)) {
            std::cout << "Edge: " << u + 1 << " -> " << v + 1 << " (weight: " << w << ")\n";
        }
    }

    //    


    return 0;
}