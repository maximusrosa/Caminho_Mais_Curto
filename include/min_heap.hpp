#include <vector>
#include <utility>
#include <algorithm>
#include <stdexcept>

using namespace std;

class MinHeap {
    private:
        int k; // Number of children per node
        vector<pair<int, int>> heap; // {vertex, distance}
        unordered_map<int, int> position; // Maps the vertice -> heap index
    
        int parent(int i) { return (i - 1) / k; }
        int child(int i, int j) { return k * i + (j + 1); }
    
        void restoreDown(int index) {
            while (true) {
                int min_child_index = -1;
                
                for (int j = 0; j < k; j++) {
                    int c = child(index, j);

                    if (c < heap.size() && 
                    (min_child_index == -1 || heap[c].second < heap[min_child_index].second)) {
                    min_child_index = c;
                    }
                }
            
                if (min_child_index == -1 || heap[index].second <= heap[min_child_index].second)
                    break;
            
                swapNodes(index, min_child_index);
                index = min_child_index;

                sift_down_operations++;
            }
        }
    
        void restoreUp(int index) {
            while (index > 0 && heap[parent(index)].second > heap[index].second) {
                swapNodes(index, parent(index));
                index = parent(index);

                sift_up_operations++;
            }
        }

        void swapNodes(int i, int j) {
            swap(heap[i], heap[j]);
            position[heap[i].first] = i;
            position[heap[j].first] = j;
        }
    
    public:
        static int sift_up_operations;
        static int sift_down_operations;
        static int insert_sifts;
        static int extract_sifts;
        static int update_sifts;

        MinHeap(int k) : k(k) {}
    
        bool isEmpty() { return heap.empty(); }
    
        void insert(int vertex, int distance) {
            pair<int, int> node = {vertex, distance};
            heap.push_back(node);

            int index = heap.size() - 1;
            position[node.first] = index;
            restoreUp(index);
        }
    
        pair<int, int> extractMin() {
            if (heap.empty()) throw runtime_error("Heap is empty");
    
            pair<int, int> minVal = heap[0];
            position.erase(minVal.first); 

            if (heap.size() > 1){
                heap[0] = heap.back();
                position[heap[0].first] = 0; 
            }

            heap.pop_back();
            restoreDown(0);
    
            return minVal;
        }

        bool contains(int vertex) {
            return position.find(vertex) != position.end();
        }

        void decreaseKey(int vertex, int newDist) {
            if (position.find(vertex) == position.end()) return; // If vertex not in heap, do nothing

            int index = position[vertex];
            
            if (heap[index].second > newDist) {
                heap[index].second = newDist;
                restoreUp(index);
            }
        }

        void print() {
            int level = 0;
            int nodesInLevel = 1;
            int count = 0;

            for (const auto& [dist, vertex] : heap) {
            cout << "(" << dist << ", " << vertex << ") ";
            count++;

            if (count == nodesInLevel) {
                cout << endl;
                level++;
                nodesInLevel *= k;
                count = 0;
            }
            }

            if (count > 0) {
            cout << endl; 
            }
        }

};