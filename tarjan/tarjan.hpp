#ifndef TARJAN_GRAPH_H
#define TARJAN_GRAPH_H

#include <utility>
#include <vector>
#include <string>
#include <string>
#include "graph.cpp"

typedef std::vector<std::vector<int>> AdjGraph;
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



#include <vector>

/**
 * @brief Holds data used by Tarjan's algorithm to find bridges in a graph.
 *
 * The struct is used to reduce the excessive number of arguments and for
 * other optimizations
 * 
 * Stores the information per vertices to run Tarjan's algorithm for finding bridges
 * 
 * # Global information:
 * - time (keep track of current timestamp)
 * - graph (constant reference of the analyzed graph) 
 * - bridges (list of bridges to be returned)
 * 
 * The graph received must map vertexes as integers that range from 0 to n-1, otherwise
 * they cannot be properly used as indexes on information per vertex arrays
 * 
 * 
 * # Information per vertex: 
 * 
 * A single array is used to improve locality, and each subarray contains n elements (each vertex)
 * - timestamps (tin) [0...n-1]
 * - visited states (true if visited, false if not visited) [n...2n-1]
 * - low-link values (lowest indirect link found) [2n...3n-1]
 * 
 * Pointers for each subarray is stored for easier use, each allowed to only access indexes [0...n-1]
 */
struct TarjanData {
    private: 
        int *data;
    
    public: 
        const Graph *graph;
        EdgeVector bridges;

        int time = 0;
        int *visited, *tin, *low;


        /**
         * @brief Constructs a TarjanData object for the provided graph.
         * 
         * Initializes and allocated the raw array, and define start of each pointer
         * Stores graph reference and initializes bridges list
         * 
         * @param graph The graph on which Tarjan's algorithm will be applied.
         */
        
        TarjanData(const Graph &graph);
        ~TarjanData();
};


/**
 * @brief Recursive suboperation used on tarjan algorithm
 *
 * Uses breadth-first-search logic to update vertexes timestamps, and
 * check if it has neighbors that are connected to vertexes of lower timestamps (V) 
 * than its parent, which would constitute a loop as V is also connected to the parent
 * due to having connection to lower timestamps on the search
 * 
 * Global arguments are carried and results are loaded on custom struct for the operation
 * 
 * @param argsPtr Custom arguments struct carried as pointer
 * @param v Current vertex
 * @param parent Vertex that initiated this recursion on its "child". First vertex receives parent as itself
 */
void _dfs(TarjanData *argsPtr, int v, int parent);


/**
 * @brief Performs execution of tarjan algorithm and return list of bridges
 *
 * Executes tarjan with custom _bfs recursive operations
 *
 * @param graph Graph that the algorithm will execute on
 * @return EdgeVector containing all found bridges
 * 
 * @warning The graph must have vertexes identified as integer numbers on range [0 ... n-1]
 */
EdgeVector tarjan(const AdjGraph &graph);


#endif