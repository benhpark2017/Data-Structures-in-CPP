/**
 * Solutions to Chapter 6, Section 4, Exercise 2 of Horowitz, Sahni, and Mehta's
 * Fundamentals of Data Structures in C++.
 * 
 * Task 2:
 * Let G be a directed, acyclic graph with n vertices. Assume that the vertices
 * are numbered 0 through n - 1 such that all edges are of the form <i, j>, 
 * where i < j. Assume that the graph is available as a set of adjacency lists
 * and that each edge has a length (which may be negative) associated with it.
 * Write a C++ function to determine the length of the shortest paths from
 * vertex 0 to the remaining vertices. The complexity of your function should
 * be O(n + e), where e is the number of edges in the graph. Show that this is
 * the case.
 * 
 * Answer to Task 2:
 * The first step of the shortest path function, shortestPath(), is to call a
 * sorting function using depth-first search. This step takes O(n + e) time,
 * because each vertex is visited once and each edge is considered only once
 * during the sorting process. Next, for each edge, updating the shortest path
 * takes O(e) time because each edge is considered once. Therefore, the total
 * time results in O(n + e) + O(e), which translates to an overall time
 * complexity of O(n + e).
 * 
 */

#include <iostream>
#include <vector>
#include <limits>

enum Boolean { FALSE, TRUE };

class Graph {
  private:
    static const int nMax = 100; // Define the maximum number of vertices
    int length[nMax][nMax]; // Adjacency matrix for edge lengths
    int dist[nMax];         // Stores the shortest distance from vertex 0
    Boolean s[nMax];        // Keeps track of processed vertices
    int n;                  // Number of vertices

    // Helper function to perform topological sort
    void topologicalSortUtil(int v, Boolean visited[], std::vector<int> &stack) {
        visited[v] = TRUE;
        for (int i = 0; i < n; ++i) {
            if (length[v][i] != std::numeric_limits<int>::max() && visited[i] == FALSE) {
                topologicalSortUtil(i, visited, stack);
            }
        }
        stack.push_back(v);
    }

  public:
    // Constructor
    Graph(int vertices) : n(vertices) {
        // Initialize lengths and distances
        for (int i = 0; i < nMax; ++i) {
            dist[i] = std::numeric_limits<int>::max();
            s[i] = FALSE;
            for (int j = 0; j < nMax; ++j) {
                length[i][j] = std::numeric_limits<int>::max(); // No edge initially
            }
        }
    }

    // Function to add edge with given length
    void addEdge(int u, int v, int len) {
        if (u < n && v < n && u < v) {
            length[u][v] = len;
        }
    }

    // Function to find shortest paths from vertex 0
    void shortestPath() {
        std::vector<int> stack;
        Boolean visited[nMax] = { FALSE };

        // Step 1: Perform topological sort
        for (int i = 0; i < n; i++) {
            if (visited[i] == FALSE) {
                topologicalSortUtil(i, visited, stack);
            }
        }

        // Step 2: Initialize distances to all vertices as infinity, except for the source
        dist[0] = 0;

        // Step 3: Process vertices in topological order
        while (!stack.empty()) {
            int u = stack.back();
            stack.pop_back();

            // Update distances of all adjacent vertices of the dequeued vertex
            if (dist[u] != std::numeric_limits<int>::max()) {
                for (int v = 0; v < n; v++) {
                    if (length[u][v] != std::numeric_limits<int>::max() && dist[v] > dist[u] + length[u][v]) {
                        dist[v] = dist[u] + length[u][v];
                    }
                }
            }
        }

        // Output the shortest distances
        for (int i = 0; i < n; i++) {
            if (dist[i] == std::numeric_limits<int>::max()) {
                std::cout << "Distance from 0 to " << i << ": INF\n";
            } else {
                std::cout << "Distance from 0 to " << i << ": " << dist[i] << "\n";
            }
        }
    }
};

int main() {
    // Create a graph with 6 vertices
    Graph g(6);

    // Add edges with their lengths
    g.addEdge(0, 1, 5);
    g.addEdge(0, 2, 3);
    g.addEdge(1, 3, 6);
    g.addEdge(1, 2, 2);
    g.addEdge(2, 4, 4);
    g.addEdge(2, 5, 2);
    g.addEdge(2, 3, 7);
    g.addEdge(3, 5, 1);
    g.addEdge(4, 5, -2);

    // Call the shortestPath function
    std::cout << "Shortest paths from vertex 0:\n";
    g.shortestPath();

    return 0;
}
