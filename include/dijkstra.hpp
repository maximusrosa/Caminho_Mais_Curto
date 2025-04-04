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
            MinHeap minHeap(numVertices, k);
    
            dist[startVertex] = 0;
            update_operations++;
            
            minHeap.insert(startVertex, 0);
            insert_operations++;
    
            while (!minHeap.isEmpty()) {
                auto [u, uDist] = minHeap.extractMin(); 
                delete_min_operations++;

                //if (uDist > dist[u]) throw;
                if (uDist > dist[u]) continue; // Se já temos um caminho melhor, ignore esse nó.
    
                for (const auto& [v, weight] : g.getAdjList(u)) {
                    if (dist[v] > uDist + weight) {
                        dist[v] = uDist + weight;
                    
                        if (minHeap.contains(v)) {  // Se já está no heap, atualize a chave.
                            minHeap.decreaseKey(dist[v], v); 
                            update_operations++;
                        } else { 
                            minHeap.insert(v, dist[v]); // Caso contrário, insira no heap.
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