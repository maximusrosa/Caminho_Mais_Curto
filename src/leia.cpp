// Leitura (exemplo)
void read_dimacs(std::istream& in, unsigned& n, unsigned& m, MyGraph& a) {
  std::string line="", dummy;
  while (line.substr(0,4) != "p sp")
    getline(in,line);
 
  // (1) get nodes and edges
  std::stringstream linestr;
  linestr.str(line);
  linestr >> dummy >> dummy >> n >> m;
  a.resize(n);
  unsigned i=0;
  while (i++ < m) {
    getline(in,line);
    if (line.substr(0,2) == "a ") {
      std::stringstream arc(line);
      unsigned u,v,w;
      char ac;
      arc >> ac >> u >> v >> w;
      // processa arco (u,v) com peso w
    }
  }
}
