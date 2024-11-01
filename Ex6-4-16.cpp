/**
 * Solutions to Chapter 6, Section 4, Exercise 15 of Horowitz, Sahni, and Mehta's
 * Fundamentals of Data Structures in C++.
 * 
 * Task 16:
 * Use function ALlLengths() to obtain the lengths of the shortest paths between
 * all pairs of vertices in the graph initialized in the main() function (the
 * driver code). Does AllLengths() function give the right answers? Why?
 * 
 * Answer:
 * The AllLengths() function gives the right answer except when there is a
 * negative cycle. A negative cycle means a cycle for which the sum of all the 
 * lengths is negative. In the case of the defined graph with 6 vertices and
 * 10 edges, there is no such negative cycle, which means that the AllLengths()
 * function will properly print accurate values for all pairs of vertices.
 * 
 * By the way, the AllLengths() function is due to computer scientists Robert 
 * Floyd and Stephen Warshall, hence its name the Floyd-Warshall algorithm.
 */

/**********************************main.cpp************************************/
#include "Graph.h"

int main() {
    const int n = 6;
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
    
    g.AllLengths(n);
    g.PrintDistances(n);
    
    return 0;
}



/**********************************Graph.h*************************************/
#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <limits>

const int nMax = 100;

enum Boolean {FALSE, TRUE};

class Graph {
private:
    int length[nMax][nMax];  // Distance matrix
    int dist[nMax];
    Boolean s[nMax];
    
public:
    Graph(int n);
    void addEdge(int start, int end, int weight);
    void AllLengths(const int n);
    void PrintDistances(const int n) const;  // Renamed from PrintShortestPaths
};

#endif // GRAPH_H



/**********************************Graph.cpp***********************************/
#include "Graph.h"

Graph::Graph(int n) {  // Constructor
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            length[i][j] = 0;
        }
    }
}

void Graph::addEdge(int start, int end, int weight) {
    if (start >= 0 && end >= 0 && weight > 0) {
        length[start][end] = weight;
    }
}

// Implementation file
void Graph::AllLengths(const int n) {
    int a[nMax][nMax];
    
    // Initialize the distance matrix
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == j)
                a[i][j] = 0;
            else if (length[i][j] == 0)
                a[i][j] = std::numeric_limits<int>::max();
            else
                a[i][j] = length[i][j];
        }
    }
    
    // Floyd-Warshall algorithm (simplified)
    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (a[i][k] != std::numeric_limits<int>::max() 
                    && a[k][j] != std::numeric_limits<int>::max()) {
                    if (a[i][k] + a[k][j] < a[i][j]) {
                        a[i][j] = a[i][k] + a[k][j];
                    }
                }
            }
        }
    }
    
    // Copy results back to length matrix
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            length[i][j] = a[i][j];
        }
    }
}

void Graph::PrintDistances(const int n) const {
    std::cout << "\nShortest Distances between all pairs of vertices:\n";
    std::cout << "----------------------------------------\n";
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i != j) {
                std::cout << "From vertex " << i << " to vertex " << j << ": ";
                if (length[i][j] == std::numeric_limits<int>::max())
                    std::cout << "No path exists";
                else
                    std::cout << length[i][j];
                std::cout << "\n";
            }
        }
    }
}
