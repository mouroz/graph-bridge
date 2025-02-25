#include "tarjan.hpp"
#include "graph_reader.hpp"
#include "graph.hpp"

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


EdgeVector tarjan(const Graph &graph) {
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




