#ifndef GRAPH_READER_H
#define GRAPH_READER_H

#include "tarjan.hpp" // Graph structures
#include "graph_reader.hpp"
#include "graph.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>

/**
 * @brief A structure to encapsulate a response with a value and status
 *
 * The `Response` structure is used to store both the result (a value of type T) 
 * and the status (an integer). By default, the status is set to `0` (indicating success),
 * and the value holds the response data. If the status is set to `-1`, it indicates an error 
 * or failure condition. This structure is helpful for functions that may return both 
 * a result and a status code (success or failure).
 *
 * @tparam T The type of the value stored in the response
 */
template <typename T>
struct Response {
    int status = 0; //default value is ok
    T val;

    bool is_ok();

    // Standart "not ok" value without any meaning
    void set_not_ok();
};



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

#endif