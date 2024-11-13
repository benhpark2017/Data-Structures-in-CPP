/**
 * Solutions to Chapter 6, Section 7, Exercise 8 of Horowitz, Sahni, and Mehta's 
 * Fundamentals of Data Structures in C++.
 * 
 * Task 8:
 * Another way to represent a graph is by its incidence matrix, INC. There is
 * one row for each vertex and one column for each edge. Then INC[i][j] = 1 
 * if edge j is incident to vertex i. The incidence matrix for the graph of
 * Figure 6.16(a) is given in Figure 6.45. The edges of Figure 6.16(a) have
 * been numbered from left to right and top to bottom. Rewrite function DFS()
 * so that it works on a graph represented by its incidence matrix.
 */
/*********************************main.cpp*************************************/

#include "Graph.h"

int main() {
    Graph graph(50, 100);
    
    graph.ReadIncMatrix();
    graph.DFSIncMatrix();    
    
    std::cout << "End of program." << std::endl;
    return 0;
}



/**********************************Graph.h*************************************/
#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <vector>

enum Boolean {FALSE, TRUE};

class Graph {
private:
    int n;              // number of vertices
    int numEdges;       // number of edges
    bool** incMatrix;   // incidence matrix
    bool* visited;      // for DFS traversal
    
    void DFSIncMatrixUtil(int vertex);
    void CleanupIncMatrix();
    std::vector<int> getAdjacentVertices(int vertex);

public:
    Graph();
    Graph(const int vertices, const int edges);  // Constructor with both parameters
    void InitializeIncMatrix();
    void ReadIncMatrix();
    void PrintIncMatrix() const;
    void DFSIncMatrix();

    ~Graph();
};

#endif //GRAPH_H



/*********************************Graph.cpp************************************/
#include "Graph.h"
#include <sstream>
#include <iomanip>
#include <limits>

Graph::Graph() : n(0), numEdges(0), incMatrix(nullptr), visited(nullptr) {}

Graph::Graph(const int vertices, const int edges) : n(vertices), numEdges(edges) {
    InitializeIncMatrix();
    visited = nullptr;
}

void Graph::InitializeIncMatrix() {
    incMatrix = new bool*[n];
    for (int i = 0; i < n; i++) {
        incMatrix[i] = new bool[numEdges];
        for (int j = 0; j < numEdges; j++) {
            incMatrix[i][j] = FALSE;
        }
    }
}

void Graph::CleanupIncMatrix() {
    if (incMatrix) {
        for (int i = 0; i < n; i++) {
            delete[] incMatrix[i];
        }
        delete[] incMatrix;
        incMatrix = nullptr;
    }
}

std::vector<int> Graph::getAdjacentVertices(int vertex) {
    std::vector<int> adjacent;
    
    // For each edge
    for (int e = 0; e < numEdges; e++) {
        // If current vertex is incident to this edge
        if (incMatrix[vertex][e]) {
            // Find the other vertex incident to this edge
            for (int v = 0; v < n; v++) {
                if (v != vertex && incMatrix[v][e]) {
                    adjacent.push_back(v);
                    break;
                }
            }
        }
    }
    
    return adjacent;
}

void Graph::DFSIncMatrixUtil(int vertex) {
    visited[vertex] = TRUE;
    std::cout << vertex << " ";
    
    // Get all adjacent vertices of the current vertex
    std::vector<int> adjacent = getAdjacentVertices(vertex);
    
    // Recursively visit all unvisited adjacent vertices
    for (int adjVertex : adjacent) {
        if (!visited[adjVertex]) {
            DFSIncMatrixUtil(adjVertex);
        }
    }
}

void Graph::DFSIncMatrix() {
    visited = new bool[n];
    for (int i = 0; i < n; i++) {
        visited[i] = FALSE;
    }
    
    std::cout << "\nDFS traversal starting from vertex 0: ";
    DFSIncMatrixUtil(0);
    
    // Check for disconnected components
    for (int i = 0; i < n; i++) {
        if (!visited[i]) {
            std::cout << "\nDisconnected component starting from vertex " << i << ": ";
            DFSIncMatrixUtil(i);
        }
    }
    std::cout << std::endl;
    
    delete[] visited;
}

void Graph::ReadIncMatrix() {
    CleanupIncMatrix(); // Clean up any existing matrix
    
    std::cout << "\n=== Graph Input ===" << std::endl;
    std::cout << "For each edge in the graph, you will specify which two vertices it connects." << std::endl;
    
    // Get number of vertices
    do {
        std::cout << "\nHow many vertices are in your graph? (Enter a positive number): ";
        std::cin >> n;
        
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            n = 0;
            std::cout << "Invalid input. Please enter a number." << std::endl;
        }
    } while (n <= 0);
    
    // Get number of edges
    do {
        std::cout << "\nHow many edges are in your graph? (Enter a positive number): ";
        std::cin >> numEdges;
        
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            numEdges = 0;
            std::cout << "Invalid input. Please enter a number." << std::endl;
        }
    } while (numEdges <= 0);
    
    // Clear the input buffer after reading numbers
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    InitializeIncMatrix();
    
    std::cout << "\n=== Incidence Matrix Input ===" << std::endl;
    std::cout << "The matrix will have " << n << " rows (vertices) and " << numEdges << " columns (edges)" << std::endl;
    std::cout << "For each row (vertex), enter " << numEdges << " values (0 or 1) separated by spaces" << std::endl;
    std::cout << "Press Enter after entering all values for each row" << std::endl;
    std::cout << "Enter 1 if the vertex is connected to the edge, 0 if it is not\n" << std::endl;
    
    for (int i = 0; i < n; i++) {
        bool validRow = false;
        
        while (!validRow) {
            std::cout << "Row " << i << " (vertex " << i << "): ";
            std::string line;
            std::getline(std::cin, line);
            
            std::istringstream iss(line);
            std::vector<int> rowValues;
            int value;
            bool validInput = true;
            
            // Read all values from the line
            while (iss >> value) {
                if (value != 0 && value != 1) {
                    validInput = false;
                    std::cout << "Invalid value found. Please use only 0 or 1." << std::endl;
                    break;
                }
                rowValues.push_back(value);
            }
            
            // Check if we got the correct number of values
            if (validInput && rowValues.size() != numEdges) {
                validInput = false;
                std::cout << "Wrong number of values. Please enter exactly " << numEdges 
                         << " values separated by spaces." << std::endl;
            }
            
            // If the input is valid, store it in the matrix
            if (validInput) {
                for (int j = 0; j < numEdges; j++) {
                    incMatrix[i][j] = rowValues[j];
                }
                validRow = true;
            }
        }
    }
    
    // Validate that each edge connects exactly two vertices
    bool validGraph = true;
    for (int j = 0; j < numEdges; j++) {
        int vertexCount = 0;
        for (int i = 0; i < n; i++) {
            if (incMatrix[i][j]) vertexCount++;
        }
        if (vertexCount != 2) {
            std::cout << "\nError: Edge " << j << " connects " << vertexCount 
                     << " vertices. Each edge must connect exactly 2 vertices." << std::endl;
            validGraph = false;
        }
    }
    
    if (!validGraph) {
        std::cout << "\nThe graph structure is invalid. Please ensure each edge connects exactly two vertices." << std::endl;
    }
    
    std::cout << "\nEntered incidence matrix:" << std::endl;
    PrintIncMatrix();
    
    if (validGraph) {
        std::cout << "\nGraph structure is valid! You can now perform DFS traversal." << std::endl;
    }
}

// Also modify PrintIncMatrix() to be more readable:
void Graph::PrintIncMatrix() const {
    std::cout << "\n     ";  // Spacing for vertex labels
    for (int j = 0; j < numEdges; j++) {
        std::cout << "E" << std::setw(2) << std::left << j << " ";  // Edge labels
    }
    std::cout << std::endl;
    
    std::cout << "     ";  // Spacing for separator line
    for (int j = 0; j < numEdges; j++) {
        std::cout << "--- ";
    }
    std::cout << std::endl;
    
    for (int i = 0; i < n; i++) {
        std::cout << "V" << std::setw(2) << std::left << i << " |";  // Vertex labels
        for (int j = 0; j < numEdges; j++) {
            std::cout << " " << (incMatrix[i][j] ? "1" : "0") << "  ";
        }
        std::cout << std::endl;
    }
}

Graph::~Graph() {
    CleanupIncMatrix();
    delete[] visited;
}
