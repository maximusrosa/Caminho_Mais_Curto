#include <vector>
#include <iostream>
#include <sstream>
#include <string>
#include <limits>
#include "min_heap.hpp"
#define INF numeric_limits<int>::max()
using namespace std;

class Graph {
private:
    int numVertices;
    vector<vector<pair<int, int>>> adjVector; // Adjacency list {neighbor, weight}

public:
    // Default constructor
    Graph(int v) : numVertices(v), adjVector(v) {}

    /* Constructors based on code by Marcus Ritt */
    /*          <mrpritt@inf.ufrgs.br>          */

    // Alternative constructor to read from DIMACS format
    Graph(istream& in) {
        string line, dummy;
        unsigned n, m;

        // Skip comments and find "p sp"
        while (getline(in, line) && line.substr(0, 4) != "p sp");

        // Read number of vertices and edges
        stringstream linestr(line);
        linestr >> dummy >> dummy >> n >> m;

        numVertices = n;
        adjVector.resize(n);

        // Read edges
        for (unsigned i = 0; i < m; i++) {
            getline(in, line);
            if (line.substr(0, 2) == "a ") {
                stringstream arc(line);
                unsigned u, v, w;
                char ac;
                arc >> ac >> u >> v >> w;
                u--; v--; // Convert to 0-indexed
                addEdge(u, v, w);
            }
        }
    }

    // Random graph constructor
    Graph (unsigned n, double p, int maxWeight) {
        srand(time(nullptr)); // Seed for random numbers

        for (unsigned i = 0; i < n; i++) {
            for (unsigned j = 0; j < n; j++) {
                if (i != j && (rand() / (double)RAND_MAX) < p) {
                    addEdge(i, j, rand() % maxWeight + 1); // Weight in the range [1, maxWeight]
                }
            }
        }
    }

    /*--------------------------------------------*/

    void addEdge(int u, int v, int w) {
        adjVector[u].push_back({v, w});
        //adjVector[v].push_back({u, w}); // Uncomment for directed graph
    }

    void printGraph() {
        for (int i = 0; i < numVertices; i++) {
            cout << "Vertex " << i + 1 << " -> ";
            for (const auto& [v, w] : adjVector[i]) {
                cout << "(" << v + 1 << ", " << w << ") ";
            }
            cout << "\n";
        }
    }

    void shortestPath(int src, int k) {
        MinHeap minHeap(k); // Using k-ary heap

        vector<int> dist(numVertices, INF);
        dist[src] = 0;
        minHeap.insert({src, 0}); // {vertex, distance}

        while (!minHeap.isEmpty()) {
            auto [uDist, u] = minHeap.extractMin();

            for (auto [v, weight] : adjVector[u]) {
                if (dist[v] > uDist + weight) {
                    dist[v] = uDist + weight;
                    minHeap.insert({dist[v], v});
                }
            }
        }

        // Print shortest distances
        cout << "Vertex\tDistance from Source\n";
        for (int i = 0; i < numVertices; ++i)
            if (dist[i] == INF)
                cout << i << "\t\tINF\n";
            else
                cout << i << "\t\t" << dist[i] << "\n";
    }
};
