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
    Edge<std::pair<int, int>>* edgeData;
    while (edgeData = bfsIter.NextEdge()) {
        std::cout << "Edge: (" << edgeData->src << ", "
                 << edgeData->dest << ")" << std::endl;
    }

    // Use DFS iterator to traverse edges
    std::cout << "\nDFS Edge Traversal:\n";
    DFSEdgeIter<std::pair<int, int>> dfsIter(&graph);
    while (edgeData = dfsIter.NextEdge()) {
        std::cout << "Edge: (" << edgeData->src << ", "
                 << edgeData->dest << ")" << std::endl;
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

// Edge structure that represents source and destination vertices
template <typename KeyType>
struct Edge {
    int src;    // Source vertex
    int dest;   // Destination vertex
    KeyType data; // Associated data with the edge

    Edge(int s, int d, const KeyType& dt) : src(s), dest(d), data(dt) {}
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
    void AddEdge(int v1, int v2, const KeyType& data);
    void ReadGraph();
    void ReadGraphFromEdgeList(const std::vector<std::pair<int, int>>& edges);
    void PrintGraph() const;
};

#include "Graph.tpp"

#endif // GRAPH_H



/**********************************Graph.tpp************************************/
template <typename KeyType>
void Graph<KeyType>::ClearGraph() {
    adjList.clear();
}

template <typename KeyType>
Graph<KeyType>::Graph() : numVertices(0) {}

template <typename KeyType>
Graph<KeyType>::~Graph() { ClearGraph(); }

template <typename KeyType>
void Graph<KeyType>::AddEdge(int v1, int v2, const KeyType& data) {
    if (v1 >= numVertices || v2 >= numVertices) return;

    // Add edge v1->v2
    adjList[v1].push_back(Edge<KeyType>(v1, v2, data));

    // Add edge v2->v1 (since it's undirected)
    adjList[v2].push_back(Edge<KeyType>(v2, v1, data));
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
            AddEdge(v1, v2, std::make_pair(v1, v2));
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
        AddEdge(edge.first, edge.second, edge);
    }
}

template <typename KeyType>
void Graph<KeyType>::PrintGraph() const {
    for (int i = 0; i < numVertices; i++) {
        std::cout << "Vertex " << i << ": ";
        for (const auto& edge : adjList[i]) {
            std::cout << "(" << edge.src << ", " << edge.dest << ") ";
        }
        std::cout << std::endl;
    }
}



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
    Edge<KeyType>* NextEdge();
};

#include "BFSEdgeIter.tpp"

#endif



/******************************BFSEdgeIter.tpp*********************************/
template <typename KeyType>
void BFSEdgeIter<KeyType>::InitializeIterator() {
    visitedVertices[0] = true; // Mark starting vertex as visited

    // Add all edges from vertex 0
    for (size_t i = 0; i < graph->adjList[0].size(); ++i) {
        const auto& edge = graph->adjList[0][i];
        int v2 = edge.dest;

        // Only add edges to unvisited vertices
        if (!visitedVertices[v2]) {
            edgeQueue.push(std::make_pair(0, i));
        }
    }
}

template <typename KeyType>
BFSEdgeIter<KeyType>::BFSEdgeIter(const Graph<KeyType>* g) : graph(g) {
    visitedVertices.resize(g->numVertices, false);
    InitializeIterator();
}

template <typename KeyType>
Edge<KeyType>* BFSEdgeIter<KeyType>::NextEdge() {
    while (!edgeQueue.empty()) {
        std::pair<int, int> current = edgeQueue.front();
        int currentVertex = current.first;
        int edgeIndex = current.second;
        edgeQueue.pop();

        const auto& edge = graph->adjList[currentVertex][edgeIndex];
        int destVertex = edge.dest;

        // Skip this edge if the destination is already visited
        if (visitedVertices[destVertex]) {
            continue;
        }

        // Mark the destination vertex as visited
        visitedVertices[destVertex] = true;

        // Add all edges from the destination vertex to unvisited vertices
        for (size_t i = 0; i < graph->adjList[destVertex].size(); ++i) {
            const auto& nextEdge = graph->adjList[destVertex][i];
            int nextDest = nextEdge.dest;

            // Only add edges to unvisited vertices
            if (!visitedVertices[nextDest]) {
                edgeQueue.push(std::make_pair(destVertex, i));
            }
        }

        return const_cast<Edge<KeyType>*>(&edge);
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
    Edge<KeyType>* NextEdge();
};

#include "DFSEdgeIter.tpp"

#endif



/******************************DFSEdgeIter.tpp*********************************/
template <typename KeyType>
void DFSEdgeIter<KeyType>::InitializeIterator() {
    visitedVertices[0] = true; // Mark starting vertex as visited

    // Add all edges from vertex 0 in reverse order
    for (int i = graph->adjList[0].size() - 1; i >= 0; --i) {
        const auto& edge = graph->adjList[0][i];
        int v2 = edge.dest;

        // Only add edges to unvisited vertices
        if (!visitedVertices[v2]) {
            edgeStack.push(std::make_pair(0, i));
        }
    }
}

template <typename KeyType>
DFSEdgeIter<KeyType>::DFSEdgeIter(const Graph<KeyType>* g) : graph(g) {
    visitedVertices.resize(g->numVertices, false);
    InitializeIterator();
}

template <typename KeyType>
Edge<KeyType>* DFSEdgeIter<KeyType>::NextEdge() {
    while (!edgeStack.empty()) {
        std::pair<int, int> current = edgeStack.top();
        int currentVertex = current.first;
        int edgeIndex = current.second;
        edgeStack.pop();

        const auto& edge = graph->adjList[currentVertex][edgeIndex];
        int destVertex = edge.dest;

        // Skip this edge if the destination is already visited
        if (visitedVertices[destVertex]) {
            continue;
        }

        // Mark the destination vertex as visited
        visitedVertices[destVertex] = true;

        // Add all edges from the destination vertex to unvisited vertices
        for (int i = graph->adjList[destVertex].size() - 1; i >= 0; --i) {
            const auto& nextEdge = graph->adjList[destVertex][i];
            int nextDest = nextEdge.dest;

            // Only add edges to unvisited vertices
            if (!visitedVertices[nextDest]) {
                edgeStack.push(std::make_pair(destVertex, i));
            }
        }

        return const_cast<Edge<KeyType>*>(&edge);
    }
    return nullptr;
}
