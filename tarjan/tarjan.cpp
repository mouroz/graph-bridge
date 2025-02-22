#include "tarjan.hpp"
#include "graph_reader.cpp"

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <utility>
#include <cstring> 
#include <vector>
#include <list>
#include <random>
#include <sstream>
#include <fstream>
#include <string>


typedef std::vector<std::pair<int,int>> EdgeVector;


std::string edgelist_to_string(EdgeVector list) {
    std::ostringstream s;

    for (int i = 0; i < list.size(); i++) {
        s << "{" << list[i].first << ", "  << list[i].second << "}" << std::endl;
    }

    return s.str();
}


TarjanData::TarjanData(const Graph& graph) {
    int n = graph.getVertexQuantity();
    data = new int[n * 3];
    memset(data, 0, n * 3 * sizeof(int));

    visited = data;
    tin = visited + n;
    low = tin + n;

    this->graph = &graph;
    bridges = EdgeVector();
}

TarjanData::~TarjanData() {
    delete[] data;
}


EdgeVector tarjan(Graph &graph) {
    TarjanData args(graph);

    _dfs(&args, 0, 0);

    return args.bridges;
}



void _dfs(TarjanData *argsPtr, int v, int parent) {
    TarjanData &args = *argsPtr;

    args.visited[v] = true;
    args.tin[v] = args.low[v] = args.time++; 

    for (int u : args.graph->adj[v]) {
        if (u == parent) continue;

        if (args.visited[u]) {
            // If *u* was already visited; 
            // tin[u] always < tin[v]
            // Ensure lowest value among other neighbors with min
            args.low[v] = std::min(args.low[v], args.tin[u]);
        
        } else {
            _dfs(argsPtr, u, v);

            //After fully exploring, *u* may have found a previous vertex
            args.low[v] = std::min(args.low[v], args.low[u]);

            if (args.low[u] > args.tin[v]) {
                args.bridges.push_back({v, u});
            }
        }
    }

    *argsPtr = std::move(args); 
}




/*int main() {
    // Graph graph = Graph::createConectedGraph(10, 0.3);
    // std::cout << graph.toString();

    std::ifstream file("ex1.txt");

    if (!file) {
        std::cerr << "Error opening file!" << std::endl;
        return 1;
    }

    Graph graph = read_graph_from_file(file);
    std::cout << graph.toString();


    // AdjGraph graph = create_random_graph(10, 0.3);
    // std::cout << adjgraph_to_string(graph);

    // TarjanData data(10);
    // data.visited[0] = 1;
    // data.tin[0] = 1;
    // data.low[9] = 1;
    // for (int i = 0; i < 30; i++) {
    //     std::cout << data.visited[i];
    // }
    

    EdgeVector bridges = tarjan(graph);
    std::cout << edgelist_to_string(bridges);

}*/
