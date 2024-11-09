/**
 * Solutions to Chapter 6, Section 5, Exercise 5 of Horowitz, Sahni, and Mehta's
 * Fundamentals of Data Structures in C++.
 * 
 * Task 5:
 * Define an iterator class TopoIterator for iterating through the vertices of a
 * directed acyclic graph in topological order.
 */
/*********************************main.cpp*************************************/
#include "Graph.h"
#include "List.h"
#include "DAG.h"
#include "TopoIterator.h"

int main() {
    DAG graph1(11); // Directed acyclic graph on p.385
    DAG graph2(14); // Directed acyclic graph on p.393
    
    // Add edges (activities) with durations
    graph1.addEdge(0, 1, 6); 
    graph1.addEdge(0, 2, 4);  
    graph1.addEdge(0, 3, 5);  
    graph1.addEdge(1, 4, 1);  
    graph1.addEdge(2, 4, 1);  
    graph1.addEdge(3, 5, 2);
    graph1.addEdge(4, 6, 9);
    graph1.addEdge(4, 7, 7);
    graph1.addEdge(5, 7, 4);
    graph1.addEdge(6, 8, 2);
    graph1.addEdge(7, 8, 4);
    
    graph2.addEdge(0, 1, 5);
    graph2.addEdge(0, 2, 6);
    graph2.addEdge(1, 3, 3);
    graph2.addEdge(2, 3, 6);
    graph2.addEdge(2, 4, 3);
    graph2.addEdge(3, 4, 3);
    graph2.addEdge(3, 5, 4);
    graph2.addEdge(3, 6, 4);
    graph2.addEdge(4, 6, 1);
    graph2.addEdge(4, 7, 4);
    graph2.addEdge(5, 9, 4);
    graph2.addEdge(6, 8, 5);
    graph2.addEdge(7, 8, 2);
    graph2.addEdge(8, 9, 2);
    
    std::cout << "Graph 1: ";
    // Iterate through vertices in topological order
    for (auto it = TopoIterator::Begin(graph1); it != TopoIterator::End(graph1); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    std::cout << "Graph 2: ";
    // Iterate through vertices in topological order
    for (auto it = TopoIterator::Begin(graph2); it != TopoIterator::End(graph2); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    std::cout << "End of program." << std::endl;
    
    return 0;
}



/**********************************Graph.h************************************/
#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <vector>

enum Boolean { FALSE, TRUE };

template <typename Type>
class List;

class Graph {
    friend class TopoIterator;
protected:
    struct Pair {
        int vertex;  // destination vertex
        int dur;     // activity duration
        Pair(int v = 0, int d = 0) : vertex(v), dur(d) {}
    };

    List<Pair>* HeadNodes;
    bool* visited;
    int* count;
    int n;

public:
    Graph(int vertices = 0);
    virtual ~Graph();
    void TopologicalOrder();
};

#endif // GRAPH_H



/*********************************Graph.cpp************************************/
#include "Graph.h"
#include "List.h"
#include <queue>
#include <limits>

Graph::Graph(const int vertices) : n(vertices) {
    HeadNodes = new List<Pair>[n];
    count = new int[n];
}

Graph::~Graph() {
    delete[] HeadNodes;
    delete[] count;
}

void Graph::TopologicalOrder() {
    int top = -1;
    
    for (int i = 0; i < n; i++) {
        if (count[i] == 0) {
            count[i] = top;
            top = i;
        }
    }

    for (int i = 0; i < n; i++) {
        if (top == -1) {
            std::cout << "Network has a cycle." << std::endl;
            return;
        } else {
            int j = top;
            top = count[top];
            std::cout << j << std::endl;
            
            // Create iterator with reference to the actual list
            ListIterator<Pair> li(HeadNodes[j]);
            if (!li.NotNull()) continue;
            
            Pair p = *li.First();
            while (TRUE) {
                int k = p.vertex;
                count[k]--;
                if (count[k] == 0) {
                    count[k] = top;
                    top = k;
                }
                if (li.NextNotNull()) {
                    p = *li.Next();
                } else break;
            }
        }
    }
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
    friend class DAG;
    // Add this line to declare operator<< as a friend of ListNode
    template <typename T>
    friend std::ostream& operator<<(std::ostream& os, const List<T>& list);

protected:
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



/*******************************TopoIterator.h*********************************/
class TopoIterator {
private:
    Graph& graph;
    int* count;     // Copy of in-degree counts
    int currentTop; // Current vertex at the top of zero-degree vertices
    bool isEnd;     // Flag to indicate end of iteration

    void InitializeCount() {
        count = new int[graph.n];
        // Initialize count array
        for (int i = 0; i < graph.n; i++) {
            count[i] = 0;
        }

        // Calculate in-degrees
        for (int i = 0; i < graph.n; i++) {
            ListIterator<Graph::Pair> li(graph.HeadNodes[i]);
            if (!li.NotNull()) continue;

            Graph::Pair p = *li.First();
            while (TRUE) {
                count[p.vertex]++;
                if (li.NextNotNull()) {
                    p = *li.Next();
                } else break;
            }
        }

        // Initialize currentTop with first vertex having zero in-degree
        currentTop = -1;
        for (int i = 0; i < graph.n; i++) {
            if (count[i] == 0) {
                count[i] = currentTop;
                currentTop = i;
                break;
            }
        }
        isEnd = (currentTop == -1);
    }

public:
    explicit TopoIterator(Graph& g) : graph(g), isEnd(false) {
        InitializeCount();
    }

    ~TopoIterator() {
        delete[] count;
    }

    // Iterator operations
    int operator*() const {
        return currentTop;
    }

    TopoIterator& operator++() {
        if (isEnd) return *this;

        int j = currentTop;
        currentTop = count[currentTop];

        // Process neighbors of current vertex
        ListIterator<Graph::Pair> li(graph.HeadNodes[j]);
        if (!li.NotNull()) {
            if (currentTop == -1) isEnd = true;
            return *this;
        }

        Graph::Pair p = *li.First();
        while (TRUE) {
            int k = p.vertex;
            count[k]--;
            if (count[k] == 0) {
                count[k] = currentTop;
                currentTop = k;
            }
            if (li.NextNotNull()) {
                p = *li.Next();
            } else break;
        }

        if (currentTop == -1) isEnd = true;
        return *this;
    }

    TopoIterator operator++(int) {
        TopoIterator temp = *this;
        ++(*this);
        return temp;
    }

    bool operator==(const TopoIterator& other) const {
        return (isEnd && other.isEnd) || 
               (!isEnd && !other.isEnd && currentTop == other.currentTop);
    }

    bool operator!=(const TopoIterator& other) const {
        return !(*this == other);
    }

    // Begin and end iterator functions
    static TopoIterator Begin(Graph& g) {
        return TopoIterator(g);
    }

    static TopoIterator End(Graph& g) {
        TopoIterator iter(g);
        iter.isEnd = true;
        return iter;
    }
};



/***********************************DAG.h**************************************/
#ifndef DAG_H
#define DAG_H

#include "Graph.h"
#include "List.h"

class DAG : public Graph {
public:
    // Constructor - creates an empty DAG with n vertices
    DAG(int n);

    // Destructor
    ~DAG() {}

    // Add an edge (activity) from vertex i to vertex j with duration d
    void addEdge(int i, int j, int d);

    // Function to check if adding an edge would create a cycle
    bool wouldCreateCycle(int from, int to);

    // Safe version of addEdge that checks for cycles
    bool safeAddEdge(int i, int j, int d);

    // Get the in-degree of a vertex
    int getInDegree(int vertex) const;

    // Get the number of vertices
    int getVertexCount() const;

    // Get the list of outgoing edges for a vertex
    const List<Pair>& getAdjList(int vertex) const;
};

#endif // DAG_H



/***********************************DAG.cpp*************************************/
#include "DAG.h"

DAG::DAG(int n) : Graph(n) {
    // Initialize count array for in-degrees
    count = new int[n];
    for (int i = 0; i < n; i++) {
        count[i] = 0;
    }
}

// Add an edge (activity) from vertex i to vertex j with duration d
void DAG::addEdge(int i, int j, int d) {
    if (i < 0 || i >= n || j < 0 || j >= n) {
        throw std::out_of_range("Vertex index out of range");
    }

    // Create a new Pair for the destination vertex and duration
    Pair newEdge(j, d);

    // Insert the edge at the end of the adjacency list for vertex i
    ListNode<Pair>* current = HeadNodes[i].getFirst();
    
    // If the list is empty, insert at the beginning
    if (!current) {
        HeadNodes[i].Insert(nullptr, newEdge);
    } else {
        // Find the last node in the list
        while (current->link) {
            current = current->link;
        }
        HeadNodes[i].Insert(current, newEdge);
    }

    // Increment the in-degree count for vertex j
    count[j]++;
}

// Function to check if adding an edge would create a cycle
bool DAG::wouldCreateCycle(int from, int to) {
    if (from == to) return true;

    // Temporary array for DFS
    bool* visited = new bool[n];
    for (int i = 0; i < n; i++) {
        visited[i] = false;
    }

    // Stack for DFS
    std::vector<int> stack;
    stack.push_back(to);
    visited[to] = true;

    while (!stack.empty()) {
        int current = stack.back();
        stack.pop_back();

        ListIterator<Pair> li(HeadNodes[current]);
        if (!li.NotNull()) continue;

        Pair p = *li.First();
        while (TRUE) {
            int next = p.vertex;
            if (next == from) {
                delete[] visited;
                return true;  // Would create a cycle
            }
            if (!visited[next]) {
                visited[next] = true;
                stack.push_back(next);
            }
            if (li.NextNotNull()) {
                p = *li.Next();
            } else break;
        }
    }

    delete[] visited;
    return false;
}

// Safe version of addEdge that checks for cycles
bool DAG::safeAddEdge(int i, int j, int d) {
    if (wouldCreateCycle(i, j)) {
        return false;  // Edge would create a cycle
    }
    addEdge(i, j, d);
    return true;
}

// Get the in-degree of a vertex
int DAG::getInDegree(int vertex) const {
    if (vertex < 0 || vertex >= n) {
        throw std::out_of_range("Vertex index out of range");
    }
    return count[vertex];
}

// Get the number of vertices
int DAG::getVertexCount() const {
    return n;
}

// Get the list of outgoing edges for a vertex
const List<Graph::Pair>& DAG::getAdjList(int vertex) const {
    if (vertex < 0 || vertex >= n) {
        throw std::out_of_range("Vertex index out of range");
    }
    return HeadNodes[vertex];
}
