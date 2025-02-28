#include "graph.hpp"
#include "graph_reader.hpp"
#include "response_struct.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <string>
#include <filesystem>



namespace graphtext {
    Response<int> read_number(std::string &s) {

        size_t pos;
        int num = std::stoi(s, &pos); // Convert to integer using std::stoi
        if (pos != s.size()) {
            return Response<int>("Number conversion from string to int failed", 0);
        }

        return Response<int>(num);
    }

    Response<std::vector<int>> read_numbers(std::string line) {
        std::istringstream iss(line);
        std::vector<int> numbers;
        std::string temp;


        // Loop extracting next inner string
        while (iss >> temp) {
            try {
                Response<int> num = read_number(temp);
                if (!num.isOk()) {
                    return Response<std::vector<int>>(std::move(num.message), {});
                }

                numbers.push_back(num.value); 
            } catch (const std::invalid_argument& e) {
                return Response<std::vector<int>>("Invalid character found", {});

            } catch (const std::out_of_range& e) {
                return Response<std::vector<int>>("Out of range error", {});
            }
        }

        return Response<std::vector<int>>(numbers);
    }



    Graph read_graph_from_file(std::ifstream &file) {
        std::string line;
        
        if (! std::getline(file,line)) {
            std::cerr << "Graph Read Error: Could not read first line" << std::endl;
            return {};
        }
    

        Response<std::vector<int>> res = read_numbers(line);
        
        if (!res.isOk()) {
            std::cerr << "Graph Read Error: Invalid parameters on first line" << std::endl;
            return {};
        }

        std::vector<int> values = res.value;
        if (values.size() != 1) {
            std::cerr << "Graph Read  Error: Incorrect number of paramenters on first line" << std::endl;
            return {};
        }

        int n = values[0];



        Graph graph(n);

        // Read adjacency list for each vertex
        for (int i = 0; i < n; ++i) {
            if (!std::getline(file, line)) {
                std::cerr << "Graph Read Error: Missing adjacency list for vertex" 
                    << i << ", or error reading" << std::endl;
                return {};
            }

            
            Response<std::vector<int>> res = read_numbers(line);
            if (!res.isOk()) {
                std::cerr << "Graph Read Error: Invalid parameters for vertex " << i << std::endl;
                return {};
            }

            for (int v : res.value) {
                graph.pushEdge(i,v);
            }
            
        }

        return graph;
    }
    

    void writeToFile(const Graph& graph, std::ofstream& outFile) {
    
        // Write number of vertices
        outFile << graph.adj.size() << "\n";

        // Write adjacency list
        for (size_t i = 0; i < graph.adj.size(); ++i) {
            for (int neighbor : graph.adj[i]) {
                outFile << neighbor << " ";
            }
            outFile << "\n";
        }

        outFile.close();
    }
}

namespace graphformat {
    long __getDuration(std::chrono::steady_clock::time_point begin, std::chrono::steady_clock::time_point end) {
        return std::chrono::duration_cast<std::chrono::milliseconds> (end - begin).count();
    }
    
    Response<Graph> readGraphFromFile(const std::string& filename, std::vector<char>& ioBuffer) {
        std::ifstream in(filename);
        if (!in) return Response<Graph>("Error: Could not open input file for reading", {});
        
        in.rdbuf()->pubsetbuf(ioBuffer.data(), ioBuffer.capacity());

        int number;
        in.read(reinterpret_cast<char*>(&number), sizeof(number));

        Graph graph(number);

        int *buffer = new int[number];
        for (int i = 0; i < number; i++) {
            int length;
            in.read(reinterpret_cast<char*>(&length), sizeof(int));
            in.read(reinterpret_cast<char*>(buffer), sizeof(int) * length);

            for (int *ptr = buffer; ptr < buffer+length; ptr++) {
                graph.pushEdge(i, *ptr);
            }
        }

        delete[] buffer;
        in.close();

        return Response(graph);
    }

    TimedResponse<Graph> timedReadGraphFromFile(const std::string& filename, std::vector<char>& ioBuffer) {
        std::chrono::steady_clock::time_point begin, end; 

        begin = std::chrono::steady_clock::now();

        Response<Graph> res = readGraphFromFile(filename, ioBuffer);
        if (!res.isOk()) return TimedResponse<Graph>(std::move(res.message), 0, {});
        
        end = std::chrono::steady_clock::now();
        

        return TimedResponse<Graph>(__getDuration(begin, end), res.value);
    }








    Response<void> writeGraphToFile(const std::string& filename, std::vector<char>& ioBuffer, const Graph &graph) {
        std::chrono::steady_clock::time_point begin, end; 
        
        std::ofstream out(filename, std::ios::binary | std::ios::trunc);
        if (!out) return Response<void>("Error: Could not open output file for writing");
    

        out.rdbuf()->pubsetbuf(ioBuffer.data(), ioBuffer.capacity());

        // Write number of vertices
        int numVertices = graph.adj.size();
        out.write(reinterpret_cast<const char*>(&numVertices), sizeof(numVertices));

        // Write adjacency list
    
        for (const auto& neighbors : graph.adj) {
            int numNeighbors = neighbors.size();
            out.write(reinterpret_cast<const char*>(&numNeighbors), sizeof(numNeighbors));

            // Write each neighbor
            for (int neighbor : neighbors) {
                out.write(reinterpret_cast<const char*>(&neighbor), sizeof(neighbor));
            }
        }
        
        out.close();

        return Response<void>();
    }


    TimedResponse<void> timedWriteGraphToFile(const std::string& filename, std::vector<char>& ioBuffer, const Graph &graph) {
        std::chrono::steady_clock::time_point begin, end; 

        begin = std::chrono::steady_clock::now();

        Response<void> res = writeGraphToFile(filename, ioBuffer, graph);
        if (!res.isOk()) return TimedResponse<void>(std::move(res.message), 0);
        
        end = std::chrono::steady_clock::now();

        return TimedResponse<void>(std::move(__getDuration(begin,end)));
    }

}