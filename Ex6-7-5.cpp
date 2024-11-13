/**
 * Solutions to Chapter 6, Section 7, Exercises 5 and 6 of Horowitz, Sahni, and 
 * Mehta's Fundamentals of Data Structures in C++
 * 
 * Task 5:
 * The radius of a tree is the maximum distance from the root to a leaf. Given
 * a connected, undirected graph, write a function to find a spanning tree of
 * minimum radius (Hint: Use breadth-first search). Prove that your algorithm
 * is correct.
 * 
 * Task 6:
 * The diameter of a tree is the maximum distance between any two vertices.
 * Given a connected, undirected graph, write a function for finding a spanning
 * tree of minimum diameter. Prove the correctness of your function.
 */

/**********************************main.cpp************************************/
#include "Graph.h"

int main() {
    Graph g1(17);
    Graph g2(16);
    
//==============================================================================
    // Add edges in a way that vertex 0 is the center
    g1.addEdge(0, 1); 
    g1.addEdge(0, 2);
    g1.addEdge(0, 3);
    g1.addEdge(1, 4);
    g1.addEdge(1, 5);
    g1.addEdge(3, 6);
    g1.addEdge(3, 7);
    g1.addEdge(4, 8);
    g1.addEdge(5, 9);
    g1.addEdge(5, 10);
    g1.addEdge(5, 11);
    g1.addEdge(6, 12);
    g1.addEdge(7, 13);
    g1.addEdge(8, 14);
    g1.addEdge(12, 15);
    
    std::vector<int> centers1 = g1.findCenters();
    g1.printPathsToFarthestNodes(centers1[0]);
    g1.printMinDiameterSpanningTree();
    
//=============================================================================    
    g2.addEdge(0, 3); 
    g2.addEdge(0, 4);
    g2.addEdge(3, 5);
    g2.addEdge(3, 6);
    g2.addEdge(5, 7);
    g2.addEdge(5, 9);
    g2.addEdge(6, 11);
    g2.addEdge(9, 10);
    g2.addEdge(11, 12);
    g2.addEdge(11, 13);
    g2.addEdge(10, 2);
    g2.addEdge(13, 15);
    
    std::vector<int> centers2 = g2.findCenters();
    g2.printPathsToFarthestNodes(centers2[0]);
    g2.printMinDiameterSpanningTree();
    return 0;
}



/**********************************Graph.h*************************************/
#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <queue>
#include <algorithm>
#include <iostream>

class Graph {
private:
    int V;
    std::vector<std::vector<int>> adj;
    
    // Helper function to reconstruct path using parent array
    std::vector<int> reconstructPath(const std::vector<int>& parent, int target);

    // Helper function to find farthest node and its distance using BFS
    std::pair<int, int> findFarthestNode(int start);


public:
    Graph(int vertices);
    
    void addEdge(int u, int v);
    
    std::vector<int> findCenters();
    
    // Modified to find path(s) to farthest node(s)
    void printPathsToFarthestNodes(int center);

    std::pair<std::vector<int>, int> findDiameterPathAndCenter();
    
    // Modified minimum diameter spanning tree function
    void printMinDiameterSpanningTree();
};

#endif // GRAPH_H



/*********************************Graph.cpp************************************/
#include "Graph.h"

// Helper function to reconstruct path using parent array
std::vector<int> Graph::reconstructPath(const std::vector<int>& parent, int target) {
    std::vector<int> path;
    for (int current = target; current != -1; current = parent[current]) {
        path.push_back(current);
    }
    std::reverse(path.begin(), path.end());
    return path;
}

// Helper function to find farthest node and its distance using BFS
std::pair<int, int> Graph::findFarthestNode(int start) {
    std::vector<bool> visited(V, false);
    std::vector<int> distance(V, -1);
    std::queue<int> q;

    q.push(start);
    visited[start] = true;
    distance[start] = 0;

    int farthestNode = start;
    int maxDist = 0;

    while (!q.empty()) {
        int current = q.front();
        q.pop();

        for (int neighbor : adj[current]) {
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                distance[neighbor] = distance[current] + 1;
                q.push(neighbor);

                if (distance[neighbor] > maxDist) {
                    maxDist = distance[neighbor];
                    farthestNode = neighbor;
                }
            }
        }
    }

    return {farthestNode, maxDist};
}

Graph::Graph(int vertices) : V(vertices) {
    adj.resize(vertices);
}
    
void Graph::addEdge(int u, int v) {
    adj[u].push_back(v);
    adj[v].push_back(u);
}
    
std::vector<int> Graph::findCenters() {
    std::vector<int> degrees(V);
    std::queue<int> leaves;
    
    for (int i = 0; i < V; i++) {
        degrees[i] = adj[i].size();
        if (degrees[i] == 1) {
            leaves.push(i);
        }
    }
    
    int processed = leaves.size();
    
    while (processed < V - 2) {
        int leafCount = leaves.size();
        processed += leafCount;
        
        for (int i = 0; i < leafCount; i++) {
            int leaf = leaves.front();
            leaves.pop();
            
            for (int neighbor : adj[leaf]) {
                degrees[neighbor]--;
                if (degrees[neighbor] == 1) {
                    leaves.push(neighbor);
                }
            }
        }
    }
    
    std::vector<int> centers;
    for (int i = 0; i < V; i++) {
        if (degrees[i] > 0) {
            centers.push_back(i);
        }
    }
    return centers;
}
    
// Modified to find path(s) to farthest node(s)
void Graph::printPathsToFarthestNodes(int center) {
    std::vector<bool> visited(V, false);
    std::vector<int> distance(V, -1);
    std::vector<int> parent(V, -1);
    std::queue<int> q;
    
    // BFS to find distances and maintain parent information
    q.push(center);
    visited[center] = true;
    distance[center] = 0;
    
    int maxDist = 0;
    std::vector<int> farthestNodes;
    
    while (!q.empty()) {
        int current = q.front();
        q.pop();
        
        for (int neighbor : adj[current]) {
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                distance[neighbor] = distance[current] + 1;
                parent[neighbor] = current;
                q.push(neighbor);
                
                // Track maximum distance and farthest nodes
                if (distance[neighbor] > maxDist) {
                    maxDist = distance[neighbor];
                    farthestNodes.clear();
                    farthestNodes.push_back(neighbor);
                }
                else if (distance[neighbor] == maxDist) {
                    farthestNodes.push_back(neighbor);
                }
            }
        }
    }
    
    // Print all paths to farthest nodes
    std::cout << "Paths from center to farthest nodes (radius = " << maxDist << "):\n";
    for (int farthestNode : farthestNodes) {
        std::vector<int> path = reconstructPath(parent, farthestNode);
        for (size_t i = 0; i < path.size(); i++) {
            std::cout << path[i];
            if (i < path.size() - 1) {
                std::cout << " -> ";
            }
        }
        std::cout << "\n";
    }
}

std::pair<std::vector<int>, int> Graph::findDiameterPathAndCenter() {
    // First BFS: Find one end of diameter
    std::pair<int, int> firstBFSResult = findFarthestNode(0);
    int end1 = firstBFSResult.first;  // Instead of auto [end1, _]
    
    // Second BFS: Find other end of diameter and the actual diameter path
    std::vector<bool> visited(V, false);
    std::vector<int> distance(V, -1);
    std::vector<int> parent(V, -1);
    std::queue<int> q;

    q.push(end1);
    visited[end1] = true;
    distance[end1] = 0;
    
    int end2 = end1;
    int maxDist = 0;

    while (!q.empty()) {
        int current = q.front();
        q.pop();
        
        for (int neighbor : adj[current]) {
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                distance[neighbor] = distance[current] + 1;
                parent[neighbor] = current;
                q.push(neighbor);
                
                if (distance[neighbor] > maxDist) {
                    maxDist = distance[neighbor];
                    end2 = neighbor;
                }
            }
        }
    }

    // Reconstruct the diameter path
    std::vector<int> diameterPath = reconstructPath(parent, end2);
    
    // Find the center vertex of the diameter path
    int center = diameterPath[diameterPath.size() / 2];

    // Use make_pair instead of brace initialization
    return std::make_pair(diameterPath, center);
}
    
// Modified minimum diameter spanning tree function
void Graph::printMinDiameterSpanningTree() {
    auto result = findDiameterPathAndCenter();
    std::vector<int> diameterPath = result.first;
    int center = result.second;
    
    if (diameterPath.empty()) {
        std::cout << "Empty diameter path - likely disconnected graph\n";
        return;
    }
    
    // Build spanning tree using BFS from center
    std::vector<bool> visited(V, false);
    std::vector<std::pair<int, int>> treeEdges;  // Store edges for printing
    std::queue<int> q;

    q.push(center);
    visited[center] = true;

    while (!q.empty()) {
        int current = q.front();
        q.pop();

        for (int neighbor : adj[current]) {
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                treeEdges.push_back({current, neighbor});
                q.push(neighbor);
            }
        }
    }

    // Print results
    std::cout << "Root of minimum diameter spanning tree: " << center << "\n";
    std::cout << "Diameter path: ";
    for (size_t i = 0; i < diameterPath.size(); i++) {
        std::cout << diameterPath[i];
        if (i < diameterPath.size() - 1) std::cout << " -> ";
    }
    std::cout << "\nLength of diameter: " << diameterPath.size() - 1 << "\n";

    std::cout << "\nSpanning tree edges in BFS order:\n";
    for (const auto& edge : treeEdges) {
        std::cout << edge.first << " -- " << edge.second << "\n";
    }
}
