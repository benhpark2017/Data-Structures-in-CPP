/**
 * Solutions to Chapter 6, Section 4, Exercise 13 of Horowitz, Sahni, and Mehta's
 * Fundamentals of Data Structures in C++.
 * 
 * Task 13:
 * Compare the run-time performance of the Bellman-Ford functions of Exercise
 * 10, 11, and 12. For this, generate test graphs that will expose the relative
 * performance of the three functions.
 */
 
/**********************************main.cpp************************************/

#include "BellmanFord.h"
#include "Benchmark.h"

int main() {
    GraphBenchmark benchmark;
    
    // Define test cases
    std::vector<int> verticesSizes = {10, 20, 50, 100};
    std::vector<double> densities = {0.1, 0.3, 0.7};
    
    std::cout << "Starting benchmark...\n";
    benchmark.runBenchmark(verticesSizes, densities);
    benchmark.printResults();
    
    return 0;
}


/********************************Benchmark.h**********************************/
#ifndef BENCHMARK_H
#define BENCHMARK_H

#include <random>
#include <chrono>

// Forward declarations of the three Graph classes
class AdjListGraph;    // From Ex6-4-10
class MatrixGraph;     // From Ex6-4-11
class QueueGraph;      // From Ex6-4-12

class BenchmarkResult {
public:
    double adjListTime;
    double matrixTime;
    double queueTime;
    int vertices;
    double density;
    
    BenchmarkResult(int v, double d) : vertices(v), density(d),
        adjListTime(0), matrixTime(0), queueTime(0) {}
};

class GraphBenchmark {
private:
    std::mt19937 rng;
    std::vector<BenchmarkResult> results;
    
    void generateTestGraph(AdjListGraph& g1,
                           MatrixGraph& g2, 
                           QueueGraph& g3,
                           int vertices,
                           double density) {
        // Number of edges based on density
        int maxEdges = vertices * (vertices - 1);
        int numEdges = static_cast<int>(density * maxEdges);
        
        // Generate random edges
        std::uniform_int_distribution<int> vertexDist(0, vertices - 1);
        std::uniform_int_distribution<int> weightDist(-100, 100);
        
        // Ensure connectivity first (minimum spanning tree)
        for (int i = 1; i < vertices; i++) {
            int parent = vertexDist(rng) % i;  // Connect to any previous vertex
            int weight = weightDist(rng);
            g1.addEdge(parent, i, weight);
            g2.addEdge(parent, i, weight);
            g3.addEdge(parent, i, weight);
        }
        
        // Add remaining random edges
        for (int i = 0; i < numEdges - (vertices - 1); i++) {
            int source, dest;
            do {
                source = vertexDist(rng);
                dest = vertexDist(rng);
            } while (source == dest);
            
            int weight = weightDist(rng);
            g1.addEdge(source, dest, weight);
            g2.addEdge(source, dest, weight);
            g3.addEdge(source, dest, weight);
        }
    }
    
    template<typename GraphType>
    double timeOperation(GraphType& graph, int vertices, int sourceVertex) {
        auto start = std::chrono::high_resolution_clock::now();
        graph.BellmanFord(vertices, sourceVertex);
        auto end = std::chrono::high_resolution_clock::now();
        
        std::chrono::duration<double, std::micro> duration = end - start;
        return duration.count();
    }

public:
    GraphBenchmark() : rng(std::chrono::steady_clock::now().time_since_epoch().count()) {}
    
    void runBenchmark(const std::vector<int>& verticesSizes,
                     const std::vector<double>& densities,
                     int numTrials = 3) {
        for (int vertices : verticesSizes) {
            for (double density : densities) {
                BenchmarkResult result(vertices, density);
                
                // Run multiple trials and average the results
                for (int trial = 0; trial < numTrials; trial++) {
                    // Create graphs of each type
                    AdjListGraph g1(vertices);
                    MatrixGraph g2(vertices);
                    QueueGraph g3(vertices);
                    
                    // Generate the same test graph for all implementations
                    generateTestGraph(g1, g2, g3, vertices, density);
                    
                    // Time each implementation
                    result.adjListTime += timeOperation(g1, vertices, 0);
                    result.matrixTime += timeOperation(g2, vertices, 0);
                    result.queueTime += timeOperation(g3, vertices, 0);
                }
                
                // Average the results
                result.adjListTime /= numTrials;
                result.matrixTime /= numTrials;
                result.queueTime /= numTrials;
                
                results.push_back(result);
            }
        }
    }
    
    void printResults() const {
        std::cout << std::fixed << std::setprecision(2);
        std::cout << "\nBenchmark Results (times in microseconds):\n";
        std::cout << std::setw(10) << "Vertices" 
                  << std::setw(10) << "Density"
                  << std::setw(15) << "Adj List"
                  << std::setw(15) << "Matrix"
                  << std::setw(15) << "Queue" << std::endl;
        std::cout << std::string(65, '-') << std::endl;
        
        for (const auto& result : results) {
            std::cout << std::setw(10) << result.vertices
                      << std::setw(10) << result.density
                      << std::setw(15) << result.adjListTime
                      << std::setw(15) << result.matrixTime
                      << std::setw(15) << result.queueTime << std::endl;
        }
    }
};

#endif


/*******************************BellmanFord.h*********************************/
#ifndef BELLMAN_FORD_H
#define BELLMAN_FORD_H

#include <iostream>
#include <queue>
#include <vector>
#include <iomanip>
#include <limits>

// Common constants
const int nMax = 100;  // Maximum number of vertices
enum Boolean {FALSE, TRUE};

//=============================================================================
// Implementation 1: Adjacency List Version (from Ex6-4-10.cpp)
//=============================================================================
class AdjListGraph {
private:
    struct Node {
        int vertex;    // Destination vertex
        int length;    // Edge weight
        Node* link;    // Pointer to next node in adjacency list
        Node(int v, int len = 0, Node* nxt = nullptr) 
            : vertex(v), length(len), link(nxt) {}
    };
    
    int n;                 // Number of vertices
    Node** adjList;        // Array of pointers to adjacency lists
    int* dist;            // Distance array for shortest paths
    
public:
    AdjListGraph(int vertices) : n(vertices) {
        adjList = new Node*[n];
        for (int i = 0; i < n; i++) {
            adjList[i] = nullptr;
        }
        dist = new int[n];
    }
    
    ~AdjListGraph() {
        for (int i = 0; i < n; i++) {
            Node* current = adjList[i];
            while (current != nullptr) {
                Node* temp = current;
                current = current->link;
                delete temp;
            }
        }
        delete[] adjList;
        delete[] dist;
    }
    
    void addEdge(int u, int v, int length) {
        adjList[u] = new Node(v, length, adjList[u]);
    }
    
    void BellmanFord(const int n, const int v) {
        // Initialize distances
        for (int i = 0; i < n; i++) {
            dist[i] = std::numeric_limits<int>::max();
        }
        dist[v] = 0;

        // Initialize distances from source to neighbors
        Node* current = adjList[v];
        while (current != nullptr) {
            dist[current->vertex] = current->length;
            current = current->link;
        }

        // Relax edges n-1 times
        for (int k = 2; k <= n - 1; k++) {
            for (int u = 0; u < n; u++) {
                if (u == v) continue;
                
                for (int i = 0; i < n; i++) {
                    Node* current = adjList[i];
                    while (current != nullptr) {
                        if (current->vertex == u && 
                            dist[i] != std::numeric_limits<int>::max() && 
                            dist[u] > dist[i] + current->length) {
                            dist[u] = dist[i] + current->length;
                        }
                        current = current->link;
                    }
                }
            }
        }
    }
    
    void printDistances() const {
        for (int i = 0; i < n; i++) {
            if (dist[i] == std::numeric_limits<int>::max())
                std::cout << "Distance to " << i << ": INF\n";
            else
                std::cout << "Distance to " << i << ": " << dist[i] << "\n";
        }
        std::cout << std::endl;
    }
};

//=============================================================================
// Implementation 2: Matrix Version (from Ex6-4-11.cpp)
//=============================================================================
class MatrixGraph {
private:
    std::vector<std::vector<int>> length; // Dynamic length-adjacency matrix
    std::vector<int> dist; // Dynamic distance array
    std::vector<Boolean> s; // Dynamic processed vertices set
    int n; // Current number of vertices

public:
    MatrixGraph(int vertices) : n(vertices) {
        length.resize(n, std::vector<int>(n));
        dist.resize(n);
        s.resize(n);
        
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                length[i][j] = (i == j) ? 0 : std::numeric_limits<int>::max();
            }
        }
    }

    void addEdge(int u, int v, int weight) {
        if (u >= 0 && u < n && v >= 0 && v < n) {
            length[u][v] = weight;
        }
    }

    void BellmanFord(const int n, const int v) {
        // Initialize distances
        for (int i = 0; i < n; i++) {
            dist[i] = length[v][i];
        }

        // Relax edges
        bool changed;
        for (int k = 2; k <= n - 1; k++) {
            changed = false;

            for (int u = 0; u < n; u++) {
                if (u != v) {
                    bool hasIncomingEdge = false;

                    for (int i = 0; i < n; i++) {
                        if (length[i][u] < std::numeric_limits<int>::max()) {
                            hasIncomingEdge = true;
                            break;
                        }
                    }

                    if (hasIncomingEdge) {
                        for (int i = 0; i < n; i++) {
                            if (length[i][u] < std::numeric_limits<int>::max() &&
                                dist[i] != std::numeric_limits<int>::max() &&
                                dist[u] > dist[i] + length[i][u]) {
                                dist[u] = dist[i] + length[i][u];
                                changed = true;
                            }
                        }
                    }
                }
            }

            if (!changed) break;
        }
    }

    void printDistances() const {
        for (int i = 0; i < n; i++) {
            if (dist[i] == std::numeric_limits<int>::max())
                std::cout << "Distance to " << i << ": INF\n";
            else
                std::cout << "Distance to " << i << ": " << dist[i] << "\n";
        }
    }
};

//=============================================================================
// Implementation 3: Queue Version (from Ex6-4-12.cpp)
//=============================================================================
class QueueGraph {
private:
    std::vector<std::vector<int>> length; // Dynamic length-adjacency matrix
    std::vector<int> dist; // Dynamic distance array
    std::vector<Boolean> s; // Dynamic processed vertices set
    int n; // Current number of vertices

    bool isInQueue(const std::queue<int>& q,
                  const std::vector<bool>& inQueue,
                  int vertex) const {
        return inQueue[vertex];
    }

public:
    QueueGraph(int vertices) : n(vertices) {
        length.resize(n, std::vector<int>(n));
        dist.resize(n);
        s.resize(n);
        
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                length[i][j] = (i == j) ? 0 : std::numeric_limits<int>::max();
            }
        }
    }

    void addEdge(int u, int v, int weight) {
        if (u >= 0 && u < n && v >= 0 && v < n) {
            length[u][v] = weight;
        }
    }

    void BellmanFord(const int n, const int v) {
        // Initialize distances
        for (int i = 0; i < n; i++) {
            dist[i] = (i == v) ? 0 : std::numeric_limits<int>::max();
        }

        std::queue<int> vertexQueue;
        std::vector<bool> inQueue(n, false);

        // Add vertices adjacent to source
        for (int i = 0; i < n; i++) {
            if (i != v && length[v][i] < std::numeric_limits<int>::max()) {
                dist[i] = length[v][i];
                vertexQueue.push(i);
                inQueue[i] = true;
            }
        }

        // Process queue
        int iterations = 0;
        while (!vertexQueue.empty() && iterations <= n * (n-1)) {
            int i = vertexQueue.front();
            vertexQueue.pop();
            inQueue[i] = false;

            for (int u = 0; u < n; u++) {
                if (length[i][u] < std::numeric_limits<int>::max() &&
                    dist[i] != std::numeric_limits<int>::max() &&
                    dist[u] > dist[i] + length[i][u]) {

                    dist[u] = dist[i] + length[i][u];

                    if (!isInQueue(vertexQueue, inQueue, u)) {
                        vertexQueue.push(u);
                        inQueue[u] = true;
                    }
                }
            }
            iterations++;
        }
    }

    void printDistances() const {
        for (int i = 0; i < n; i++) {
            if (dist[i] == std::numeric_limits<int>::max())
                std::cout << "Distance to " << i << ": INF\n";
            else
                std::cout << "Distance to " << i << ": " << dist[i] << "\n";
        }
    }
};

#endif // BELLMAN_FORD_H
