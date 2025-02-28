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
        Response<int> response;

        size_t pos;
        int num = std::stoi(s, &pos); // Convert to integer using std::stoi
        if (pos != s.size()) {
            response.status = -1; // Error: Invalid number
            return response;
        }

        response.value = num;
        return response;
    }

    Response<std::vector<int>> read_numbers(std::string line) {
        std::istringstream iss(line);
        std::vector<int> numbers;
        std::string temp;
        Response<std::vector<int>> response;


        // for (char ch : line) {
        //     if (!(isdigit(ch) || ch == ' ' || ch == '-' || ch == '+')) {
        //         response.setNotOk(); 
        //         return response;
        //     }
        // }

        // Loop extracting next inner string
        while (iss >> temp) {
            try {
                Response<int> num = read_number(temp);
                if (!num.isOk()) {
                    response.setNotOk();
                    return response;
                }

                numbers.push_back(num.value); 
            } catch (const std::invalid_argument& e) { // Invalid number format
                response.setNotOk(); 
                return response;
            } catch (const std::out_of_range& e) { // Catch out-of-range exceptions
                response.setNotOk();
                return response;
            }
        }

        response.value = numbers;
        return response;
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

    // ifstream is already buffered for 4 - 8kb. 
    // There are no extra system calls when reading numbers individually
    Graph read_graph_from_file(std::ifstream &file) {
        if (!file) {
            std::cerr << "Error opening file.\n";
            return 1;
        }

        int number;
        file.read(reinterpret_cast<char*>(&number), sizeof(number));

        Graph graph(number);

        int *buffer = new int[number];
        for (int i = 0; i < number; i++) {
            int length;
            file.read(reinterpret_cast<char*>(&length), sizeof(int));
            file.read(reinterpret_cast<char*>(buffer), sizeof(int) * length);

            for (int *ptr = buffer; ptr < buffer+length; ptr++) {
                graph.pushEdge(i, *ptr);
            }
        }

        delete[] buffer;
        return graph;
    }

    void writeToFile(const Graph& graph, std::ofstream& outFile) {
        if (!outFile.is_open()) {
            std::cerr << "Error: Unable to open file for writing.\n" << std::endl;
            return;
        }

        // Write number of vertices
        int numVertices = graph.adj.size();
        outFile.write(reinterpret_cast<const char*>(&numVertices), sizeof(numVertices));

        // Write adjacency list
    
        for (const auto& neighbors : graph.adj) {
            int numNeighbors = neighbors.size();
            outFile.write(reinterpret_cast<const char*>(&numNeighbors), sizeof(numNeighbors));

            // Write each neighbor
            for (int neighbor : neighbors) {
                outFile.write(reinterpret_cast<const char*>(&neighbor), sizeof(neighbor));
            }
        }
    }




    long __getDuration(std::chrono::steady_clock::time_point begin, std::chrono::steady_clock::time_point end) {
        return std::chrono::duration_cast<std::chrono::milliseconds> (end - begin).count();
    }

    TimedResponse<Graph> simpleReadGraph(const std::string& filename) {
        size_t bufferSize = 64 * 1024;  // 64KB
        std::vector<char> ioBuffer(bufferSize);

        return readGraph(filename, ioBuffer);
    }

    TimedResponse<Graph> readGraph(const std::string& filename, std::vector<char>& ioBuffer) {
        std::chrono::steady_clock::time_point begin, end; 

        std::ifstream in(filename);
        if (!in) return {"Error: Could not open input file for reading", 0, {}};
        
        in.rdbuf()->pubsetbuf(ioBuffer.data(), ioBuffer.capacity());

        begin = std::chrono::steady_clock::now();
        Graph graph = graphformat::read_graph_from_file(in);
        in.close();
        end = std::chrono::steady_clock::now();

        return {"", __getDuration(begin, end), graph};
    }

    TimedResponse<void> writeGraph(const std::string& filename, std::vector<char>& ioBuffer, const Graph &graph) {
        std::chrono::steady_clock::time_point begin, end; 
        
        std::ofstream out(filename, std::ios::binary | std::ios::trunc);
        if (!out) return {"Error: Could not open output file for writing", 0};
    

        out.rdbuf()->pubsetbuf(ioBuffer.data(), ioBuffer.capacity());

        begin = std::chrono::steady_clock::now();
        graphformat::writeToFile(graph, out);
        end = std::chrono::steady_clock::now();
        
        out.close();

        return {"", __getDuration(begin, end)};
    }

}