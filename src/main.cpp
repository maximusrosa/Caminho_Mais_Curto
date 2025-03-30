#include <iostream>
#include <vector>
#include <limits>
#include <fstream>
#include "../include/graph.hpp"

using namespace std;


int main() {
    string filename = "USA-road-d.NY.gr";

    ifstream file("../files/" + filename);
    if (!file) {
        cerr << "Error opening file\n";
        return 1;
    }
    Graph g(file);


    //Graph g(30, 0.9, 100); // 10 vertices, 50% chance of edge creation, max weight 100

    cout << "Graph created successfully\n";
    //graph.printGraph();

    int startVertex = 0; 
    int k = 3; // Number of children for MinHeap
    g.shortestPath(startVertex, k);

    return 0;
}
