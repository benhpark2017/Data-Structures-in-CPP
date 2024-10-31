/**
 * Solutions to Chapter 6, Section 4, Exercise 6 part (b) of Horowitz, Sahni,
 * and Mehta's Fundamentals of Data Structures in C++.
 * 
 * Task 6 (b):
 * Rewrite algorithm ShortestPath under the following assumptions:
 * 
 * Instead of S (the set of vertices to which the shortest paths have already
 * been found), the set T = V(G) - S is represented using a linked list. 
 */
 
/**********************************main.cpp************************************/
#include "Graph.h"
#include <iostream>

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



/**********************************Graph.h*************************************/
#include <iostream>
#include <limits>

class Graph {
private:
    struct Node {
        int vertex;    // Destination vertex (or vertex ID in T list)
        int length;    // Edge weight (or 0 in T list for simplicity)
        Node* link;    // Pointer to the next node in the adjacency list or T list
        Node(int v, int len = 0, Node* nxt = nullptr) : vertex(v), length(len), link(nxt) {}
    };

    int n;                 // Number of vertices
    Node** adjList;        // Array of pointers to represent the adjacency list
    int* dist;             // Distance array to store shortest distances
    Node* T;               // Linked list head for the set T (unprocessed vertices)

    int choose() {         // Main difference between exercise parts a and b
        Node* prev = nullptr;
        Node* minNode = nullptr;
        Node* minNodePrev = nullptr;
        int minDist = std::numeric_limits<int>::max();
        
        // Traverse T to find the vertex with the minimum distance
        for (Node* curr = T; curr != nullptr; prev = curr, curr = curr->link) {
            if (dist[curr->vertex] < minDist) {
                minDist = dist[curr->vertex];
                minNode = curr;
                minNodePrev = prev;
            }
        }

        if (minNode == nullptr) return -1; // No vertex found

        // Remove minNode from T list
        int chosenVertex = minNode->vertex;
        if (minNodePrev) minNodePrev->link = minNode->link;
        else T = minNode->link;
        delete minNode;

        return chosenVertex;
    }

public:
    Graph(int vertices) : n(vertices), T(nullptr) {
        adjList = new Node*[n];
        for (int i = 0; i < n; i++) adjList[i] = nullptr;
        dist = new int[n];

        // Initialize T list with all vertices
        for (int i = n - 1; i >= 0; i--) {
            T = new Node(i, 0, T);
        }
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

        // Clean up the T list
        while (T != nullptr) {
            Node* temp = T;
            T = T->link;
            delete temp;
        }
    }

    void addEdge(int u, int v, int length) {
        adjList[u] = new Node(v, length, adjList[u]);
    }

    void ShortestPath(int start) {
        // Initialize distance array
        for (int i = 0; i < n; i++) {
            dist[i] = std::numeric_limits<int>::max();
        }
        dist[start] = 0;

        for (int i = 0; i < n - 1; i++) {
            int u = choose();
            if (u == -1) break; // No reachable unprocessed vertex left

            Node* p = adjList[u];
            while (p != nullptr) {
                int w = p->vertex;
                int weight = p->length;
                if (dist[u] != std::numeric_limits<int>::max() && dist[u] + weight < dist[w]) {
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
                std::cout << "Distance to " << i << " is " << dist[i] << std::endl;
        }
        std::cout << std::endl;
    }
};
