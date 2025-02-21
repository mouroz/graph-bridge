#ifndef TARJAN_GRAPH_H
#define TARJAN_GRAPH_H

#include <utility>
#include <vector>
#include <string>
#include <string>


typedef std::vector<std::vector<int>> AdjGraph;
typedef std::vector<std::pair<int,int>> EdgeVector;

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
bool randomBoolean(float trueChance);


/**
 * @brief Formats graph content to string text
 *
 * Every vertex from 0...n-1 is displayed on each line is increasing order
 * 
 * For each vertex I return:
 * "Vertex I: a1, a2, ... , an" Where a represents a connection from vertex I 
 * "Vertex I: (no connection)" When there are no connections from vertex I
 * 
 * @param graph The graph structure
 * @return string containing display text
 */
std::string adjgraph_to_string(AdjGraph graph);

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




/// @brief custom random graph generator log behaviour
void create_random_graph_log(const std::string& s);

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
AdjGraph create_random_graph(int n, float add_rate);



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
        const AdjGraph *graph;
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
        
        TarjanData(const AdjGraph &graph);
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