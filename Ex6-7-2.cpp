/**
 * Solutions to Chapter 6, Section 7, Exercise 2 of Horowitz, Sahni, and Mehta's
 * Fundamentals of Data Structures in C++.
 * 
 * Task 2:
 * A bipartite graph G = (V, E) is an undirected graph whose vertices can be
 * partitioned into two disjoint sets, A and B = V - A, with the following
 * properties.
 * (1) No two vertices in A are adjacent in G;
 * (2) No two vertices in B are adjacent in G;
 * The graph G4 of Figure 6.5 is bipartite. A possible partitioning of V is 
 * A = {o, 3, 4, 6} and B = {1, 2, 5, 7}. Write a C++ function to determine
 * whether a graph G is bipartite. If G is bipartite your algorithm should
 * obtain a partitioning of the vertices into two disjoint sets, A and B, 
 * satisfying properties (1) and (2) above. Show that if G is represented by its
 * adjacency lists, then this algorithm can be made to work in time O(n + e),
 * where n = |V| and e = |E| and |A| represents the cardinality of set A.
 */

/**********************************main.cpp************************************/
#include "Graph.h"

int main() {
    std::cout << "A graph with two connected components as shown in p.335."
              << std::endl;
    Graph graph1(8);
    graph1.AddEdge(0, 1);
    graph1.AddEdge(0, 2);
    graph1.AddEdge(2, 3);
    graph1.AddEdge(1, 3);
    graph1.AddEdge(4, 5);
    graph1.AddEdge(5, 6);
    graph1.AddEdge(6, 7);
    graph1.isBipartite();
    
    std::cout << "A graph that is not bipartite." << std::endl;
    Graph graph2(6);
    graph2.AddEdge(0, 1);
    graph2.AddEdge(1, 2);
    graph2.AddEdge(2, 3);
    graph2.AddEdge(3, 4);
    graph2.AddEdge(4, 5);
    graph2.AddEdge(5, 0);
    graph2.isBipartite();
    
    std::cout << "A graph to show that reversing the vertices of an incident\n"
              << "edge does not change the graph's bipartite property."
              << std::endl;
    Graph graph3(6);
    graph3.AddEdge(0, 1);
    graph3.AddEdge(2, 3);
    graph3.AddEdge(4, 3); //Let's try a curveball by reversing the order.
    graph3.AddEdge(4, 5);
    graph3.isBipartite();
    
    std::cout << "Let's end the program with a complicated final example."
              << std::endl;
    Graph graph4(14);
    graph4.AddEdge(0, 1);
    graph4.AddEdge(0, 2);
    graph4.AddEdge(1, 3);
    graph4.AddEdge(1, 4);
    graph4.AddEdge(2, 5);
    graph4.AddEdge(2, 6);
    graph4.AddEdge(4, 5);
    graph4.AddEdge(1, 6);
    graph4.AddEdge(7, 8);
    graph4.AddEdge(8, 9);
    graph4.AddEdge(9, 10);
    graph4.AddEdge(7, 10);
    graph4.AddEdge(11, 10);
    graph4.AddEdge(10, 12);
    graph4.AddEdge(12, 13);
    graph4.isBipartite();
    
    std::cout << "End of program." << std::endl;
    
    return 0;
}



/**********************************Graph.h*************************************/
#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <vector>

enum Boolean {FALSE, TRUE};

struct Vertex {
    int id;
    Boolean mark;
    
    Vertex(int identifier = 0);
    bool operator==(const Vertex& other) const;
};

class GraphEdge {
    friend class Graph;
  private:
    Boolean m;
    Vertex vertex1, vertex2;
    GraphEdge *path1, *path2;
};

using EdgePtr = GraphEdge*;

class Graph {
  private:
    bool* visited;
    int n;
    EdgePtr* HeadNodes;
  
  public:
    Graph();
    Graph(const int vertices);
    void AddEdge(int from, int to);
    bool isBipartite();
    ~Graph() {}
};

#endif // GRAPH_H


/*********************************Graph.cpp************************************/
#include "Graph.h"
#include <queue>
#include <algorithm>
#include <limits>

Vertex::Vertex(int identifier) : id(identifier), mark(FALSE) {}

bool Vertex::operator==(const Vertex& other) const {
    return id == other.id;
}

Graph::Graph() : HeadNodes(nullptr), n(0), visited(nullptr) {}

Graph::Graph(const int vertices) : n(vertices) {
    visited = new bool[n]();
    HeadNodes = new EdgePtr[n]();
}

void Graph::AddEdge(int from, int to) {
    EdgePtr newEdge = new GraphEdge;
    newEdge->vertex1.id = from;
    newEdge->vertex2.id = to;
    newEdge->m = FALSE;
    
    newEdge->path1 = HeadNodes[from];
    HeadNodes[from] = newEdge;
    
    EdgePtr reverseEdge = new GraphEdge;
    reverseEdge->vertex1.id = to;
    reverseEdge->vertex2.id = from;
    reverseEdge->m = FALSE;
    
    reverseEdge->path1 = HeadNodes[to];
    HeadNodes[to] = reverseEdge;
}

// Function to check if the graph is bipartite and display partitions if it is
bool Graph::isBipartite() {
    if (n <= 0) return true;

    std::vector<int> color(n, -1); // -1 means uncolored
    std::vector<std::vector<int>> components; // Store components
    std::vector<bool> visited(n, false);

    // First identify all components using DFS
    for (int start = 0; start < n; ++start) {
        if (!visited[start]) {
            std::vector<int> currentComponent;
            std::queue<int> q;

            q.push(start);
            visited[start] = true;

            while (!q.empty()) {
                int v = q.front();
                q.pop();
                currentComponent.push_back(v);

                EdgePtr edge = HeadNodes[v];
                while (edge) {
                    int u = edge->vertex2.id;
                    if (!visited[u]) {
                        visited[u] = true;
                        q.push(u);
                    }
                    edge = edge->path1;
                }
            }

            std::sort(currentComponent.begin(), currentComponent.end());
            components.push_back(currentComponent);
        }
    }

    // Color vertices - alternating between components
    for (size_t i = 0; i < components.size(); ++i) {
        int componentColor = i % 2; // Alternate between 0 and 1
        for (int vertex : components[i]) {
            color[vertex] = componentColor;
        }
    }

    // Check for empty sets - this indicates a non-bipartite graph
    bool hasSetA = false, hasSetB = false;
    for (int i = 0; i < n; ++i) {
        if (color[i] == 0) hasSetA = true;
        if (color[i] == 1) hasSetB = true;
    }

    if (!hasSetA || !hasSetB) {
        std::cout << "The graph is not bipartite.\n";
        std::cout << "Set A: ";
        for (int i = 0; i < n; ++i) {
            std::cout << i << " ";
        }
        std::cout << "\nSet B: \n";
        std::cout << std::endl;
        return false;
    }

    // Display the partitions
    std::cout << "The graph is bipartite.\n";
    
    std::cout << "Set A: ";
    for (int i = 0; i < n; ++i) {
        if (color[i] == 0) std::cout << i << " ";
    }
    std::cout << std::endl;
    
    std::cout << "Set B: ";
    for (int i = 0; i < n; ++i) {
        if (color[i] == 1) std::cout << i << " ";
    }
    std::cout << std::endl << std::endl;

    return true;
}
