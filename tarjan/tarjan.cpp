#include "tarjan.hpp"
#include "graph_reader.hpp"

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


typedef std::vector<std::vector<int>> AdjGraph;
typedef std::vector<std::pair<int,int>> EdgeVector;







bool randomBoolean(float trueChance) {
    // Use random_device and mt19937 for a better random number generator
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0.0f, 1.0f);

    // Generate a random float between 0 and 1
    float randVal = dis(gen);

    // Return true if randVal is less than or equal to trueChance
    return randVal <= trueChance;
}


std::string adjgraph_to_string(AdjGraph graph) {
    
    std::ostringstream s;

    for (int i = 0; i < graph.size(); i++) {
        s << "Vertex " << i << ": ";

        if (graph[i].empty()) {  // Check if the list is empty
            s << "(no connections)" << std::endl;
            continue;
        }

        int end = graph[i].size()-1;
        for (int j = 0; j < end; j++) {
            s << graph[i][j] << ", ";
        }
        s << graph[i][end] << std::endl;
    }

    return s.str();
}


std::string edgelist_to_string(EdgeVector list) {
    std::ostringstream s;

    for (int i = 0; i < list.size(); i++) {
        s << "{" << list[i].first << ", "  << list[i].second << "}" << std::endl;
    }

    return s.str();
}


void create_random_graph_log(const std::string& s) {
    std::cout << s;
}

AdjGraph create_random_graph(int n, float add_rate) { 
    AdjGraph graph(n);

    std::ostringstream s;
    

    // explore all edge combinations
    for (int i = 0; i < n; i++) {
        for (int j = i+1; j < n; j++) {
            if (randomBoolean(add_rate)) {
                graph[i].push_back(j);
                graph[j].push_back(i);
                
                s << "{" << i << " " << j << "}" << std::endl;
            }
        }
    }

    create_random_graph_log(s.str());
    
    
    return graph;
}




TarjanData::TarjanData(const AdjGraph& graph) {
    int n = graph.size();
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



EdgeVector tarjan(AdjGraph &graph) {
    TarjanData args(graph);

    _dfs(&args, 0, 0);

    return args.bridges;
}



void _dfs(TarjanData *argsPtr, int v, int parent) {
    TarjanData &args = *argsPtr;

    args.visited[v] = true;
    args.tin[v] = args.low[v] = args.time++; 

    for (int u : args.graph->at(v)) {
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




int main() {
    std::ifstream file("ex1.txt");

    if (!file) {
        std::cerr << "Error opening file!" << std::endl;
        return 1;
    }

    AdjGraph graph = read_graph_from_file(file);
    std::cout << adjgraph_to_string(graph);


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

}
