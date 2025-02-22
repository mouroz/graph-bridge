#include "naive.hpp"
#include "graph.hpp"

#include <vector>
#include <iostream>
#include <chrono>

using namespace std;

/**
 * @brief Checks if the edge (u, v) is a bridge in the given graph.
 *
 * A bridge is an edge that, if removed, will increase the number of connected components in the graph.
 *
 * @param u The starting vertex of the edge.
 * @param v The ending vertex of the edge.
 * @param g The graph object where the edge is located.
 * @return true if the edge (u, v) is a bridge, false otherwise.
 */
bool isBridge(int u, int v, Graph &g)
{
    g.removeEdge(u, v);

    bool bridge = !(g.isConnected());

    g.addEdge(u, v);

    return bridge;
}

/**
 * @brief Identifies all bridges in the graph using a naive approach.
 *
 * This function iterates over all edges of the graph, removes each edge temporarily,
 * and checks if the graph becomes disconnected (i.e., the edge is a bridge).
 *
 * @param g The graph object to check for bridges.
 * @return A vector of pairs representing the bridges found in the graph.
 */
vector<pair<int, int>> executeNaive(Graph &g)
{
    vector<pair<int, int>> bridges; // Vector to store the bridges found in the graph
    vector<pair<int, int>> edges;   // Vector to store all edges before removing any

    // Collect all edges from the graph
    for (int u = 0; u < g.V; u++) { 
        for (int v : g.adj[u]) {    
            if (u < v) { // Avoid duplicate edges for undirected graph (e.g., (u,v) and (v,u))
                edges.push_back({u, v});
            }
        }
    }

    // Process each edge without modifying the list during iteration
    for (auto [u, v] : edges) {
        if (isBridge(u, v, g)) {
            bridges.push_back({u, v});
        }
    }

    return bridges;
}


int main(void)
{
  Graph graph = Graph::createConectedGraph(100000, 0.0001);
  graph.show();

  vector<pair<int, int>> bridges; 

  // calculating naive execution time
  chrono::steady_clock::time_point begin = chrono::steady_clock::now();
  bridges = executeNaive(graph);
  chrono::steady_clock::time_point end = chrono::steady_clock::now();

  if(!bridges.empty()){
    cout << "Bridges found:\n";
    for (auto bridge : bridges)
    {
      cout << "{ " << bridge.first << "-" << bridge.second << " }" << endl;
    }
  } else {
    cout << "Bridges not found!\n";
  }

  cout << "Naive execution time for " << graph.V << " vertices = " << chrono::duration_cast<chrono::milliseconds> (end - begin).count() << "[ms]" << endl;
}
