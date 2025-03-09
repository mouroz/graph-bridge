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

void outputGraph(const Graph& graph, const std::string& str);



long getDuration(std::chrono::steady_clock::time_point begin, std::chrono::steady_clock::time_point end) {
    return std::chrono::duration_cast<std::chrono::milliseconds> (end - begin).count();
}

long getDurationInNano(std::chrono::steady_clock::time_point begin, std::chrono::steady_clock::time_point end) {
    return std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count();
}


int _bridgeTimes(Graph& graph, std::ofstream& out) {
    std::chrono::steady_clock::time_point begin, end; 


    std::vector<long> times;
    times.reserve(5);
    
    out << std::endl << "## TARJAN" << std::endl;
    for (int i = 0; i < 4; i++) {
        EdgeVector bridges; 
        begin = std::chrono::steady_clock::now();
        bridges = tarjan(graph);
        end = std::chrono::steady_clock::now();

        long duration = getDurationInNano(begin,end);
        out << "[" << i << "]: " << duration << "[ns]" << " -> ";
        times.push_back(duration);
        
        if (bridges.empty()) {
            out << "No bridges found" << std::endl;
        
        } else {
            out << bridges.size() << " bridges found: [";
            for (long unsigned j = 0; j < bridges.size()-1; j++){
                auto bridge = bridges[j];
                out << "{ " << bridge.first << "-" << bridge.second << " }, ";
            }
            auto bridge = bridges[bridges.size()-1];
            out << "{ " << bridge.first << "-" << bridge.second << " }]" << std::endl;
        }
    }
    long average = std::accumulate(times.begin(), times.end(), 0L) / times.size();
    out << "Average time: " << average << "[ns]" << std::endl;
    

    times.clear();
    out << std::endl << "## NAIVE" << std::endl;
    for (int i = 0; i < 4; i++) {
        EdgeVector bridges; 
        begin = std::chrono::steady_clock::now();
        bridges = executeNaive(graph);
        end = std::chrono::steady_clock::now();

        long duration = getDurationInNano(begin,end);
        out << "naive [" << i << "]: " << duration << "[ns]" << " -> ";
        times.push_back(duration);
        
        if (bridges.empty()) {
            out << "No bridges found" << std::endl;
        
        } else {
            out << bridges.size() << " bridges found: [";
            for (long unsigned j = 0; j < bridges.size()-1; j++){
                auto bridge = bridges[j];
                out << "{ " << bridge.first << "-" << bridge.second << " }, ";
            }
            auto bridge = bridges[bridges.size()-1];
            out << "{ " << bridge.first << "-" << bridge.second << " }]" << std::endl;
        }
    }

    return 0;
}

int _readGraph(const std::string& filename, std::vector<char> ioBuffer, std::ofstream& out) {
    std::chrono::steady_clock::time_point begin, end; 

    begin = std::chrono::steady_clock::now();
    Response<Graph> result = graphformat::readGraphFromFile(filename, ioBuffer);
    end = std::chrono::steady_clock::now();

    if (!result.isOk()) {
        std::cerr << result.message << std::endl;
        return -1;
    }

    Graph graph = std::move(result.value);

    out << "# " << filename << std::endl;
    out << "read time: " << getDurationInNano(begin,end) << "[ns]" << std::endl;
    ioBuffer.clear();

    return _bridgeTimes(graph, out);
}

int _readGraphs(const std::string& filenameContext, std::vector<char> ioBuffer, std::ofstream& out) {
    for (int i = 0; i < 10; i++) {
        std::string filename = filenameContext + std::to_string(i) + ".graph";
        int res =_readGraph(filename, ioBuffer, out);
        if (res == -1) return res;

        out << std::endl << std::endl;
        std::cout << "DONE" << std::endl;
    }

    return 0;
}

int execute() {
    const std::string outputFilename = "bridgesResult1000";



    std::ofstream out(outputFilename, std::ios::binary | std::ios::trunc);
    if (!out) {
        std::cerr << "Error: Could not open output file for writing" << std::endl;
    }

    const size_t bufferSize = 64 * 1024;  // 64KB
    std::vector<char> ioBuffer(bufferSize);
    std::string filenameContext = "examples/1000/";


    _readGraphs(filenameContext, ioBuffer, out);
}