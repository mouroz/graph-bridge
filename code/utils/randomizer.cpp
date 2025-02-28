#include "randomizer.hpp"
#include "graph.hpp"

#include <algorithm>
#include <sstream>
#include <random>
#include <variant>
#include <cstring>
#include <unordered_set>
#include <vector>

namespace randomgraph {
  // Global random generator, seeded once
  static std::random_device rd;
  static std::mt19937 gen(rd());
  static std::uniform_real_distribution<float> randomBooleanDis(0.0f, 1.0f);

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
    return randomBooleanDis(gen) <= trueChance;
  }

  int randomInt(int min, int max) {
    std::uniform_int_distribution<int> dis(min, max);
    return dis(gen);
  }


  // Where graph is empty
  void makeGraphRandomlyConnected(Graph& graph) {
    int n = graph.adj.size();
    
    int *array = new int[n];
    for (int i = 0; i < n; i++) {
      array[i] = i;
    }
    
    // Search and get starting index
    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(array,array+n, gen);

    for (int i = 1; i < n; i++) {
        graph.addEdge(array[i - 1], array[i]);
    }
  }


  int __connectEmptyGraph(Graph& graph, std::unordered_set<std::pair<int, int>, pair_hash>& addedEdges) {
    int n = graph.adj.size();
    
    int *array = new int[n];
    for (int i = 0; i < n; i++) {
      array[i] = i;
    }
    
    std::shuffle(array,array+n, gen);

    for (int *it = array+1; it < array+n; it++) {
      int u = *(it-1);
      int v = *it;
      graph.addEdge(u, v);

      addedEdges.insert({u, v});
      addedEdges.insert({v, u});
    }

    // Return last element
    return array[n-1];
  
  }



  // LIMIT of 10MB edges (Each edge is stored on linked blocks of 12 so that would give 120MB max)
  const int EDGELIMIT = 10485760;
  const long BASETOMB = 1048576;
  Graph createConectedGraph(int n, int edges) {
    int extraEdges = edges - n - 1;
    
    if (extraEdges >= EDGELIMIT) {
      long valueInMB = (extraEdges/BASETOMB) * 12;
      std::cerr << "EXCEEDED MAXIMUM SIZE (120MB) ALLOWED FOR NORMAL USE, EXPECTED: " 
      << valueInMB << "[MB] ON END OF OPERATION" << std::endl;
      return {};
    }

    Graph graph(n);  
    std::unordered_set<std::pair<int, int>, pair_hash> edgeSet;
    edgeSet.reserve(edges);
    
    __connectEmptyGraph(graph, edgeSet);

    std::cout << extraEdges << edgeSet.size() << std::endl;

    std::uniform_int_distribution<int> dis(0, n-1);

    while (extraEdges > 0) {
      int u = dis(gen);
      int v = dis(gen);
      if (u != v && edgeSet.find({u, v}) == edgeSet.end()) {
        graph.addEdge(u, v);
        edgeSet.insert({u, v});
        edgeSet.insert({v, u});
        extraEdges--;
      }
    }

    return graph;
  }

  Graph createConectedGraph(int n, float edgePercentage) {
    // Calculate the total number of edges based on the percentage
    int edges = static_cast<int>((n * (n - 1)) / 2 * edgePercentage);  // Maximum possible edges in an undirected graph
    return createConectedGraph(n, edges);
  }


  Graph createEulerianGraph(int n, int edges) {
    int extraEdges = edges - n - 1;
   
    if (extraEdges >= EDGELIMIT) {
      long valueInMB = (extraEdges/BASETOMB) * 12;
      std::cerr << "EXCEEDED MAXIMUM SIZE (120MB) ALLOWED FOR NORMAL USE, EXPECTED: " 
      << valueInMB << "[MB] ON END OF OPERATION" << std::endl;
      return {};
    }

    Graph graph(n);  
    std::unordered_set<std::pair<int, int>, pair_hash> edgeSet;
    edgeSet.reserve(edges);
    
    int u = __connectEmptyGraph(graph, edgeSet);

    std::cout << extraEdges << edgeSet.size() << std::endl;

    std::uniform_int_distribution<int> dis(0, n-1);

    while (extraEdges > 0) {
      int v = dis(gen);
      if (u != v && edgeSet.find({u, v}) == edgeSet.end()) {
        graph.addEdge(u, v);
        edgeSet.insert({u, v});
        edgeSet.insert({v, u});
        extraEdges--;
        u = v;
      }
    }

    return graph;
  }

  Graph createEulerianGraph(int n, float edgePercentage) {
    // Calculate the total number of edges based on the percentage
    int edges = static_cast<int>((n * (n - 1)) / 2 * edgePercentage);  // Maximum possible edges in an undirected graph
    return createEulerianGraph(n, edges);
  }



// Does not get stuck waiting for valid random number, but runs on all possible edges of the graph
Graph bruteForceCreateConnected(int n, float edgeProbability) {
  Graph graph(n);

  // Add minimum requirement
  for (int i = 1; i < n; i++) {
    graph.addEdge(i - 1, i); 
  }


  for (int i = 0; i < n; i++) {
    for (int j = i + 2; j < n; j++) {
      if (randomgraph::randomBoolean(edgeProbability)) {
        graph.addEdge(i, j);
      }
    }
  }

  return graph;
}


}