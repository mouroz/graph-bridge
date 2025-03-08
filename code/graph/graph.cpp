#include "graph.hpp"
#include "randomizer.hpp"

#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <sstream>
#include <random>
#include <fstream>
#include <stack>

std::string edgelist_to_string(EdgeVector list) {
    std::ostringstream s;

    for (size_t i = 0; i < list.size(); i++) {
        s << "{" << list[i].first << ", "  << list[i].second << "}" << std::endl;
    }

    return s.str();
}





///GRAFO

//---- Métodos auxiliares (private) ----//
void Graph::createRandomGraphLog(const std::string& s) {
  std::cout << s;
}


//---- Métodos públicos ----//

Graph::Graph() : V(0) {
  adj.resize(V);
}

Graph::Graph(int v) : V(v) {
  adj.resize(V);
}

bool Graph::hasEdge(int u, int v){
  bool test = false;
  for(int neigbour : adj[u]){
    if(neigbour == v){
      test = true;
    }
  }
  return test;
}

void Graph::pushEdge(int u, int v) {
  adj[u].push_back(v);
}

void Graph::addEdge(int u, int v) {
  adj[u].push_back(v);
  adj[v].push_back(u);
}

void Graph::removeEdge(int u, int v) {
  adj[u].remove(v);
  adj[v].remove(u);
}



bool Graph::isConnected() const {
  int start = 0;

  std::vector<bool> visited(V, false);
  
  // DFS iterativa para evitar estouro de pilha (stack overflow)
  std::stack<int> stack;
  stack.push(start);
  visited[start] = true;

  while (!stack.empty()) {
      int u = stack.top();
      stack.pop();

      for (int v : adj[u]) {
          if (!visited[v]) {
              visited[v] = true;
              stack.push(v);
          }
      }
  }

  // Verificar se todos os vértices foram visitados
  for (int i = 0; i < V; ++i) {
      if (!visited[i]) return false;
  }
  
  return true;
}




int Graph::getEdgeQuantity(int v) const{
  return adj[v].size();
}

int Graph::getVertexQuantity() const{
  return V;
}




/// @section Displays
std::string Graph::toString() const {
  std::ostringstream s;
  for (int i = 0; i < V; ++i) {
    s << "Vertex " << i << ": ";
    if(adj[i].empty()){
      s << "(no connections)";
    }
    for (int neighbor : adj[i]) {
      s << neighbor << (neighbor != adj[i].back() ? ", " : "");
    }
    s << std::endl;
  }
  return s.str();
}

std::string Graph::toStringBrief() const {
    std::ostringstream oss;
    oss << "Resulting Graph:\n";
    for (int i = 0; i < V; i++) {
        oss << i << " -> ";
        for (int element : adj[i]) {
            oss << element << " ";
        }
        oss << "\n";
    }
    return oss.str();
}

void Graph::show() {
  std::cout << toStringBrief();
}

bool Graph::isEqual(const Graph& graph) {
  if(adj.size() != graph.adj.size()) return false;
  
  return std::equal(adj.begin(), adj.end(), graph.adj.begin());
  
}

int Graph::getTotalQuantityEdges() const {
  int total = 0;
  for (int i = 0; i < V; i++) {
    total += adj[i].size();
  }
  return total/2;
}







