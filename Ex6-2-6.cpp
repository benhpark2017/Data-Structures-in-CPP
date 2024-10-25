/**
 * Solutions to Chapter 6, Section 2, Exercise 6 of Fundamentals of Data
 * Structures in C++ by Horowitz, Sahni and Mehta.
 *   
 * Task 6:
 * Show how to modify function DFS(i) as it is used in the Components()
 * function to produce a list of all newly visited vertices.
 */

/**********************************main.cpp************************************/
#include "Graph.h"

int main() {
    Graph graph;
    graph.ReadAdjMatrix();
    graph.PrintAdjMatrix();
    graph.Components();
    
    std::cout << "End of program." << std::endl;
    
    return 0;
}



/**********************************Graph.h************************************/
#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>

enum Boolean { FALSE, TRUE };

// Define the Vertex structure
struct Vertex {
    int id;          // Unique identifier for the vertex
    Boolean mark;    // Mark for graph algorithms
    
    // Constructor
    Vertex(int identifier = 0) : id(identifier), mark(FALSE) {}
    
    // Equality operator for vertex comparison
    bool operator==(const Vertex& other) const {
        return id == other.id;
    }
};

class GraphEdge {
    friend class Graph;
private:
    Boolean m;               // mark field for edge examination
    Vertex vertex1, vertex2; // Changed from int to Vertex
    GraphEdge *path1, *path2;
};

using EdgePtr = GraphEdge*;

class Graph {
private:
    bool* visited;
    int n;
    bool** adjMatrix;
    EdgePtr* HeadNodes;
    int componentCount;
    std::vector<int> currentComponent;
    std::vector<std::pair<int, int>> currentComponentEdges;

    void DFSAdjMatrixUtil(int vertex);      
    void DFSAdjListUtil(int vertex);        
    void BFSAdjMatrixUtil(int startVertex); 
    void BFSAdjListUtil(int startVertex);  
    void ClearGraph();
    void CleanupAdjMatrix();
    void AddEdge(int from, int to);
    void OutputNewComponent();

public:
    Graph();
    Graph(const int vertices);
    ~Graph();

    void Components();
    void InitializeAdjMatrix();
    void ReadAdjMatrix();
    void PrintAdjMatrix() const;
    void DFSAdjMatrix();
    void BFSAdjMatrix();
};

#endif // GRAPH_H



/**********************************Graph.cpp***********************************/
#include "Graph.h"

Graph::Graph() : HeadNodes(nullptr), n(0), visited(nullptr), componentCount(0) {}

Graph::Graph(const int vertices) : n(vertices), componentCount(0) {
    InitializeAdjMatrix();
}

Graph::~Graph() {
    ClearGraph();
    CleanupAdjMatrix();
}

void Graph::InitializeAdjMatrix() {
    adjMatrix = new bool*[n];
    for (int i = 0; i < n; i++) {
        adjMatrix[i] = new bool[n];
        for (int j = 0; j < n; j++) {
            adjMatrix[i][j] = FALSE;
        }
    }
}

void Graph::CleanupAdjMatrix() {
    if (adjMatrix) {
        for (int i = 0; i < n; i++) {
            delete[] adjMatrix[i];
        }
        delete[] adjMatrix;
        adjMatrix = nullptr;
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

void Graph::ReadAdjMatrix() {
    CleanupAdjMatrix();
    
    do {
        std::cout << "Enter the number of vertices (must be positive): ";
        std::cin >> n;
        
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            n = 0;
        }
    } while (n <= 0);
    
    InitializeAdjMatrix();
    
    std::cout << "\nEnter the adjacency matrix (" << n << "x" << n << "):\n";
    
    for (int i = 0; i < n; i++) {
        std::cout << "Row " << i << ": ";
        for (int j = 0; j < n; j++) {
            int value;
            bool validInput = FALSE;
            
            do {
                std::cin >> value;
                if (std::cin.fail()) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Invalid input. Please enter 0 or 1 for position [" << i << "][" << j << "]: ";
                    continue;
                }
                
                if (value != 0 && value != 1) {
                    std::cout << "Invalid value. Please enter 0 or 1 for position [" << i << "][" << j << "]: ";
                    continue;
                }
                
                validInput = TRUE;
            } while (!validInput);
            
            adjMatrix[i][j] = (value == 1);
        }
    }
}

void Graph::PrintAdjMatrix() const {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            std::cout << (adjMatrix[i][j] ? "1 " : "0 ");
        }
        std::cout << std::endl;
    }
}

void Graph::Components() {
    visited = new bool[n];
    for (int i = 0; i < n; i++) visited[i] = FALSE;
    
    componentCount = 0;
    
    for (int i = 0; i < n; i++) {
        if (!visited[i]) {
            currentComponent.clear();
            currentComponentEdges.clear();
            
            componentCount++;
            DFSAdjMatrixUtil(i);
            
            OutputNewComponent();
        }
    }
    delete[] visited;
}

void Graph::DFSAdjMatrixUtil(int vertex) {
    visited[vertex] = true;
    currentComponent.push_back(vertex);
    
    for (int i = 0; i < n; i++) {
        if (adjMatrix[vertex][i]) {
            if (vertex < i) {
                currentComponentEdges.push_back({vertex, i});
            }
            
            if (!visited[i]) {
                DFSAdjMatrixUtil(i);
            }
        }
    }
}

void Graph::OutputNewComponent() {
    std::sort(currentComponent.begin(), currentComponent.end());
    std::cout << "\nComponent " << componentCount << ":\n";
    std::cout << "Vertices: ";
    for (size_t i = 0; i < currentComponent.size(); i++) {
        std::cout << currentComponent[i];
        if (i < currentComponent.size() - 1) std::cout << ", ";
    }
    std::cout << "\n";
    
    std::sort(currentComponentEdges.begin(), currentComponentEdges.end());
    
    std::cout << "Edges: ";
    if (currentComponentEdges.empty()) {
        std::cout << "none";
    } else {
        for (size_t i = 0; i < currentComponentEdges.size(); i++) {
            std::cout << "(" << currentComponentEdges[i].first 
                     << "," << currentComponentEdges[i].second << ")";
            if (i < currentComponentEdges.size() - 1) std::cout << ", ";
        }
    }
    std::cout << "\n";
}

void Graph::DFSAdjMatrix() {
    visited = new bool[n];
    for (int i = 0; i < n; i++) {
        visited[i] = FALSE;
    }
    
    std::cout << "\nDFS traversal starting from vertex 0: ";
    DFSAdjMatrixUtil(0);
    
    for (int i = 0; i < n; i++) {
        if (!visited[i]) {
            std::cout << "\nDisconnected component starting from vertex " << i << ": ";
            DFSAdjMatrixUtil(i);
        }
    }
    std::cout << std::endl;
    
    delete[] visited;
}


void Graph::DFSAdjListUtil(int vertex) {
    visited[vertex] = TRUE;
    std::cout << vertex << " ";
        
    EdgePtr current = HeadNodes[vertex];
    while (current) {
        int nextVertex;
        if (current->vertex1.id == vertex) {
            nextVertex = current->vertex2.id;
        } else {
            nextVertex = current->vertex1.id;
        }
            
        if (!visited[nextVertex]) {
            DFSAdjListUtil(nextVertex);
        }
            
        // Always move to path1 since we're storing edges in a single direction
        current = current->path1;
    }
}

void Graph::BFSAdjMatrix() {
    if (!adjMatrix) {
        std::cout << "Graph is empty. Please read the adjacency matrix first.\n";
        return;
    }

    visited = new bool[n];
    for (int i = 0; i < n; i++) {
        visited[i] = FALSE;
    }
    
    std::cout << "\nBFS traversal starting from vertex 0: ";
    BFSAdjMatrixUtil(0);
    
    for (int i = 0; i < n; i++) {
        if (!visited[i]) {
            std::cout << "\nDisconnected component starting from vertex " << i << ": ";
            BFSAdjMatrixUtil(i);
        }
    }
    std::cout << std::endl;
    
    delete[] visited;
}

void Graph::BFSAdjMatrixUtil(int startVertex) {
    // Create a queue for BFS
    std::vector<int> queue;
        
    // Mark the current vertex as visited and enqueue it
    visited[startVertex] = TRUE;
    queue.push_back(startVertex);
        
    while (!queue.empty()) {
        // Dequeue a vertex and print it
        int currentVertex = queue.front();
        std::cout << currentVertex << " ";
        queue.erase(queue.begin());
            
        // Get all adjacent vertices of the dequeued vertex
        // If an adjacent has not been visited, then mark it visited
        // and enqueue it
        for (int i = 0; i < n; i++) {
            if (adjMatrix[currentVertex][i] && !visited[i]) {
                visited[i] = TRUE;
                queue.push_back(i);
            }
        }
    }
}

void Graph::BFSAdjListUtil(int startVertex) {
    // Create a queue for BFS
    std::vector<int> queue;
        
    // Mark the current vertex as visited and enqueue it
    visited[startVertex] = TRUE;
    queue.push_back(startVertex);
        
    while (!queue.empty()) {
        // Dequeue a vertex and print it
        int currentVertex = queue.front();
        std::cout << currentVertex << " ";
        queue.erase(queue.begin());
            
        // Get all adjacent vertices of the dequeued vertex
        EdgePtr current = HeadNodes[currentVertex];
        while (current) {
            // Get the adjacent vertex ID
            int adjacentVertex;
            if (current->vertex1.id == currentVertex) {
                adjacentVertex = current->vertex2.id;
            } else {
                adjacentVertex = current->vertex1.id;
            }
                
            // If adjacent vertex is not visited, mark it and enqueue it
            if (!visited[adjacentVertex]) {
                visited[adjacentVertex] = TRUE;
                queue.push_back(adjacentVertex);
            }
                
            // Move to next edge in the adjacency list
            current = current->path1;
        }
    }
}
