#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <sstream>
#include <random>

typedef std::vector<std::pair<int,int>> EdgeVector;
/**
 * @brief Formats EdgeList content to string text
 *
 * Every edge from 0...n-1 is displayed on each line is stored order
 * 
 * For each edge I return:
 * "{u, v}" where u is first and v is second element on the pair of vertices
 * 
 * @param list The EdgeList structure
 * @return string containing display text
 */
std::string edgelist_to_string(EdgeVector list);




class Graph {
  private:
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
    std::string toStringBrief() const;
    

    /// @brief Verify if the graph is connected
    bool isConnected() const;

    int getVertexQuantity() const;

    int getEdgeQuantity(int v) const;

    //static Graph read_graph_from_file();

    void show();

    bool isEqual(const Graph& graph);

    int getTotalQuantityEdges() const;

    /// @brief Creates a deep copy of the given graph
    static Graph clone(const Graph& original);
};




#endif  // GRAPH_HPP