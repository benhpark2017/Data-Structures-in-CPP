/**
 * Solutions to Chapter 6, Section 4, Exercise 11 of Horowitz, Sahni, and Mehta's
 * Fundamentals of Data Structures in C++.
 * 
 * Task 11:
 * Rewrite function BellmanFord() so that the second outer loop terminates either
 * after n - 1 iterations or after the first iteration in which no dist values
 * are changed, whichever occurs first.
 */

/**********************************main.cpp************************************/
#include "Graph.h"

int main() {
    // Create a graph with 5 vertices
    Graph g(5);
    
    // Add edges (source, destination, weight)
    g.addEdge(0, 1, 6);
    g.addEdge(0, 2, 7);
    g.addEdge(1, 2, 8);
    g.addEdge(1, 3, -4);
    g.addEdge(2, 3, 9);
    g.addEdge(2, 4, -3);
    g.addEdge(3, 4, 7);
    g.addEdge(3, 0, 2);
    g.addEdge(4, 1, 5);

    // Print the graph structure
    g.printGraph();

    // Find shortest paths from vertex 0
    g.BellmanFord(5, 0);

    // Print the results
    g.printDistances();

    return 0;
}



/**********************************Graph.h************************************/
#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <iomanip>
#include <limits>

const int nMax = 100;  // Maximum number of vertices
enum Boolean {FALSE, TRUE};

class Graph {
private:
    int length[nMax][nMax];  // Length-adjacency matrix
    int dist[nMax];          // Distance array
    Boolean s[nMax];         // Processed vertices set
    int n;                   // Current number of vertices

public:
    // Constructor initializes the graph with n vertices
    Graph(int vertices);

    // Add an edge from vertex u to vertex v with given weight
    void addEdge(int u, int v, int weight);

    // Bellman-Ford algorithm implementation
    void BellmanFord(const int n, const int v);

    // Print the shortest distances from the source vertex
    void printDistances() const;
    
    // Optional: Function to print the adjacency matrix (useful for debugging)
    void printGraph() const;
};

#endif // GRAPH_H 



/*********************************Graph.cpp************************************/
#include "Graph.h"

// Constructor initializes the graph with n vertices
Graph::Graph(int vertices) : n(vertices) {
    // Initialize length matrix with std::numeric_limits<int>::max() (indicating no edge)
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            length[i][j] = (i == j) ? 0 : std::numeric_limits<int>::max();
        }
    }
}

// Add an edge from vertex u to vertex v with given weight
void Graph::addEdge(int u, int v, int weight) {
    if (u >= 0 && u < n && v >= 0 && v < n) {
        length[u][v] = weight;
    } else {
        std::cout << "Invalid vertex indices. Edge not added." << std::endl;
    }
}

// Bellman-Ford algorithm implementation
void Graph::BellmanFord(const int n, const int v) {
    // Initialize distances from source vertex
    for (int i = 0; i < n; i++) {
        dist[i] = length[v][i];
    }
    
    // Relax edges up to n-1 times
    bool changed;
    for (int k = 2; k <= n - 1; k++) {
        changed = false;  // Track if any distance was updated
        
        // For each vertex u except the source
        for (int u = 0; u < n; u++) {
            if (u != v) {  // Skip source vertex
                bool hasIncomingEdge = false;
                
                // Check if u has any incoming edges
                for (int i = 0; i < n; i++) {
                    if (length[i][u] < std::numeric_limits<int>::max()) {
                        hasIncomingEdge = true;
                        break;
                    }
                }
                
                if (hasIncomingEdge) {
                    // For each possible edge to u
                    for (int i = 0; i < n; i++) {
                        if (length[i][u] < std::numeric_limits<int>::max() &&  // Edge exists
                            dist[i] != std::numeric_limits<int>::max() &&      // Source distance is not infinite
                            dist[u] > dist[i] + length[i][u]) {
                            dist[u] = dist[i] + length[i][u];
                            changed = true;  // Mark that we made a change
                        }
                    }
                }
            }
        }
        
        // If no distances were updated in this iteration, we can stop
        if (!changed) {
            std::cout << "Early termination at iteration " << k << std::endl;
            break;
        }
    }
}

// Print the shortest distances from the source vertex
void Graph::printDistances() const {
    std::cout << "\nShortest distances from source vertex:" << std::endl;
    std::cout << "Destination\tDistance" << std::endl;
    std::cout << "-----------\t--------" << std::endl;
    
    for (int i = 0; i < n; i++) {
        std::cout << std::setw(11) << i << "\t";
        if (dist[i] == std::numeric_limits<int>::max()) {
            std::cout << "INF" << std::endl;
        } else {
            std::cout << std::setw(8) << dist[i] << std::endl;
        }
    }
}

// Function to print the adjacency matrix (useful for debugging)
void Graph::printGraph() const {
    std::cout << "\nAdjacency Matrix:" << std::endl;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (length[i][j] == std::numeric_limits<int>::max()) {
                std::cout << std::setw(4) << "INF";
            } else {
                std::cout << std::setw(4) << length[i][j];
            }
        }
        std::cout << std::endl;
    }
}

