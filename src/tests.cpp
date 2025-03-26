#include "../include/graph.hpp"
#include "../include/min_heap.hpp"
#include <ctime>

// Driver program to test methods of graph and minHeap classes
int main() {
    // Seed for random numbers
    srand(time(0));

/*
    // create the graph given in above figure
    int V = 9;
    Graph g(V);

    // making above shown graph
    g.addEdge(0, 1, 4);
    g.addEdge(0, 7, 8);
    g.addEdge(1, 2, 8);
    g.addEdge(1, 7, 11);
    g.addEdge(2, 3, 7);
    g.addEdge(2, 8, 2);
    g.addEdge(2, 5, 4);
    g.addEdge(3, 4, 9);
    g.addEdge(3, 5, 14);
    g.addEdge(4, 5, 10);
    g.addEdge(5, 6, 2);
    g.addEdge(6, 7, 1);
    g.addEdge(6, 8, 6);
    g.addEdge(7, 8, 7);

    //g.shortestPath(0);
*/
    // Number of elements for our minHeap
    int n = rand() % 100;
    int k = rand() % 5;

    vector <int> v = {4, 5, 6, 7, 8, 9, 10};

    // Declare a heap with space for n elements
    minHeap minHeap(n, k);


    // Insert N random numbers
    for (int i = 0; i < n; i++){
        cout << "Inserting element " << i << endl;
        minHeap.insert(rand() % 100);
        minHeap.printHeap();
        cout << endl;
    }


    // Print the heap
    minHeap.printHeap();
    cout << endl;

    // Verifies if it is indeed a heap
    cout << "Is it a heap? " << (minHeap.isMinHeap() ? "Yes" : "No") << std::endl;
    

    return 0;
}