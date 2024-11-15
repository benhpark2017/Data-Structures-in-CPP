/**
 * Solutions to Chapter 6, Section 7, Exercise 11 of Horowitz, Sahni, and
 * Mehta's Fundamentals of Data Structures in C++.
 * 
 * Task 11:
 * Write a set of computer programs for manipulating graphs. Such a collection
 * should allow input and output of arbitrary graphs, determining connected
 * components, spanning trees, minimum-cost spanning trees, biconnected
 * components, shortest paths, and so on. The capability of attaching weights to
 * the edges should also be provided. Test the correctness of your program suite
 * using suitable test data.
 */

/**********************************main.cpp************************************/
#include "Graph.h"

int main() {
    
    char choice;
    char options;
    char hasWeights;
    char whichAlgo;
    char pickAlgo;
    int source;
    
    Graph graph;
    
    std::cout << "Launching the test suite." << std::endl;
    std::cout << "First, tell us a little about your graph." << std::endl;
    std::cout << "Is the graph undirected or directed?" << std::endl;
    std::cout << "Enter 'u' for undirected or 'd' for directed graphs." << std::endl;
    std::cin >> choice;
    std::cout << "Does the graph have weights on the edges(paths)? Enter Y/N" << std::endl;
    std::cin >> hasWeights;
    
    if ( (choice == 'u' || choice == 'U') && (hasWeights == 'N' || hasWeights == 'n') ) {
        std::cout << "You chose an undirected graph without weights." << std::endl;
        std::cout << "Please choose from the following options:" << std::endl;
        
        std::cout << "Print Adjacency Matrix (Enter X)" << std::endl;          
        std::cout << "Print Adjacency List (Enter L)" << std::endl;             
        std::cout << "Print Adjacency Multilist (Enter M)" << std::endl;        
        std::cout << "Find connected components of a graph (Enter C)" << std::endl; 
        std::cout << "Find biconnected components of a graph (Enter F)" << std::endl;
        std::cout << "Print BFS spanning tree (Enter B)" << std::endl;       
        std::cout << "Print DFS spanning tree (Enter D)" << std::endl;       
        std::cin >> options;
        
        switch (options) {
            case 'X': 
            case 'x':
                graph.GraphIntoAdjacencyMatrix(); 
                break;
            case 'L': 
            case 'l': 
                graph.GraphIntoAdjacencyList();
                graph.DisplayAdjacencyList();
                break;
            case 'M': 
            case 'm': 
                graph.GraphIntoMultilist();
                graph.DisplayMultilist();
                break;
            case 'B':
            case 'b':
                graph.ReadAdjList();
                graph.BFSAdjList();
                break;
            case 'C':    
            case 'c':
                graph.ReadAdjList();
                graph.FindConnectedComponents();
                break;
            case 'F':
            case 'f':
                graph.ReadAdjList();
                graph.findBiconnectedComponents();
                break;
            case 'D':
            case 'd':
                graph.ReadAdjList();
                graph.DFSAdjList();
                break;
            default:
                std::cout << "Wrong input for options. Please try again." << std::endl;
                break;
        }
    } else if ((choice == 'u' || choice == 'U') && (hasWeights == 'Y' || hasWeights == 'y')) {
        std::cout << "Please choose from the following options:" << std::endl;
        std::cout << "Generate minimum-cost spanning tree using Kruskal's algorithm (Enter K)" << std::endl;
        std::cout << "Generate minimum-cost spanning tree using Prim's algorithm (Enter P)" << std::endl;
        std::cout << "Generate minimum-cost spanning tree using Sollin's algorithm (Enter S)" << std::endl;
        std::cin >> whichAlgo;
        
        switch (whichAlgo) {
            case 'K': case 'k': 
                graph.ReadWeightedAdjList();
                graph.KruskalMST();
                break;
            case 'P': case 'p':
                graph.ReadWeightedAdjList();
                graph.PrimMST();
                break;
            case 'S': case 's':
                graph.ReadWeightedAdjList();
                graph.SollinMST();
                break;
            default: 
                std::cout << "Wrong input for options. Please try again." << std::endl;
                break;
        }
        
    } else if ( ((choice == 'd') || (choice == 'D')) && ((hasWeights == 'Y') || (hasWeights == 'y')) ) {
        std::cout << "Find shortest path of an undirected graph using Dijkstra's algorithm (Enter D)" << std::endl;
        std::cout << "Find shortest path of an undirected graph using Bellman-Ford's algorithm (Enter B)" << std::endl;
        std::cin >> pickAlgo;
        switch (pickAlgo) {
            case 'D': case 'd':
                graph.ReadDirectedWeightedAdjList();
                std::cout << "Please specify the source vertex (starting vertex) with a number: ";
                std::cin >> source;
                graph.Dijkstra(source);
                break;
            case 'B': case 'b':
                graph.ReadDirectedWeightedAdjList();
                std::cout << "Please specify the source vertex (starting vertex) with a number: ";
                std::cin >> source;
                graph.BellmanFord(source);
                break;
            default:
                std::cout << "Wrong input for options. Please try again." << std::endl;
                break;
        }
    }

    std::cout << "End of program." << std::endl;
    return 0;
}



/**********************************Graph.h*************************************/
#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
#include <queue>
#include <stack>

enum Boolean {FALSE, TRUE};

class GraphEdge {
    friend class Graph;
  private:
    int vertex1, vertex2;
    GraphEdge* path1;
    GraphEdge* path2;
    Boolean m;
  public:
    GraphEdge() {};
    GraphEdge(int v1, int v2);
};

using EdgePtr = GraphEdge*;

class Graph {
  private:
    std::vector<std::vector<int>> adjacencyList;
    std::vector<EdgePtr> allEdges;
    EdgePtr* HeadNodes;
    int n;
    bool* visited;
    bool** adjMatrix;
    
    struct Edge {
        int src, dest, weight;
        Edge(int s, int d, int w) : src(s), dest(d), weight(w) {}
    };
    
    class DisjointSet {
        std::vector<int> parent, rank;
      public:
        DisjointSet(int n);
        int find(int x);
        void unite(int x, int y);
    };
    
    std::vector<int> disc;    // Discovery times
    std::vector<int> low;     // Earliest reachable vertex
    std::vector<int> parent;  // Parent vertices in DFS tree
    int time;                 // Time counter for DFS
    
    // Modified to use pairs instead of Edge struct since we're working with undirected edges
    std::stack<std::pair<int, int>> st;
    
    std::vector<Edge> edges;  // Store edges with weights
    
    void DFSAdjMatrixUtil(int vertex);       
    void DFSAdjListUtil(int vertex);         
    void BFSAdjMatrixUtil(int startVertex);
    void BFSAdjListUtil(int startVertex);    
    void ClearGraph();                      
    void CleanupAdjMatrix();                 
    void AddEdge(int from, int to);          
    
    void DFSUtilForComponents(int vertex, int compNum, std::vector<int>& component);

    // Helper functions for Sollin's algorithm
    int findComponent(int vertex, std::vector<int>& components);
    void mergeTrees(int comp1, int comp2, std::vector<int>& components);
    Edge findMinEdge(int component, const std::vector<int>& components,
                    const std::vector<std::vector<std::pair<int, int>>>& adjListWithWeights);

    void BCCUtil(int u, std::vector<std::vector<std::pair<int, int>>>& components);

    void printPath(const std::vector<int>& parent, int vertex);
    
  public:
    Graph();
    Graph(const int vertices);
    ~Graph();

    void InsertVertex(int v);
    void InsertEdge(int u, int v);
    void GraphIntoAdjacencyMatrix();
    void GraphIntoAdjacencyList();
    void DisplayAdjacencyList();
    void GraphIntoMultilist();
    void DisplayMultilist();
    void FindConnectedComponents();

    void InitializeAdjMatrix();              
    void ReadAdjMatrix();                   
    void PrintAdjMatrix() const;             
    void DFSAdjMatrix();                     
    void BFSAdjMatrix();                     
    void ReadAdjList();                      
    void PrintAdjList() const;               
    void DFSAdjList();                       
    void BFSAdjList();                       
    
    void AddWeightedEdge(int from, int to, int weight);
    void ReadWeightedAdjList();
    void KruskalMST();
    void PrimMST();
    void SollinMST();
    
    void findBiconnectedComponents();

    void AddDirectedWeightedEdge(int from, int to, int weight);
    void ReadDirectedWeightedAdjList();
    void Dijkstra(int source);
    void BellmanFord(int source);
};

#endif // GRAPH_H



/*********************************Graph.cpp************************************/
#include "Graph.h"

// Constructor: Initialize parent and rank arrays
Graph::DisjointSet::DisjointSet(int n) {
    parent.resize(n);
    rank.resize(n);
    for (int i = 0; i < n; i++) {
        parent[i] = i;
        rank[i] = 0;
    }
}

// Find: Path compression to find the representative of the set
int Graph::DisjointSet::find(int x) {
    if (parent[x] != x) {
        parent[x] = find(parent[x]); // Path compression
    }
    return parent[x];
}

// Unite: Union by rank to merge two sets
void Graph::DisjointSet::unite(int x, int y) {
    int px = find(x), py = find(y);
    if (px == py) return;

    if (rank[px] < rank[py]) {
        parent[px] = py;
    } else if (rank[px] > rank[py]) {
        parent[py] = px;
    } else {
        parent[py] = px;
        rank[px]++;
    }
}

void Graph::DFSAdjMatrixUtil(int vertex) {
    visited[vertex] = TRUE;
    std::cout << vertex << " ";
    
    for (int i = 0; i < n; i++) {
        if (adjMatrix[vertex][i] && !visited[i]) {
            DFSAdjMatrixUtil(i);
        }
    }
}

void Graph::DFSAdjListUtil(int vertex) {
    visited[vertex] = TRUE;
    std::cout << vertex << " ";
    
    EdgePtr current = HeadNodes[vertex];
    while (current) {
        int nextVertex;
        if (current->vertex1 == vertex) nextVertex = current->vertex2;
        else nextVertex = current->vertex1;
        
        if (!visited[nextVertex]) {
            DFSAdjListUtil(nextVertex);
        }
        
        current = current->path1;
    }
}

void Graph::BFSAdjMatrixUtil(int startVertex) {
    std::vector<int> queue;
    
    visited[startVertex] = TRUE;
    queue.push_back(startVertex);
    
    while (!queue.empty()) {
        int currentVertex = queue.front();
        std::cout << currentVertex << " ";
        queue.erase(queue.begin());
        
        for (int i = 0; i < n; i++) {
            if (adjMatrix[currentVertex][i] && !visited[i]) {
                visited[i] = TRUE;
                queue.push_back(i);
            }
        }
    }
}

void Graph::BFSAdjListUtil(int startVertex) {
    std::vector<int> queue;
    
    visited[startVertex] = TRUE;
    queue.push_back(startVertex);
    
    while (!queue.empty()) {
        int currentVertex = queue.front();
        std::cout << currentVertex << " ";
        queue.erase(queue.begin());
        
        EdgePtr current = HeadNodes[currentVertex];
        while (current) {
            int adjacentVertex;
            
            if (current->vertex1 == currentVertex) {
                adjacentVertex = current->vertex2;
            } else {
                adjacentVertex = current->vertex1;
            }
            
            if (!visited[adjacentVertex]) {
                visited[adjacentVertex] = TRUE;
                queue.push_back(adjacentVertex);
            }
            
            current = current->path1;
        }
    }
}

void Graph::ClearGraph() {
    if (HeadNodes) {
        delete[] HeadNodes;
        HeadNodes = nullptr;
    }
}

void Graph::CleanupAdjMatrix() {
    if (adjMatrix) {
        for (int i = 0; i < n; i++) {
            delete[] adjMatrix[i];
        }
        delete[] adjMatrix;
        adjMatrix = nullptr;
    }
}

void Graph::AddEdge(int from, int to) {
    // Update the adjacencyList when adding edges
    if (from >= adjacencyList.size()) {
        adjacencyList.resize(from + 1);
    }
    if (to >= adjacencyList.size()) {
        adjacencyList.resize(to + 1);
    }
    adjacencyList[from].push_back(to);
    adjacencyList[to].push_back(from); // For undirected graph

    EdgePtr newEdge = new GraphEdge;
    newEdge->vertex1 = from;
    newEdge->vertex2 = to;
    newEdge->m = FALSE;

    newEdge->path1 = HeadNodes[from];
    HeadNodes[from] = newEdge;

    EdgePtr reverseEdge = new GraphEdge;
    reverseEdge->vertex1 = to;
    reverseEdge->vertex2 = from;
    reverseEdge->m = FALSE;

    reverseEdge->path1 = HeadNodes[to];
    HeadNodes[to] = reverseEdge;

    allEdges.push_back(newEdge);
    allEdges.push_back(reverseEdge);
}

GraphEdge::GraphEdge(int v1, int v2)
    : vertex1(v1), vertex2(v2), path1(nullptr), path2(nullptr), m(FALSE) {}

Graph::Graph() : n(0), HeadNodes(nullptr) {}

Graph::Graph(const int vertices) : n(vertices) {
    HeadNodes = new EdgePtr[n]();
    adjacencyList.resize(n);
    visited = new bool[n]();
}

void Graph::InsertVertex(int v) {
    if (v >= n) {
        EdgePtr* newHeadNodes = new EdgePtr[v + 1];
        for (int i = 0; i < n; i++) {
            newHeadNodes[i] = HeadNodes[i];
        }
        for (int i = n; i <= v; i++) {
            newHeadNodes[i] = nullptr;
        }
        delete[] HeadNodes;
        HeadNodes = newHeadNodes;
        n = v + 1;
    }
}

void Graph::InsertEdge(int u, int v) {
    EdgePtr newEdge = new GraphEdge(u, v);
    allEdges.push_back(newEdge);

    if (HeadNodes[u] == nullptr) {
        HeadNodes[u] = newEdge;
    } else {
        EdgePtr current = HeadNodes[u];
        while (current->path1 != nullptr) {
            current = current->path1;
        }
        current->path1 = newEdge;
    }

    if (HeadNodes[v] == nullptr) {
        HeadNodes[v] = newEdge;
    } else {
        EdgePtr current = HeadNodes[v];
        while (current->path2 != nullptr) {
            current = current->path2;
        }
        current->path2 = newEdge;
    }
}

void Graph::GraphIntoAdjacencyMatrix() {
    int numVertices, numEdges;
    std::cout << "Enter the number of vertices: ";
    std::cin >> numVertices;
    std::cout << "Enter the number of edges: ";
    std::cin >> numEdges;

    if (numVertices > n) {
        InsertVertex(numVertices - 1);
    }

    std::vector<std::vector<int>> adjacencyMatrix(numVertices, std::vector<int>(numVertices, 0));

    std::cout << "Enter the edges (vertex pairs) separated by new lines:\n";
    for (int i = 0; i < numEdges; ++i) {
        int u, v;
        std::cin >> u >> v;
        InsertEdge(u, v);
        adjacencyMatrix[u][v] = 1;
        adjacencyMatrix[v][u] = 1;
    }

    std::cout << "\nAdjacency Matrix:\n";
    for (int i = 0; i < numVertices; ++i) {
        for (int j = 0; j < numVertices; ++j) {
            std::cout << adjacencyMatrix[i][j] << " ";
        }
        std::cout << "\n";
    }
}

void Graph::GraphIntoAdjacencyList() {
    int numEdges;
    std::cout << "Enter the number of vertices: ";
    std::cin >> n;
    adjacencyList.resize(n);

    std::cout << "Enter the number of edges: ";
    std::cin >> numEdges;

    std::cout << "Enter the pairs of vertices for each edge:\n";
    for (int i = 0; i < numEdges; ++i) {
        int u, v;
        std::cin >> u >> v;
        if (u >= n || v >= n) {
            std::cerr << "Invalid vertex index. Vertex indices should be between 0 and "
                     << n - 1 << ".\n";
            continue;
        }
        adjacencyList[u].push_back(v);
        adjacencyList[v].push_back(u);
    }
}

void Graph::DisplayAdjacencyList() {
    std::cout << "Adjacency List:\n";
    for (int i = 0; i < n; ++i) {
        std::cout << i << ": ";
        for (size_t j = 0; j < adjacencyList[i].size(); ++j) {
            std::cout << adjacencyList[i][j];
            if (j < adjacencyList[i].size() - 1) {
                std::cout << " -> ";
            }
        }
        std::cout << "\n";
    }
}

void Graph::GraphIntoMultilist() {
    std::cout << "Enter the number of vertices: ";
    std::cin >> n;

    delete[] HeadNodes;
    HeadNodes = new GraphEdge*[n]();

    int numEdges;
    std::cout << "Enter the number of edges: ";
    std::cin >> numEdges;

    std::cout << "Enter the pairs of vertices for each edge:\n";
    for (int i = 0; i < numEdges; ++i) {
        int u, v;
        std::cin >> u >> v;

        if (u >= n || v >= n || u < 0 || v < 0) {
            std::cerr << "Invalid vertex indices. Must be between 0 and " << n - 1 << "\n";
            --i;
            continue;
        }

        GraphEdge* newEdge = new GraphEdge(u, v);
        allEdges.push_back(newEdge);

        if (HeadNodes[u] == nullptr) {
            HeadNodes[u] = newEdge;
        } else {
            GraphEdge* current = HeadNodes[u];
            while (current->path1 != nullptr &&
                   ((current->vertex1 == u && current->path1 != nullptr) ||
                    (current->vertex2 == u && current->path2 != nullptr))) {
                current = (current->vertex1 == u) ? current->path1 : current->path2;
            }
            if (current->vertex1 == u) {
                current->path1 = newEdge;
            } else {
                current->path2 = newEdge;
            }
        }

        if (HeadNodes[v] == nullptr) {
            HeadNodes[v] = newEdge;
        } else {
            GraphEdge* current = HeadNodes[v];
            while (current->path2 != nullptr &&
                   ((current->vertex1 == v && current->path1 != nullptr) ||
                    (current->vertex2 == v && current->path2 != nullptr))) {
                current = (current->vertex1 == v) ? current->path1 : current->path2;
            }
            if (current->vertex1 == v) {
                current->path1 = newEdge;
            } else {
                current->path2 = newEdge;
            }
        }
    }
}

void Graph::DisplayMultilist() {
    std::cout << "\nVertex Lists:\n";
    for (int i = 0; i < n; i++) {
        std::cout << "vertex " << i << ": ";
        GraphEdge* current = HeadNodes[i];
        while (current != nullptr) {
            int edgeIndex = std::find(allEdges.begin(), allEdges.end(), current) - allEdges.begin();
            std::cout << "N" << edgeIndex;

            if (current->vertex1 == i) {
                current = current->path1;
            } else {
                current = current->path2;
            }

            if (current != nullptr) std::cout << " -> ";
        }
        std::cout << " -> null" << std::endl;
    }

    std::cout << "\nEdge Nodes:\n";
    for (size_t i = 0; i < allEdges.size(); ++i) {
        GraphEdge* edge = allEdges[i];
        std::string next1 = "0", next2 = "0";

        if (edge->path1 != nullptr) {
            next1 = "N" + std::to_string(
                std::find(allEdges.begin(), allEdges.end(), edge->path1) - allEdges.begin()
            );
        }
        if (edge->path2 != nullptr) {
            next2 = "N" + std::to_string(
                std::find(allEdges.begin(), allEdges.end(), edge->path2) - allEdges.begin()
            );
        }

        std::cout << "N" << i << ": " << edge->vertex1 << " " << edge->vertex2
                 << " " << next1 << " " << next2
                 << " Edge (" << edge->vertex1 << "," << edge->vertex2 << ")" << std::endl;
    }
}

void Graph::InitializeAdjMatrix() {
    adjMatrix = new bool*[n];
    for (int i = 0; i < n; i++) {
        adjMatrix[i] = new bool[n];
        for (int j = 0; j < n; j++) {
            adjMatrix[i][j] = FALSE;
        }
    }
}

void Graph::ReadAdjMatrix() {
    CleanupAdjMatrix();
    
    do {
        std::cout << "Enter the number of vertices (must be positive): ";
        std::cin >> n;
        
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            n = 0;
        }
    } while (n <= 0);
    
    InitializeAdjMatrix();
    
    std::cout << "\nEnter the adjacency matrix (" << n << "x" << n << "):\n";
    std::cout << "Use 1 for edge, 0 for no edge\n";
    std::cout << "Enter row by row, separating each number by space:\n\n";
    
    for (int i = 0; i < n; i++) {
        std::cout << "Row " << i << ": ";
        for (int j = 0; j < n; j++) {
            int value;
            bool validInput = FALSE;
            
            do {
                std::cin >> value;
                
                if (std::cin.fail()) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Invalid input. Please enter 0 or 1 for position [" 
                              << i << "][" << j << "]: ";
                    continue;
                }
                
                if (value != 0 && value != 1) {
                    std::cout << "Invalid value. Please enter 0 or 1 for position [" 
                              << i << "][" << j << "]: ";
                    continue;
                }
                
                validInput = TRUE;
            } while (!validInput);
            
            adjMatrix[i][j] = (value == 1);
        }
        
        if (std::cin.get() != '\n') {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
    
    bool isSymmetric = TRUE;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < i; j++) {
            if (adjMatrix[i][j] != adjMatrix[j][i]) {
                isSymmetric = FALSE;
                break;
            }
        }
        if (!isSymmetric) break;
    }
    
    if (!isSymmetric) {
        std::cout << "\nWarning: The input matrix is not symmetric. ";
        std::cout << "This means the graph is directed.\n";
    }
    
    std::cout << "\nEntered adjacency matrix:\n";
    PrintAdjMatrix();
}

void Graph::PrintAdjMatrix() const {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            std::cout << (adjMatrix[i][j] ? "1 " : "0 ");
        }
        std::cout << std::endl;
    }
}

void Graph::DFSAdjMatrix() {
    visited = new bool[n];
    for (int i = 0; i < n; i++) {
        visited[i] = FALSE;
    }
    
    std::cout << "\nDFS traversal starting from vertex 0: ";
    DFSAdjMatrixUtil(0);
    
    for (int i = 0; i < n; i++) {
        if (!visited[i]) {
            std::cout << "\nDisconnected component starting from vertex " << i << ": ";
            DFSAdjMatrixUtil(i);
        }
    }
    std::cout << std::endl;
    
    delete[] visited;
}

void Graph::BFSAdjMatrix() {
    if (!adjMatrix) {
        std::cout << "Graph is empty. Please read the adjacency matrix first.\n";
        return;
    }

    visited = new bool[n];
    for (int i = 0; i < n; i++) {
        visited[i] = FALSE;
    }
    
    std::cout << "\nBFS traversal starting from vertex 0: ";
    BFSAdjMatrixUtil(0);
    
    for (int i = 0; i < n; i++) {
        if (!visited[i]) {
            std::cout << "\nDisconnected component starting from vertex " << i << ": ";
            BFSAdjMatrixUtil(i);
        }
    }
    std::cout << std::endl;
    
    delete[] visited;
    visited = nullptr;
}

void Graph::ReadAdjList() {
    std::cout << "Enter the number of vertices: ";
    std::cin >> n;

    ClearGraph();

    // Initialize both HeadNodes and adjacencyList
    HeadNodes = new EdgePtr[n];
    adjacencyList.clear();
    adjacencyList.resize(n);
    
    for (int i = 0; i < n; i++) {
        HeadNodes[i] = nullptr;
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

            bool edgeExists = false;
            for (int adj : adjacencyList[i]) {
                if (adj == adjacent) {
                    edgeExists = true;
                    break;
                }
            }

            if (!edgeExists) {
                AddEdge(i, adjacent);
            }
        }
    }

    std::cout << "\nGraph has been successfully read.\n";
}

void Graph::PrintAdjList() const {
    if (!HeadNodes) {
        std::cout << "Graph is empty. Please read the adjacency list first.\n";
        return;
    }

    std::cout << "\nAdjacency List:\n";
    for (int i = 0; i < n; i++) {
        std::cout << i << " -> ";
        EdgePtr current = HeadNodes[i];
        bool first = TRUE;
        
        while (current) {
            if (!first) {
                std::cout << ", ";
            }
            std::cout << current->vertex2;
            current = current->path1;
            first = FALSE;
        }
        std::cout << std::endl;
    }
}

void Graph::DFSAdjList() {
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

void Graph::BFSAdjList() {
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

Graph::~Graph() {
    for (EdgePtr edge : allEdges) {
        delete edge;
    }
    allEdges.clear();
    if (visited) {
        delete[] visited;
        visited = nullptr;
    }
    ClearGraph();
    CleanupAdjMatrix();
}

void Graph::DFSUtilForComponents(int vertex, int compNum, std::vector<int>& component) {
    std::stack<int> stack;
    stack.push(vertex);
    visited[vertex] = true;
    component[vertex] = compNum;

    while (!stack.empty()) {
        int v = stack.top();
        stack.pop();

        // Bounds check to ensure v is within adjacencyList size
        if (v < 0 || v >= n) {
            std::cerr << "Error: Vertex " << v << " is out of bounds!" << std::endl;
            continue;  // Skip any invalid vertex
        }

        // Explore each neighbor
        for (int neighbor : adjacencyList[v]) {
            if (neighbor < 0 || neighbor >= n) {
                std::cerr << "Error: Neighbor " << neighbor << " is out of bounds!" << std::endl;
                continue;
            }
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                component[neighbor] = compNum;
                stack.push(neighbor);
            }
        }
    }
}

void Graph::FindConnectedComponents() {
    // Initialize visited array if it's not already
    if (!visited) {
        visited = new bool[n]();
    } else {
        std::fill(visited, visited + n, false);
    }

    // Vector to store the component number for each vertex
    std::vector<int> component(n, -1);
    int componentNumber = 0;

    // Find connected components
    for (int v = 0; v < n; ++v) {
        if (!visited[v]) {
            // Start a new component
            DFSUtilForComponents(v, componentNumber, component);
            componentNumber++;
        }
    }

    // Display the results
    std::cout << "Number of connected components: " << componentNumber << std::endl;
    for (int i = 0; i < componentNumber; ++i) {
        std::cout << "Component " << i << ": ";
        for (int v = 0; v < n; ++v) {
            if (component[v] == i) {
                std::cout << v << " ";
            }
        }
        std::cout << std::endl;
    }
}

void Graph::AddWeightedEdge(int from, int to, int weight) {
    // Update adjacencyList
    if (from >= adjacencyList.size()) {
        adjacencyList.resize(from + 1);
    }
    if (to >= adjacencyList.size()) {
        adjacencyList.resize(to + 1);
    }
    adjacencyList[from].push_back(to);
    adjacencyList[to].push_back(from);
    
    // Store the edge with weight
    edges.emplace_back(from, to, weight);
    
    // Update the existing graph structure
    EdgePtr newEdge = new GraphEdge;
    newEdge->vertex1 = from;
    newEdge->vertex2 = to;
    newEdge->m = FALSE;
    
    newEdge->path1 = HeadNodes[from];
    HeadNodes[from] = newEdge;
    
    EdgePtr reverseEdge = new GraphEdge;
    reverseEdge->vertex1 = to;
    reverseEdge->vertex2 = from;
    reverseEdge->m = FALSE;
    
    reverseEdge->path1 = HeadNodes[to];
    HeadNodes[to] = reverseEdge;
    
    allEdges.push_back(newEdge);
    allEdges.push_back(reverseEdge);
}

void Graph::ReadWeightedAdjList() {
    std::cout << "Enter the number of vertices: ";
    std::cin >> n;
    
    ClearGraph();
    edges.clear();
    
    HeadNodes = new EdgePtr[n];
    adjacencyList.clear();
    adjacencyList.resize(n);
    
    for (int i = 0; i < n; i++) {
        HeadNodes[i] = nullptr;
    }
    
    std::cout << "\nFor each vertex, enter adjacent vertices and weights in the format: vertex weight\n";
    std::cout << "Enter -1 -1 to end the list for a vertex.\n\n";
    
    for (int i = 0; i < n; i++) {
        std::cout << "Enter adjacent vertices and weights for vertex " << i << ": ";
        
        while (true) {
            int adjacent, weight;
            std::cin >> adjacent >> weight;
            
            if (adjacent == -1 && weight == -1) break;
            
            if (adjacent < 0 || adjacent >= n || adjacent == i) {
                std::cout << "Invalid vertex. Please enter a number between 0 and "
                         << n - 1 << " (excluding " << i << ") or -1 -1 to end: ";
                continue;
            }
            
            // Check if edge already exists
            bool edgeExists = false;
            for (const Edge& e : edges) {
                if ((e.src == i && e.dest == adjacent) || 
                    (e.src == adjacent && e.dest == i)) {
                    edgeExists = true;
                    break;
                }
            }
            
            if (!edgeExists) {
                AddWeightedEdge(i, adjacent, weight);
            }
        }
    }
    
    std::cout << "\nGraph has been successfully read.\n";
}

void Graph::KruskalMST() {
    if (edges.empty()) {
        std::cout << "Graph is empty. Please read the weighted adjacency list first.\n";
        return;
    }
    
    // Sort edges by weight
    std::sort(edges.begin(), edges.end(), 
              [](const Edge& a, const Edge& b) { return a.weight < b.weight; });
    
    DisjointSet ds(n);
    std::vector<Edge> mst;
    int mstWeight = 0;
    
    // Process each edge in sorted order
    for (const Edge& edge : edges) {
        int src = edge.src;
        int dest = edge.dest;
        
        // Check if including this edge creates a cycle
        if (ds.find(src) != ds.find(dest)) {
            // Include edge in MST
            mst.push_back(edge);
            mstWeight += edge.weight;
            ds.unite(src, dest);
        }
    }
    
    // Print the MST
    std::cout << "\nMinimum Spanning Tree edges:\n";
    for (const Edge& edge : mst) {
        std::cout << edge.src << " -- " << edge.dest 
                 << " \tweight: " << edge.weight << "\n";
    }
    std::cout << "Total MST weight: " << mstWeight << "\n";
}

void Graph::PrimMST() {
    if (edges.empty()) {
        std::cout << "Graph is empty. Please read the weighted adjacency list first.\n";
        return;
    }

    // Create adjacency matrix with weights for easier access
    std::vector<std::vector<int>> weightMatrix(n, std::vector<int>(n, std::numeric_limits<int>::max()));
    
    // Fill the weight matrix from edges
    for (const Edge& edge : edges) {
        weightMatrix[edge.src][edge.dest] = edge.weight;
        weightMatrix[edge.dest][edge.src] = edge.weight; // For undirected graph
    }

    // Arrays needed for MST construction
    std::vector<bool> inMST(n, false);     // Track vertices in MST
    std::vector<int> key(n, std::numeric_limits<int>::max());      // Store minimum weights
    std::vector<int> parent(n, -1);        // Store MST edges
    
    // Start with first vertex
    key[0] = 0;
    
    int mstWeight = 0;
    std::vector<Edge> mstEdges;  // Store the MST edges

    // Find MST with (V-1) edges
    for (int count = 0; count < n; count++) {
        // Find vertex with minimum key value from vertices not yet included in MST
        int minKey = std::numeric_limits<int>::max();
        int u = -1;
        
        for (int v = 0; v < n; v++) {
            if (!inMST[v] && key[v] < minKey) {
                minKey = key[v];
                u = v;
            }
        }
        
        // If no such vertex found, graph might be disconnected
        if (u == -1) {
            std::cout << "Warning: Graph might be disconnected!\n";
            break;
        }

        // Add chosen vertex to MST
        inMST[u] = true;
        mstWeight += minKey;

        // Store the edge in MST (except for first vertex)
        if (parent[u] != -1) {
            mstEdges.emplace_back(parent[u], u, weightMatrix[parent[u]][u]);
        }

        // Update key values of adjacent vertices
        for (int v = 0; v < n; v++) {
            // Update key if:
            // 1. There is an edge u-v
            // 2. v is not in MST
            // 3. Weight of edge u-v is smaller than current key of v
            if (weightMatrix[u][v] != std::numeric_limits<int>::max() && 
                !inMST[v] && 
                weightMatrix[u][v] < key[v]) {
                parent[v] = u;
                key[v] = weightMatrix[u][v];
            }
        }
    }

    // Print the MST
    std::cout << "\nMinimum Spanning Tree edges (using Simple Prim's algorithm):\n";
    for (const Edge& edge : mstEdges) {
        std::cout << edge.src << " -- " << edge.dest 
                 << " \tweight: " << edge.weight << "\n";
    }
    std::cout << "Total MST weight: " << mstWeight << "\n";

    // Print path verification
    std::cout << "\nPath verification:\n";
    for (int i = 1; i < n; i++) {
        if (parent[i] != -1) {
            std::cout << "Vertex " << i << " is connected to MST through vertex " 
                     << parent[i] << " with weight " 
                     << weightMatrix[parent[i]][i] << "\n";
        }
    }

    // Check if MST spans all vertices
    bool isConnected = true;
    for (int i = 0; i < n; i++) {
        if (!inMST[i]) {
            isConnected = false;
            std::cout << "Vertex " << i << " is not connected to the MST!\n";
        }
    }
    if (!isConnected) {
        std::cout << "Note: The graph is not fully connected.\n";
    }
}

int Graph::findComponent(int vertex, std::vector<int>& components) {
    if (components[vertex] != vertex) {
        components[vertex] = findComponent(components[vertex], components);
    }
    return components[vertex];
}

void Graph::mergeTrees(int comp1, int comp2, std::vector<int>& components) {
    int root1 = findComponent(comp1, components);
    int root2 = findComponent(comp2, components);
    if (root1 != root2) {
        components[root2] = root1;
    }
}

Graph::Edge Graph::findMinEdge(int component, 
                       const std::vector<int>& components,
                       const std::vector<std::vector<std::pair<int, int>>>& adjListWithWeights) {
    Edge minEdge(-1, -1, std::numeric_limits<int>::max());
    
    // Check all vertices in this component
    for (int v = 0; v < n; v++) {
        if (findComponent(v, const_cast<std::vector<int>&>(components)) != component) {
            continue;
        }
        
        // Check all adjacent edges of vertex v
        for (const auto& adj : adjListWithWeights[v]) {
            int u = adj.first;
            int weight = adj.second;
            
            int compU = findComponent(u, const_cast<std::vector<int>&>(components));
            
            // If edge connects to different component and has lower weight
            if (compU != component && weight < minEdge.weight) {
                minEdge = Edge(v, u, weight);
            }
        }
    }
    
    return minEdge;
}

void Graph::SollinMST() {
    if (edges.empty()) {
        std::cout << "Graph is empty. Please read the weighted adjacency list first.\n";
        return;
    }

    // Create adjacency list with weights for easier access
    std::vector<std::vector<std::pair<int, int>>> adjListWithWeights(n);
    for (const Edge& edge : edges) {
        adjListWithWeights[edge.src].push_back({edge.dest, edge.weight});
        adjListWithWeights[edge.dest].push_back({edge.src, edge.weight});
    }

    // Initialize components (each vertex starts in its own component)
    std::vector<int> components(n);
    for (int i = 0; i < n; i++) {
        components[i] = i;
    }

    // Store MST edges and total weight
    std::vector<Edge> mstEdges;
    int mstWeight = 0;
    int numComponents = n;

    // Main loop - continue until we have a single component
    while (numComponents > 1) {
        std::vector<Edge> cheapestEdges;
        std::vector<bool> processedComponents(n, false);

        // Find all unique components and their cheapest edges
        for (int i = 0; i < n; i++) {
            int comp = findComponent(i, components);
            if (!processedComponents[comp]) {
                processedComponents[comp] = true;
                Edge minEdge = findMinEdge(comp, components, adjListWithWeights);
                if (minEdge.weight != std::numeric_limits<int>::max()) {
                    cheapestEdges.push_back(minEdge);
                }
            }
        }

        // If no more edges found, break (graph might be disconnected)
        if (cheapestEdges.empty()) {
            break;
        }

        // Add all cheapest edges to MST and merge components
        for (const Edge& edge : cheapestEdges) {
            int comp1 = findComponent(edge.src, components);
            int comp2 = findComponent(edge.dest, components);

            if (comp1 != comp2) {
                mstEdges.push_back(edge);
                mstWeight += edge.weight;
                mergeTrees(comp1, comp2, components);
                numComponents--;
            }
        }
    }

    // Print the MST
    std::cout << "\nMinimum Spanning Tree edges (using Sollin's algorithm):\n";
    for (const Edge& edge : mstEdges) {
        std::cout << edge.src << " -- " << edge.dest
                 << " \tweight: " << edge.weight << "\n";
    }
    std::cout << "Total MST weight: " << mstWeight << "\n";

    // Check if the graph is connected
    std::vector<bool> foundComponents(n, false);
    int distinctComponents = 0;
    for (int i = 0; i < n; i++) {
        int comp = findComponent(i, components);
        if (!foundComponents[comp]) {
            foundComponents[comp] = true;
            distinctComponents++;
        }
    }

    if (distinctComponents > 1) {
        std::cout << "\nWarning: Graph is not connected!\n";
        std::cout << "Number of disconnected components: " << distinctComponents << "\n";

        // Print components
        std::cout << "\nComponents:\n";
        std::vector<std::vector<int>> componentVertices(n);
        for (int i = 0; i < n; i++) {
            int comp = findComponent(i, components);
            componentVertices[comp].push_back(i);
        }

        for (int i = 0; i < n; i++) {
            if (!componentVertices[i].empty()) {
                std::cout << "Component " << i << ": ";
                for (int vertex : componentVertices[i]) {
                    std::cout << vertex << " ";
                }
                std::cout << "\n";
            }
        }
    }

    // Print detailed component information
    std::cout << "\nDetailed tree information:\n";
    std::vector<std::vector<Edge>> treeEdges(n);
    for (const Edge& edge : mstEdges) {
        int comp = findComponent(edge.src, components);
        treeEdges[comp].push_back(edge);
    }

    for (int i = 0; i < n; i++) {
        if (!treeEdges[i].empty()) {
            std::cout << "Tree " << i << " edges:\n";
            int treeWeight = 0;
            for (const Edge& edge : treeEdges[i]) {
                std::cout << " " << edge.src << " -- " << edge.dest
                         << " (weight: " << edge.weight << ")\n";
                treeWeight += edge.weight;
            }
            std::cout << " Total tree weight: " << treeWeight << "\n";
        }
    }
}

// Modified implementation to work with ReadAdjList() initialized graph
void Graph::BCCUtil(int u, std::vector<std::vector<std::pair<int, int>>>& components) {
    // Initialize discovery and low time for current vertex
    disc[u] = low[u] = ++time;
    int children = 0;
    
    // Examine all adjacent vertices from adjacencyList
    for (int v : adjacencyList[u]) {
        // If vertex v is not visited
        if (disc[v] == -1) {
            children++;
            parent[v] = u;
            
            // Store the edge in stack
            st.push({u, v});
            BCCUtil(v, components);
            
            // Check if subtree rooted at v has a connection to ancestor of u
            low[u] = std::min(low[u], low[v]);
            
            // u is an articulation point in following cases:
            // (1) u is root of DFS tree and has two or more children
            // (2) u is not root and low value of one of its children is >= disc value of u
            if ((disc[u] == 1 && children > 1) || (disc[u] > 1 && low[v] >= disc[u])) {
                std::vector<std::pair<int, int>> component;
                while (!st.empty() && !(st.top().first == u && st.top().second == v)) {
                    component.push_back(st.top());
                    st.pop();
                }
                if (!st.empty()) {
                    component.push_back(st.top());
                    st.pop();
                }
                if (!component.empty()) {
                    components.push_back(component);
                }
            }
        }
        // Update low value of u for parent function calls
        else if (v != parent[u] && disc[v] < disc[u]) {
            low[u] = std::min(low[u], disc[v]);
            // Only push edge if we're discovering it backwards
            // (to avoid duplicates in undirected graph)
            if (disc[u] > disc[v]) {
                st.push({u, v});
            }
        }
    }
}

void Graph::findBiconnectedComponents() {
    // Check if graph is empty
    if (adjacencyList.empty()) {
        std::cout << "Graph is empty. Please read the graph first using ReadAdjList().\n";
        return;
    }
    
    // Initialize vectors with size n (number of vertices)
    disc.assign(n, -1);
    low.assign(n, -1);
    parent.assign(n, -1);
    time = 0;
    
    // Clear the stack (in case it's not empty from previous runs)
    while (!st.empty()) {
        st.pop();
    }
    
    std::vector<std::vector<std::pair<int, int>>> components;
    
    // Find biconnected components for each unvisited vertex
    for (int i = 0; i < n; i++) {
        if (disc[i] == -1) {
            BCCUtil(i, components);
            
            // If stack is not empty, pop remaining edges
            if (!st.empty()) {
                std::vector<std::pair<int, int>> component;
                while (!st.empty()) {
                    component.push_back(st.top());
                    st.pop();
                }
                if (!component.empty()) {
                    components.push_back(component);
                }
            }
        }
    }
    
    // Print the components
    if (components.empty()) {
        std::cout << "No biconnected components found.\n";
        return;
    }
    
    std::cout << "\nBiconnected Components:\n";
    for (size_t i = 0; i < components.size(); i++) {
        std::cout << "Component " << i + 1 << ":\n";
        for (const auto& edge : components[i]) {
            std::cout << "(" << edge.first << ", " << edge.second << ") ";
        }
        std::cout << "\n";
    }
}

void Graph::AddDirectedWeightedEdge(int from, int to, int weight) {
    // Update the adjacencyList
    if (from >= adjacencyList.size()) {
        adjacencyList.resize(from + 1);
    }
    if (to >= adjacencyList.size()) {
        adjacencyList.resize(to + 1);
    }
    adjacencyList[from].push_back(to);  // Only add in one direction

    // Store the edge with weight
    edges.emplace_back(from, to, weight);

    // Create new edge
    EdgePtr newEdge = new GraphEdge;
    newEdge->vertex1 = from;
    newEdge->vertex2 = to;
    newEdge->m = FALSE;

    // Add to HeadNodes list for 'from' vertex
    newEdge->path1 = HeadNodes[from];
    HeadNodes[from] = newEdge;

    // Add to master list of edges
    allEdges.push_back(newEdge);
}

void Graph::ReadDirectedWeightedAdjList() {
    std::cout << "Enter the number of vertices: ";
    std::cin >> n;

    ClearGraph();
    edges.clear();

    HeadNodes = new EdgePtr[n];
    adjacencyList.clear();
    adjacencyList.resize(n);

    for (int i = 0; i < n; i++) {
        HeadNodes[i] = nullptr;
    }

    std::cout << "\nFor each vertex, enter outgoing vertices and weights in the format: vertex weight\n";
    std::cout << "Enter -1 -1 to end the list for a vertex.\n\n";

    for (int i = 0; i < n; i++) {
        std::cout << "Enter outgoing vertices and weights for vertex " << i << ": ";

        while (true) {
            int adjacent, weight;
            std::cin >> adjacent >> weight;

            if (adjacent == -1 && weight == -1) break;

            if (adjacent < 0 || adjacent >= n || adjacent == i) {
                std::cout << "Invalid vertex. Please enter a number between 0 and "
                         << n - 1 << " (excluding " << i << ") or -1 -1 to end: ";
                continue;
            }

            // Check if edge already exists
            bool edgeExists = false;
            for (const Edge& e : edges) {
                if (e.src == i && e.dest == adjacent) {
                    edgeExists = true;
                    break;
                }
            }

            if (!edgeExists) {
                AddDirectedWeightedEdge(i, adjacent, weight);
            }
        }
    }

    std::cout << "\nDirected weighted graph has been successfully read.\n";
}

void Graph::printPath(const std::vector<int>& parent, int vertex) {
    // Base case: if vertex is source (parent[vertex] == -1) or unreachable
    if (parent[vertex] == -1) {
        std::cout << vertex;
        return;
    }
    
    // Recursively print the path from source to parent vertex
    printPath(parent, parent[vertex]);
    // Then print the current vertex
    std::cout << " -> " << vertex;
}

void Graph::Dijkstra(int source) {
    if (source < 0 || source >= n) {
        std::cout << "Invalid source vertex\n";
        return;
    }

    // Create adjacency list with weights
    std::vector<std::vector<std::pair<int, int>>> adjListWeighted(n);
    for (const Edge& e : edges) {
        adjListWeighted[e.src].push_back(std::make_pair(e.dest, e.weight));
        // Note: No reverse edges added since this is a directed graph
    }

    // Distance vector and parent vector for path reconstruction
    std::vector<int> dist(n, std::numeric_limits<int>::max());
    std::vector<int> parent(n, -1);
    std::vector<bool> visited(n, false);

    // Initialize source
    dist[source] = 0;

    // Main loop - Find shortest path for all vertices
    for (int count = 0; count < n - 1; count++) {
        // Find minimum distance vertex from unvisited vertices
        int minDist = std::numeric_limits<int>::max();
        int u = -1;
        
        for (int v = 0; v < n; v++) {
            if (!visited[v] && dist[v] < minDist) {
                minDist = dist[v];
                u = v;
            }
        }

        if (u == -1) break; // No reachable unvisited nodes left
        
        visited[u] = true;

        // Update distances of adjacent vertices (only outgoing edges)
        for (const auto& adj : adjListWeighted[u]) {
            int v = adj.first;
            int weight = adj.second;
            
            if (!visited[v] && dist[u] != std::numeric_limits<int>::max() && 
                dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                parent[v] = u;
            }
        }
    }

    // Print results
    std::cout << "\nDijkstra's Shortest Paths from vertex " << source << " in directed graph:\n";
    for (int i = 0; i < n; i++) {
        if (i != source) {
            std::cout << "To vertex " << i << ":\n";
            if (dist[i] == std::numeric_limits<int>::max()) {
                std::cout << "No path exists\n";
            } else {
                std::cout << "Distance: " << dist[i] << "\nPath: ";
                printPath(parent, i);
                std::cout << "\n";
            }
        }
    }
}

void Graph::BellmanFord(int source) {
    if (source < 0 || source >= n) {
        std::cout << "Invalid source vertex\n";
        return;
    }

    // Initialize distances and parent array
    std::vector<int> dist(n, std::numeric_limits<int>::max());
    std::vector<int> parent(n, -1);
    dist[source] = 0;

    // Relax all edges |V| - 1 times
    for (int i = 1; i <= n - 1; i++) {
        for (const Edge& e : edges) {
            int u = e.src;
            int v = e.dest;
            int weight = e.weight;

            // Relax edge u->v (only in forward direction for directed graph)
            if (dist[u] != std::numeric_limits<int>::max() && 
                dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                parent[v] = u;
            }
        }
    }

    // Check for negative weight cycles
    bool hasNegativeCycle = false;
    for (const Edge& e : edges) {
        int u = e.src;
        int v = e.dest;
        int weight = e.weight;

        if (dist[u] != std::numeric_limits<int>::max() && 
            dist[u] + weight < dist[v]) {
            hasNegativeCycle = true;
            break;
        }
    }

    if (hasNegativeCycle) {
        std::cout << "Graph contains negative weight cycle\n";
        return;
    }

    // Print results
    std::cout << "\nBellman-Ford Shortest Paths from vertex " << source << " in directed graph:\n";
    for (int i = 0; i < n; i++) {
        if (i != source) {
            std::cout << "To vertex " << i << ":\n";
            if (dist[i] == std::numeric_limits<int>::max()) {
                std::cout << "No path exists\n";
            } else {
                std::cout << "Distance: " << dist[i] << "\nPath: ";
                printPath(parent, i);
                std::cout << "\n";
            }
        }
    }
}
