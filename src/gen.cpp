/**
 * \file gen.cpp
 *   \author Marcus Ritt <mrpritt@inf.ufrgs.br>
 *   \date Time-stamp: <2024-07-23 13:03:31 ritt>
 * 
 *  Refactored by Maximus Rosa <mbrosa@inf.ufrgs.br>
 */

 #include <cassert>
 #include <iostream>
 #include <vector>
 #include <cstdlib>
 #include <ctime>
 #include <boost/graph/adjacency_list.hpp>
 #include <boost/graph/dijkstra_shortest_paths.hpp>
 using namespace boost;
 using namespace std;
 
 struct VertexInformation {
     unsigned component;
 };
 
 struct EdgeInformation {
     unsigned weight;
 };
 
 const unsigned maxweight = 1000;
 typedef adjacency_list<vecS, vecS, directedS, VertexInformation, EdgeInformation> Graph;
 typedef graph_traits<Graph>::vertex_descriptor Node;
 typedef graph_traits<Graph>::edge_descriptor Edge;
 
 graph_traits<Graph>::vertices_size_type get_random_vertex(const Graph& g) {
     return lrand48() % num_vertices(g);
 }
 
 void generate_random_graph(Graph& g, unsigned n, double p) {
     for (unsigned i = 0; i < n; i++) {
         add_vertex(g);
     }
     
     for (unsigned i = 0; i < n; i++) {
         for (unsigned j = 0; j < n; j++) {
             if (i != j && drand48() < p) {
                 Edge e = add_edge(i, j, g).first;
                 g[e].weight = lrand48() % maxweight;
             }
         }
     }
 }
 
 void compute_shortest_path(const Graph& g, unsigned src, unsigned dst) {
     vector<unsigned> dist(num_vertices(g));
     vector<unsigned> pred(num_vertices(g));
     dijkstra_shortest_paths(
         g, src,
         weight_map(get(&EdgeInformation::weight, g))
             .distance_map(&dist[0])
             .predecessor_map(&pred[0])
     );
     cerr << "Distance between " << src + 1 << " and " << dst + 1 << " is " << dist[dst] << endl;
 }
 
 void print_graph_dimacs(const Graph& g) {
     cout << "p sp " << num_vertices(g) << " " << num_edges(g) << endl;
     for (auto [eb, ee] = edges(g); eb != ee; eb++) {
         cout << "a " << source(*eb, g) + 1 << " " << target(*eb, g) + 1 << " " << g[*eb].weight << endl;
     }
 }
 
 int main(int argc, char *argv[]) {
     assert(argc == 3);
     unsigned n = atoi(argv[1]);
     double p = atof(argv[2]);
     
     srand48(time(0));
 
     Graph g;
     generate_random_graph(g, n, p);
     
     unsigned src = get_random_vertex(g);
     unsigned dst = get_random_vertex(g);
     compute_shortest_path(g, src, dst);
     
     print_graph_dimacs(g);
     
     return 0;
 }
 
