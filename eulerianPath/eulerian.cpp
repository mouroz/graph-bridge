#include "eulerian.hpp"

/*
@brief this method runs the graph and returns if there's a possibility of existence of an eulerian path

@params graph the graph to be analyzed
@return true if there's a possibility of existence of an eulerian path, false otherwise
*/
bool canHaveEulerianPath(Graph graph) {
  int odd = 0;
  for (int i = 0; i < graph.getVertexQuantity(); i++) {
    if (graph.adj[i].size() % 2 != 0) {
      odd++;
    }
  }
  return odd == 0 || odd == 2;
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
  if (!canHaveEulerianPath(graph)) {
    return path; // Return empty path if no Eulerian path exists
  }

  // Find a starting vertex (vertex with odd degree if exists)
  int start = 0;
  int i = 0;
  while (i < graph.getVertexQuantity() && start == 0) {
    if (graph.adj[i].size() % 2 != 0) {
      start = i;
    }
    i++;
  }

  stack<int> stack;
  stack.push(start);

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

int main(void) {
  ifstream file("eulerianPath/test_graph.txt");
  if (!file) {
    cerr << "Error opening eulerianPath/test_graph.txt!" << endl;
    return 1;
  }

  Graph graph = read_graph_from_file(file);
  cout << "Graph structure:" << endl;
  cout << graph.toString() << endl;

  cout << "Can have Eulerian path: " << (canHaveEulerianPath(graph) ? "Yes" : "No") << endl;

  cout << "Bridges in the graph:" << endl;
  EdgeVector bridges = tarjan(graph);
  cout << edgelist_to_string(bridges) << endl;

  cout << "Eulerian path:" << endl;
  vector<int> path = findEulerianPath(graph);
  for (size_t i = 0; i < path.size(); i++) {
    cout << path[i];
    if (i < path.size() - 1) {
      cout << " -> ";
    }
  }
  cout << endl;

  return 0;
}