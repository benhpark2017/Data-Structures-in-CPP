/**
 * Solutions to Chapter 6, Section 2, Exercises 12 to 14 of Fundamentals of Data
 * Structures in C++ by Horowitz, Sahni and Mehta.
 *   
 * Task 12:
 * Augment the adjacency list representation of a graph, so that a field data
 * of type KeyType is associated with each edge.
 * 
 * Task 13:
 * Assume the graph representation of the previous exercise. Define an iterator
 * class BFSEdgeIter. A BFSEdgeIter object must be associated with a Graph
 * object. Define a function BFSEdgeIter::NextEdge() that returns a pointer to
 * the data object associated with an edge in the graph in BFS order starting
 * with any edge incident on vertex 0.
 * 
 * Task 14:
 * Repeat the previous exercise assuming that the graph is to be traversed in 
 * DFS order.
 */

/**********************************main.cpp************************************/
#include "Graph.h"
#include "BFSEdgeIter.h"
#include "DFSEdgeIter.h"

int main() {
    // Create a graph with std::pair<int, int> as KeyType
    Graph<std::pair<int, int>> graph;
    
    // Create example edge list
    std::vector<std::pair<int, int>> edges = {
        {0, 1}, {0, 2}, {1, 3}, {1, 4}, {2, 5},
        {2, 6}, {3, 7}, {4, 7}, {5, 7}, {6, 7}
    };
    
    // Build graph from edge list
    graph.ReadGraphFromEdgeList(edges);
    
    // Print the graph
    std::cout << "\nGraph structure:\n";
    graph.PrintGraph();
    
    // Use BFS iterator to traverse edges
    std::cout << "\nBFS Edge Traversal:\n";
    BFSEdgeIter<std::pair<int, int>> bfsIter(&graph);
    std::pair<int, int>* edgeData;
    while (edgeData = bfsIter.NextEdge()) {
        std::cout << "Edge: (" << edgeData->first << ", " 
                  << edgeData->second << ")" << std::endl;
    }
    
    // Use DFS iterator to traverse edges
    std::cout << "\nDFS Edge Traversal:\n";
    DFSEdgeIter<std::pair<int, int>> dfsIter(&graph);
    while (edgeData = dfsIter.NextEdge()) {
        std::cout << "Edge: (" << edgeData->first << ", " 
                  << edgeData->second << ")" << std::endl;
    }
    
    return 0;
}



/**********************************Graph.h************************************/
#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <utility>

template <typename KeyType>
class Graph;

template <typename KeyType>
class BFSEdgeIter;

template <typename KeyType>
class DFSEdgeIter;

// Edge structure that stores only the pointer to data (which is a pair of vertices)
template <typename KeyType>
struct Edge {
    KeyType* data; // Will store std::pair<int, int> as data

    Edge(KeyType* dt) : data(dt) {}
};

template <typename KeyType>
class Graph {
    friend class BFSEdgeIter<KeyType>;
    friend class DFSEdgeIter<KeyType>;

private:
    int numVertices;
    std::vector<std::vector<Edge<KeyType>>> adjList;

    void ClearGraph();

public:
    Graph();

    ~Graph();

    void AddEdge(int v1, int v2);

    void ReadGraph();

    void ReadGraphFromEdgeList(const std::vector<std::pair<int, int>>& edges);

    void PrintGraph() const;

    int GetNumVertices() const { return numVertices; }
    
    const std::vector<std::vector<Edge<KeyType>>>& GetAdjList() const { return adjList; }
};

#include "Graph.tpp"

#endif // GRAPH_H



/*********************************Graph.tpp***********************************/
template <typename KeyType>
void Graph<KeyType>::ClearGraph() {
    for (auto& vertices : adjList) {
        for (auto& edge : vertices) {
            delete edge.data;
        }
    }
    adjList.clear();
}

template <typename KeyType>
Graph<KeyType>::Graph() : numVertices(0) {}

template <typename KeyType>
Graph<KeyType>::~Graph() { ClearGraph(); }

template <typename KeyType>
void Graph<KeyType>::AddEdge(int v1, int v2) {
    if (v1 >= numVertices || v2 >= numVertices) return;

    // Create new pair data for the edge
    KeyType* data = new KeyType(std::make_pair(v1, v2));

    // Add edge v1->v2
    adjList[v1].push_back(Edge<KeyType>(data));

    // Add edge v2->v1 with reversed pair (since it's undirected)
    KeyType* reverseData = new KeyType(std::make_pair(v2, v1));
    adjList[v2].push_back(Edge<KeyType>(reverseData));
}

template <typename KeyType>
void Graph<KeyType>::ReadGraph() {
    std::cout << "Enter number of vertices: ";
    std::cin >> numVertices;

    // Initialize adjacency lists
    adjList.resize(numVertices);

    std::cout << "Enter edges as pairs (v1 v2), -1 -1 to end:\n";
    while (true) {
        int v1, v2;

        std::cin >> v1 >> v2;
        if (v1 == -1 || v2 == -1) break;

        if (v1 >= 0 && v1 < numVertices && v2 >= 0 && v2 < numVertices) {
            AddEdge(v1, v2);
        }
    }
}

template <typename KeyType>
void Graph<KeyType>::ReadGraphFromEdgeList(const std::vector<std::pair<int, int>>& edges) {
    // Find the maximum vertex number to determine graph size
    numVertices = 0;
    for (const auto& edge : edges) {
        numVertices = std::max(numVertices, std::max(edge.first, edge.second) + 1);
    }

    // Initialize adjacency lists
    adjList.resize(numVertices);

    // Add edges
    for (const auto& edge : edges) {
        AddEdge(edge.first, edge.second);
    }
}

template <typename KeyType>
void Graph<KeyType>::PrintGraph() const {
    for (int i = 0; i < numVertices; i++) {
        std::cout << "Vertex " << i << ": ";
        for (const auto& edge : adjList[i]) {
            std::cout << "(" << edge.data->first << ", "
                     << edge.data->second << ") ";
        }
        std::cout << std::endl;
    }
}



/*******************************BFSEdgeIter.h**********************************/
#ifndef BFS_EDGE_ITER_H
#define BFS_EDGE_ITER_H

template <typename KeyType>
class BFSEdgeIter {
private:
    const Graph<KeyType>* graph;
    std::queue<std::pair<int, int>> edgeQueue;
    std::vector<bool> visitedVertices;

    void InitializeIterator();

public:
    BFSEdgeIter(const Graph<KeyType>* g);
    KeyType* NextEdge();
};

#include "BFSEdgeIter.tpp"

#endif



/******************************BFSEdgeIter.tpp*********************************/
template <typename KeyType>
void BFSEdgeIter<KeyType>::InitializeIterator() {
    visitedVertices[0] = true;  // Mark starting vertex as visited

    // Add all edges from vertex 0
    for (size_t i = 0; i < graph->GetAdjList()[0].size(); ++i) {
        const auto& edge = graph->GetAdjList()[0][i];
        int v2 = edge.data->second;
        
        // Only add edges to unvisited vertices
        if (!visitedVertices[v2]) {
            edgeQueue.push(std::make_pair(0, i));
        }
    }
}

template <typename KeyType>
BFSEdgeIter<KeyType>::BFSEdgeIter(const Graph<KeyType>* g) : graph(g) {
    visitedVertices.resize(g->GetNumVertices(), false);
    InitializeIterator();
}

template <typename KeyType>
KeyType* BFSEdgeIter<KeyType>::NextEdge() {
    while (!edgeQueue.empty()) {
        std::pair<int, int> current = edgeQueue.front();
        int currentVertex = current.first;
        int edgeIndex = current.second;
        edgeQueue.pop();

        const auto& edge = graph->GetAdjList()[currentVertex][edgeIndex];
        int destVertex = edge.data->second;

        // Skip this edge if the destination is already visited
        if (visitedVertices[destVertex]) {
            continue;
        }

        // Mark the destination vertex as visited
        visitedVertices[destVertex] = true;

        // Add all edges from the destination vertex to unvisited vertices
        for (size_t i = 0; i < graph->GetAdjList()[destVertex].size(); ++i) {
            const auto& nextEdge = graph->GetAdjList()[destVertex][i];
            int nextDest = nextEdge.data->second;
            
            // Only add edges to unvisited vertices
            if (!visitedVertices[nextDest]) {
                edgeQueue.push(std::make_pair(destVertex, i));
            }
        }

        return edge.data;
    }
    return nullptr;
}



/*******************************DFSEdgeIter.h**********************************/
#ifndef DFS_EDGE_ITER_H
#define DFS_EDGE_ITER_H

template <typename KeyType>
class DFSEdgeIter {
private:
    const Graph<KeyType>* graph;
    std::stack<std::pair<int, int>> edgeStack;
    std::vector<bool> visitedVertices;

    void InitializeIterator();

public:
    DFSEdgeIter(const Graph<KeyType>* g);
    KeyType* NextEdge();
};

#include "DFSEdgeIter.tpp"

#endif



/******************************DFSEdgeIter.tpp*********************************/
template <typename KeyType>
void DFSEdgeIter<KeyType>::InitializeIterator() {
    visitedVertices[0] = true;  // Mark starting vertex as visited

    // Add all edges from vertex 0 in reverse order
    for (int i = graph->GetAdjList()[0].size() - 1; i >= 0; --i) {
        const auto& edge = graph->GetAdjList()[0][i];
        int v2 = edge.data->second;
        
        // Only add edges to unvisited vertices
        if (!visitedVertices[v2]) {
            edgeStack.push(std::make_pair(0, i));
        }
    }
}

template <typename KeyType>
DFSEdgeIter<KeyType>::DFSEdgeIter(const Graph<KeyType>* g) : graph(g) {
    visitedVertices.resize(g->GetNumVertices(), false);
    InitializeIterator();
}

template <typename KeyType>
KeyType* DFSEdgeIter<KeyType>::NextEdge() {
    while (!edgeStack.empty()) {
        std::pair<int, int> current = edgeStack.top();
        int currentVertex = current.first;
        int edgeIndex = current.second;
        edgeStack.pop();

        const auto& edge = graph->GetAdjList()[currentVertex][edgeIndex];
        int destVertex = edge.data->second;

        // Skip this edge if the destination is already visited
        if (visitedVertices[destVertex]) {
            continue;
        }

        // Mark the destination vertex as visited
        visitedVertices[destVertex] = true;

        // Add all edges from the destination vertex to unvisited vertices
        for (int i = graph->GetAdjList()[destVertex].size() - 1; i >= 0; --i) {
            const auto& nextEdge = graph->GetAdjList()[destVertex][i];
            int nextDest = nextEdge.data->second;
            
            // Only add edges to unvisited vertices
            if (!visitedVertices[nextDest]) {
                edgeStack.push(std::make_pair(destVertex, i));
            }
        }

        return edge.data;
    }
    return nullptr;
}
