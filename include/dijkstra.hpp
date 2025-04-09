#include "../include/graph.hpp"
#include "min_heap.hpp"
#include <fstream>

using namespace std;

long getMemoryUsageKB() {
    std::ifstream status_file("/proc/self/status");
    std::string line;
    while (std::getline(status_file, line)) {
        if (line.rfind("VmRSS:", 0) == 0) {
            long memory_kb;
            std::istringstream iss(line.substr(6));
            iss >> memory_kb;
            return memory_kb;
        }
    }
    return 0;
}

class Dijkstra {
    public:
        static int insert_operations;
        static int delete_min_operations;
        static int update_operations;

        // Para o teste de melhor k
        static int shortestPath(Graph& g, int startVertex, int endVertex, int k) {
            insert_operations = 0;
            delete_min_operations = 0;
            update_operations = 0;
    
            int n = g.getNumVertices();
            vector<int> dist(n, INF);
            MinHeap minHeap(n, k);
    
            dist[startVertex] = 0;
            update_operations++;
            
            minHeap.insert(startVertex, 0);
            insert_operations++;
    
            while (!minHeap.isEmpty()) {
                auto [u, uDist] = minHeap.extractMin(); 
                delete_min_operations++;

                if (uDist > dist[u]) throw runtime_error("uDist > dist[u]");
    
                for (const auto& [v, weight] : g.getAdjList(u)) {
                    if (dist[v] > uDist + weight) {
                        dist[v] = uDist + weight;
                    
                        if (minHeap.contains(v)) {  // Se já está no heap, atualize a chave.
                            minHeap.decreaseKey(v, dist[v]); 
                            update_operations++;
                        } else { 
                            minHeap.insert(v, dist[v]); // Caso contrário, insira no heap.
                            insert_operations++;
                        }
                    }
                }
            }
    
            return dist[endVertex];
        }

        // Para o teste geral das operações
        static pair<int, vector<float>> shortestPath(Graph& g, int startVertex, int endVertex) {
            insert_operations = 0;
            delete_min_operations = 0;
            update_operations = 0;
    
            int k = 16; // Valor padrão de k
            int n = g.getNumVertices();
            vector<int> dist(n, INF);
            MinHeap minHeap(n, k);
    
            dist[startVertex] = 0;
            update_operations++;
            
            minHeap.insert(startVertex, 0);
            insert_operations++;
    
            while (!minHeap.isEmpty()) {
                auto [u, uDist] = minHeap.extractMin(); 
                delete_min_operations++;

                if (uDist > dist[u]) throw runtime_error("uDist > dist[u]");
    
                for (const auto& [v, weight] : g.getAdjList(u)) {
                    if (dist[v] > uDist + weight) {
                        dist[v] = uDist + weight;
                    
                        if (minHeap.contains(v)) {  // Se já está no heap, atualize a chave.
                            minHeap.decreaseKey(v, dist[v]); 
                            update_operations++;
                        } else { 
                            minHeap.insert(v, dist[v]); // Caso contrário, insira no heap.
                            insert_operations++;
                        }
                    }
                }
            }
    
            return {dist[endVertex], minHeap.rCount};
        }

        // Para o teste de tempo e memória
        static pair<int, long> shortestPath(int startVertex, int endVertex, Graph& g) {
            int n = g.getNumVertices();
            vector<int> dist(n, INF);
            int k = 16;
            MinHeap minHeap(n, k);
        
            dist[startVertex] = 0;
            minHeap.insert(startVertex, 0);
        
            // 👇 Medir a memória logo após todas as estruturas estarem alocadas
            long mem_during = getMemoryUsageKB();
        
            while (!minHeap.isEmpty()) {
                auto [u, uDist] = minHeap.extractMin();
        
                if (uDist > dist[u]) throw runtime_error("uDist > dist[u]");
        
                for (const auto& [v, weight] : g.getAdjList(u)) {
                    if (dist[v] > uDist + weight) {
                        dist[v] = uDist + weight;
        
                        if (minHeap.contains(v)) {
                            minHeap.decreaseKey(v, dist[v]);
                        } else {
                            minHeap.insert(v, dist[v]);
                        }
                    }
                }
            }
        
            return {dist[endVertex], mem_during};
        }
};

int Dijkstra::insert_operations = 0;
int Dijkstra::delete_min_operations = 0;
int Dijkstra::update_operations = 0;