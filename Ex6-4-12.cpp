/**
 * Solutions to Chapter 6, Section 4, Exercise 12 of Horowitz, Sahni, and Mehta's
 * Fundamentals of Data Structures in C++.
 * 
 * Task 12:
 * Rewrite function BellmanFord() by replacing the second outer loop with code
 * that uses a queue of vertices that may potentially result in a reduction of
 * other dist vertices. This queue initially contains all vertices that are
 * adjacent from the source vertex v. On each successive iteration of the new
 * loop, a vertex i is removed from the queue (unless the queue is empty), and
 * the dist values to vertices adjacent from i are updated as in the final if
 * statement of the original BellmanFord() algorithm. When the dist value of a
 * vertex is reduced because of this, it is added to the queue unless it is
 * already on the queue.
 * (a) Prove that the new function produces the same results as the original
 *     one.
 * (b) Show that the complexity of the new function is no more than that of
 *     the original one.
 */

/**********************************main.cpp************************************/
#include "Graph.h"

int main() {
    Graph g(5);
    
    // Add edges
    g.addEdge(0, 1, 6);
    g.addEdge(0, 2, 7);
    g.addEdge(1, 2, 8);
    g.addEdge(1, 3, -4);
    g.addEdge(2, 3, 9);
    g.addEdge(2, 4, -3);
    g.addEdge(3, 4, 7);
    
    // Find shortest paths from vertex 0
    g.BellmanFord(5, 0);
    
    // Print results
    g.printDistances();
    
    return 0;
}



/**********************************Graph.h************************************/
#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <queue>
#include <vector>
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
    
    // Helper function to check if a vertex is in the queue
    bool isInQueue(const std::queue<int>& q,
                   const std::vector<bool>& inQueue,
                   int vertex) const;

public:
    Graph(int vertices);

    void BellmanFord(const int n, const int v);

    void addEdge(int u, int v, int weight);

    void printDistances() const;
};

#endif // GRAPH_H



/*********************************Graph.cpp************************************/
#include "Graph.h"

// Helper function to check if a vertex is in the queue
bool Graph::isInQueue(const std::queue<int>& q,
                      const std::vector<bool>& inQueue,
                      int vertex) const {
    return inQueue[vertex];
}

Graph::Graph(int vertices) : n(vertices) {
    // Initialize length matrix with std::numeric_limits<int>::max() (indicating no edge)
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            length[i][j] = (i == j) ? 0 : std::numeric_limits<int>::max();
        }
    }
}

void Graph::BellmanFord(const int n, const int v) {
    // Initialize distances
    for (int i = 0; i < n; i++) {
        dist[i] = (i == v) ? 0 : std::numeric_limits<int>::max();
    }
    
    std::queue<int> vertexQueue;        // Queue for vertices to process
    std::vector<bool> inQueue(n, false); // Track vertices in queue
    
    // Add vertices adjacent to source to queue
    for (int i = 0; i < n; i++) {
        if (i != v && length[v][i] < std::numeric_limits<int>::max()) {
            dist[i] = length[v][i];  // Set initial distance
            vertexQueue.push(i);     // Add to queue
            inQueue[i] = true;       // Mark as in queue
        }
    }
    
    // Process vertices in queue
    while (!vertexQueue.empty()) {
        int i = vertexQueue.front();
        vertexQueue.pop();
        inQueue[i] = false;  // Mark as not in queue
        
        // Check all vertices adjacent from i
        for (int u = 0; u < n; u++) {
            if (length[i][u] < std::numeric_limits<int>::max() &&     // Edge exists
                dist[i] != std::numeric_limits<int>::max() &&         // Source distance is not infinite
                dist[u] > dist[i] + length[i][u]) {
                
                // Update distance
                dist[u] = dist[i] + length[i][u];
                
                // Add u to queue if not already present
                if (!isInQueue(vertexQueue, inQueue, u)) {
                    vertexQueue.push(u);
                    inQueue[u] = true;
                }
            }
        }
        
        // Optional: Add cycle detection
        // If a vertex has been processed more than n-1 times,
        // there might be a negative cycle
        static int iterations = 0;
        if (++iterations > n * (n-1)) {
            std::cout << "Warning: Possible negative cycle detected" << std::endl;
            break;
        }
    }
}

void Graph::addEdge(int u, int v, int weight) {
    if (u >= 0 && u < n && v >= 0 && v < n) {
        length[u][v] = weight;
    } else {
        std::cout << "Invalid vertex indices. Edge not added." << std::endl;
    }
}

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
