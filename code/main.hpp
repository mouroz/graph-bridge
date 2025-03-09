#ifndef MAIN
#define MAIN

#include "graph.hpp"
#include "graph_reader.hpp"
#include "tarjan.hpp"
#include "naive.hpp"
#include "eulerian.hpp"
#include "randomizer.hpp"
#include "test.hpp"

#include <functional>
#include <vector>
#include <iostream>
#include <chrono>
#include <string>
#include <filesystem>

int testNaive(Graph& graph);
int ioTest(Graph& graph);
int testNaive(Graph& graph);
int testEulerianTarjan(Graph& graph);
int testEulerianNaive(Graph& graph);
Response<Graph> generateEulerianGraph();
Response<Graph> generateEulerianGraph(int numberOfVertices);
Response<Graph> generateConnectedGraph();
void executeTest(int i, Graph& graph);







#endif // BRIDGES_TEST_HPP
