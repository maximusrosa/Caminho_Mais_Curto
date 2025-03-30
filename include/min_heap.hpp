#include <vector>
#include <utility>
#include <algorithm>
#include <stdexcept>

using namespace std;

class MinHeap {
    private:
        int k; // Number of children per node
        vector<pair<int, int>> heap; // {distance, vertex} pairs
    
        int parent(int i) { return (i - 1) / k; }
        int child(int i, int j) { return k * i + (j + 1); }
    
        void restoreDown(int index) {
            while (true) {
                int min_child_index = -1;
                
                for (int j = 0; j < k; j++) {
                    int c = child(index, j);
                    if (c < heap.size() && 
                        (min_child_index == -1 || heap[c].first < heap[min_child_index].first)) {
                        min_child_index = c;
                    }
                }
    
                if (min_child_index == -1 || heap[index].first <= heap[min_child_index].first)
                    break;
    
                swap(heap[index], heap[min_child_index]);
                index = min_child_index;
            }
        }
    
        void restoreUp(int index) {
            while (index > 0 && heap[parent(index)].first > heap[index].first) {
                swap(heap[index], heap[parent(index)]);
                index = parent(index);
            }
        }
    
    public:
        MinHeap(int k) : k(k) {}
    
        bool isEmpty() { return heap.empty(); }
    
        void insert(pair<int, int> elem) {
            heap.push_back(elem);
            restoreUp(heap.size() - 1);
        }
    
        pair<int, int> extractMin() {
            if (heap.empty()) throw runtime_error("Heap is empty");
    
            pair<int, int> minVal = heap[0];
            heap[0] = heap.back();
            heap.pop_back();
            restoreDown(0);
    
            return minVal;
        }
    };