
#include "graph_reader.hpp"
#include "response_struct.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>



Response<int> read_number(std::string &s) {
    Response<int> response;

    size_t pos;
    int num = std::stoi(s, &pos); // Convert to integer using std::stoi
    if (pos != s.size()) {
        response.status = -1; // Error: Invalid number
        return response;
    }

    response.val = num;
    return response;
}

Response<std::vector<int>> read_numbers(std::string line) {
    std::istringstream iss(line);
    std::vector<int> numbers;
    std::string temp;
    Response<std::vector<int>> response;


    // for (char ch : line) {
    //     if (!(isdigit(ch) || ch == ' ' || ch == '-' || ch == '+')) {
    //         response.set_not_ok(); 
    //         return response;
    //     }
    // }

    // Loop extracting next inner string
    while (iss >> temp) {
        try {
            Response<int> num = read_number(temp);
            if (!num.is_ok()) {
                response.set_not_ok();
                return response;
            }

            numbers.push_back(num.val); 
        } catch (const std::invalid_argument& e) { // Invalid number format
            response.set_not_ok(); 
            return response;
        } catch (const std::out_of_range& e) { // Catch out-of-range exceptions
            response.set_not_ok();
            return response;
        }
    }

    response.val = numbers;
    return response;
}




Graph read_graph_from_file(std::ifstream &file) {
    std::string line;
    
    if (! std::getline(file,line)) {
        std::cerr << "Graph Read Error: Could not read first line" << std::endl;
        return {};
    }
   

    Response<std::vector<int>> res = read_numbers(line);
    
    if (!res.is_ok()) {
        std::cerr << "Graph Read Error: Invalid parameters on first line" << std::endl;
        return {};
    }

    std::vector<int> values = res.val;
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
        if (!res.is_ok()) {
            std::cerr << "Graph Read Error: Invalid parameters for vertex " << i << std::endl;
            return {};
        }

        for (int v : res.val) {
            graph.pushEdge(i,v);
        }
        
    }

    return graph;
}