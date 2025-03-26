// Program to find Dijkstra's shortest path using STL set in C++.
// Code based on:
// https://github.com/CoffeeBeforeArch/cpp_data_structures/blob/master/algorithms/graph_algorithms/dijkstra/dijkstra.cpp

#include <bits/stdc++.h>
using namespace std;
#define INF 0xC0FFEE

// This class represents a directed graph using 
// adjacency list representation
class Graph {
private:
    int V; // No. of vertices

    // In a weighted graph, we need to store vertex 
    // and weight pair for every edge
    vector<vector<pair<int, int>>> adj;

public:
    Graph(int v); // Constructor

    // function to add an edge to graph
    void addEdge(int u, int v, int w);

    // returns the neighbors of a vertex
    const vector<pair<int, int>>& getNeighbors(int v) const {
        return adj[v];
    }

    // prints shortest path from s
    void shortestPath(int s);
};

// Allocates memory for adjacency list
Graph::Graph(int v) {
    this->V = v;
    adj.resize(V);
}

void Graph::addEdge(int u, int v, int w) {
    adj[u].push_back(make_pair(v, w));
    adj[v].push_back(make_pair(u, w));
}

// Prints shortest paths from src to all other vertices
void Graph::shortestPath(int src) {
    // Create a set to store vertices that are being processed
    set<pair<int, int>> setds;

    // Create a vector for distances and initialize all distances as infinite (INF)
    vector<int> dist(V, INF);

    // Insert source itself in Set and initialize its distance as 0.
    setds.insert(make_pair(0, src));
    dist[src] = 0;

    // Looping till all shortest distance are finalized then setds will become empty
    while (!setds.empty()) {
        // The first vertex in Set is the minimum distance vertex, extract it from set.
        pair<int, int> tmp = *(setds.begin());
        setds.erase(setds.begin());

        // vertex label is stored in second of pair (it has to be done this way to keep the vertices sorted distance (distance must be first item in pair)
        int u = tmp.second;

        // 'i' is used to get all adjacent vertices of a vertex
        for (auto i = adj[u].begin(); i != adj[u].end(); ++i) {
            // Get vertex label and weight of current adjacent of u.
            int v = (*i).first;
            int weight = (*i).second;

            // If there is shorter path to v through u.
            if (dist[v] > dist[u] + weight) {
                // If distance of v is not INF then it must be in our set, so removing it and inserting again with updated less distance.
                // Note: We extract only those vertices from Set for which distance is finalized. So for them, we would never reach here.
                if (dist[v] != INF)
                    setds.erase(setds.find(make_pair(dist[v], v)));

                // Updating distance of v
                dist[v] = dist[u] + weight;
                setds.insert(make_pair(dist[v], v));
            }
        }
    }

    // Print shortest distances stored in dist[]
    printf("Vertex Distance from Source\n");
    for (int i = 0; i < V; ++i)
        printf("%d \t\t %d\n", i, dist[i]);
}