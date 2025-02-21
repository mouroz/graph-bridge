#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <sstream>
#include <random>

class Graph {
  private:
    // Função recursiva de DFS para auxiliar na verificação de conectividade
    void DFSUtil(int v, std::vector<bool> &visited) const;
    // Log da criação do grafo (imprime as arestas criadas)
    static void createRandomGraphLog(const std::string& s);
  public:
    int V;                            // Número de vértices
    std::vector<std::list<int>> adj;  // Lista de adjacência

    Graph();

    // Construtor que inicializa o grafo com V vértices
    Graph(int vertices);

    // Adiciona uma aresta não direcionada entre u e v
    void addEdge(int u, int v);

    // Remove a aresta não direcionada entre u e v
    void removeEdge(int u, int v);

    // Verifica a existencia de uma aresta entre u e v
    bool hasEdge(int u, int v);

    // Imprime a estrutura do grafo
    std::string toString() const;

    // Verifica se o grafo (ignorando vértices isolados) é conectado.
    // Útil para o algoritmo de Fleury
    bool isConnected() const;

    // Cria um grafo aleatório com n vértices
    static Graph createRandomGraph(int n, float edgeProbability);

    static Graph createConectedGraph(int n, float edgeProbability);

    // Recupera a quantidade de vértices
    int getVertexQuantity() const;

    // Recupera a quantidade de arestas em determinado vértice
    int getEdgeQuantity(int v) const;
};

#endif  // GRAPH_HPP