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
  
    void DFSAdjListUtil(int vertex);
    void BFSAdjListUtil(int startVertex);
    void ClearGraph();

  public:
    Graph();
    Graph(const int vertices);
    void ReadAdjList();
    void PrintAdjList() const;
    void DFSAdjList();
    void BFSAdjList();
    void AddEdge(int from, int to);
    bool isBipartite();
    ~Graph() {} // implementation without ClearGraph() function.
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

void Graph::DFSAdjListUtil(int vertex) {
    visited[vertex] = TRUE;
    std::cout << vertex << " ";
    
    EdgePtr current = HeadNodes[vertex];
    while (current) {
        int nextVertex;
        if (current->vertex1.id == vertex) nextVertex = current->vertex2.id;
        else nextVertex = current->vertex1.id;
        
        if (!visited[nextVertex]) {
            DFSAdjListUtil(nextVertex);
        }
        
        current = current->path1;
    }
}

void Graph::BFSAdjListUtil(int startVertex) {
    std::vector<int> queue;
    
    visited[startVertex] = TRUE;
    queue.push_back(startVertex);
    
    while (!queue.empty()) {
        int currentVertex = queue.front();
        std::cout << currentVertex << " ";
        queue.erase(queue.begin());
        
        EdgePtr current = HeadNodes[currentVertex];
        while (current) {
            int adjacentVertex;
            
            if (current->vertex1.id == currentVertex) {
                adjacentVertex = current->vertex2.id;
            } else {
                adjacentVertex = current->vertex1.id;
            }
            
            if (!visited[adjacentVertex]) {
                visited[adjacentVertex] = TRUE;
                queue.push_back(adjacentVertex);
            }
            
            current = current->path1;
        }
    }
}

void Graph::ClearGraph() {
    if (HeadNodes) {
        for (int i = 0; i < n; i++) {
            EdgePtr current = HeadNodes[i];
            while (current) {
                EdgePtr temp = current;
                current = current->path1;
                delete temp;
            }
        }
        delete[] HeadNodes;
        HeadNodes = nullptr;
    }
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

void Graph::ReadAdjList() {
    std::cout << "Enter the number of vertices: ";
    std::cin >> n;
    
    ClearGraph();
    
    HeadNodes = new EdgePtr[n];
    for (int i = 0; i < n; i++) {
        HeadNodes[i] = nullptr;
    }
    
    std::cout << "\nFor each vertex, enter its adjacent vertices.\n";
    std::cout << "Enter -1 to end the list for a vertex.\n\n";
    
    for (int i = 0; i < n; i++) {
        std::cout << "Enter adjacent vertices for vertex " << i << ": ";
        
        while (TRUE) {
            int adjacent;
            std::cin >> adjacent;
            
            if (adjacent == -1) break;
            
            if (adjacent < 0 || adjacent >= n || adjacent == i) {
                std::cout << "Invalid vertex. Please enter a number between 0 and " 
                         << n - 1 << " (excluding " << i << ") or -1 to end: ";
                continue;
            }
            
            bool edgeExists = FALSE;
            EdgePtr current = HeadNodes[i];
            while (current) {
                if ((current->vertex1.id == i && current->vertex2.id == adjacent) ||
                    (current->vertex1.id == adjacent && current->vertex2.id == i)) {
                    edgeExists = TRUE;
                    break;
                }
                current = current->path1;
            }
            
            if (!edgeExists) {
                AddEdge(i, adjacent);
            }
        }
    }
    
    std::cout << "\nGraph has been successfully read.\n";
}

void Graph::PrintAdjList() const {
    if (!HeadNodes) {
        std::cout << "Graph is empty. Please read the adjacency list first.\n";
        return;
    }

    std::cout << "\nAdjacency List:\n";
    for (int i = 0; i < n; i++) {
        std::cout << i << " -> ";
        EdgePtr current = HeadNodes[i];
        bool first = TRUE;
        
        while (current) {
            if (!first) {
                std::cout << ", ";
            }
            std::cout << current->vertex2.id;
            current = current->path1;
            first = FALSE;
        }
        std::cout << std::endl;
    }
}

void Graph::DFSAdjList() {
    if (!HeadNodes) {
        std::cout << "Graph is empty. Please read the adjacency list first.\n";
        return;
    }

    std::cout << "\nDepth First Search starting from vertex 0: ";
    
    visited = new bool[n];
    for (int i = 0; i < n; i++) {
        visited[i] = FALSE;
    }
    
    DFSAdjListUtil(0);
    
    for (int i = 0; i < n; i++) {
        if (!visited[i]) {
            std::cout << "\nDisconnected component starting from vertex " << i << ": ";
            DFSAdjListUtil(i);
        }
    }
    
    std::cout << std::endl;
    
    delete[] visited;
    visited = nullptr;
}

void Graph::BFSAdjList() {
    if (!HeadNodes) {
        std::cout << "Graph is empty. Please read the adjacency list first.\n";
        return;
    }
    
    visited = new bool[n];
    for (int i = 0; i < n; i++) {
        visited[i] = FALSE;
    }
    
    std::cout << "\nBFS traversal starting from vertex 0: ";
    BFSAdjListUtil(0);
    
    for (int i = 0; i < n; i++) {
        if (!visited[i]) {
            std::cout << "\nDisconnected component starting from vertex " << i << ": ";
            BFSAdjListUtil(i);
        }
    }
    std::cout << std::endl;
    
    delete[] visited;
    visited = nullptr;
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
