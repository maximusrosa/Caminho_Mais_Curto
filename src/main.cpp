#include <iostream>
#include <vector>
#include <limits>
#include <fstream>
#include "../include/graph.hpp"

using namespace std;

class Dijkstra {
    public:
        static int insert_operations;
        static int delete_min_operations;
        static int update_operations;

        static void shortestPath(Graph& g, int startVertex, int k) {
            insert_operations = 0;
            delete_min_operations = 0;
            update_operations = 0;
    
            int numVertices = g.getNumVertices();
            vector<int> dist(numVertices, INF);
            MinHeap minHeap(k);
    
            dist[startVertex] = 0;
            update_operations++;
            
            minHeap.insert({0, startVertex});
            insert_operations++;
    
            while (!minHeap.isEmpty()) {
                auto [uDist, u] = minHeap.extractMin(); // {distance, vertex}
                delete_min_operations++;

                if (uDist > dist[u]) continue; // Se já temos um caminho melhor, ignore esse nó.
    
                for (const auto& [v, weight] : g.getAdjList(u)) {
                    if (dist[v] > uDist + weight) {
                        dist[v] = uDist + weight;
                    
                        if (minHeap.contains(v)) {  // Se já está no heap, atualize a chave.
                            minHeap.decreaseKey(dist[v], v); 
                            update_operations++;
                        } else { 
                            minHeap.insert({dist[v], v});
                            insert_operations++;
                        }
                    }
                }
            }
        
    
            cout << "Vertex\tDistance from Source (" << startVertex << ")\n";
            for (int i = 0; i < numVertices; ++i)
                if (dist[i] == INF)
                    cout << i << "\t\tINF\n";
                else
                    cout << i << "\t\t" << dist[i] << "\n";
        

            cout << "\nOperations:\n";
            cout << "Insert: " << insert_operations << endl;
            cout << "Delete min: " << delete_min_operations << endl;
            cout << "Update: " << update_operations << endl;
            cout << "Sift up: " << MinHeap::sift_up_operations << endl;
            cout << "Sift down: " << MinHeap::sift_down_operations << endl;
        }
    };

int Dijkstra::insert_operations = 0;
int Dijkstra::delete_min_operations = 0;
int Dijkstra::update_operations = 0;
int MinHeap::sift_up_operations = 0;
int MinHeap::sift_down_operations = 0;

int main() {
    const string filename = "test-1.gr";
    ifstream file("../files/" + filename);

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

    Dijkstra::shortestPath(g, startVertex, k);

/*
    MinHeap heap(2);
    heap.insert({10, 1});
    heap.insert({20, 2});
    heap.insert({30, 3});

    heap.print();

    heap.decreaseKey(5, 2); // Deve mover o nó 2 para cima no heap
    heap.decreaseKey(3, 3); // Deve mover o nó 3 para o topo

    while (!heap.isEmpty()) {
        auto [dist, v] = heap.extractMin();
        cout << "Extraído: (" << dist << ", " << v << ")\n";
    }
*/

    return 0;
}
