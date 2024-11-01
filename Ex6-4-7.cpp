/**
 * Solutions to Chapter 6, Section 4, Exercise 7 of Horowitz, Sahni, and Mehta's
 * Fundamentals of Data Structures in C++.
 * 
 * Task 7:
 * Modify algorithm ShortestPath() so that it obtains the shortest paths, in 
 * addition to the lengths of these paths. What is the computing time of your 
 * algorithm?
 * 
 * Response to Task 7:
 * The time complexity of function ShortestPath() is O(n^2). The initialization
 * takes O(n) while the two for loops take O(n^2), giving a combined time
 * complexity of O(n^2).
 */

/**********************************main.cpp************************************/
#include "Graph.h"
#include <iostream>

int main() {
    Graph g;
    int n = 5; // Example with 5 vertices

    // Define edges with weights using setEdge
    g.setEdge(0, 1, 10);
    g.setEdge(0, 3, 5);
    g.setEdge(1, 2, 1);
    g.setEdge(1, 3, 2);
    g.setEdge(2, 4, 4);
    g.setEdge(3, 1, 3);
    g.setEdge(3, 2, 9);
    g.setEdge(3, 4, 2);
    g.setEdge(4, 0, 7);
    g.setEdge(4, 2, 6);

    int start = 0;
    g.ShortestPath(n, start);

    // Print shortest paths and distances
    std::cout << "Shortest paths from vertex " << start << ":\n";
    for (int i = 0; i < n; i++) {
        std::cout << "Path to " << i << ": ";
        if (g.getDistance(i) == std::numeric_limits<int>::max()) {
            std::cout << "No path";
        } else {
            g.printPath(i);
            std::cout << " (Distance: " << g.getDistance(i) << ")";
        }
        std::cout << "\n";
    }

    std::cout << "End of program." << std::endl;

    return 0;
}



/**********************************Graph.h************************************/
#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <limits> // For std::numeric_limits<int>::max()

enum Boolean {FALSE, TRUE};

class Graph {
private:
    static const int nMax = 100;
    int length[nMax][nMax]; // Adjacency matrix for edge weights
    int dist[nMax];         // Array to store shortest path distances
    Boolean s[nMax];        // Array to indicate processed vertices
    int parent[nMax];       // Array to store the shortest path tree (predecessors)

public:
    Graph(); // Constructor to initialize lengths to "infinity" except the diagonal
    void ShortestPath(const int n, const int start);
    int choose(const int n);
    void printPath(int vertex) const; // Helper function to print the path

    // Accessor to set edge weights in the adjacency matrix
    void setEdge(int u, int v, int weight);
    
    int getDistance(int vertex) const;
};

#endif



/*********************************Graph.cpp************************************/
#include "Graph.h"

// Constructor to initialize the adjacency matrix
Graph::Graph() {
    for (int i = 0; i < nMax; i++) {
        for (int j = 0; j < nMax; j++) {
            length[i][j] = (i == j) ? 0 : std::numeric_limits<int>::max();
        }
    }
}

void Graph::ShortestPath(const int n, const int start) {
    for (int i = 0; i < n; i++) {
        s[i] = FALSE;
        dist[i] = length[start][i];
        if (dist[i] < std::numeric_limits<int>::max()) {
            parent[i] = start; // Set the starting vertex as the parent for initial reachable vertices
        } else {
            parent[i] = -1;    // No predecessor if initially unreachable
        }
    }
    s[start] = TRUE;
    dist[start] = 0;
    parent[start] = -1; // Start vertex has no parent

    for (int i = 0; i < n - 1; i++) {
        int u = choose(n);
        s[u] = TRUE;
        
        for (int w = 0; w < n; w++) {
            if (!s[w] && length[u][w] < std::numeric_limits<int>::max() &&
                dist[u] + length[u][w] < dist[w]) {
                dist[w] = dist[u] + length[u][w];
                parent[w] = u; // Update parent to track the path
            }
        }
    }
}

// Helper function to choose the unprocessed vertex with the minimum distance
int Graph::choose(const int n) {
    int minDist = std::numeric_limits<int>::max();
    int minIndex = -1;
    
    for (int i = 0; i < n; i++) {
        if (!s[i] && dist[i] < minDist) {
            minDist = dist[i];
            minIndex = i;
        }
    }
    return minIndex;
}

// Helper function to print the path from the start vertex to the given vertex
void Graph::printPath(int vertex) const {
    if (vertex == -1) {
        return;
    }
    printPath(parent[vertex]);
    std::cout << vertex << " ";
}

// Function to set edge weight in the adjacency matrix
void Graph::setEdge(int u, int v, int weight) {
    length[u][v] = weight;
}

// Accessor to get the distance to a vertex
int Graph::getDistance(int vertex) const {
    return dist[vertex];
}
