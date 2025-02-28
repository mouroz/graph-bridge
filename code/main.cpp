#include "graph.hpp"
#include "graph_reader.hpp"
#include "tarjan.hpp"
#include "naive.hpp"
#include "eulerian.hpp"
#include "randomizer.hpp"

#include <vector>
#include <iostream>
#include <chrono>
#include <string>
#include <filesystem>


using namespace std;

void outputGraph(const Graph& graph, const std::string& str);



long getDuration(std::chrono::steady_clock::time_point begin, std::chrono::steady_clock::time_point end) {
    return std::chrono::duration_cast<std::chrono::milliseconds> (end - begin).count();
}


int ioTest() {
    const string filename = "examples/out1.txt";
    std::chrono::steady_clock::time_point begin, end; 

    // Random generator
    begin = std::chrono::steady_clock::now();
    Graph graph = randomgraph::createConectedGraph(100000, 0.001f);
    end = std::chrono::steady_clock::now();

    std::cout << "generation time: " << getDuration(begin,end) << "[ms]" << std::endl;
    
    
    // Graph Writer
    std::filesystem::create_directory("examples");
   
    size_t bufferSize = 64 * 1024;  // 64KB
    std::vector<char> ioBuffer(bufferSize);

    TimedResponse<void> writeResult = graphformat::writeGraph(filename, ioBuffer, graph);
    if (!writeResult.isOk()) {
        std::cerr << writeResult.error << std::endl;
        return -1;
    } 
    std::cout << "write time: " << writeResult.duration << "[ms]" << std::endl;
    ioBuffer.clear();
    
    TimedResponse<Graph> result = graphformat::readGraph(filename, ioBuffer);
    if (!result.isOk()) {
        std::cerr << writeResult.error << std::endl;
        return -1;
    }
    std::cout << "read time: " << result.duration << "[ms]" << std::endl;
    ioBuffer.clear();


    // Check results
    begin = std::chrono::steady_clock::now();
    
    if (graph.isEqual(result.value)) std::cout << "WRITE/READ SUCCESFULL AND MATCHES" << std::endl;
    else std::cout << "WRITE/READ DOES NOT MATCH GENERATED GRAPH" << std::endl;
    
    end = std::chrono::steady_clock::now();
    std::cout << "comparison time: " << getDuration(begin,end) << "[ms]" << std::endl;;
    
    return 0;
}


int testNaive() {
    Graph graph = randomgraph::createConectedGraph(100000, 0.001f);
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
    Graph graph = randomgraph::createConectedGraph(100000, 0.001f);
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

    Graph graph = graphtext::read_graph_from_file(file);
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

int randomGenTest() {
    Graph graph = randomgraph::createEulerianGraph(6, 10);
    
    graph.show();

    return 0;
}

int main() {
    // Graph Writer

    
    return randomGenTest();
    //return ioTest();
    // return testNaive();
    //return testTarjan();
}




