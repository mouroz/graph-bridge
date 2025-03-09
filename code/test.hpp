#ifndef BRIDGES_TEST_HPP
#define BRIDGES_TEST_HPP


namespace test {
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
    int execute(const std::vector<int>& nums);
    int execute();
    int generateGraphs(const std::string& fileContext, int quantidadeVertices, float edgePercentage, std::vector<char>& ioBuffer);
    int executeGen(const std::vector<int>& nums);
    int executeGen();

}

#endif // BRIDGES_TEST_HPP
