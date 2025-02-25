#ifndef EULERIAN_PATH_H
#define EULERIAN_PATH_H

#include <iostream>
#include <stack>
#include "tarjan.hpp"
#include "graph_reader.hpp"
#include "graph.hpp"

using namespace std;

/*
@brief this method runs the graph and returns if there's a possibility of existence of an eulerian path
*/
pair<bool, int> canHaveEulerianPath(Graph graph);

/*
@brief Finds an Eulerian path in the graph using Fleury's algorithm
@param graph The graph to be analyzed
@return A list of vertices representing the Eulerian path, or an empty list if no path exists
*/
vector<int> findEulerianPath(Graph& graph);

#endif // EULERIAN_PATH_H