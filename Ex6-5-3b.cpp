/**
 * Solutions to Chapter 6, Section 5, Exercise 3(b) of Horowitz, Sahni, and
 * Mehta's Fundamentals of Data Structure in C++.
 * 
 * Task 3(b):
 * Write an O(n + e) C++ function using adjacency lists to determine whether
 * the connected graph G has a bridge (aka isthmus). A bridge is an edge in G
 * that lies on every path from the start vertex to the finish vertex.
 * If G has a bridge, your algorithm should print one such bridge.
 */
 
/*********************************main.cpp*************************************/
#include "Graph.h"

int main() {
    // Example usage
    
    Graph graph1(11), graph2(5), graph3(14), graph4(11);
    
    std::cout << "Graph as shown in p.385" << std::endl;
    graph1.AddEdge(0, 1);
    graph1.AddEdge(0, 2);
    graph1.AddEdge(0, 3);
    graph1.AddEdge(1, 4);
    graph1.AddEdge(2, 4);
    graph1.AddEdge(3, 5);
    graph1.AddEdge(4, 6);
    graph1.AddEdge(4, 7);    
    graph1.AddEdge(5, 7);
    graph1.AddEdge(6, 8);
    graph1.AddEdge(7, 8);    
    
    graph1.findAllBridges();
    std::cout << std::endl;
    
    std::cout << "A simple user-defined graph." << std::endl;
    graph2.AddEdge(1, 0);
    graph2.AddEdge(0, 2);
    graph2.AddEdge(2, 1);
    graph2.AddEdge(0, 3);
    graph2.AddEdge(3, 4);

    graph2.findAllBridges();
    std::cout << "An example of a bridge is: " << std::endl;
    graph2.FindBridge();
    std::cout << std::endl;
    
    std::cout << "Graph as shown in p.393" << std::endl;
    graph3.AddEdge(0, 1);
    graph3.AddEdge(0, 2);
    graph3.AddEdge(1, 3);
    graph3.AddEdge(2, 3);    
    graph3.AddEdge(2, 4);
    graph3.AddEdge(3, 4);
    graph3.AddEdge(3, 5);
    graph3.AddEdge(3, 6);    
    graph3.AddEdge(4, 6);
    graph3.AddEdge(4, 7);    
    graph3.AddEdge(5, 9);
    graph3.AddEdge(6, 8);
    graph3.AddEdge(7, 8);
    graph3.AddEdge(8, 9);
    
    graph3.findAllBridges();
    std::cout << std::endl;
    
    std::cout << "Graph as shown in p.351" << std::endl;
    graph4.AddEdge(0, 1);
    graph4.AddEdge(1, 2);
    graph4.AddEdge(2, 4);
    graph4.AddEdge(3, 4);
    graph4.AddEdge(1, 3);
    graph4.AddEdge(3, 5);
    graph4.AddEdge(5, 6);
    graph4.AddEdge(6, 7);
    graph4.AddEdge(5, 7);
    graph4.AddEdge(7, 8);
    graph4.AddEdge(7, 9);
    
    graph4.findAllBridges();
    std::cout << "An example of a bridge is: " << std::endl;
    graph4.FindBridge();
    std::cout << std::endl;
    
    std::cout << "End of program." << std::endl;
    return 0;
}



/**********************************Graph.h*************************************/
#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <vector>
#include <utility>

enum Boolean {FALSE, TRUE};

template <typename Type> class List;

struct Vertex {
    int id;
    Boolean mark;
    
    Vertex(int identifier = 0);
    bool operator==(const Vertex& other) const;
};

class GraphEdge {
    friend class Graph;
  private:
    Boolean m;
    Vertex vertex1, vertex2;
    GraphEdge *path1, *path2;
};

using EdgePtr = GraphEdge*;

class Graph {
  private:
    bool* visited;
    int n;
    EdgePtr* HeadNodes;

    void DFSAdjListUtil(int vertex);
    void BFSAdjListUtil(int startVertex);
    void ClearGraph();

    // Utility functions for detection of multiple bridges
    void bridgeUtil(int u, int parent, int* discovery,
                    int* low, bool* visited, int& discoveryTime,
                    List<std::pair<int, int>>& bridges);
                    
    // Bridge detection utility functions
    bool FindBridgeUtil(int u, std::vector<int>& disc, std::vector<int>& low, int parent);

  public:
    Graph();
    Graph(const int vertices);
    void AddEdge(int from, int to);
    bool FindBridge();
    void findAllBridges();
    ~Graph();
};

#endif // GRAPH_H



/*********************************Graph.cpp************************************/
#include "Graph.h"
#include "List.h"
#include <queue>
#include <limits>

Vertex::Vertex(int identifier) : id(identifier), mark(FALSE) {}

bool Vertex::operator==(const Vertex& other) const {
    return id == other.id;
}

Graph::Graph() : HeadNodes(nullptr), n(0), visited(nullptr) {}

Graph::Graph(const int vertices) : n(vertices) {
    // Initialize visited array to mark each vertex as unvisited
    visited = new bool[n];
    for (int i = 0; i < n; ++i) {
        visited[i] = FALSE;
    }

    // Initialize the HeadNodes array to store adjacency lists for each vertex
    HeadNodes = new EdgePtr[n];
    for (int i = 0; i < n; ++i) {
        HeadNodes[i] = nullptr;
    }
}

void Graph::ClearGraph() {
    if (HeadNodes) {
        for (int i = 0; i < n; i++) {
            EdgePtr current = HeadNodes[i];
            while (current) {
                EdgePtr temp = current;
                current = current->path1;
                delete temp;
            }
        }
        delete[] HeadNodes;
        HeadNodes = nullptr;
    }
}

void Graph::AddEdge(int from, int to) {
    EdgePtr newEdge = new GraphEdge;
    newEdge->vertex1.id = from;
    newEdge->vertex2.id = to;
    newEdge->m = FALSE;
    
    newEdge->path1 = HeadNodes[from];
    HeadNodes[from] = newEdge;
    
    EdgePtr reverseEdge = new GraphEdge;
    reverseEdge->vertex1.id = to;
    reverseEdge->vertex2.id = from;
    reverseEdge->m = FALSE;
    
    reverseEdge->path1 = HeadNodes[to];
    HeadNodes[to] = reverseEdge;
}

bool Graph::FindBridgeUtil(int u, std::vector<int>& disc, std::vector<int>& low, int parent) {
    static int time = 0;
    visited[u] = TRUE;
    disc[u] = low[u] = ++time;
    
    EdgePtr current = HeadNodes[u];
    while (current) {
        int v = (current->vertex1.id == u) ? current->vertex2.id : current->vertex1.id;

        if (!visited[v]) {
            if (FindBridgeUtil(v, disc, low, u)) {
                return true; // Bridge already found in recursion
            }
            
            low[u] = std::min(low[u], low[v]);
            
            if (low[v] > disc[u]) {
                std::cout << u << " -- " << v << std::endl;
                return true; // Return immediately upon finding a bridge
            }
        } else if (v != parent) {
            low[u] = std::min(low[u], disc[v]);
        }
        
        current = current->path1;
    }
    return false;
}

// newly added.
void Graph::bridgeUtil(int u, int parent, 
                       int* discovery, int* low,
                       bool* visited, int& discoveryTime,
                       List<std::pair<int, int>>& bridges) {
    visited[u] = true;
    discovery[u] = low[u] = ++discoveryTime;

    EdgePtr current = HeadNodes[u];
    while (current) {
        int v = (current->vertex1.id == u) ? current->vertex2.id : current->vertex1.id;
        
        if (!visited[v]) {  // If `v` is not visited
            bridgeUtil(v, u, discovery, low, visited, discoveryTime, bridges);
            
            low[u] = std::min(low[u], low[v]);
            
            // Check if the edge u-v is a bridge
            if (low[v] > discovery[u]) {
                bridges.InsertBack(std::make_pair(u, v));
            }
        } else if (v != parent) {
            low[u] = std::min(low[u], discovery[v]);
        }

        current = current->path1;
    }
}

// Solution to the assignment, which asks to print exactly one bridge out of many. 
bool Graph::FindBridge() {
    if (!HeadNodes) {
        std::cout << "Graph is empty. Please read the adjacency list first.\n";
        return false;
    }

    std::vector<int> disc(n, -1);
    std::vector<int> low(n, -1);
    visited = new bool[n]{FALSE};

    for (int i = 0; i < n; i++) {
        if (!visited[i]) {
            if (FindBridgeUtil(i, disc, low, -1)) {
                delete[] visited;
                return true; // Bridge found
            }
        }
    }

    std::cout << "No bridge found in the graph." << std::endl;
    delete[] visited;
    return false;
}

// newly added.
void Graph::findAllBridges() {
    List<std::pair<int, int>> bridges;
    
    // Ensure `discovery` and `low` arrays are allocated
    int* discovery = new int[n];
    int* low = new int[n];
    bool* visited = new bool[n];  // Track visited vertices to avoid unnecessary recursion

    // Initialize arrays
    for (int i = 0; i < n; i++) {
        discovery[i] = -1;
        low[i] = -1;
        visited[i] = false;
    }

    // Reset discovery time before starting DFS
    int discoveryTime = 0;

    // Call DFS for each unvisited vertex
    for (int i = 0; i < n; i++) {
        if (discovery[i] == -1) {
            bridgeUtil(i, -1, discovery, low, visited, discoveryTime, bridges);
        }
    }

    // Check if any bridges were found and print the appropriate message
    if (bridges.isEmpty()) {
        std::cout << "No bridges found in the graph.\n";
    } else {
        std::cout << "Bridges in the graph:\n";
        ListIterator<std::pair<int, int>> it(bridges);
        for (auto p = it.First(); p != nullptr; p = it.Next()) {
            std::cout << p->first << " -- " << p->second << std::endl;
        }
    }

    // Free allocated memory
    delete[] discovery;
    delete[] low;
    delete[] visited;
}

Graph::~Graph() {
    ClearGraph();
}



/*********************************List.h***********************************/
#ifndef LIST_H
#define LIST_H

#include <iostream>
#include <limits> // For std::numeric_limits

template <typename Type>
class List;

template <typename Type>
class ListIterator;

template <typename Type>
class ListNode {
    friend class List<Type>;
    friend class ListIterator<Type>;
    
    // Add this line to declare operator<< as a friend of ListNode
    template <typename T>
    friend std::ostream& operator<<(std::ostream& os, const List<T>& list);

private:
    Type data;
    ListNode *link;

public:
    ListNode(Type element = 0, ListNode *next = nullptr) : data(element), link(next) {}
};

template <typename Type>
class List {
    friend class ListIterator<Type>;
    
    template <typename T>
    friend std::ostream& operator<<(std::ostream& os, const List<T>& list);
public:
    List();
    ~List();

    ListNode<Type> *getFirst() const;
    ListNode<Type> *Insert(ListNode<Type> *x, Type i);
    Boolean isEmpty() const;
    int Length() const;
    void Delete(ListNode<Type> *x);
    void InsertBack(const Type &item);
    void Concatenate(List<Type> &b);
    void Reverse();
    Type findMin() const;

private:
    ListNode<Type> *first;
    ListNode<Type> *last;
};

// In List.h, modify the ListIterator class declaration:
template <typename Type>
class ListIterator {
public:
    ListIterator(List<Type>& l);  // Constructor takes non-const reference
    Type getData();

    Boolean NotNull() const;
    Boolean NextNotNull() const;
    Type* First();
    Type* Next();
    const ListNode<Type>* CurrentPosition() const;

private:
    List<Type>& list;  // Reference to the list
    ListNode<Type>* current;
};

// Declaration of the operator<< function
template <typename Type>
std::ostream& operator<<(std::ostream& os, const List<Type>& list);

#include "List.tpp"

#endif // LIST_H



/********************************List.tpp**********************************/
template <class Type>
List<Type>::List() : first(nullptr), last(nullptr) {}

template <class Type>
List<Type>::~List() {
    ListNode<Type> *current = first;
    while (current != nullptr) {
        ListNode<Type> *next = current->link;
        delete current;
        current = next;
    }
}

template <typename Type>
Boolean List<Type>::isEmpty() const {
    if (first == nullptr) return TRUE;  // If first is nullptr, the list is empty
    else return FALSE;
}

template <class Type>
ListNode<Type> *List<Type>::getFirst() const {
    return first;
}

template <class Type>
ListNode<Type> *List<Type>::Insert(ListNode<Type> *x, Type i) {
    if (first) {
        ListNode<Type> *temp = new ListNode<Type>(i, x->link);
        x->link = temp;
        if (x == last) {
            last = temp;
        }
        return temp;
    } else {
        first = last = new ListNode<Type>(i);
        return first;
    }
}

template <class Type>
int List<Type>::Length() const {
    int len = 0;
    ListNode<Type> *temp = first;
    while (temp != nullptr) {
        temp = temp->link;
        len++;
    }
    return len;
}

template <class Type>
void List<Type>::Delete(ListNode<Type> *x) {
    if (first == nullptr) {
        std::cout << "The list is empty" << std::endl;
        return;
    }
    if (x == first) {
        first = first->link;
        if (first == nullptr) {
            last = nullptr;
        }
    } else {
        ListNode<Type> *temp = first;
        while (temp->link != x) {
            temp = temp->link;
        }
        temp->link = x->link;
        if (x == last) {
            last = temp;
        }
    }
    delete x;
}

template <class Type>
void List<Type>::InsertBack(const Type &item) {
    if (first) {
        last->link = new ListNode<Type>(item);
        last = last->link;
    } else {
        first = last = new ListNode<Type>(item);
    }
}

template <class Type>
void List<Type>::Concatenate(List<Type> &b) {
    if (first) {
        last->link = b.first;
        if (b.last) {
            last = b.last;
        }
    } else {
        first = b.first;
        last = b.last;
    }
    b.first = b.last = nullptr;
}

template <class Type>
void List<Type>::Reverse() {
    ListNode<Type> *current = first, *previous = nullptr;
    last = first;
    while (current) {
        ListNode<Type> *next = current->link;
        current->link = previous;
        previous = current;
        current = next;
    }
    first = previous;
}

template <class Type>
Type List<Type>::findMin() const {
    if (first == nullptr) {
        throw std::runtime_error("The list is empty");
    }

    Type min = first->data;
    ListNode<Type> *temp = first->link;

    while (temp != nullptr) {
        if (temp->data < min) {
            min = temp->data;
        }
        temp = temp->link;
    }

    return min;
}

// Definition of the operator<< function
template <class Type>
std::ostream& operator<<(std::ostream& os, const List<Type>& list) {
    ListNode<Type>* current = list.first;
    while (current != nullptr) {
        os << current->data;
        if (current->link != nullptr) {
            os << "->";
        }
        current = current->link;
    }
    return os;
}

// ListIterator implementations
template <class Type>
ListIterator<Type>::ListIterator(List<Type> &l) : list(l), current(l.first) {}

template <class Type>
Type ListIterator<Type>::getData() {
    return current->data;
}

template <class Type>
Boolean ListIterator<Type>::NotNull() const {
    return current != nullptr ? TRUE : FALSE;
}

template <class Type>
Boolean ListIterator<Type>::NextNotNull() const {
    return current != nullptr && current->link != nullptr ? TRUE : FALSE;
}

template <class Type>
Type* ListIterator<Type>::First() {
    current = list.first;
    return current != nullptr ? &current->data : nullptr;
}

template <class Type>
Type* ListIterator<Type>::Next() {
    if (current != nullptr) {
        current = current->link;
        return current != nullptr ? &current->data : nullptr;
    }
    return nullptr;
}

template <class Type>
const ListNode<Type>* ListIterator<Type>::CurrentPosition() const {
    return current;
}
