/**
 * Solutions to Chapter 6, Section 1, Exercises 3, 10, and 11 of Horowitz's
 * Fundamentals of Data Structures in C++.
 * 
 * Task 3:
 * Devise a suitable representation for graphs so that they can be stored on 
 * disk. Write an algorithm that reads in such a graph and creates its 
 * adjacency matrix. Write another algorithm that creates the adjacency lists
 * from the disk input.
 * 
 * Task 10:
 * Write a C++ function to input the number of vertices in an undirected graph
 * and its edges one by one and to set up the adjacency-list representation of
 * the graph. You may assume that no edge is input twice. What is the run time
 * of your function as a function of the number of vertices and the number of
 * edges?
 * 
 * Task 11:
 * Do the preceding exercise but this time set up the multilist representation.
 */

/**********************************main.cpp************************************/
#include "Graph.h"

int main() {
    Graph graph1, graph2, graph3;
    graph1.GraphIntoAdjacencyMatrix();

    graph2.GraphIntoAdjacencyList();
    graph2.DisplayAdjacencyList();

    graph3.GraphIntoMultilist();
    graph3.DisplayMultilist();

    std::cout << "End of program." << std::endl;
    return 0;
}



/**********************************Graph.h*************************************/
#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <vector>
#include <algorithm>

struct Vertex {
    int id;
    Vertex(int id) : id(id) {}
};

class GraphEdge {
    friend class Graph;
  private:
    Vertex vertex1, vertex2;
    GraphEdge* path1;
    GraphEdge* path2;
    bool m;
  public:
    GraphEdge(Vertex v1, Vertex v2);
};

using EdgePtr = GraphEdge*;

class Graph {
  private:
    std::vector<std::vector<int>> adjacencyList;
    std::vector<EdgePtr> allEdges;
    EdgePtr* HeadNodes;
    int n;

  public:
    Graph();
    Graph(int vertices);
    ~Graph();

    void InsertVertex(Vertex v);
    void InsertEdge(Vertex u, Vertex v);
    void GraphIntoAdjacencyMatrix();
    void GraphIntoAdjacencyList();
    void DisplayAdjacencyList();
    void GraphIntoMultilist();
    void DisplayMultilist();
};

#endif // GRAPH_H



/*********************************Graph.cpp************************************/
#include "Graph.h"

GraphEdge::GraphEdge(Vertex v1, Vertex v2) 
    : vertex1(v1), vertex2(v2), path1(nullptr), path2(nullptr), m(false) {}

Graph::Graph() : n(0), HeadNodes(nullptr) {}

Graph::Graph(int vertices) : n(vertices) {
    HeadNodes = new EdgePtr[n]();
}

Graph::~Graph() {
    for (EdgePtr edge : allEdges) {
        delete edge;
    }
    delete[] HeadNodes;
}

void Graph::InsertVertex(Vertex v) {
    if (v.id >= n) {
        EdgePtr* newHeadNodes = new EdgePtr[v.id + 1];
        for (int i = 0; i < n; i++) {
            newHeadNodes[i] = HeadNodes[i];
        }
        for (int i = n; i <= v.id; i++) {
            newHeadNodes[i] = nullptr;
        }
        delete[] HeadNodes;
        HeadNodes = newHeadNodes;
        n = v.id + 1;
    }
}

void Graph::InsertEdge(Vertex u, Vertex v) {
    EdgePtr newEdge = new GraphEdge(u, v);
    allEdges.push_back(newEdge);

    if (HeadNodes[u.id] == nullptr) {
        HeadNodes[u.id] = newEdge;
    } else {
        EdgePtr current = HeadNodes[u.id];
        while (current->path1 != nullptr) {
            current = current->path1;
        }
        current->path1 = newEdge;
    }

    if (HeadNodes[v.id] == nullptr) {
        HeadNodes[v.id] = newEdge;
    } else {
        EdgePtr current = HeadNodes[v.id];
        while (current->path2 != nullptr) {
            current = current->path2;
        }
        current->path2 = newEdge;
    }
}

void Graph::GraphIntoAdjacencyMatrix() {
    int numVertices, numEdges;
    std::cout << "Enter the number of vertices: ";
    std::cin >> numVertices;
    std::cout << "Enter the number of edges: ";
    std::cin >> numEdges;

    if (numVertices > n) {
        InsertVertex(Vertex(numVertices - 1));
    }

    std::vector<std::vector<int>> adjacencyMatrix(numVertices, std::vector<int>(numVertices, 0));

    std::cout << "Enter the edges (vertex pairs) separated by new lines:\n";
    for (int i = 0; i < numEdges; ++i) {
        int u, v;
        std::cin >> u >> v;
        InsertEdge(Vertex(u), Vertex(v));
        adjacencyMatrix[u][v] = 1;
        adjacencyMatrix[v][u] = 1;
    }

    std::cout << "\nAdjacency Matrix:\n";
    for (int i = 0; i < numVertices; ++i) {
        for (int j = 0; j < numVertices; ++j) {
            std::cout << adjacencyMatrix[i][j] << " ";
        }
        std::cout << "\n";
    }
}

void Graph::GraphIntoAdjacencyList() {
    int numEdges;
    std::cout << "Enter the number of vertices: ";
    std::cin >> n;
    adjacencyList.resize(n);

    std::cout << "Enter the number of edges: ";
    std::cin >> numEdges;

    std::cout << "Enter the pairs of vertices for each edge:\n";
    for (int i = 0; i < numEdges; ++i) {
        int u, v;
        std::cin >> u >> v;
        if (u >= n || v >= n) {
            std::cerr << "Invalid vertex index. Vertex indices should be between 0 and "
                      << n - 1 << ".\n";
            continue;
        }
        adjacencyList[u].push_back(v);
        adjacencyList[v].push_back(u);
    }
}

void Graph::DisplayAdjacencyList() {
    std::cout << "Adjacency List:\n";
    for (int i = 0; i < n; ++i) {
        std::cout << i << ": ";
        for (size_t j = 0; j < adjacencyList[i].size(); ++j) {
            std::cout << adjacencyList[i][j];
            if (j < adjacencyList[i].size() - 1) {
                std::cout << " -> ";
            }
        }
        std::cout << "\n";
    }
}

void Graph::GraphIntoMultilist() {
    // Ask for number of vertices
    std::cout << "Enter the number of vertices: ";
    std::cin >> n;
        
    // Reallocate HeadNodes array with new size
    delete[] HeadNodes;
    HeadNodes = new GraphEdge*[n]();
        
    int numEdges;
    std::cout << "Enter the number of edges: ";
    std::cin >> numEdges;
        
    std::cout << "Enter the pairs of vertices for each edge:\n";
    for (int i = 0; i < numEdges; ++i) {
        int u, v;
        std::cin >> u >> v;
            
        // Validate input
        if (u >= n || v >= n || u < 0 || v < 0) {
            std::cerr << "Invalid vertex indices. Must be between 0 and " << n - 1 << "\n";
            --i; // Retry this edge
            continue;
        }
            
        // Create new edge
        GraphEdge* newEdge = new GraphEdge(Vertex(u), Vertex(v));
        allEdges.push_back(newEdge);

        // Insert for vertex u
        if (HeadNodes[u] == nullptr) {
            HeadNodes[u] = newEdge;
        } else {
            GraphEdge* current = HeadNodes[u];
            while (current->path1 != nullptr && 
                  ((current->vertex1.id == u && current->path1 != nullptr) ||
                   (current->vertex2.id == u && current->path2 != nullptr))) {
                current = (current->vertex1.id == u) ? current->path1 : current->path2;
            }
            if (current->vertex1.id == u) {
                current->path1 = newEdge;
            } else {
                current->path2 = newEdge;
            }
        }
    
        // Insert for vertex v
        if (HeadNodes[v] == nullptr) {
            HeadNodes[v] = newEdge;
        } else {
            GraphEdge* current = HeadNodes[v];
            while (current->path2 != nullptr && 
                  ((current->vertex1.id == v && current->path1 != nullptr) ||
                   (current->vertex2.id == v && current->path2 != nullptr))) {
                current = (current->vertex1.id == v) ? current->path1 : current->path2;
            }
            if (current->vertex1.id == v) {
                current->path1 = newEdge;
            } else {
                current->path2 = newEdge;
            }
        }
    }
}

void Graph::DisplayMultilist() {
    std::cout << "\nVertex Lists:\n";
    for (int i = 0; i < n; i++) {
        std::cout << "vertex " << i << ": ";
        GraphEdge* current = HeadNodes[i];
        while (current != nullptr) {
            int edgeIndex = std::find(allEdges.begin(), allEdges.end(), current) - allEdges.begin();
            std::cout << "N" << edgeIndex;
            
            // Determine which path to follow based on which vertex we're currently processing
            if (current->vertex1.id == i) {
                current = current->path1;
            } else {
                current = current->path2;
            }
            
            if (current != nullptr) std::cout << " -> ";
        }
        std::cout << " -> null" << std::endl;
    }
        
    std::cout << "\nEdge Nodes:\n";
    for (size_t i = 0; i < allEdges.size(); ++i) {
        GraphEdge* edge = allEdges[i];
        std::string next1 = "0", next2 = "0";
            
        if (edge->path1 != nullptr) {
            next1 = "N" + std::to_string(
                std::find(allEdges.begin(), allEdges.end(), edge->path1) - allEdges.begin()
            );
        }
        if (edge->path2 != nullptr) {
            next2 = "N" + std::to_string(
                std::find(allEdges.begin(), allEdges.end(), edge->path2) - allEdges.begin()
            );
        }
            
        std::cout << "N" << i << ": " << edge->vertex1.id << " " << edge->vertex2.id 
                  << " " << next1 << " " << next2
                  << " Edge (" << edge->vertex1.id << "," << edge->vertex2.id << ")" << std::endl;
    }
}
