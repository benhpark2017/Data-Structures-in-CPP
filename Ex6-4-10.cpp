/**
 * Solutions to Chapter 6, Section 4, Exercise 10 of Horowitz, Sahni, and Mehta's
 * Fundamentals of Data Structures in C++.
 * 
 * Task 10:
 * Transform pseudocode BellmanFord() into a C++ function. Assume that the graphs
 * are represented using adjacency lists in which each node has an additional
 * field called length that gives the length of the edge represented by that
 * node. As a result of this, there is no length-adjacency matrix. Generate some
 * test graphs and test the correctness of your function.
 */

/**********************************main.cpp************************************/
#include "Graph.h"
#include <iostream>

int main() {
    int numVertices = 5;
    Graph g(numVertices);

    // Add edges (u, v, weight)
    g.addEdge(0, 1, 6);
    g.addEdge(0, 2, 7);
    g.addEdge(1, 2, 8);
    g.addEdge(1, 3, -4);
    g.addEdge(2, 3, 9);
    g.addEdge(2, 4, -3);
    g.addEdge(3, 4, 7);
    g.addEdge(3, 0, 2);
    g.addEdge(4, 1, 5);

    // Find shortest paths from vertex 0
    g.BellmanFord(numVertices, 0);
    g.printDistances();

    return 0;
}



/**********************************Graph.h************************************/
#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <limits>

class Graph {
private:
    struct Node {
        int vertex;    // Destination vertex
        int length;    // Edge weight
        Node* link;    // Pointer to next node in adjacency list
        Node(int v, int len = 0, Node* nxt = nullptr) : vertex(v), length(len), link(nxt) {}
    };
    
    int n;                 // Number of vertices
    Node** adjList;        // Array of pointers to adjacency lists
    int* dist;            // Distance array for shortest paths
    
public:
    Graph(int vertices);
    
    ~Graph();

    void addEdge(int u, int v, int length);

    void BellmanFord(const int n, const int v);

    void printDistances() const;
};

#endif // GRAPH_H



/*********************************Graph.cpp************************************/
#include "Graph.h"

Graph::Graph(int vertices) : n(vertices) {
    adjList = new Node*[n];
    for (int i = 0; i < n; i++) {
        adjList[i] = nullptr;
    }
    dist = new int[n];
}

Graph::~Graph() {
    // Clean up adjacency lists
    for (int i = 0; i < n; i++) {
        Node* current = adjList[i];
        while (current != nullptr) {
            Node* temp = current;
            current = current->link;
            delete temp;
        }
    }
    delete[] adjList;
    delete[] dist;
}

void Graph::addEdge(int u, int v, int length) {
    adjList[u] = new Node(v, length, adjList[u]);
}

void Graph::BellmanFord(const int n, const int v) {
    // Initialize distances
    for (int i = 0; i < n; i++) {
        dist[i] = std::numeric_limits<int>::max();
    }
    dist[v] = 0;  // Distance to source vertex is 0

    // Initialize distances from source to its immediate neighbors
    Node* current = adjList[v];
    while (current != nullptr) {
        dist[current->vertex] = current->length;
        current = current->link;
    }

    // Relax edges n-1 times
    for (int k = 2; k <= n - 1; k++) {
        // For each vertex u except the source
        for (int u = 0; u < n; u++) {
            if (u == v) continue;  // Skip source vertex
            
            // For each incoming edge to u
            for (int i = 0; i < n; i++) {
                Node* current = adjList[i];
                while (current != nullptr) {
                    if (current->vertex == u && 
                        dist[i] != std::numeric_limits<int>::max() && 
                        dist[u] > dist[i] + current->length) {
                        dist[u] = dist[i] + current->length;
                    }
                    current = current->link;
                }
            }
        }
    }
}

void Graph::printDistances() const {
    for (int i = 0; i < n; i++) {
        if (dist[i] == std::numeric_limits<int>::max())
            std::cout << "Distance to " << i << ": INF" << std::endl;
        else
            std::cout << "Distance to " << i << ": " << dist[i] << std::endl;
    }
    std::cout << std::endl;
}
