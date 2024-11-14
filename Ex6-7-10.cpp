/**
 * Solutions to Chapter 6, Section 7, Exercise 10 of Horowitz, Sahni, and 
 * Mehta's Fundamentals of Data Structures in C++. 
 * 
 * Task 10:
 * An edge (u, v) of a connected, undirected graph G is a bridge if and only if
 * its deletion from G results in a graph that is not connected. Using the ideas
 * of the set of functions Biconnected(), write an algorithm that runs in 
 * O(|V|+|E|) time to find the bridges of G. |V| and |E| are the cardinalities
 * of the set of vertices V and the set of edges E of G, respectively.
 */

/**********************************main.cpp************************************/
#include "Graph.h"
#include <iostream>

int main() {
    
    std::cout << "Graph as shown in Figure 6.19 of Chapter 6, " 
              << "Section 2, page 351 of the textbook." << std::endl;
    Graph g(10);  // Create a graph with 10 vertices from 0 to 9.
    
    g.AddEdge(0, 1);
    g.AddEdge(1, 2);
    g.AddEdge(1, 3);
    g.AddEdge(2, 3);
    g.AddEdge(2, 4);
    g.AddEdge(3, 4);
    g.AddEdge(3, 5);
    g.AddEdge(5, 6);
    g.AddEdge(5, 7);
    g.AddEdge(6, 7);
    g.AddEdge(7, 8);
    g.AddEdge(7, 9);
    
    std::vector<std::pair<int, int>> bridges = g.FindBridges();
    for (auto bridge : bridges) {
        std::cout << bridge.first << " -- " << bridge.second << " is a bridge.\n";
    }
    
    std::cout << "End of program." << std::endl;
}



/***********************************Graph.h************************************/
#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <stack>
#include <utility>
#include <vector>

class Graph {
private:
    int num;
    int* dfn;
    int* low;
    std::stack<std::pair<int, int>> graphStack;
    bool* visited;
    int n;
    std::vector<std::vector<int>> adj;  // Changed to vector implementation for adjacency list
    
    void FindBridgesDFS(const int u,
                        const int parent,
                        std::vector<std::pair<int, int>>& bridges);
    void DfnLow(const int u, const int v);
    void Biconnected(const int u, const int v);
    int minimum(int a, int b);
    
public:
    Graph(int vertices);
    ~Graph();
    void DfnLow(const int x);
    void Biconnected();
    void AddEdge(int u, int v);
    std::vector<std::pair<int, int>> FindBridges();
};

#endif //GRAPH_H



/**********************************Graph.cpp***********************************/
#include "Graph.h"

Graph::Graph(int vertices) : n(vertices) {
    adj.resize(n);
    dfn = new int[n];
    low = new int[n];
    visited = new bool[n];
}

Graph::~Graph() {
    delete[] dfn;
    delete[] low;
    delete[] visited;
}

void Graph::DfnLow(const int x) {
    num = 1;
    for (int i = 0; i < n; i++) {
        dfn[i] = low[i] = 0;
    }
    DfnLow(x, -1);
}

void Graph::Biconnected() {
    num = 1;
    for (int i = 0; i < n; i++) {
        dfn[i] = low[i] = 0;
        visited[i] = false;
    }
    Biconnected(0, -1);
}

void Graph::AddEdge(int u, int v) {
    adj[u].push_back(v);
    adj[v].push_back(u);
}

std::vector<std::pair<int, int>> Graph::FindBridges() {
    // Initialize arrays
    num = 0;
    std::vector<std::pair<int, int>> bridges;
    
    for (int i = 0; i < n; i++) {
        dfn[i] = low[i] = 0;
        visited[i] = false;
    }
    
    // Start DFS from vertex 0
    FindBridgesDFS(0, -1, bridges);
    
    return bridges;
}

void Graph::FindBridgesDFS(const int u, 
                           const int parent, 
                           std::vector<std::pair<int, int>>& bridges) {
    // Mark current vertex as visited
    visited[u] = true;
    
    // Initialize discovery time and low value
    dfn[u] = low[u] = ++num;
    
    // Go through all vertices adjacent to this
    for (int w : adj[u]) {
        // If w is not visited yet, then make it a child of u
        // in DFS tree and recur for it
        if (!visited[w]) {
            FindBridgesDFS(w, u, bridges);
            
            // Check if the subtree rooted at w has a connection to
            // one of the ancestors of u
            low[u] = std::min(low[u], low[w]);
            
            // If the lowest vertex reachable from subtree under w is
            // below u in DFS tree, then u-w is a bridge
            if (low[w] > dfn[u]) {
                bridges.push_back({u, w});
            }
        }
        // Update low value of u for parent function calls
        else if (w != parent) {
            low[u] = std::min(low[u], dfn[w]);
        }
    }
}

void Graph::DfnLow(const int u, const int v) {
    dfn[u] = low[u] = num++;
    
    // Iterate through all adjacent vertices of u
    for (int w : adj[u]) {
        if (dfn[w] == 0) {  // Tree edge
            DfnLow(w, u);
            low[u] = minimum(low[u], low[w]);
        }
        else if (w != v) {  // Back edge
            low[u] = minimum(low[u], dfn[w]);
        }
    }
}

void Graph::Biconnected(const int u, const int v) {
    dfn[u] = low[u] = num++;
    
    // Iterate through all adjacent vertices of u
    for (int w : adj[u]) {
        if (dfn[w] == 0) {  // Tree edge
            // Push the edge before recursive call
            graphStack.push({u, w});
            Biconnected(w, u);
            low[u] = minimum(low[u], low[w]);
            
            // If w is a cut-vertex
            if (low[w] >= dfn[u]) {
                std::cout << "New biconnected component:\n";
                std::pair<int, int> edge;
                do {
                    edge = graphStack.top();
                    graphStack.pop();
                    std::cout << "(" << edge.first << ", " << edge.second << ")\n";
                } while (edge != std::make_pair(u, w));
            }
        }
        else if (w != v && dfn[w] < dfn[u]) {  // Back edge
            graphStack.push({u, w});
            low[u] = minimum(low[u], dfn[w]);
        }
    }
}

int Graph::minimum(int a, int b) {
    return (a <= b) ? a : b;
}
