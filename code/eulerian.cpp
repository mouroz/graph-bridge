#include "eulerian.hpp"

/*
@brief this method checks if there's a possibility of existence of an eulerian path in the graph
@params graph the graph to be analyzed
@return pair of (bool, int) where bool indicates if Eulerian path exists, and int is the starting vertex (or -1 if no path)
*/
pair<bool, int> canHaveEulerianPath(Graph graph) {
  int odd = 0;
  int start = -1;
  for (int i = 0; i < graph.getVertexQuantity(); i++) {
    if (graph.adj[i].size() % 2 != 0) {
      odd++;
      if (start == -1) {
        start = i;
      }
    }
  }
  if (odd == 0) {
    // Eulerian circuit, can start anywhere
    return {true, 0};
  } else if (odd == 2) {
    // Eulerian path, must start at one of the odd vertices
    return {true, start};
  }
  return {false, -1};
}


/*
@brief this method checks if an edge is a bridge
@params u the first vertex of the edge
@params v the second vertex of the edge
@params bridges the vector of bridges
@return true if the edge is a bridge, false otherwise
*/
bool isEdgeBridge(int u, int v, const EdgeVector &bridges) {
  for (const auto &bridge : bridges) {
    if ((bridge.first == u && bridge.second == v) ||
        (bridge.first == v && bridge.second == u)) {
      return true;
    }
  }
  return false;
}


/*
@brief Finds an Eulerian path in the graph using Fleury's algorithm
@param graph The graph to be analyzed
@return A list of vertices representing the Eulerian path, or an empty list if no path exists
*/
vector<int> findEulerianPath(Graph &graph) {
  vector<int> path;
  pair<bool, int> result = canHaveEulerianPath(graph);
  bool hasPath = result.first;
  int startVertex = result.second;
  
  if (!hasPath) {
    return path; // Return empty path if no Eulerian path exists
  }


  stack<int> stack;
  stack.push(startVertex);

  while (!stack.empty()) {
    int u = stack.top();

    if (graph.adj[u].size() == 0) {
      path.push_back(u);
      stack.pop();
    } else {
      EdgeVector bridges = tarjan(graph);

      int chosen = -1;

      for (int v : graph.adj[u]) {
        if (bridges.empty() || !isEdgeBridge(u, v, bridges)) {
          chosen = v;
          break;
        }
      }
      if (chosen == -1) {
        chosen = graph.adj[u].front();
      }

      stack.push(chosen);
      graph.removeEdge(u, chosen);
    }
  }

  return path;
}
