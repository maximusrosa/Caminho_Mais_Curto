#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

class MinHeap {
private:
    int k; // Number of children per node
    vector<int> heap;

    int parent(int i) { return (i - 1) / k; }
    int child(int i, int j) { return k * i + (j + 1); }

    void restoreDown(int index) {
        while (true) {
            int min_child = -1, min_child_index = -1;
            
            for (int j = 0; j < k; j++) {
                int c = child(index, j);
                if (c < heap.size() && (min_child == -1 || heap[c] < min_child)) {
                    min_child = heap[c];
                    min_child_index = c;
                }
            }
            
            if (min_child_index == -1 || heap[index] <= heap[min_child_index])
                break;
            
            swap(heap[index], heap[min_child_index]);
            index = min_child_index;
        }
    }

    void restoreUp(int index) {
        while (index > 0 && heap[parent(index)] > heap[index]) {
            swap(heap[index], heap[parent(index)]);
            index = parent(index);
        }
    }

public:
    MinHeap(int k) : k(k) {}

    void buildHeap(const vector<int>& v) {
        heap = v;
        for (int i = (heap.size() - 1) / k; i >= 0; i--) {
            restoreDown(i);
        }
    }

    void insert(int elem) {
        heap.push_back(elem);
        restoreUp(heap.size() - 1);
    }

    int extractMin() {
        if (heap.empty()) throw runtime_error("Heap is empty");
        int minVal = heap[0];
        heap[0] = heap.back();
        heap.pop_back();
        restoreDown(0);
        return minVal;
    }

    void printHeap() {
        int index = 0, levelSize = 1;
        while (index < heap.size()) {
            for (int i = 0; i < levelSize && index < heap.size(); i++) {
                cout << heap[index++] << " ";
            }
            cout << endl;
            levelSize *= k;
        }
    }
};

int main() {
    MinHeap minHeap(3);
    vector<int> v = {10, 20, 15, 30, 40, 50, 100};
    minHeap.buildHeap(v);
    
    cout << "Built Heap:" << endl;
    minHeap.printHeap();
    
    int element = 5;
    minHeap.insert(element);
    cout << "\nHeap after insertion of " << element << ":" << endl;
    minHeap.printHeap();
    
    cout << "\nExtracted min is " << minHeap.extractMin() << endl;
    cout << "\nHeap after extract min:" << endl;
    minHeap.printHeap();
    
    return 0;
}
