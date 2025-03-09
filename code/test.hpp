#ifndef BRIDGES_TEST_HPP
#define BRIDGES_TEST_HPP

#include <fstream>
#include <vector>
#include <chrono>
#include <string>
#include "graph.hpp"  // Assuming these are your custom includes
#include "graph_reader.hpp"
#include "tarjan.hpp"
#include "naive.hpp"
#include "eulerian.hpp"
#include "randomizer.hpp"

// Function prototypes
void outputGraph(const Graph& graph, const std::string& str);

long getDuration(std::chrono::steady_clock::time_point begin, std::chrono::steady_clock::time_point end);
long getDurationInNano(std::chrono::steady_clock::time_point begin, std::chrono::steady_clock::time_point end);

int _bridgeTimes(Graph& graph, std::ofstream& out);
int _readGraph(const std::string& filename, std::vector<char> ioBuffer, std::ofstream& out);
int _readGraphs(const std::string& filenameContext, std::vector<char> ioBuffer, std::ofstream& out);
int execute();

#endif // BRIDGES_TEST_HPP