#include "graph.hpp"
#include "graph_reader.hpp"
#include "tarjan.hpp"
#include "naive.hpp"
#include "eulerian.hpp"

#include <vector>
#include <iostream>
#include <chrono>
#include <string>



using namespace std;

void outputGraph(const Graph& graph, const std::string& str);

int ioTest() {

    std::ifstream file("examples/out1.txt");

    if (!file) {
        std::cerr << "Error opening file!" << std::endl;
        return 1;
    }

    Graph graph = read_graph_from_file(file);
    graph.show();

    outputGraph(graph, "examples/out1.txt");
    
    return 0;
}


int testNaive() {
    Graph graph = Graph::createConectedGraph(1000, 0.0001);
    graph.show();

    EdgeVector bridges; 

    // calculating naive execution time
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    bridges = executeNaive(graph);

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

    if(!bridges.empty()){
    std::cout << "Bridges found:\n";
    for (auto bridge : bridges)
    {
        std::cout << "{ " << bridge.first << "-" << bridge.second << " }" << std::endl;
    }
    } else {
    std::cout << "Bridges not found!\n";
    }

    std::cout << "Naive execution time for " << graph.V << " vertices = " 
    << std::chrono::duration_cast<std::chrono::milliseconds> (end - begin).count() << "[ms]" << std::endl;

    return 0;
}

int testTarjan() {
    Graph graph = Graph::createConectedGraph(5, 1);
    graph.show();

    EdgeVector bridges; 

    // calculating naive execution time
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    bridges = tarjan(graph);

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

    if(!bridges.empty()){
    std::cout << "Bridges found:\n";
    for (auto bridge : bridges)
    {
        std::cout << "{ " << bridge.first << "-" << bridge.second << " }" << std::endl;
    }
    } else {
    std::cout << "Bridges not found!\n";
    }

    std::cout << "Tarjan execution time for " << graph.V << " vertices = " 
    << std::chrono::duration_cast<std::chrono::milliseconds> (end - begin).count() << "[ms]" << std::endl;

    return 0;
}

int testEulerian() {
    ifstream file("eulerianPath/test_graph.txt");
    if (!file) {
    cerr << "Error opening eulerianPath/test_graph.txt!" << endl;
    return 1;
    }

    Graph graph = read_graph_from_file(file);
    cout << "Graph structure:" << endl;
    cout << graph.toString() << endl;

    pair<bool, int> result = canHaveEulerianPath(graph);
    cout << "Can have Eulerian path: " << (result.first ? "Yes" : "No") << endl;

    cout << "Bridges in the graph:" << endl;
    EdgeVector bridges = tarjan(graph);
    cout << edgelist_to_string(bridges) << endl;

    cout << "Eulerian path:" << endl;
    vector<int> path = findEulerianPath(graph);
    for (size_t i = 0; i < path.size(); i++) {
    cout << path[i];
    if (i < path.size() - 1) {
        cout << " -> ";
    }
    }
    cout << endl;

    return 0;
}

int main() {
    //return ioTest();
    // return testNaive();
    return testTarjan();
}



void outputGraph(const Graph& graph, const std::string& str) {
    std::ofstream file(str);
    
    
    if (!file) {
        std::cerr << "Error creating / getting access to output file!" << std::endl;
    }

    graph.writeToFile(file);
}
