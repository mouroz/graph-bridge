#include "graph.hpp"
#include "graph_reader.hpp"
#include "tarjan.hpp"
#include "naive.hpp"
#include "eulerian.hpp"
#include "randomizer.hpp"

#include <functional>
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





int ioTest(Graph& graph) {
    const string filename = "examples/out1.txt";
    std::chrono::steady_clock::time_point begin, end; 

    // Dedicated buffer
    const size_t bufferSize = 64 * 1024;  // 64KB
    std::vector<char> ioBuffer(bufferSize);

    // Graph Writer
    std::filesystem::create_directory("examples");
    
    begin = std::chrono::steady_clock::now();
    Response<void> writeResult = graphformat::writeGraphToFile(filename, ioBuffer, graph);
    end = std::chrono::steady_clock::now();

    if (!writeResult.isOk()) {
        std::cerr << writeResult.message << std::endl;
        return -1;
    } 
    std::cout << "write time: " << getDuration(begin,end) << "[ms]" << std::endl;
    ioBuffer.clear();


    // Graph Reader
    begin = std::chrono::steady_clock::now();
    Response<Graph> result = graphformat::readGraphFromFile(filename, ioBuffer);
    end = std::chrono::steady_clock::now();

    if (!result.isOk()) {
        std::cerr << result.message << std::endl;
        return -1;
    }

    std::cout << "read time: " << getDuration(begin,end) << "[ms]" << std::endl;
    ioBuffer.clear();


    // Check results
    begin = std::chrono::steady_clock::now();
    if (graph.isEqual(result.value)) {
        std::cout << "WRITE/READ SUCCESFULL AND MATCHES" << std::endl;
    } else {
        std::cout << "WRITE/READ DOES NOT MATCH GENERATED GRAPH" << std::endl;
    }
    end = std::chrono::steady_clock::now();


    std::cout << "comparison time: " << getDuration(begin,end) << "[ms]" << std::endl;;
    return 0;
}


int testNaive(Graph& graph) {
    std::chrono::steady_clock::time_point begin, end;
    
    // calculating naive execution time
    begin = std::chrono::steady_clock::now();
    EdgeVector bridges = executeNaive(graph);
    end = std::chrono::steady_clock::now();


    if(!bridges.empty()){
        std::cout << "Bridges found:\n";
        for (auto bridge : bridges) {
            std::cout << "{ " << bridge.first << "-" << bridge.second << " }" << std::endl;
        }
    } else {
        std::cout << "Bridges not found!\n";
    }

    std::cout << "Naive execution time for " << graph.V << " vertices = " 
    <<  getDuration(begin, end) << "[ms]" << std::endl;

    return 0;
}

int testTarjan(Graph& graph) {
    std::chrono::steady_clock::time_point begin, end;
    EdgeVector bridges; 

    // calculating naive execution time
    begin = std::chrono::steady_clock::now();
    bridges = tarjan(graph);
    end = std::chrono::steady_clock::now();

    if(!bridges.empty()){
        std::cout << "Bridges found:\n";
        for (auto bridge : bridges){
            std::cout << "{ " << bridge.first << "-" << bridge.second << " }" << std::endl;
        }
    } else {
        std::cout << "Bridges not found!\n";
    }

    std::cout << "Tarjan execution time for " << graph.V << " vertices = " 
    << getDuration(begin, end) << "[ms]" << std::endl;

    return 0;
}

int testEulerian(Graph& graph) {
    std::chrono::steady_clock::time_point begin, end;

    // Check if can have eulerian path
    begin = std::chrono::steady_clock::now();
    pair<bool, int> result = canHaveEulerianPath(graph);
    end = std::chrono::steady_clock::now();
    
    std::cout << "Can have Eulerian path: " << (result.first ? "Yes" : "No") << endl;
    std::cout << getDuration(begin, end) << "[ms]" << std::endl;

    // Find bridges
    std::cout << "Bridges in the graph:" << std::endl;
    
    begin = std::chrono::steady_clock::now();
    EdgeVector bridges = tarjan(graph);
    end = std::chrono::steady_clock::now();
    
    std::cout << edgelist_to_string(bridges) << std::endl;
    std::cout << getDuration(begin, end) << "[ms]" << std::endl;

    // Get eulerian path
    std::cout << "Eulerian path:" << std::endl;
    begin = std::chrono::steady_clock::now();
    vector<int> path = findEulerianPath(graph);
    for (size_t i = 0; i < path.size(); i++) {
        std::cout << path[i];
        if (i < path.size() - 1) {
            std::cout << " -> ";
        }
    }
    end = std::chrono::steady_clock::now();
    std::cout << getDuration(begin, end) << "[ms]" << std::endl;

    return 0;
}





Response<Graph> generateEulerianGraph() {
    // Graph Writer
    std::chrono::steady_clock::time_point begin, end;
    
    begin = std::chrono::steady_clock::now();
    Response<Graph> res = randomgraph::createEulerianGraph(1000, 80000);
    end = std::chrono::steady_clock::now();
    
    std::cout << "Time for Graph Generation: " << getDuration(begin, end) << "[ms]" << std::endl;

    return res;
}

Response<Graph> generateConnectedGraph() {
    // Graph Writer
    std::chrono::steady_clock::time_point begin, end;
    
    begin = std::chrono::steady_clock::now();
    Response<Graph> res = randomgraph::createConectedGraph(100000, 0.001f);
    end = std::chrono::steady_clock::now();
    
    std::cout << "Time for Graph Generation: " << getDuration(begin, end) << "[ms]" << std::endl;

    return res;
}



Response<Graph> readGraph(const string& filename) {
    std::chrono::steady_clock::time_point begin, end;
    
    // Dedicated buffer
    const size_t bufferSize = 64 * 1024;  // 64KB
    std::vector<char> ioBuffer(bufferSize);

    begin = std::chrono::steady_clock::now();
    Response<Graph> result = graphformat::readGraphFromFile(filename, ioBuffer);
    end = std::chrono::steady_clock::now();

    std::cout << "Read time: " << getDuration(begin,end) << "[ms]" << std::endl;
    ioBuffer.clear();

    return result;
}

int main() {

    Response<Graph> graph = generateEulerianGraph();
    if (!graph.isOk()) {
        std::cerr << graph.message << std::endl;
        return -1;
    }

    auto res = canHaveEulerianPath(graph.value);
    if (res.first) {
        std::cout << "Is eulerian" << std::endl;
    }
   
    switch(2) {
        case 0:
            ioTest(graph.value);
            break;
        case 1:
            testNaive(graph.value);
            break;
        case 2:
            testTarjan(graph.value);
            break;
        case 3:
            testEulerian(graph.value);
            break;
        default:
            break;
        
    }
}




