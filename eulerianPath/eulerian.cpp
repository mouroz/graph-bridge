#include "eulerian.hpp"

/*
@brief this method runs the graph and returns if there's a possibility of existence of an eulerian path

@params graph the graph to be analyzed
@return true if there's a possibility of existence of an eulerian path, false otherwise
*/

bool canHaveEulerianPath(Graph graph){
  int odd = 0;
  for(int i = 0; i < graph.getVertexQuantity(); i++){
    if(graph.adj[i].size() % 2 != 0){
      odd++;
    }
  }
  return odd == 0 || odd == 2;
}


//Main function
int main(void){
  Graph graph = Graph::createRandomGraph(3, 1);

  canHaveEulerianPath(graph) ? cout << "The graph can have an eulerian path" : cout << "The graph can't have an eulerian path";

  return 0;  
}