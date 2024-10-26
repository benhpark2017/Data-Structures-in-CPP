/**
 * Solutions to Chapter 6, Section 2, Exercises 9 to 11 of Fundamentals of Data
 * Structures in C++ by Horowitz, Sahni and Mehta.
 *   
 * Task 9: (done)
 * Augment the adjacency list representation of a graph, so that a field of data
 * of type KeyType is associated with each vertex.
 * 
 * Task 10: (done)
 * Assume the graph representation of the previous exercise. Define an iterator
 * class BFSVertIter. A BFSVertIter object must be associated with a Graph
 * object. Define a function BFSVertIter::NextVertex() that returns a pointer
 * to the data object associated with a vertex in the graph in BFS order
 * starting with vertex 0.
 * 
 * Task 11: (done)
 * Repeat Exercise 10 assuming that the graph is to be traversed in DFS order.
 */

/**********************************main.cpp************************************/
#include "Graph.h"
#include "BFSVertIter.h"
#include "DFSVertIter.h"
#include <iostream>

int main() {
    // Create and populate the graph
    Graph<int> graph;
    std::cout << "Please enter the graph's adjacency list:" << std::endl;
    graph.ReadAdjList();

    // Test BFS Iterator
    {
        std::cout << "\nBFS Traversal of the graph:" << std::endl;
        BFSVertIter<int> bfsIterator(graph);
        const int* vertexData;
        while ((vertexData = bfsIterator.NextVertex()) != nullptr) {
            std::cout << *vertexData << " ";
        }
        std::cout << std::endl;
    }  // vertexData goes out of scope here

    // Test DFS Iterator
    {
        std::cout << "\nDFS Traversal of the graph:" << std::endl;
        DFSVertIter<int> dfsIterator(graph);
        const int* vertexData;  // This is fine because previous vertexData is out of scope
        while ((vertexData = dfsIterator.NextVertex()) != nullptr) {
            std::cout << *vertexData << " ";
        }
        std::cout << std::endl;
    }

    std::cout << "\nEnd of program." << std::endl;
    return 0;
}



/**********************************Graph.h************************************/
#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <limits>
#include <algorithm>

enum Boolean { FALSE, TRUE };

template <typename KeyType> class Graph; //forward declaration

template <typename KeyType> class BFSVertIter;

template<typename KeyType>
struct Vertex {
    int id;          // Unique identifier for the vertex
    Boolean mark;    // Mark for graph algorithms
    KeyType data;    // Associated data with the vertex
    
    // Constructors
    Vertex(int identifier = 0) : id(identifier), mark(FALSE) {}
    Vertex(int identifier, const KeyType& vertexData) 
        : id(identifier), mark(FALSE), data(vertexData) {}
};

template<typename KeyType>
class GraphEdge {
    friend class Graph<KeyType>;
private:
    Boolean m;                          // mark field for edge examination
    Vertex<KeyType> vertex1, vertex2;   // Changed to use templated Vertex
    GraphEdge* path1, *path2;
};

template<typename KeyType>
using EdgePtr = GraphEdge<KeyType>*;

template<typename KeyType>
class Graph {
  private:
    bool* visited;
    int n;
    EdgePtr<KeyType>* HeadNodes;
    std::vector<Vertex<KeyType>> vertices;  // Store vertex data

    void DFSAdjListUtil(int vertex);
    void BFSAdjListUtil(int startVertex);
    
    void ClearGraph();
    void CleanupAdjMatrix();
    void AddEdge(int from, int to);

  public:
    Graph();
    Graph(const int vertices);
    ~Graph();

    void DFSAdjList();
    void BFSAdjList();
    void ReadAdjList();
    void PrintAdjList() const;
    
    int GetNumVertices() const { return vertices.size(); }
    const KeyType* GetVertexData(int v) const { return &vertices[v].data; }
    bool IsAdjacent(int v1, int v2) const;
    const KeyType* GetVertexDataPtr(int vertex) const;
};

#include "Graph.tpp"

#endif // GRAPH_H



/**********************************Graph.tpp***********************************/
#include <vector>

template<typename KeyType>
using EdgePtr = GraphEdge<KeyType>*;

template<typename KeyType>
Graph<KeyType>::Graph() : HeadNodes(nullptr), n(0), visited(nullptr) {}

template<typename KeyType>
Graph<KeyType>::Graph(const int numVertices) : n(numVertices) {
    
    // Initialize the vertices vector with default vertices
    vertices.resize(n);
    for (int i = 0; i < n; i++) {
        vertices[i].id = i;
        vertices[i].mark = FALSE;
    }
    
    // Initialize the head nodes array for adjacency list representation
    HeadNodes = new EdgePtr<KeyType>[n];
    for (int i = 0; i < n; i++) {
        HeadNodes[i] = nullptr;
    }
    
    // Initialize visited array
    visited = new bool[n];
    for (int i = 0; i < n; i++) {
        visited[i] = FALSE;
    }
}

template<typename KeyType>
Graph<KeyType>::~Graph() {
    // Clean up adjacency list
    ClearGraph();
    
    // Clean up visited array
    delete[] visited;
    visited = nullptr;
    
    // vertices vector will be automatically cleaned up when it goes out of scope
    // since it's not dynamically allocated
}

// DFS using adjacency list
template<typename KeyType>
void Graph<KeyType>::DFSAdjListUtil(int vertex) {
    visited[vertex] = TRUE;
    std::cout << vertex << " ";
    
    EdgePtr<KeyType> current = HeadNodes[vertex];
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
        
        current = current->path1;
    }
}

// BFS using adjacency list
template<typename KeyType>
void Graph<KeyType>::BFSAdjListUtil(int startVertex) {
    std::vector<int> queue;
    
    visited[startVertex] = TRUE;
    queue.push_back(startVertex);
    
    while (!queue.empty()) {
        int currentVertex = queue.front();
        std::cout << currentVertex << " ";
        queue.erase(queue.begin());
        
        EdgePtr<KeyType> current = HeadNodes[currentVertex];
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

// Clear the adjacency list
template<typename KeyType>
void Graph<KeyType>::ClearGraph() {
    if (HeadNodes) {
        for (int i = 0; i < n; i++) {
            EdgePtr<KeyType> current = HeadNodes[i];
            while (current) {
                EdgePtr<KeyType> temp = current;
                current = current->path1;
                delete temp;
            }
        }
        delete[] HeadNodes;
        HeadNodes = nullptr;
    }
}

// Add an edge to the adjacency list
template<typename KeyType>
void Graph<KeyType>::AddEdge(int from, int to) {
    EdgePtr<KeyType> newEdge = new GraphEdge<KeyType>;
    newEdge->vertex1.id = from;
    newEdge->vertex2.id = to;
    newEdge->m = FALSE;
    
    // If vertices exist, copy their data
    if (static_cast<size_t>(from) < vertices.size()) {
        newEdge->vertex1.data = vertices[from].data;
    }
    if (static_cast<size_t>(to) < vertices.size()) {
        newEdge->vertex2.data = vertices[to].data;
    }
    
    newEdge->path1 = HeadNodes[from];
    HeadNodes[from] = newEdge;
    
    EdgePtr<KeyType> reverseEdge = new GraphEdge<KeyType>;
    reverseEdge->vertex1.id = to;
    reverseEdge->vertex2.id = from;
    reverseEdge->m = FALSE;
    
    // Copy vertex data for reverse edge
    if (static_cast<size_t>(to) < vertices.size()) {
        reverseEdge->vertex1.data = vertices[to].data;
    }
    if (static_cast<size_t>(from) < vertices.size()) {
        reverseEdge->vertex2.data = vertices[from].data;
    }
    
    reverseEdge->path1 = HeadNodes[to];
    HeadNodes[to] = reverseEdge;
}

template<typename KeyType>
void Graph<KeyType>::DFSAdjList() {
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

template<typename KeyType>
void Graph<KeyType>::BFSAdjList() {
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

template<typename KeyType>
void Graph<KeyType>::ReadAdjList() {
    std::cout << "Enter the number of vertices: ";
    std::cin >> n;

    ClearGraph();

    // Initialize HeadNodes array
    HeadNodes = new EdgePtr<KeyType>[n];
    for (int i = 0; i < n; i++) {
        HeadNodes[i] = nullptr;
    }

    // Initialize vertices vector with default values
    vertices.resize(n);
    for (int i = 0; i < n; i++) {
        vertices[i].id = i;
        vertices[i].data = i;  // Initialize data with vertex number
        vertices[i].mark = FALSE;
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
            GraphEdge<KeyType>* current = HeadNodes[i];
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

    /**ATTENTION: The final task will be merging the id and data integers into 
     * one to serve one functionality of storing a unique identifier for each
     * vertex. That way, we can get rid of the need to call SetVertexData.
     */
    std::cout << "\nGraph has been successfully read.\n";
}

//Added this new function.
template<typename KeyType>
void Graph<KeyType>::PrintAdjList() const {
    if (!HeadNodes) {
        std::cout << "Graph is empty. Please read the adjacency list first.\n";
        return;
    }

    std::cout << "\nAdjacency List:\n";
    for (int i = 0; i < n; i++) {
        std::cout << i << " -> ";
        GraphEdge<KeyType>* current = HeadNodes[i];
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

template <typename KeyType>
bool Graph<KeyType>::IsAdjacent(int v1, int v2) const {
    if (v1 < 0 || v2 < 0 || v1 >= n || v2 >= n) return false;
    EdgePtr<KeyType> current = HeadNodes[v1];
    while (current) {
        if ((current->vertex1.id == v1 && current->vertex2.id == v2) ||
            (current->vertex1.id == v2 && current->vertex2.id == v1)) {
            return true;
        }
        current = current->path1;
    }
    return false;
}

template <typename KeyType>
const KeyType* Graph<KeyType>::GetVertexDataPtr(int vertex) const {
    if (vertex < 0 || vertex >= n) return nullptr;
    return &vertices[vertex].data;
}



/*******************************BFSVertIter.h**********************************/
#ifndef BFS_VERT_ITER_H
#define BFS_VERT_ITER_H

#include <queue>
#include <vector>

template <typename KeyType>
class BFSVertIter {
private:
    const Graph<KeyType>& graph;
    std::queue<int> vertexQueue;
    std::vector<bool> visited;
    int numVertices;

public:
    // Constructor
    BFSVertIter(const Graph<KeyType>& g);

    // Changed return type to const KeyType*
    const KeyType* NextVertex();
};

#include "BFSVertIter.tpp"

#endif //BFS_VERT_ITER_H



/******************************BFSVertIter.tpp*********************************/
template <typename KeyType>
BFSVertIter<KeyType>::BFSVertIter(const Graph<KeyType>& g) : graph(g) {
    numVertices = g.GetNumVertices();
    if (numVertices > 0) {
        visited.resize(numVertices, false);
        vertexQueue.push(0);
        visited[0] = true;
    }
}

template <typename KeyType> // Change return type to const KeyType*
const KeyType* BFSVertIter<KeyType>::NextVertex() {
    if (vertexQueue.empty()) {
        for (int i = 0; i < numVertices; i++) {
            if (!visited[i]) {
                vertexQueue.push(i);
                visited[i] = true;
                break;
            }
        }

        if (vertexQueue.empty()) {
            return nullptr;
        }
    }

    int currentVertex = vertexQueue.front();
    vertexQueue.pop();

    for (int i = 0; i < numVertices; i++) {
        if (graph.IsAdjacent(currentVertex, i) && !visited[i]) {
            vertexQueue.push(i);
            visited[i] = true;
        }
    }

    return graph.GetVertexDataPtr(currentVertex);
}



/*******************************DFSVertIter.h**********************************/
#ifndef DFS_VERT_ITER_H
#define DFS_VERT_ITER_H

#include <stack>
#include <vector>

template <typename KeyType>
class DFSVertIter {
private:
    const Graph<KeyType>& graph;
    std::vector<bool> visited;
    std::vector<int> stack;  // Using vector as a stack for DFS
    int numVertices;

public:
    // Constructor
    DFSVertIter(const Graph<KeyType>& g);

    // Returns pointer to next vertex's data in DFS order
    const KeyType* NextVertex();
};

#include "DFSVertIter.tpp"

#endif //DFS_VERT_ITER_H



/******************************DFSVertIter.tpp*********************************/
// Constructor
template <typename KeyType>
DFSVertIter<KeyType>::DFSVertIter(const Graph<KeyType>& g) : graph(g) {
    numVertices = g.GetNumVertices();
    if (numVertices > 0) {
        visited.resize(numVertices, false);
        // Start from vertex 0
        stack.push_back(0);
        visited[0] = true;
    }
}

// Returns pointer to next vertex's data in DFS order
template <typename KeyType>
const KeyType* DFSVertIter<KeyType>::NextVertex() {
    // If stack is empty, look for unvisited vertices
    if (stack.empty()) {
        for (int i = 0; i < numVertices; i++) {
            if (!visited[i]) {
                stack.push_back(i);
                visited[i] = true;
                break;
            }
        }

        // If still empty, we're done
        if (stack.empty()) {
            return nullptr;
        }
    }

    // Get current vertex from stack
    int currentVertex = stack.back();
    stack.pop_back();

    // Push unvisited adjacent vertices to stack
    // Note: We iterate in reverse order to maintain traditional DFS order
    for (int i = numVertices - 1; i >= 0; i--) {
        if (graph.IsAdjacent(currentVertex, i) && !visited[i]) {
            stack.push_back(i);
            visited[i] = true;
        }
    }

    return graph.GetVertexDataPtr(currentVertex);
}
