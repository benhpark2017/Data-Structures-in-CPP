/**
 * Solutions to Chapter 6, Section 3, Exercises 1, 3, and 6 of Horowitz's
 * Fundamentals of Data Structures in C++.
 * 
 * Task 1:
 * Write out Kruskal's minimum-cost spanning tree algorithm as a complete
 * program. You may use functions WeightedUnion() and CollapsingFind(). Use
 * selection sort function sort() to sort the edges into nondecreasing order
 * by weight.
 * 
 * Task 3:
 * Refine Prim's algorithm into a C++ function to find a minimum-cost spanning
 * tree. The complexity of your function should be O(n^2), where n is the
 * number of vertices in the input graph. Show that this is the case.
 * 
 * Task 6:
 * Obtain a C++ function to find a minimum-cost spanning tree using Sollin's
 * algorithm. What is the complexity of your function?
 */

/*******************************main.cpp**********************************/
#include "Kruskal.h"
#include "Prim.h"
#include "Boruvka.h"
#include <iostream>
#include <vector>

int main() {
    
    std::cout << "Implementation of Kruskal's algorithm.\n";
    Kruskal k;
    std::cin >> k;
    k.Initialize();
    k.MST();
    std::cout << std::endl;
    
    std::cout << "Implementation of Prim's algorithm.\n";
    Prim p;
    std::cin >> p;
    p.Initialize();
    p.MST();
    std::cout << std::endl;
    
    std::cout << "Implementation of Boruvka's (Sollin's) algorithm.\n";
    Boruvka b;
    std::cin >> b;
    b.Initialize();
    b.MST();
    std::cout << std::endl;

    return 0;
}



/*******************************Kruskal.h**********************************/
#ifndef KRUSKAL_H
#define KRUSKAL_H

#include <iostream>
#include <vector>

class Kruskal {
private:
    typedef struct _graph {
        int v1;     // Vertex 1
        int v2;     // Vertex 2
        int weight; // Weight
    } Graph;

    Graph g[20];
    int total_edges, total_vertices;
    std::vector<int> parent;

    void sort();

public:
    Kruskal() {} 
    void Initialize();
    int CollapsingFind(int v2, std::vector<int>& parent);
    void WeightedUnion(int i, int j, std::vector<int>& parent);
    void MST();

    friend std::istream& operator>>(std::istream& is, Kruskal& k);
};

#endif // KRUSKAL_H


/******************************Kruskal.cpp*********************************/
#include "Kruskal.h"

void Kruskal::sort() {
    for (int i = 0; i < total_edges - 1; i++) {
        for (int j = i + 1; j < total_edges; j++) {
            if (g[i].weight > g[j].weight) {
                Graph temp = g[i];
                g[i] = g[j];
                g[j] = temp;
            }
        }
    }
}


void Kruskal::Initialize() {
    std::cout << "Enter edges and weights!" << std::endl;
    for (int i = 0; i < total_edges; i++) {
        std::cout << "Enter edge (two vertices): ";
        std::cin >> g[i].v1 >> g[i].v2;
        std::cout << "Enter the weight of the edge: ";
        std::cin >> g[i].weight;
    }
    // Initialize parent vector here with proper size and values
    parent.resize(total_vertices);
    for (int i = 0; i < total_vertices; i++) {
        parent[i] = -1;  // Initialize all nodes as roots with size 1
    }
}

int Kruskal::CollapsingFind(int v2, std::vector<int>& parent) {
    int r = v2;
    // Find the root
    while (parent[r] >= 0) {  // Changed condition to check for negative values
        r = parent[r];
    }
    // Compress the path
    while (v2 != r) {
        int s = parent[v2];
        parent[v2] = r;
        v2 = s;
    }
    return r;
}

void Kruskal::WeightedUnion(int i, int j, std::vector<int>& parent) {
    // Both parent[i] and parent[j] should be negative, representing set sizes
    int temp = parent[i] + parent[j];  // temp will be more negative
    if (parent[i] > parent[j]) {  // j has more nodes (more negative)
        parent[i] = j;
        parent[j] = temp;
    } else {  // i has more nodes or equal
        parent[j] = i;
        parent[i] = temp;
    }
}

void Kruskal::MST() {
    sort();

    int count = 0, k = 0, sum = 0;
    int v1, v2, v1Parent, v2Parent, tree[10][10];

    for (int i = 0; i < total_edges && count < total_vertices - 1; i++) {
        v1 = g[i].v1;
        v2 = g[i].v2;
        v1Parent = CollapsingFind(v1, parent);
        v2Parent = CollapsingFind(v2, parent);

        if (v1Parent != v2Parent) {
            tree[k][0] = v1;
            tree[k][1] = v2;
            k++;
            count++;
            sum += g[i].weight;
            WeightedUnion(v1Parent, v2Parent, parent);
        }
    }

    if (count == total_vertices - 1) {
        std::cout << "\nMST Output" << std::endl;
        for (int i = 0; i < total_vertices - 1; i++) {
            int temp1 = tree[i][0];
            int temp2 = tree[i][1];
            if (temp1 > temp2) {
                std::swap(temp1, temp2);
            }
            std::cout << "[" << temp1 << "--" << temp2 << "]" << std::endl;
        }
        std::cout << "Total weight: " << sum << std::endl;
    } else {
        std::cout << "\nMST does not exist" << std::endl;
    }
}

std::istream& operator>>(std::istream& is, Kruskal& k) {
    std::cout << "Enter the total number of vertices: ";
    is >> k.total_vertices;
    std::cout << "Enter the total number of edges: ";
    is >> k.total_edges;
    return is;
}



/********************************Prim.h***********************************/
#ifndef PRIM_H
#define PRIM_H

#include <limits>
#include <iostream>
#include <vector>

class Prim {
private:
    typedef struct _graph {
        int v1;     // Vertex 1
        int v2;     // Vertex 2
        int weight; // Weight
    } Graph;

    Graph g[20];
    int total_edges, total_vertices;
    std::vector<std::vector<int>> adjMatrix;  // Adjacency matrix

    // Convert edge list to adjacency matrix
    void buildAdjMatrix();

    // Find vertex with minimum key value
    int minKey(const std::vector<int>& key, const std::vector<bool>& mstSet);

public:
    Prim() {} // Default constructor
    void Initialize();
    void MST();

    friend std::istream& operator>>(std::istream& is, Prim& p);
};

#endif //PRIM_H



/*******************************Prim.cpp**********************************/
#include "Prim.h"

// Convert edge list to adjacency matrix
void Prim::buildAdjMatrix() {
    // Initialize matrix with maximum number (representing infinity)
    adjMatrix = std::vector<std::vector<int>>(
        total_vertices, 
        std::vector<int>(total_vertices, std::numeric_limits<int>::max())
    );

    // Set diagonal to 0
    for (int i = 0; i < total_vertices; i++) {
        adjMatrix[i][i] = 0;
    }

    // Fill matrix with edge weights
    for (int i = 0; i < total_edges; i++) {
        int v1 = g[i].v1;
        int v2 = g[i].v2;
        int weight = g[i].weight;
        adjMatrix[v1][v2] = weight;
        adjMatrix[v2][v1] = weight;  // Undirected graph
    }
}

// Find vertex with minimum key value
int Prim::minKey(const std::vector<int>& key, const std::vector<bool>& mstSet) {
    int min = std::numeric_limits<int>::max();
    int min_index = -1;

    for (int v = 0; v < total_vertices; v++) {
        if (!mstSet[v] && key[v] < min) {
            min = key[v];
            min_index = v;
        }
    }
    return min_index;
}

void Prim::Initialize() {
    std::cout << "Enter edges and weights!" << std::endl;
    for (int i = 0; i < total_edges; i++) {
        std::cout << "Enter edge (two vertices): ";
        std::cin >> g[i].v1 >> g[i].v2;
        std::cout << "Enter the weight of the edge: ";
        std::cin >> g[i].weight;
    }
    buildAdjMatrix();
}

void Prim::MST() {
    std::vector<int> parent(total_vertices);    // Store the MST
    std::vector<int> key(total_vertices, std::numeric_limits<int>::max());  // Key values
    std::vector<bool> mstSet(total_vertices, false); // Vertices included in MST

    // Start with vertex 0
    key[0] = 0;        // Make first vertex the starting point
    parent[0] = -1;    // First node is the root

    int totalWeight = 0;
    std::vector<std::pair<int, int>> mstEdges;  // Store edges for sorted output

    for (int count = 0; count < total_vertices; count++) {
        int u = minKey(key, mstSet);
        if (u == -1) {
            std::cout << "\nMST does not exist (graph is not connected)" << std::endl;
            return;
        }

        mstSet[u] = true;  // Add to MST

        // If this isn't the starting vertex, add the edge to our result
        if (u != 0) {
            mstEdges.push_back({std::min(parent[u], u), std::max(parent[u], u)});
            totalWeight += adjMatrix[u][parent[u]];
        }

        // Update key values of adjacent vertices
        for (int v = 0; v < total_vertices; v++) {
            if (adjMatrix[u][v] != std::numeric_limits<int>::max() && 
                !mstSet[v] && 
                adjMatrix[u][v] < key[v]) {
                parent[v] = u;
                key[v] = adjMatrix[u][v];
            }
        }
    }

    // Print the MST edges in sorted order
    std::cout << "\nMST Output" << std::endl;
    for (const auto& edge : mstEdges) {
        std::cout << "[" << edge.first << "--" << edge.second << "]" << std::endl;
    }
    std::cout << "Total weight: " << totalWeight << std::endl;
}

std::istream& operator>>(std::istream& is, Prim& p) {
    std::cout << "Enter the total number of vertices: ";
    is >> p.total_vertices;
    std::cout << "Enter the total number of edges: ";
    is >> p.total_edges;
    return is;
}



/******************************Boruvka.h*********************************/
#ifndef BORUVKA_H
#define BORUVKA_H

#include <iostream>
#include <vector>
#include <limits>

class Boruvka {
private:
    typedef struct _graph {
        int v1;     // Vertex 1
        int v2;     // Vertex 2
        int weight; // Weight
    } Graph;

    Graph g[20];
    int total_edges, total_vertices;
    std::vector<std::vector<int>> adjMatrix;

    void buildAdjMatrix();

    // Find function for Union-Find data structure
    int find(std::vector<int>& parent, int i);

    // Union function for Union-Find data structure
    void unionSets(std::vector<int>& parent, std::vector<int>& rank, int x, int y);

    // Find the cheapest edge from a component to any other component
    void findCheapestEdge(int component, 
                         std::vector<int>& parent,
                         std::vector<int>& cheapest_edge_src,
                         std::vector<int>& cheapest_edge_dest,
                         std::vector<int>& cheapest_edge_weight);

public:
    Boruvka() {}
    void Initialize();
    void MST();
    
    friend std::istream& operator>>(std::istream& is, Boruvka& b);
};

#endif //BORUVKA_H



/******************************Boruvka.cpp********************************/
#include "Boruvka.h"

void Boruvka::buildAdjMatrix() {
    adjMatrix = std::vector<std::vector<int>>(
        total_vertices, 
        std::vector<int>(total_vertices, std::numeric_limits<int>::max())
    );

    // Set diagonal to 0
    for (int i = 0; i < total_vertices; i++) {
        adjMatrix[i][i] = 0;
    }

    // Fill matrix with edge weights
    for (int i = 0; i < total_edges; i++) {
        int v1 = g[i].v1;
        int v2 = g[i].v2;
        int weight = g[i].weight;
        adjMatrix[v1][v2] = weight;
        adjMatrix[v2][v1] = weight;
    }
}

// Find function for Union-Find data structure
int Boruvka::find(std::vector<int>& parent, int i) {
    if (parent[i] == i)
        return i;
    return parent[i] = find(parent, parent[i]); // Path compression
}

// Union function for Union-Find data structure
void Boruvka::unionSets(std::vector<int>& parent, std::vector<int>& rank, int x, int y) {
    int xroot = find(parent, x);
    int yroot = find(parent, y);

    if (rank[xroot] < rank[yroot])
        parent[xroot] = yroot;
    else if (rank[xroot] > rank[yroot])
        parent[yroot] = xroot;
    else {
        parent[yroot] = xroot;
        rank[xroot]++;
    }
}

// Find the cheapest edge from a component to any other component
void Boruvka::findCheapestEdge(int component, 
                     std::vector<int>& parent,
                     std::vector<int>& cheapest_edge_src,
                     std::vector<int>& cheapest_edge_dest,
                     std::vector<int>& cheapest_edge_weight) {
                         
    int min_weight = std::numeric_limits<int>::max();
    int component_root = find(parent, component);

    // Check all possible edges from this vertex
    for (int j = 0; j < total_vertices; j++) {
        if (adjMatrix[component][j] != std::numeric_limits<int>::max()) {
            int adjacent_root = find(parent, j);
            if (component_root != adjacent_root && 
                adjMatrix[component][j] < cheapest_edge_weight[component_root]) {
                cheapest_edge_src[component_root] = component;
                cheapest_edge_dest[component_root] = j;
                cheapest_edge_weight[component_root] = adjMatrix[component][j];
            }
        }
    }
}

void Boruvka::Initialize() {
    std::cout << "Enter edges and weights!" << std::endl;
    for (int i = 0; i < total_edges; i++) {
        std::cout << "Enter edge (two vertices): ";
        std::cin >> g[i].v1 >> g[i].v2;
        std::cout << "Enter the weight of the edge: ";
        std::cin >> g[i].weight;
    }
    buildAdjMatrix();
}

void Boruvka::MST() {
    // Initialize Union-Find data structures
    std::vector<int> parent(total_vertices);
    std::vector<int> rank(total_vertices, 0);
    for (int i = 0; i < total_vertices; i++)
        parent[i] = i;

    // Vector to store MST edges
    std::vector<std::pair<std::pair<int, int>, int>> mst_edges;
    int totalWeight = 0;

    // Count of components
    int components = total_vertices;

    // Continue until we have a single component
    while (components > 1) {
        // Arrays to store the cheapest edge for each component
        std::vector<int> cheapest_edge_src(total_vertices, -1);
        std::vector<int> cheapest_edge_dest(total_vertices, -1);
        std::vector<int> cheapest_edge_weight(total_vertices, std::numeric_limits<int>::max());

        // Find the cheapest edge for each component
        for (int i = 0; i < total_vertices; i++) {
            findCheapestEdge(i, parent, cheapest_edge_src, cheapest_edge_dest, cheapest_edge_weight);
        }

        // Add edges to MST and perform unions
        for (int i = 0; i < total_vertices; i++) {
            if (cheapest_edge_src[i] != -1) {
                int set1 = find(parent, cheapest_edge_src[i]);
                int set2 = find(parent, cheapest_edge_dest[i]);

                if (set1 != set2) {
                    mst_edges.push_back({
                        {std::min(cheapest_edge_src[i], cheapest_edge_dest[i]),
                         std::max(cheapest_edge_src[i], cheapest_edge_dest[i])},
                        cheapest_edge_weight[i]
                    });
                    totalWeight += cheapest_edge_weight[i];
                    unionSets(parent, rank, set1, set2);
                    components--;
                }
            }
        }
    }

    // Print the MST
    std::cout << "\nMST Output" << std::endl;
    for (const std::pair<std::pair<int, int>, int>& edge : mst_edges) {
        std::cout << "[" << edge.first.first << "--" << edge.first.second << "]" << std::endl;
    }
    std::cout << "Total weight: " << totalWeight << std::endl;
}

std::istream& operator>>(std::istream& is, Boruvka& b) {
    std::cout << "Enter the total number of vertices: ";
    is >> b.total_vertices;
    std::cout << "Enter the total number of edges: ";
    is >> b.total_edges;
    return is;
}

