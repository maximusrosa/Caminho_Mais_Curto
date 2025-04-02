#include <vector>
#include <utility>
#include <algorithm>
#include <stdexcept>

using namespace std;

class MinHeap {
    private:
        int k; // Number of children per node
        vector<pair<int, int>> heap; // {distance, vertex} pairs
        unordered_map<int, int> position; // Maps the vertice -> heap index
    
        int parent(int i) { return (i - 1) / k; }
        int child(int i, int j) { return k * i + (j + 1); }
    
        // Function to restore the heap property by moving an element down
        // This is called after extracting the minimum element to ensure the min-heap property
        void restoreDown(int index) {
            while (true) {
                int min_child_index = -1;
                
                // Iterate through all children of the current node
                for (int j = 0; j < k; j++) {
                    int c = child(index, j);
                    // Find the smallest child that satisfies the heap property
                    if (c < heap.size() && 
                    (min_child_index == -1 || heap[c].first < heap[min_child_index].first)) {
                    min_child_index = c;
                    }
                }
            
                // If no child is smaller or heap property is satisfied, stop
                if (min_child_index == -1 || heap[index].first <= heap[min_child_index].first)
                    break;
            
                // Swap the current node with the smallest child
                swapNodes(index, min_child_index);

                index = min_child_index; // Move to the child index
            }
        }
    
        // Function to restore the heap property by moving an element up
        // This is called after inserting a new element to ensure the min-heap property
        void restoreUp(int index) {
            while (index > 0 && heap[parent(index)].first > heap[index].first) {
                swapNodes(index, parent(index));
                index = parent(index);
            }
        }

        void swapNodes(int i, int j) {
            swap(heap[i], heap[j]);
            position[heap[i].second] = i;
            position[heap[j].second] = j;
        }
    
    public:
        static int sift_up_operations;
        static int sift_down_operations;

        MinHeap(int k) : k(k) {}
    
        bool isEmpty() { return heap.empty(); }
    
        void insert(pair<int, int> node) {
            heap.push_back(node);
            int index = heap.size() - 1;
            position[node.second] = index; // Register the vertex position
            restoreUp(index);

            sift_up_operations++;
        }
    
        pair<int, int> extractMin() {
            if (heap.empty()) throw runtime_error("Heap is empty");
    
            pair<int, int> minVal = heap[0];
            position.erase(minVal.second); // Remove from position map

            if (heap.size() > 1){
                heap[0] = heap.back();
                position[heap[0].second] = 0; // Update position of the new root
            }

            heap.pop_back();
            restoreDown(0);

            sift_down_operations++;
    
            return minVal;
        }

        bool contains(int vertex) {
            return position.find(vertex) != position.end();
        }

        void decreaseKey(int newDist, int vertex) {
            if (position.find(vertex) == position.end()) return; // If vertex not in heap, do nothing
    
            int index = position[vertex];
            if (heap[index].first > newDist) {
                heap[index].first = newDist;
                restoreUp(index);
                
                sift_up_operations++;
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
            cout << endl; // Print remaining nodes in the last level
            }
        }

};