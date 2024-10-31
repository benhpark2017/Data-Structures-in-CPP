/**
 * Solutions to Chapter 6, Section 4, Exercise 5 of Horowitz, Sahni, and Mehta's
 * Fundamentals of Data Structures in C++.
 * 
 * Task 5:
 * Use algorithm ShortestPath() to obtain, in non-decreasing order, the lengths
 * of the shortest paths from vertex 0 to all other remaining vertices in the
 * directed graph (digraph) as provided in the driver code below.
 * 
 */

#include <iostream>
#include <vector>
#include <algorithm> // For std::sort
#include <climits>   // For INT_MAX

const int nMax = 100; // Define a maximum number of vertices

enum Boolean { FALSE, TRUE };

class Graph {
private:
    int length[nMax][nMax]; // Adjacency matrix to store edge weights
    int dist[nMax];         // Used for Dijkstra's algorithm
    Boolean s[nMax];        // Included vertices for Dijkstra's algorithm

public:
    Graph() {
        // Initialize the adjacency matrix with 0s
        for (int i = 0; i < nMax; i++) {
            for (int j = 0; j < nMax; j++) {
                length[i][j] = (i == j) ? 0 : INT_MAX; // Self-loop weights as 0, others as infinity
            }
        }
    }

    void ShortestPath(const int n, const int v);
    int choose(const int n);
    void PrintShortestPaths(const int n, const int start);
    void addEdge(int u, int v, int weight);
};

void Graph::ShortestPath(const int n, const int v) {
    for (int i = 0; i < n; i++) {
        s[i] = FALSE;
        dist[i] = length[v][i];
    }
    s[v] = TRUE;
    dist[v] = 0;

    for (int i = 0; i < n - 2; i++) {
        int u = choose(n);
        s[u] = TRUE;
        for (int w = 0; w < n; w++) {
            if (!s[w] && length[u][w] != INT_MAX) { // Ensure there is an edge
                if (dist[u] + length[u][w] < dist[w]) {
                    dist[w] = dist[u] + length[u][w];
                }
            }
        }
    }
}

int Graph::choose(const int n) {
    int minDist = INT_MAX;
    int minIndex = -1;

    for (int i = 0; i < n; i++) {
        if (!s[i] && dist[i] < minDist) {
            minDist = dist[i];
            minIndex = i;
        }
    }

    return minIndex;
}

void Graph::PrintShortestPaths(const int n, const int start) {
    ShortestPath(n, start); // Calculate the shortest paths

    // Create a vector to store distances along with their vertex indices
    std::vector<std::pair<int, int>> distances; // pair<length, vertex index>
    for (int i = 0; i < n; i++) {
        if (dist[i] < INT_MAX) { // Ignore unreachable vertices
            distances.emplace_back(dist[i], i);
        }
    }

    // Sort distances based on length (first element of the pair)
    std::sort(distances.begin(), distances.end());

    // Print the sorted distances
    std::cout << "Shortest path lengths from vertex " << start << ":\n";
    for (const auto& p : distances) {
        std::cout << "Vertex " << p.second << " : " << p.first << std::endl;
    }
}

void Graph::addEdge(int u, int v, int weight) {
    length[u][v] = weight; // Add directed edge from u to v
}

// Testing the Graph class
int main() {
    Graph g;

    // Add edges to the graph (directed)
    g.addEdge(0, 1, 20);
    g.addEdge(1, 4, 10);
    g.addEdge(1, 5, 30);
    g.addEdge(1, 0, 2);
    g.addEdge(0, 2, 15);
    g.addEdge(2, 3, 4);
    g.addEdge(2, 5, 10);
    g.addEdge(4, 3, 15);
    g.addEdge(5, 3, 4);
    g.addEdge(5, 4, 10);
    
    // Number of vertices
    int n = 6;

    // Print shortest paths from vertex 0
    g.PrintShortestPaths(n, 0);

    return 0;
}
