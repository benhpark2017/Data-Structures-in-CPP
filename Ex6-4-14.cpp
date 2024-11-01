/**
 * Solutions to Chapter 6, Section 4, Exercise 14 of Horowitz, Sahni, and Mehta's
 * Fundamentals of Data Structures in C++.
 * 
 * Task 14:
 * Modify the algorithm BellmanFord() so that it obtains the shortest paths, in 
 * addition to the lengths of these paths. What is the computing time of your
 * function?
 */
 
 /***********************************main.cpp***********************************/
#include "Graph.h"

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
    g.printDistancesAndPaths();

    return 0;
}



/***********************************Graph.h***********************************/
#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <limits>
#include <vector>
#include <stack>

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
    int* predecessor;     // Array to store predecessors for path reconstruction
    
public:
    Graph(int vertices);
    
    ~Graph();

    void addEdge(int u, int v, int length);

    void BellmanFord(const int n, const int v);

    void printDistancesAndPaths() const;

private:
    void printPath(int vertex) const;
};

#endif // GRAPH_H



/**********************************Graph.cpp***********************************/
#include "Graph.h"

Graph::Graph(int vertices) : n(vertices) {
    adjList = new Node*[n];
    for (int i = 0; i < n; i++) {
        adjList[i] = nullptr;
    }
    dist = new int[n];
    predecessor = new int[n];
}

Graph::~Graph() {
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
    delete[] predecessor;
}

void Graph::addEdge(int u, int v, int length) {
    adjList[u] = new Node(v, length, adjList[u]);
}

void Graph::BellmanFord(const int n, const int v) {
    // Initialize distances and predecessors
    for (int i = 0; i < n; i++) {
        dist[i] = std::numeric_limits<int>::max();
        predecessor[i] = -1;  // -1 indicates no predecessor
    }
    dist[v] = 0;  // Distance to source vertex is 0

    // Initialize distances from source to its immediate neighbors
    Node* current = adjList[v];
    while (current != nullptr) {
        dist[current->vertex] = current->length;
        predecessor[current->vertex] = v;  // Set source as predecessor
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
                        predecessor[u] = i;  // Update predecessor
                    }
                    current = current->link;
                }
            }
        }
    }

    // Check for negative weight cycles
    for (int u = 0; u < n; u++) {
        for (int i = 0; i < n; i++) {
            Node* current = adjList[i];
            while (current != nullptr) {
                if (current->vertex == u && 
                    dist[i] != std::numeric_limits<int>::max() && 
                    dist[u] > dist[i] + current->length) {
                    std::cout << "Graph contains negative weight cycle!" << std::endl;
                    return;
                }
                current = current->link;
            }
        }
    }
}

void Graph::printDistancesAndPaths() const {
    for (int i = 0; i < n; i++) {
        if (dist[i] == std::numeric_limits<int>::max()) {
            std::cout << "Distance to " << i << ": INF" << std::endl;
            std::cout << "Path: No path exists" << std::endl;
        } else {
            std::cout << "Distance to " << i << ": " << dist[i] << std::endl;
            std::cout << "Path: ";
            printPath(i);
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
}

void Graph::printPath(int vertex) const {
    if (predecessor[vertex] == -1) {
        std::cout << vertex;
        return;
    }
    
    // Use a stack to store the path
    std::stack<int> path;
    int current = vertex;
    
    // Build path from end to start
    while (current != -1) {
        path.push(current);
        current = predecessor[current];
    }
    
    // Print path from start to end
    while (!path.empty()) {
        std::cout << path.top();
        path.pop();
        if (!path.empty()) {
            std::cout << " -> ";
        }
    }
}
