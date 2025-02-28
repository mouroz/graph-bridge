#ifndef GRAPH_READER_H
#define GRAPH_READER_H


#include "graph.hpp"
#include "response_struct.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <chrono>


namespace graphtext {


    /**
     * @brief Reads a single number from a string
     *
     * This function attempts to parse an integer from the provided string.
     * If the string is not a valid number or contains invalid characters, 
     * the status is set to -1 indicating an error.
     *
     * @param s The string to parse
     * @return Response<int> A response containing the status and parsed value
     */
    Response<int> read_number(std::string &s);


    /**
     * @brief Reads a list of numbers from a string
     *
     * This function attempts to extract a sequence of integers from a string.
     * If any non-digit characters (other than space, plus, or minus signs) are encountered,
     * the status is set to -1, indicating an error. Additionally, if any number is invalid
     * (e.g., out of range), the error status is triggered.
     *
     * @param line The string containing the list of numbers
     * @return Response<std::vector<int>> A response containing the status and parsed list of integers
     */
    Response<std::vector<int>> read_numbers(std::string line);



    /**
     * @brief Reads a graph from a file
     *
     * This function attempts to read a graph from the provided file. The first line 
     * should contain the number of vertices in the graph. Each subsequent line contains 
     * the adjacency list for each vertex. If any errors are encountered during reading, 
     * an empty graph is returned.
     *
     * @param file The input file stream
     * @return Graph The parsed graph structure, or an empty graph if an error occurred
     */
    Graph read_graph_from_file(std::ifstream &file);

    void writeToFile(const Graph& graph, std::ofstream& outFile);
    
}



namespace graphformat {


    // ifstream is already buffered for 4 - 8kb. 
    // There are no extra system calls when reading numbers individually
    Graph read_graph_from_file(std::ifstream &file);
    void writeToFile(const Graph& graph, std::ofstream& outFile);

    long __getDuration(std::chrono::steady_clock::time_point begin, std::chrono::steady_clock::time_point end);
    TimedResponse<Graph> simpleReadGraph(const std::string& filename);
    TimedResponse<Graph> readGraph(const std::string& filename, std::vector<char>& ioBuffer);
    TimedResponse<void> writeGraph(const std::string& filename, std::vector<char>& ioBuffer, const Graph &graph);
}

#endif