/**
 * Solutions to Chapter 6, Section 4, Exercise 1 of Horowitz, Sahni, and Mehta's
 * Fundamentals of Data Structures in C++.
 * 
 * Task 1:
 * Let T be a tree with root v. The edges of T are undirected, wigh each edge
 * having a nonnegative length. Write a C++ function to determine the length of
 * the shortest paths from v to the remaining vertices of T. Show that the C++
 * function should have a time complexity of O(n), where n is the number of
 * vertices in T.
 * 
 * Answer to Task 1:
 * Since there are n - 1 edges in a tree with n vertices, with each edge
 * traversed exactly once, the time complexity is O(n).
 */

#include <iostream>
#include <limits>

class Tree {
public:
    Tree(int n) : n(n) {
        // Initialize the 2D length array with -1 to indicate no edge
        length = new int*[n];
        for (int i = 0; i < n; ++i) {
            length[i] = new int[n];
            for (int j = 0; j < n; ++j) {
                length[i][j] = -1;
            }
        }

        // Initialize the dist array with maximum integer values
        dist = new int[n];
        for (int i = 0; i < n; ++i) {
            dist[i] = std::numeric_limits<int>::max();
        }
    }

    ~Tree() {
        // Deallocate the 2D length array
        for (int i = 0; i < n; ++i) {
            delete[] length[i];
        }
        delete[] length;

        // Deallocate the dist array
        delete[] dist;
    }

    // Add an edge between vertices u and v with weight w
    void addEdge(int u, int v, int w) {
        length[u][v] = w;
        length[v][u] = w; // undirected edge
    }

    // Calculate shortest paths from root to all other vertices
    void shortestPathFromRoot(int root) {
        dist[root] = 0; // Distance from root to itself is 0
        dfs(root, -1);  // Start DFS from the root
    }

    // Print distances from the root to all vertices
    void printDistances() {
        for (int i = 0; i < n; i++) {
            std::cout << "Distance from root to vertex " << i << " is " << dist[i] << std::endl;
        }
    }

private:
    int n; // Number of vertices
    int** length; // 2D array to store edge weights
    int* dist; // 1D array to store distances from the root

    // Helper function for DFS
    void dfs(int node, int parent) {
        for (int next = 0; next < n; ++next) {
            int weight = length[node][next];
            
            // Only proceed if there is an edge and it’s not the parent node
            if (weight != -1 && next != parent) {
                dist[next] = dist[node] + weight;
                dfs(next, node);
            }
        }
    }
};

// Driver code to test the function with an example tree
int main() {
    int n = 5; // Number of vertices in the tree
    Tree tree(n);

    // Adding edges to create an example tree
    tree.addEdge(0, 1, 2);
    tree.addEdge(0, 2, 4);
    tree.addEdge(1, 3, 1);
    tree.addEdge(1, 4, 3);

    int root = 0; // Set the root of the tree
    std::cout << "Calculating shortest paths from root " << root << "...\n";
    tree.shortestPathFromRoot(root);
    
    // Print the distances from the root to each vertex
    tree.printDistances();

    return 0;
}

