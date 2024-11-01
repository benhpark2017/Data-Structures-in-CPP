/**
 * Solutions to Chapter 6, Section 4, Exercise 15 of Horowitz, Sahni, and Mehta's
 * Fundamentals of Data Structures in C++.
 * 
 * Task 15:
 * Modify function AllLengths() so that it obtains a shortest path for all pairs
 * of vertices. What is the computing time of your new function?
 *
 * The time complexity of AllLengths() is still O(n^2) + O(n^3) + O(n^2) =
 * O(n^3), where n is the number of vertices. 
 */
 
/**********************************main.cpp************************************/
#include "Graph.h"

// Example usage
int main() {
    int n = 6;
    Graph g(n);
    
    g.addEdge(0, 1, 20); 
    g.addEdge(1, 0, 2);  
    g.addEdge(0, 2, 15);  
    g.addEdge(1, 4, 10);  
    g.addEdge(1, 5, 30);
    g.addEdge(2, 5, 10);
    g.addEdge(2, 3, 4);  
    g.addEdge(5, 3, 4);  
    g.addEdge(5, 4, 10);  
    g.addEdge(4, 3, 15); 
    
    // Find and print all shortest paths
    g.AllLengths(n);
    g.PrintShortestPaths(n);
    
    return 0;
}



/***********************************Graph.h************************************/
#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <limits>

const int nMax = 100;

enum Boolean {FALSE, TRUE};

class Graph {
private:
    int length[nMax][nMax];  // Distance matrix
    int path[nMax][nMax];    // Path matrix
    int dist[nMax];
    Boolean s[nMax];
    
    void PrintPath(int start, int end) const;
    
public:
    Graph(int n);
    void addEdge(int start, int end, int weight);
    void AllLengths(const int n);
    void PrintShortestPaths(const int n) const;
};

#endif // GRAPH_H



/**********************************Graph.cpp***********************************/
#include "Graph.h"

void Graph::PrintPath(int start, int end) const {
    if (path[start][end] == -1) {
        std::cout << start << " -> " << end;
        return;
    }
    PrintPath(start, path[start][end]);
    std::cout << " -> " << end;
}

Graph::Graph(int n) {  // Constructor to initialize the graph
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            length[i][j] = 0;  // Initialize with no edges
            path[i][j] = -1;
        }
    }
}
    
void Graph::addEdge(int start, int end, int weight) {
    if (start >= 0 && end >= 0 && weight > 0) {
        length[start][end] = weight;
        // For undirected graph, uncomment the following line:
        // length[end][start] = weight;
    }
}

// Modified implementation of AllLengths following directions of Exercise 15
void Graph::AllLengths(const int n) {
    int a[nMax][nMax];
    
    // Initialize the distance matrix and path matrix
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == j) {
                a[i][j] = 0;
                path[i][j] = -1;
            }
            else if (length[i][j] == 0) {
                a[i][j] = std::numeric_limits<int>::max();
                path[i][j] = -1;
            }
            else {
                a[i][j] = length[i][j];
                path[i][j] = -1;
            }
        }
    }
    
    // Floyd-Warshall algorithm with path tracking
    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (a[i][k] != std::numeric_limits<int>::max() && a[k][j] != std::numeric_limits<int>::max()) {
                    if (a[i][k] + a[k][j] < a[i][j]) {
                        a[i][j] = a[i][k] + a[k][j];
                        path[i][j] = k;
                    }
                }
            }
        }
    }
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            length[i][j] = a[i][j];
        }
    }
}

void Graph::PrintShortestPaths(const int n) const {
    std::cout << "\nShortest Paths between all pairs of vertices:\n";
    std::cout << "----------------------------------------\n";
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i != j) {
                std::cout << "\nFrom vertex " << i << " to vertex " << j << ":\n";
                if (length[i][j] == std::numeric_limits<int>::max()) {
                    std::cout << "No path exists\n";
                } else {
                    std::cout << "Distance: " << length[i][j] << "\n";
                    std::cout << "Path: ";
                    PrintPath(i, j);
                    std::cout << "\n";
                }
            }
        }
    }
}
