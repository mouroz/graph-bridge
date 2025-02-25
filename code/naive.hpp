#ifndef NAIVE_HPP
#define NAIVE_HPP

// Graph library import
#include "graph.hpp"


#include <iostream>
#include <vector>

/// @brief verifies if a pair of vertices is a bridge
EdgeVector executeNaive(Graph &g);

bool isBridge(int u, int v);


#endif  // NAIVE_HPP