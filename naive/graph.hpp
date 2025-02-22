#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <sstream>
#include <random>

class Graph {
  private:
    /// @brief Recursive DFS auxiliar function to verify the graph connectivity.
    void DFSUtil(int v, std::vector<bool> &visited) const;
    
    /// @brief custom random graph generator log behaviour
    static void createRandomGraphLog(const std::string& s);
  public:
    int V;                            // Vertex quantity
    std::vector<std::list<int>> adj;  // Adjascent list

    Graph();

    Graph(int V);

    /// @brief Inserts an directed edge from u to v
    void pushEdge(int u, int v);

    /// @brief Inserts an undirected edge between u and v
    void addEdge(int u, int v);

    /// @brief removes an undirected edge between u and v
    void removeEdge(int u, int v);

    /// @brief Checks the existence of an undirected edge between u and v
    bool hasEdge(int u, int v);

    /**
     * @brief Formats graph content to string text
     *
     * Every vertex is displayed on each line is increasing order
     * 
     * For each vertex I return:
     * "Vertex I: a1, a2, ... , an" Where a represents a connection from vertex I 
     * "Vertex I: (no connection)" When there are no connections from vertex I
     * 
     * @param graph The graph structure
     * @return string containing display text
     */
    std::string toString() const;

    /// @brief Verify if the graph is connected
    bool isConnected() const;


     /**
     * @brief Creates random graph
     *
     * For every possible edge for n vertices: 
     *   If add_rate hits on a random number generator, include edge on both vertices
     *   If it does not, ignore edge
     * 
     * @param n Number of vertices on graph
     * @param add_rate Chance of creating a edge
     * @return graph that contains all the generated edges
     * 
     * @warning add_rate will show undefined behaviour if its not in [0...1] range
     */
    static Graph createRandomGraph(int n, float edgeProbability);

    /// @brief Creates a random connected graph
    static Graph createConectedGraph(int n, float edgeProbability);

  
    int getVertexQuantity() const;

    int getEdgeQuantity(int v) const;

    /// @brief show out a graph
    void show();
};

#endif  // GRAPH_HPP