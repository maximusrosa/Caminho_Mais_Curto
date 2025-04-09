#include <vector>
#include <iostream>
#include <sstream>
#include <string>
#include <limits>
#define INF numeric_limits<int>::max()
#define MAX_WEIGHT 1000
using namespace std;

class Graph {
private:
    int numVertices;
    int numEdges; // Number of edges in the graph
    vector<vector<pair<int, int>>> adjVector; // Adjacency list {neighbor, weight}

public:
    // Default constructor
    Graph(int v) : numVertices(v), numEdges(0), adjVector(v) {}

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
        numEdges = m;

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
    Graph (unsigned n, float p) {
        numVertices = n;
        adjVector.resize(n);
        numEdges = 0;
    
        for (unsigned i = 0; i < n; i++) {
            for (unsigned j = 0; j < n; j++) {
                if (i != j && (drand48() < p)) {
                    addEdge(i, j, lrand48() % MAX_WEIGHT); // Weight in the range [1, maxWeight]
                    numEdges++;
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

    const int getNumVertices() const {
        return numVertices;
    }

    const int getNumEdges() const {
        return numEdges;
    }

    const vector<pair<int, int>> &getAdjList(int u) const {
        return adjVector[u];
    }
};
