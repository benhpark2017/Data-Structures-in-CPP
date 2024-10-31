/**
 * Solutions to Chapter 6, Section 4, Exercise 4 of Horowitz, Sahni, and Mehta's
 * Fundamentals of Data Structures in C++.
 * 
 * Task 4:
 * Using the idea of algorithm ShortestPath(), give a function to find a
 * minimum-cost spanning tree whose worst-case time is O(n^2).
 */
 
#include <iostream>
#include <limits> // For std::numeric_limits<int>::max()

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
                length[i][j] = (i == j) ? 0 : std::numeric_limits<int>::max(); // Self-loop weights as 0, others as infinity
            }
        }
    }

    void ShortestPath(const int n, const int v);
    int choose(const int n);
    void MinimumSpanningTree(const int n, const int start);
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
            if (!s[w])
                if (dist[u] + length[u][w] < dist[w]) 
                    dist[w] = dist[u] + length[u][w];
        }
    }
}

int Graph::choose(const int n) {
    int minDist = std::numeric_limits<int>::max();
    int minIndex = -1;

    for (int i = 0; i < n; i++) {
        if (!s[i] && dist[i] < minDist) {
            minDist = dist[i];
            minIndex = i;
        }
    }

    return minIndex;
}

void Graph::MinimumSpanningTree(const int n, const int start) {
    bool included[nMax];  // Using nMax instead of n for array size
    int key[nMax];       // Key values used to pick minimum weight edge
    int parent[nMax];    // Array to store constructed MST

    // Initialize all keys as infinite and included as false
    for (int i = 0; i < n; i++) {
        key[i] = std::numeric_limits<int>::max();
        included[i] = false;
        parent[i] = -1;
    }

    // Start vertex should have key 0
    key[start] = 0;

    // Find MST with n vertices
    for (int i = 0; i < n; i++) {
        // Find vertex with minimum key value from the set of vertices not yet included
        int minKey = std::numeric_limits<int>::max();
        int u = -1;
        
        for (int v = 0; v < n; v++) {
            if (!included[v] && key[v] < minKey) {
                minKey = key[v];
                u = v;
            }
        }

        // If no valid vertex found, break
        if (u == -1) break;

        // Add the picked vertex to the included set
        included[u] = true;

        // Update key values and parent index of adjacent vertices
        // Consider only those vertices which are not yet included
        for (int v = 0; v < n; v++) {
            // Update only if:
            // 1. There is an edge from u to v
            // 2. v is not included
            // 3. Weight of edge u-v is smaller than current key of v
            if (length[u][v] != std::numeric_limits<int>::max() && 
                !included[v] && 
                length[u][v] < key[v]) {
                parent[v] = u;
                key[v] = length[u][v];
            }
        }
    }

    // Print the constructed MST
    std::cout << "Minimum Spanning Tree edges starting from vertex " << start << ":\n";
    int totalWeight = 0;
    for (int i = 0; i < n; i++) {
        if (parent[i] != -1) {
            std::cout << "Edge: " << parent[i] << " - " << i 
                     << " Weight: " << length[parent[i]][i] << std::endl;
            totalWeight += length[parent[i]][i];
        }
    }
    std::cout << "Total MST Weight: " << totalWeight << std::endl;
}

void Graph::addEdge(int u, int v, int weight) {
    length[u][v] = weight;
    length[v][u] = weight; // For undirected graph
}

// Testing the Graph class
int main() {
    Graph g;

    // Add edges to the graph (undirected)
    g.addEdge(0, 1, 28);
    g.addEdge(0, 5, 10);
    g.addEdge(1, 2, 16);
    g.addEdge(1, 6, 14);
    g.addEdge(2, 3, 12);
    g.addEdge(3, 4, 22);
    g.addEdge(3, 6, 18);
    g.addEdge(4, 5, 25);
    g.addEdge(4, 6, 24);
    
    // Number of vertices
    int n = 7;

    // Run Minimum Spanning Tree algorithm
    std::cout << "Minimum Spanning Tree starting from vertex 0:\n";
    g.MinimumSpanningTree(n, 0);

    return 0;
}
