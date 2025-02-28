#ifndef RANDOMIZER_H
#define RANDOMIZER_H

#include "graph.hpp"
#include "response_struct.hpp"

#include <algorithm>
#include <sstream>
#include <random>
#include <variant>
#include <cstring>
#include <unordered_set>
#include <vector>


namespace randomgraph {
    const int EDGELIMIT = 10485760;
    const long BYTE_TO_MB = 1048576;
    
    struct pair_hash {
        template <class T1, class T2>
        std::size_t operator ( ) (const std::pair<T1, T2>& p) const {
            auto h1 = std::hash<T1>{}(p.first);
            auto h2 = std::hash<T2>{}(p.second);
            return h1 ^ h2; // Combine the hashes
        }
    };

    /**
     * @brief Generates a random boolean value based on a given probability.
     *
     * This function returns `true` with a probability of `trueChance` and `false` otherwise.
     * The probability value should be between `0.0f` and `1.0f`, inclusive.
     *
     * @param trueChance The probability (between 0.0 and 1.0) of returning `true`.
     * @return `true` with a probability of `trueChance`, otherwise `false`.
     *
     * @note Uses `std::random_device` and `std::mt19937` for better randomness.
     * @warning If `trueChance` is outside the range [0.0, 1.0], behavior is undefined.
     */
    bool randomBoolean(float trueChance);


    int randomInt(int min, int max);

    void makeGraphRandomlyConnected(Graph& graph);

    int __connectEmptyGraph(Graph& graph, std::unordered_set<std::pair<int, int>, pair_hash>& addedEdges);

    Response<Graph> createConectedGraph(int n, int edges);
    Response<Graph> createConectedGraph(int n, float edgePercentage); 

    Response<Graph> createEulerianGraph(int n, int edges);
    Response<Graph> createEulerianGraph(int n, float edgePercentage);

    Graph bruteForceCreateConnected(int, float edgePercentage);
}
#endif