
#include "graph.hpp"
#include "graph_reader.hpp"
#include "tarjan.hpp"
#include "naive.hpp"
#include "eulerian.hpp"
#include "randomizer.hpp"
#include "test.hpp"
#include "main.hpp"

#include <functional>
#include <vector>
#include <iostream>
#include <chrono>
#include <string>
#include <filesystem>


namespace test {


    void outputGraph(const Graph& graph, const std::string& str);



    long getDuration(std::chrono::steady_clock::time_point begin, std::chrono::steady_clock::time_point end) {
        return std::chrono::duration_cast<std::chrono::milliseconds> (end - begin).count();
    }

    long getDurationInNano(std::chrono::steady_clock::time_point begin, std::chrono::steady_clock::time_point end) {
        return std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count();
    }
 
    long getDurationInMicro(std::chrono::steady_clock::time_point begin, std::chrono::steady_clock::time_point end) {
        return std::chrono::duration_cast<std::chrono::microseconds> (end - begin).count();
    }
 

    
    void displayTimeOnIndexAndRes(int i, unsigned long time, std::ostream& out) {
        out << "[" << i << "]: " << time << "[ns]" << " -> ";
    }




    void displayBridges(EdgeVector edges, std::ostream& out) {
        if (edges.empty()) {
            out << "No bridges found" << std::endl;
        
        } else {
            out << edges.size() << " bridges found: [";
            for (long unsigned j = 0; j < edges.size()-1; j++){
                auto bridge = edges[j];
                out << "{ " << bridge.first << "-" << bridge.second << " }, ";
            }
            auto bridge = edges[edges.size()-1];
            out << "{ " << bridge.first << "-" << bridge.second << " }]" << std::endl;
        }
    }




    int _bridgeTimes(Graph& graph, std::ostream& out) {
        std::chrono::steady_clock::time_point begin, end; 


        std::vector<long> times;
        times.reserve(5);
        
        out << std::endl << "## TARJAN" << std::endl;

        out << "{";
        for (int i = 0; i < 10; i++) {
            begin = std::chrono::steady_clock::now();
            EdgeVector bridges = tarjan(graph);
            end = std::chrono::steady_clock::now();

            long duration = getDurationInMicro(begin,end);
            times.push_back(duration);
            out << duration << ", ";
            //displayBridges(bridges, out);
           
        }
        out << "}" << std::endl;

        long average = std::accumulate(times.begin(), times.end(), 0L) / times.size();
        out << "Average time: " << average << "[micro]" << std::endl;
        

        times.clear();
        out << std::endl << "## NAIVE" << std::endl;

        out << "{";
        for (int i = 0; i < 10; i++) {
            begin = std::chrono::steady_clock::now();
            EdgeVector bridges = executeNaive(graph);
            end = std::chrono::steady_clock::now();

            long duration = getDurationInMicro(begin,end);
            times.push_back(duration);
            out << duration << ", ";
            //displayBridges(bridges, out);
        }
        out << "}" << std::endl;

        average = std::accumulate(times.begin(), times.end(), 0L) / times.size();
        out << "Average time: " << average << "[micro]" << std::endl;

        return 0;
    }



    int _eulerianTimes(Graph& graph, std::ostream& out) {
        std::chrono::steady_clock::time_point begin, end; 


        std::vector<long> times;
        times.reserve(5);
        
        out << std::endl << "## EULERIAN TARJAN" << std::endl;
        out << "{";
        for (int i = 0; i < 10; i++) {
            begin = std::chrono::steady_clock::now();
            Response<Graph> auxiliar = Graph::clone(graph);
            if (!auxiliar.isOk()) {
                std::cerr << "ERROR WHILE CLONING GRAPH" << std::endl;
            }
            vector<int> path = findEulerianPathTarjan(auxiliar.value);
            end = std::chrono::steady_clock::now();

            long duration = getDurationInMicro(begin,end);
            times.push_back(duration);
            out << duration << ", ";
            
            
            // ## HOW TO SHOW OUTPUTS?
        }
        out << "}" << std::endl;

        long average = std::accumulate(times.begin(), times.end(), 0L) / times.size();
        out << "Average time: " << average << "[micro]" << std::endl;
        

        times.clear();
        out << std::endl << "## EULERIAN NAIVE" << std::endl;
        out << "{";
        for (int i = 0; i < 10; i++) {
            begin = std::chrono::steady_clock::now();
            Response<Graph> auxiliar = Graph::clone(graph);
            if (!auxiliar.isOk()) {
                std::cerr << "ERROR WHILE CLONING GRAPH" << std::endl;
            }
            vector<int> path = findEulerianPathNaive(auxiliar.value);
            end = std::chrono::steady_clock::now();

            long duration = getDurationInMicro(begin,end);
            times.push_back(duration);
            out << duration << ", ";
            
            
            // ## HOW TO SHOW OUTPUTS?
        }
        out << "}" << std::endl;
        
        average = std::accumulate(times.begin(), times.end(), 0L) / times.size();
        out << "Average time: " << average << "[micro]" << std::endl;

        return 0;
    }


    Response<Graph> _writeResult(const std::string& filename, std::vector<char> ioBuffer, std::ostream& out) {
        std::chrono::steady_clock::time_point begin, end; 

        begin = std::chrono::steady_clock::now();
        Response<Graph> result = graphformat::readGraphFromFile(filename, ioBuffer);
        end = std::chrono::steady_clock::now();

        if (!result.isOk()) {
            std::cerr << result.message << std::endl;
            return Response<Graph>("COULD NOT READ", {});
        }

        Graph graph = std::move(result.value);

        out << "# " << filename << std::endl;
        out << "read time: " << getDurationInMicro(begin,end) << "[micro]" << std::endl;
        ioBuffer.clear();

        _bridgeTimes(graph, out);
        _eulerianTimes(graph, out);
        
        

        return graph;
    }
    

    int _writeResults(const std::string& folder, std::vector<char> ioBuffer, std::ostream& out) {
        for (int i = 0; i < 10; i++) {
            std::string filename = folder + std::to_string(i) + ".graph";
            Response<Graph> res =_writeResult(filename, ioBuffer, out);
            if (!res.isOk()) return -1;


            out << std::endl << std::endl;
            std::cout << "DONE";
        }
        std::cout << std::endl;
        std::cout << folder << std::endl;
        return 0;
    }



    int execute(const std::vector<int>& nums) {
        const size_t bufferSize = 64 * 1024;  // 64KB
        std::vector<char> ioBuffer(bufferSize);
        std::string graphInputContext = "examples/0.3f/";
        const std::string logOutputContext = "examples/bridgesResult/";
        
        

        for (auto num : nums) {
            const std::string inFolderPath = graphInputContext + std::to_string(num) + "/";
            const std::string logPath = logOutputContext + std::to_string(num);

            std::ofstream out(logPath, std::ios::binary | std::ios::trunc);
            if (!out) {
                std::cerr << "Error: Could not open output file for writing" << std::endl;
                return -1;
            }

            if (_writeResults(inFolderPath, ioBuffer, out) != 0) return -1;
        }

        return 0;
    }

    int execute() {
        return execute({100, 1000, 10000, 11000, 12000, 13000, 14000, 15000});
    }






    // # GENERATE AND STORE GRAPHS
    int storeGraph(Graph& graph, std::string filename, std::vector<char> ioBuffer) {
        std::chrono::steady_clock::time_point begin, end;     

        begin = std::chrono::steady_clock::now();
        Response<void> writeResult = graphformat::writeGraphToFile(filename, ioBuffer, graph);
        end = std::chrono::steady_clock::now();
        if (!writeResult.isOk()) {
            std::cerr << writeResult.message << std::endl;
            return -1;
        } 
        std::cout << "write time: " << getDurationInMicro(begin,end) << "[micro]" << std::endl;
        ioBuffer.clear();
        return 0;
    }


    int generateGraphs(const std::string& fileContext, int quantidadeVertices, float edgePercentage, std::vector<char>& ioBuffer) {
        std::filesystem::create_directory(fileContext);

        for(int i = 0; i < 10; i++) {
            cout << "I: " << i << endl;
            Response<Graph> graph = generateEulerianGraph(quantidadeVertices, edgePercentage);
            if (!graph.isOk()) {
                std::cerr << graph.message << std::endl;
                return -1;
            }
            std::cout << "graph generated" << std::endl;
            std::string filename = fileContext + std::to_string(i) + ".graph";
            std::cout << filename << std::endl;
            storeGraph(graph.value, filename, ioBuffer);
            cout << "Quantity of Edges: " << endl << graph.value.getTotalQuantityEdges() << endl;
        }

        return 0;
    }


    int executeGen(const std::vector<int>& nums) {
        const size_t bufferSize = 64 * 1024;  // 64KB
        std::vector<char> ioBuffer(bufferSize);

        const float edgePercentage = 0.003f;
        const std::string prefix = "examples/0.3f/";

        for (auto num : nums) {
            const std::string filepath = prefix + std::to_string(num) + "/";
            int res = generateGraphs(filepath, num, edgePercentage, ioBuffer) != 0;
            if (res != 0) return res;
        }

        return 0;
    }

    int executeGen() {
        return executeGen({100, 1000, 10000, 11000, 12000, 13000, 14000, 15000});
    }

    int executeGen2() {
        return executeGen({100, 250, 500, 750, 1000, 1250, 1500, 1750, 2000, 2250, 2500, 2750, 3000});
    }
    
}