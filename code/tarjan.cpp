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
#include <stack>




TarjanData::TarjanData(const Graph& graph) {
    int n = graph.getVertexQuantity();
    data = new int[n * 2];
    memset(data, -1, n * 2 * sizeof(int));

    tin = data;
    low = tin + n;

    this->graph = &graph;
    bridges = EdgeVector();
}

TarjanData::~TarjanData() {
    delete[] data;
}

EdgeVector tarjan(const Graph &graph) {
    TarjanData args(graph);
    //iteration in all vertex to make sure every vertex will be visited, even if the graph isn't connected
    /*for (int i = 0; i < graph.getVertexQuantity(); ++i) {
        if (!wasVisited(i, &args)) {
           
        }
    }*/
    // _dfs(&args, 0, -1);
    _dfs(&args, 0);
    return args.bridges;
}

bool wasVisited(int vertex, TarjanData* args){
    return args->tin[vertex] != -1;
}



// void _dfs(TarjanData *argsPtr, int v, int parent) {
//     TarjanData &args = *argsPtr;

//     args.visited[v] = true;
//     args.tin[v] = args.low[v] = args.time++; 

//     for (int u : args.graph->adj[v]) {
//         if (u == parent) continue;

//         if (args.visited[u]) {
//             // If *u* was already visited; 
//             // tin[u] always < tin[v]
//             // Ensure lowest value among other neighbors with min
//             args.low[v] = std::min(args.low[v], args.tin[u]);
        
//         } else {
//             _dfs(argsPtr, u, v);

//             //After fully exploring, *u* may have found a previous vertex
//             args.low[v] = std::min(args.low[v], args.low[u]);

//             if (args.low[u] > args.tin[v]) {
//                 args.bridges.push_back({v, u});
//             }
//         }
//     }

//     *argsPtr = std::move(args); 
// }

void _dfs(TarjanData *argsPtr, int start) {
    TarjanData &args = *argsPtr;
    std::stack<std::pair<int, int>> stack;
    
    // stack.push({start, -1});
    //first iteration to remove a comparison from the while
    args.tin[start] = args.low[start] = args.time++;
    for (int u : args.graph->adj[start]) {
        if (!wasVisited(u, &args)) {
            stack.push({u, start});
        }
    }

    while (!stack.empty()) {
        auto [v, parent] = stack.top();
        
        if (!wasVisited(v, &args)) {
            args.tin[v] = args.low[v] = args.time++;
            for (int u : args.graph->adj[v]) {
                if (u == parent){ 
                    continue;
                } else if (wasVisited(u, &args)) {
                    args.low[v] = std::min(args.low[v], args.tin[u]);
                } else {
                    stack.push({u, v});
                }
            }
        } else {
            stack.pop();
            args.low[parent] = std::min(args.low[v], args.low[parent]);
            if (args.low[v] > args.tin[parent]) {
                args.bridges.push_back({parent, v});
            }
        }
    }
}




