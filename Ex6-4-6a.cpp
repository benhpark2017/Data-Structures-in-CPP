/**
 * Solutions to Chapter 6, Section 4, Exercise 6 part (a) of Horowitz, Sahni, 
 * and Mehta's Fundamentals of Data Structures in C++.
 * 
 * Task 6 (a):
 * Rewrite algorithm ShortestPath under the following assumptions:
 * 
 * G is represented by its adjacency lists, where each node has three fields:
 * vertex, length, and link. length is the length of the corresponding edge,
 * and n the number of vertices of G.
 */

/**********************************main.cpp************************************/
#include "Graph.h"

int main() {
    int numVertices = 5;
    Graph g(numVertices);

    // Add edges to the graph (directed edges with weights)
    g.addEdge(0, 1, 10);
    g.addEdge(0, 3, 5);
    g.addEdge(1, 2, 1);
    g.addEdge(1, 3, 2);
    g.addEdge(2, 4, 4);
    g.addEdge(3, 1, 3);
    g.addEdge(3, 2, 9);
    g.addEdge(3, 4, 2);
    g.addEdge(4, 0, 7);
    g.addEdge(4, 2, 6);

    // Choose a starting vertex for the shortest path search
    int startVertex = 0;
    std::cout << "Shortest paths from vertex " << startVertex << ":\n";
    
    // Run the shortest path algorithm
    g.ShortestPath(startVertex);

    // Print the shortest path distances
    g.printDistances();

    return 0;
}



/**********************************Graph.h************************************/

#include <iostream>
#include <limits> // For std::numeric_limits<int>::max()

enum Boolean {FALSE, TRUE};

class Graph {
private:
    struct Node {
        int vertex;     // Destination vertex
        int length;     // Edge weight
        Node* link;     // Pointer to the next node in the list
        Node(int v, int len, Node* nxt = nullptr) : vertex(v), length(len), link(nxt) {}
    };
    
    int n;                // Number of vertices
    Node** adjList;       // Array of pointers to represent the adjacency list
    int* dist;            // Distance array
    bool* s;              // Processed set

    int choose() {
        int minDist = std::numeric_limits<int>::max();
        int minVertex = -1;
        for (int i = 0; i < n; i++) {
            if (!s[i] && dist[i] < minDist) {
                minDist = dist[i];
                minVertex = i;
            }
        }
        return minVertex;
    }
    
public:
    Graph(int vertices) : n(vertices) {
        adjList = new Node*[n];
        for (int i = 0; i < n; i++) adjList[i] = nullptr;
        dist = new int[n];
        s = new bool[n];
    }
    
    ~Graph() {
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
        delete[] s;
    }

    void addEdge(int u, int v, int length) {
        adjList[u] = new Node(v, length, adjList[u]);
    }

    void ShortestPath(int start) {
        for (int i = 0; i < n; i++) {
            dist[i] = std::numeric_limits<int>::max();
            s[i] = false;
        }
        dist[start] = 0;
        
        for (int i = 0; i < n - 1; i++) {
            int u = choose();
            if (u == -1) break; // No reachable unprocessed vertex left
            s[u] = true;

            Node* p = adjList[u];
            while (p != nullptr) {
                int w = p->vertex;
                int weight = p->length;
                if (!s[w] && dist[u] != std::numeric_limits<int>::max() && dist[u] + weight < dist[w]) {
                    dist[w] = dist[u] + weight;
                }
                p = p->link;
            }
        }
    }

    void printDistances() const {
        for (int i = 0; i < n; i++) {
            if (dist[i] == std::numeric_limits<int>::max())
                std::cout << "INF ";
            else
                std::cout << "Distance to " << i << " is "<<  dist[i] << std::endl;
        }
        std::cout << std::endl;
    }
};

