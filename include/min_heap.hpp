#include <vector>
#include <utility>
#include <unordered_map>
#include <algorithm>
#include <stdexcept>
#include <iostream>
#include <string>
#include <cmath>

#define INSERT 0
#define DELETE_MIN 1
#define UPDATE 2
using namespace std;

class MinHeap {
private:
    int n; // Max number of nodes
    int k; // Number of children per node
    vector<pair<int, int>> heap; // {vertex, distance}
    unordered_map<int, int> position; // Maps vertex -> heap index

    int parent(int i) { return (i - 1) / k; }
    int child(int i, int j) { return k * i + (j + 1); }

    int restoreDown(int index) {
        int sifts = 0;

        while (true) {
            int min_child_index = -1;

            for (int j = 0; j < k; j++) {
                int c = child(index, j);
                if (c < heap.size() && (min_child_index == -1 || heap[c].second < heap[min_child_index].second)) {
                    min_child_index = c;
                }
            }

            if (min_child_index == -1 || heap[index].second <= heap[min_child_index].second)
                break;

            swapNodes(index, min_child_index);
            index = min_child_index;
            sifts++;
        }

        return sifts;
    }

    int restoreUp(int index) {
        int sifts = 0;

        while (index > 0 && heap[parent(index)].second > heap[index].second) {
            swapNodes(index, parent(index));
            index = parent(index);
            sifts++;
        }

        return sifts;
    }

    void swapNodes(int i, int j) {
        swap(heap[i], heap[j]);
        position[heap[i].first] = i;
        position[heap[j].first] = j;
    }

public:
    // {insert, delete_min, update}
    vector<float> rCount = {0, 0, 0}; 

    MinHeap(int n, int k) : n(n), k(k) {}

    bool isEmpty() { return heap.empty(); }

    void insert(int vertex, int distance) {
        pair<int, int> node = {vertex, distance};
        heap.push_back(node);

        int index = heap.size() - 1;
        position[node.first] = index;

        int sifts = restoreUp(index);
        // Logarithm base k of n
        float log_kn = log(n) / log(k);

        float r = sifts / log_kn;
        rCount[INSERT] += r;
    }

    pair<int, int> extractMin() {
        if (heap.empty()) throw runtime_error("Heap is empty");

        pair<int, int> minVal = heap[0];
        position.erase(minVal.first);

        if (heap.size() > 1) {
            heap[0] = heap.back();
            position[heap[0].first] = 0;
        }

        heap.pop_back();

        int sifts = restoreDown(0);
        float log_kn = log(n) / log(k);

        float r = sifts / log_kn;
        rCount[DELETE_MIN] += r;

        return minVal;
    }

    bool contains(int vertex) {
        return position.find(vertex) != position.end();
    }

    void decreaseKey(int vertex, int newDist) {
        if (position.find(vertex) == position.end()) return;

        int index = position[vertex];

        if (heap[index].second > newDist) {
            heap[index].second = newDist;
            int sifts = restoreUp(index);
            float log_kn = log(n) / log(k);

            float r = sifts / log_kn;
            rCount[UPDATE] += r;
        }
    }

    void printHeap() {
        int level = 0;
        int nodesInLevel = 1;
        int count = 0;

        for (const auto& [vertex, dist] : heap) {
            cout << "(" << vertex << ", " << dist << ") ";
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

    void printLog() {
        cout << "Operations Log:\n";
        cout << "Insert: " << rCount[INSERT] << endl;
        cout << "Delete min: " << rCount[DELETE_MIN] << endl;
        cout << "Update: " << rCount[UPDATE] << endl;
    }
};
