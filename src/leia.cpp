#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <utility> // for std::pair
#include <unordered_map>

// Define MyGraph as an adjacency list using unordered_map
using MyGraph = std::vector<std::unordered_map<unsigned, unsigned>>;


// Resize the graph to accommodate 'n' vertices
void resize(MyGraph& graph, unsigned n) {
    graph.resize(n);
}

// Add an edge from u to v with weight w
void add_edge(MyGraph& graph, unsigned u, unsigned v, unsigned w) {
    graph[u][v] = w; // Armazena o peso da aresta de u para v
}

void read_dimacs(std::istream& in, unsigned& n, unsigned& m, MyGraph& a) {
    std::string line;
    std::string dummy;
    while (line.substr(0, 4) != "p sp") {
        std::getline(in, line);
    }

    // (1) Get nodes and edges
    std::stringstream linestr(line);
    linestr >> dummy >> dummy >> n >> m;
    resize(a, n); // Resize the graph to hold 'n' vertices

    unsigned i = 0;
    while (i++ < m) {
        std::getline(in, line);
        if (line.substr(0, 2) == "a ") {
            std::stringstream arc(line);
            unsigned u, v, w;
            char ac;
            arc >> ac >> u >> v >> w;
            u--; v--; // Convert to 0-indexed (if DIMACS uses 1-indexed vertices)
            add_edge(a, u, v, w); // Add the edge to the graph
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
    MyGraph graph;
    read_dimacs(input, n, m, graph);

    // Print the graph (for verification)
    for (unsigned u = 0; u < n; ++u) {
        for (const auto& [v, w] : graph[u]) {
            std::cout << "Edge: " << u + 1 << " -> " << v + 1 << " (weight: " << w << ")\n";
        }
    }

    return 0;
}