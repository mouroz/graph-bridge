#include "graph.hpp"

/**
 * @brief Generates a random boolean value based on a given probability.
 *
 * This function returns `true` with a probability of `trueChance` and `false` otherwise.
 * The probability value should be between `0.0f` and `1.0f`, inclusive.
 *
 * @param trueChance The probability (between 0.0 and 1.0) of returning `true`.
 * @return `true` with a probability of `trueChance`, otherwise `false`.
 *
 * @note Uses `std::random_device` and `std::mt19937` for better randomness.
 * @warning If `trueChance` is outside the range [0.0, 1.0], behavior is undefined.
 */
bool randomBoolean(float trueChance) {
  // Use random_device and mt19937 for a better random number generator
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<float> dis(0.0f, 1.0f);

  // Generate a random float between 0 and 1
  float randVal = dis(gen);

  // Return true if randVal is less than or equal to trueChance
  return randVal <= trueChance;
}

//---- Métodos auxiliares (private) ----//
void Graph::createRandomGraphLog(const std::string& s) {
  std::cout << s;
}

void Graph::DFSUtil(int v, std::vector<bool> &visited) const {
  visited[v] = true;
  for (int neighbor : adj[v]) {
    if (!visited[neighbor])
      DFSUtil(neighbor, visited);
  }
}

//---- Métodos públicos ----//

Graph::Graph() : V(0) {
  adj.resize(V);
}

Graph::Graph(int v) : V(v), adj(v) {
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


Graph Graph::createRandomGraph(int n, float edgeProbability) { 
  Graph graph(n);

  std::ostringstream s;
  
  for (int i = 0; i < n; i++) {
      for (int j = i+1; j < n; j++) {
          if (randomBoolean(edgeProbability)) {
              graph.addEdge(i, j);
          }
      }
  }

  createRandomGraphLog(s.str());

  
  return graph;
}

/*ANTIGO MÉTODO PARA GERAR UM GRAFO ALEATÓRIO CONEXO*/

// Graph Graph::createConectedGraph(int n, float edgeProbability){
//   Graph graph;
//   do {
//     graph = createRandomGraph(n, edgeProbability);
//   } while (!graph.isConnected());
  
//   return graph;
// }

// int Graph::getEdgeQuantity(int v) const{
//   return adj[v].size();
// }

// int Graph::getVertexQuantity() const{
//   return V;
// }

Graph Graph::createConectedGraph(int n, float edgeProbability) {
  Graph graph(n);

  for (int i = 1; i < n; i++) {
      graph.addEdge(i - 1, i); 
  }

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<float> dis(0.0f, 1.0f);

  for (int i = 0; i < n; i++) {
      for (int j = i + 1; j < n; j++) {
          if (dis(gen) <= edgeProbability) {
              graph.addEdge(i, j);
          }
      }
  }

  return graph;
}

void Graph::show() {
  std::cout << "Resulting Graph: \n";
  for ( int i = 0; i < V; i++ ) {
      std::cout << i << " -> ";
      for ( int element : adj[i] ) {
          std::cout << element << " ";
      }
      std::cout << std::endl;
  }
  std::cout << std::endl;
}