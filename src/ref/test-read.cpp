#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <fstream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

using namespace boost;

// Define the graph type using Boost
typedef adjacency_list<vecS, vecS, directedS, no_property, property<edge_weight_t, int>> BoostGraph;
typedef graph_traits<BoostGraph>::vertex_descriptor Vertex;

void read_dimacs(std::istream& in, unsigned& n, unsigned& m, BoostGraph& graph, property_map<BoostGraph, edge_weight_t>::type& weightmap) {
    std::string line;
    std::string dummy;
    while (line.substr(0, 4) != "p sp") {
        std::getline(in, line);
    }

    // (1) Get nodes and edges
    std::stringstream linestr(line);
    linestr >> dummy >> dummy >> n >> m;
    graph = BoostGraph(n); // Initialize the graph with 'n' vertices
    weightmap = get(edge_weight, graph);

    unsigned i = 0;
    while (i++ < m) {
        std::getline(in, line);
        if (line.substr(0, 2) == "a ") {
            std::stringstream arc(line);
            unsigned u, v, w;
            char ac;
            arc >> ac >> u >> v >> w;
            u--; v--; // Convert to 0-indexed (if DIMACS uses 1-indexed vertices)
            auto edge = add_edge(u, v, graph).first;
            weightmap[edge] = w; // Set the edge weight
        }
    }
}

int main() {
    // Example usage
    const std::string filename = "USA-road-d.NY.gr"; // Input file name
    std::ifstream file("../../../files/" + filename);

    if (!file) {
        std::cerr << "Error opening file\n";
        return 1;
    }

    unsigned n, m;
    BoostGraph graph;
    property_map<BoostGraph, edge_weight_t>::type weightmap;
    read_dimacs(file, n, m, graph, weightmap);

    // Run Dijkstra's algorithm
    std::vector<Vertex> predecessors(num_vertices(graph)); // To store the predecessors
    std::vector<int> distances(num_vertices(graph));       // To store the distances

    Vertex source = 0; // Assuming the source vertex is 0
    dijkstra_shortest_paths(graph, source,
        predecessor_map(&predecessors[0]).distance_map(&distances[0]));

    // Print the results
    std::cout << "Distances from source vertex " << source << ":\n";
    for (size_t i = 0; i < distances.size(); ++i) {
        std::cout << "Vertex " << i << ": " << distances[i] << "\n";
    }

    return 0;
}