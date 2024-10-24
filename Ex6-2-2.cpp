/**
 * Solutions to Chapter 6, Section 2, Exercises 2 to 5 of Horowitz's 
 * Fundamentals of Data Structures in C++.
 * 
 * Task 2:
 * Write a complete C++ function for depth-first search under the assumption
 * that graphs are represented using adjacency matrices. Test the correctness
 * of your function using suitable graphs.
 * 
 * Task 3:
 * Write a complete C++ function for depth-first search under the assumption
 * that graphs are represented using adjacency lists. Test the correctness of
 * your function using suitable graphs.
 * 
 * Task 4:
 * Write a complete C++ function for breadth-first search under the assumption
 * that graphs are represented using adjacency matrices. Test the correctness
 * of your function using suitable graphs.
 * 
 * Task 5 
 * Write a complete C++ function for breadth-first search under the assumption
 * that graphs are represented using adjacency lists. Test the correctness
 * of your function using suitable graphs.
 * 
 */
/*********************************main.cpp*************************************/

#include "Graph.h"

int main() {
    Graph graph1(50), graph2(50), graph3(50), graph4(50);
    
    graph1.ReadAdjMatrix();
    graph1.DFSAdjMatrix();    
    
    graph2.ReadAdjList();
    graph2.PrintAdjList();
    graph2.DFSAdjList();
    
    graph3.ReadAdjMatrix();
    graph3.BFSAdjMatrix();
    
    graph4.ReadAdjList();
    graph4.PrintAdjList();
    graph4.BFSAdjList();
    
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
    bool** adjMatrix;
    EdgePtr* HeadNodes;
  
    void DFSAdjMatrixUtil(int vertex);
    void DFSAdjListUtil(int vertex);
    void BFSAdjMatrixUtil(int startVertex);
    void BFSAdjListUtil(int startVertex);
    void ClearGraph();
    void CleanupAdjMatrix();
    void AddEdge(int from, int to);

  public:
    Graph();
    Graph(const int vertices);
    void InitializeAdjMatrix();
    void ReadAdjMatrix();
    void PrintAdjMatrix() const;
    void DFSAdjMatrix();
    void BFSAdjMatrix();
    void ReadAdjList();
    void PrintAdjList() const;
    void DFSAdjList();
    void BFSAdjList();
    ~Graph();
};

#endif // GRAPH_H



/*********************************Graph.cpp************************************/
#include "Graph.h"
#include <queue>
#include <limits>

Vertex::Vertex(int identifier) : id(identifier), mark(FALSE) {}

bool Vertex::operator==(const Vertex& other) const {
    return id == other.id;
}

Graph::Graph() : HeadNodes(nullptr), n(0), visited(nullptr) {}

Graph::Graph(const int vertices) : n(vertices) {
    InitializeAdjMatrix();
}

void Graph::DFSAdjMatrixUtil(int vertex) {
    visited[vertex] = TRUE;
    std::cout << vertex << " ";
    
    for (int i = 0; i < n; i++) {
        if (adjMatrix[vertex][i] && !visited[i]) {
            DFSAdjMatrixUtil(i);
        }
    }
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

void Graph::BFSAdjMatrixUtil(int startVertex) {
    std::vector<int> queue;
    
    visited[startVertex] = TRUE;
    queue.push_back(startVertex);
    
    while (!queue.empty()) {
        int currentVertex = queue.front();
        std::cout << currentVertex << " ";
        queue.erase(queue.begin());
        
        for (int i = 0; i < n; i++) {
            if (adjMatrix[currentVertex][i] && !visited[i]) {
                visited[i] = TRUE;
                queue.push_back(i);
            }
        }
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

void Graph::CleanupAdjMatrix() {
    if (adjMatrix) {
        for (int i = 0; i < n; i++) {
            delete[] adjMatrix[i];
        }
        delete[] adjMatrix;
        adjMatrix = nullptr;
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

void Graph::InitializeAdjMatrix() {
    adjMatrix = new bool*[n];
    for (int i = 0; i < n; i++) {
        adjMatrix[i] = new bool[n];
        for (int j = 0; j < n; j++) {
            adjMatrix[i][j] = FALSE;
        }
    }
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
    std::cout << "Use 1 for edge, 0 for no edge\n";
    std::cout << "Enter row by row, separating each number by space:\n\n";
    
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
                    std::cout << "Invalid input. Please enter 0 or 1 for position [" 
                              << i << "][" << j << "]: ";
                    continue;
                }
                
                if (value != 0 && value != 1) {
                    std::cout << "Invalid value. Please enter 0 or 1 for position [" 
                              << i << "][" << j << "]: ";
                    continue;
                }
                
                validInput = TRUE;
            } while (!validInput);
            
            adjMatrix[i][j] = (value == 1);
        }
        
        if (std::cin.get() != '\n') {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
    
    bool isSymmetric = TRUE;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < i; j++) {
            if (adjMatrix[i][j] != adjMatrix[j][i]) {
                isSymmetric = FALSE;
                break;
            }
        }
        if (!isSymmetric) break;
    }
    
    if (!isSymmetric) {
        std::cout << "\nWarning: The input matrix is not symmetric. ";
        std::cout << "This means the graph is directed.\n";
    }
    
    std::cout << "\nEntered adjacency matrix:\n";
    PrintAdjMatrix();
}

void Graph::PrintAdjMatrix() const {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            std::cout << (adjMatrix[i][j] ? "1 " : "0 ");
        }
        std::cout << std::endl;
    }
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
    visited = nullptr;
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

Graph::~Graph() {
    ClearGraph();
    CleanupAdjMatrix();
}
