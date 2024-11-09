/**
 * Solutions to Chapter 6, Section 5, Exercise 4 of Horowitz, Sahni, and Mehta's
 * Fundamentals of Data Structure in C++. 
 * 
 * Task 4:
 * Write a C++ program that inputs an AOE network and outputs the following:
 * (a) A table of all events together with their earliest and latest times.
 * (b) A table of all activities together with their earliest and latest
 *     times. This table should also list the slack for each activity and
 *     identify all critical activities. 
 * (c) The critical network.
 * (d) Whether or not the project length can be reduced by speeding a single
 *     activity. If so, then by how much?
 */
 
/*********************************main.cpp*************************************/
#include "AOENetwork.h"
#include <iostream>

int main() {

    std::cout << "Example as shown in p.385 of the textbook." << std::endl;
    AOENetwork network1(9);

    network1.addEdge(0, 1, 6);
    network1.addEdge(0, 2, 4);
    network1.addEdge(0, 3, 5);    
    network1.addEdge(1, 4, 1);
    network1.addEdge(2, 4, 1);
    network1.addEdge(3, 5, 2);   
    network1.addEdge(4, 6, 9);
    network1.addEdge(4, 7, 7);
    network1.addEdge(5, 7, 4);   
    network1.addEdge(6, 8, 2);
    network1.addEdge(7, 8, 4);
    
    network1.analyzeNetwork();
    network1.printActivityTable();
    network1.printCriticalPaths();
    network1.printAllReductionPotentials();
    
//======================================================================//

    std::cout << "Example as shown in p.393 of the textbook." << std::endl;
    AOENetwork network2(10);

    network2.addEdge(0, 1, 5);
    network2.addEdge(0, 2, 6);
    network2.addEdge(1, 3, 3);
    network2.addEdge(2, 3, 6);
    network2.addEdge(2, 4, 3);
    network2.addEdge(3, 4, 3);
    network2.addEdge(3, 5, 4);
    network2.addEdge(3, 6, 4);
    network2.addEdge(4, 6, 1);
    network2.addEdge(4, 7, 4);
    network2.addEdge(5, 9, 4);
    network2.addEdge(6, 8, 5);
    network2.addEdge(7, 8, 2);
    network2.addEdge(8, 9, 2);

    network2.analyzeNetwork();
    network2.printActivityTable();
    network2.printCriticalPaths();
    network2.printAllReductionPotentials();
    
//===============================================================//
    
    int n, m;
    std::cout << "Enter number of events (vertices): ";
    std::cin >> n;
    
    AOENetwork network3(n);
    
    std::cout << "Enter number of activities (edges): ";
    std::cin >> m;
    
    std::cout << "Enter activities in format: from_event to_event duration\n";
    
    int validEdgesCount = 0;
    while (validEdgesCount < m) {
        int from, to, duration;
        std::cin >> from >> to >> duration;
        
        if (from >= 0 && from < n && to >= 0 && to < n) {
            network3.addEdge(from, to, duration);
            validEdgesCount++;
        } else {
            std::cout << "Invalid event numbers. Try again.\n";
        }
    }
    
    // Now perform the analysis
    std::cout << "\nNetwork Analysis:\n";
    network3.analyzeNetwork();
    network3.printActivityTable();
    network3.printCriticalPaths();
    network3.printAllReductionPotentials();
    
    return 0;
}



/**********************************Graph.h*************************************/
#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <vector>

enum Boolean { FALSE, TRUE };

template <typename Type>
class List;

class Graph {
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



/***********************************List.h*************************************/
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



/**********************************List.tpp************************************/
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



/********************************AOENetwork.h**********************************/
#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <algorithm>
#include "Graph.h"  // Assuming your code is in Graph.h and Graph.cpp
#include "List.h"

class AOENetwork : public Graph {
private:
    std::vector<int> earliestTimes;
    std::vector<int> latestTimes;
    
    void dfsCriticalPaths(int node, std::vector<int>& path);
 
public:
    AOENetwork(int vertices);

    // Function to add an edge to the network
    void addEdge(int u, int v, int duration);

    void calculateEarliestTimes();

    void calculateLatestTimes(int projectDuration);

    void printActivityTable();

    void printEventTimesTable();

    // Main function to initiate DFS from the start node and print all critical paths
    void printCriticalPaths();

    void analyzeNetwork();
    
    void printAllReductionPotentials();
};



/*******************************AOENetwork.cpp*********************************/
#include "AOENetwork.h" 

void AOENetwork::dfsCriticalPaths(int node, std::vector<int>& path) {
    path.push_back(node);

    // Check if we've reached the last node in the critical path
    if (node == n - 1) {
        // Print the current critical path
        for (int i = 0; i < path.size(); ++i) {
            std::cout << path[i];
            if (i != path.size() - 1) std::cout << " -> ";
        }
        std::cout << "\n";
    } else {
        // Continue DFS along critical activities
        ListIterator<Pair> li(HeadNodes[node]);
        while (li.NotNull()) {
            Pair activity = li.getData();
            int nextNode = activity.vertex;
            int duration = activity.dur;

            // Check if (node -> nextNode) is a critical activity
            if (earliestTimes[node] == latestTimes[node] &&
                earliestTimes[nextNode] == latestTimes[nextNode] &&
                earliestTimes[nextNode] == earliestTimes[node] + duration) {
                dfsCriticalPaths(nextNode, path);
            }

            li.Next();
        }
    }

    path.pop_back(); // Backtrack
}
 

AOENetwork::AOENetwork(int vertices) : Graph(vertices) {
    earliestTimes.resize(vertices, 0);
    latestTimes.resize(vertices, std::numeric_limits<int>::max());
}

// Function to add an edge to the network
void AOENetwork::addEdge(int u, int v, int duration) {
    HeadNodes[u].InsertBack(Pair(v, duration));
}

void AOENetwork::calculateEarliestTimes() {
    std::queue<int> q;
    std::vector<int> inDegree(n, 0);

    // Calculate in-degrees
    for (int i = 0; i < n; i++) {
        ListIterator<Pair> li(HeadNodes[i]);
        while (li.NotNull()) {
            int v = li.getData().vertex;
            inDegree[v]++;
            li.Next();
        }
    }

    // Initialize queue with zero in-degree nodes
    for (int i = 0; i < n; i++) {
        if (inDegree[i] == 0) q.push(i);
    }

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        ListIterator<Pair> li(HeadNodes[u]);
        while (li.NotNull()) {
            Pair activity = li.getData();
            int v = activity.vertex;
            int duration = activity.dur;
            
            // Relax edge (u, v)
            if (earliestTimes[v] < earliestTimes[u] + duration) {
                earliestTimes[v] = earliestTimes[u] + duration;
            }

            // Decrease in-degree and add to queue if it becomes zero
            if (--inDegree[v] == 0) {
                q.push(v);
            }

            li.Next();
        }
    }
}

void AOENetwork::calculateLatestTimes(int projectDuration) {
    // Initialize all latest times to the project duration
    latestTimes.assign(n, projectDuration);

    // Get a reverse topological order by reusing TopologicalOrder() result
    std::vector<int> topologicalOrder;
    std::queue<int> q;
    std::vector<int> inDegree(n, 0);

    // Calculate in-degrees again for use in reverse topological ordering
    for (int i = 0; i < n; i++) {
        ListIterator<Pair> li(HeadNodes[i]);
        while (li.NotNull()) {
            int v = li.getData().vertex;
            inDegree[v]++;
            li.Next();
        }
    }

    // Start by pushing nodes with zero in-degrees
    for (int i = 0; i < n; i++) {
        if (inDegree[i] == 0) q.push(i);
    }

    while (!q.empty()) {
        int u = q.front();
        q.pop();
        topologicalOrder.push_back(u);

        ListIterator<Pair> li(HeadNodes[u]);
        while (li.NotNull()) {
            int v = li.getData().vertex;
            if (--inDegree[v] == 0) {
                q.push(v);
            }
            li.Next();
        }
    }

    // Now go in reverse topological order to calculate latest times
    for (auto it = topologicalOrder.rbegin(); it != topologicalOrder.rend(); ++it) {
        int u = *it;

        ListIterator<Pair> li(HeadNodes[u]);
        while (li.NotNull()) {
            Pair activity = li.getData();
            int v = activity.vertex;
            int duration = activity.dur;

            // Relax edge (u -> v) for latest times
            if (latestTimes[u] > latestTimes[v] - duration) {
                latestTimes[u] = latestTimes[v] - duration;
            }

            li.Next();
        }
    }
}

void AOENetwork::printActivityTable() {
    std::cout << "Table for early and late activity times" << std::endl;
    std::cout << "Activity\tEarly Time\tLate Time\tSlack\t\tCritical\n";
    
    // Iterate over each vertex to find all activities (edges) and their times
    for (int u = 0; u < n; ++u) {
        ListIterator<Pair> li(HeadNodes[u]);
        while (li.NotNull()) {
            Pair activity = li.getData();
            int v = activity.vertex;
            int duration = activity.dur;

            // Calculate early time, late time, and slack for each activity (u -> v)
            int earlyTime = earliestTimes[u];
            int lateTime = latestTimes[v] - duration;
            int slack = lateTime - earlyTime;
            bool isCritical = (slack == 0);

            // Print activity information
            std::cout << "(" << u << " -> " << v << ")\t" 
                      << earlyTime << "\t\t"
                      << lateTime << "\t\t"
                      << slack << "\t\t"
                      << (isCritical ? "Yes" : "No") << "\n";
            
            li.Next();
        }
    }
    std::cout << std::endl;
}

void AOENetwork::printEventTimesTable() {
    std::cout << "Event\tEarliest Time\tLatest Time\n";
    for (int i = 0; i < n; i++) {
        std::cout << i << "\t" << earliestTimes[i] << "\t\t" << latestTimes[i] << "\n";
    }
}

// Main function to initiate DFS from the start node and print all critical paths
void AOENetwork::printCriticalPaths() {
    std::vector<int> path;
    std::cout << "Critical Paths:\n";
    dfsCriticalPaths(0, path);  // Start DFS from the first node (assumed to be node 0)
    std::cout << std::endl;
}

void AOENetwork::analyzeNetwork() {
    std::cout << "Computations of earliest and latest times for event" << std::endl;
    calculateEarliestTimes();
    int projectDuration = *std::max_element(earliestTimes.begin(), earliestTimes.end());
    calculateLatestTimes(projectDuration);
    printEventTimesTable();
    std::cout << std::endl;
}
    
void AOENetwork::printAllReductionPotentials() {
    std::cout << "Accelerating project lengths" << std::endl;

    bool reductionPossible = false;

    std::cout << "Activity\tPotential Project Length Reduction\n";

    // Iterate over each activity (u -> v) to find possible reductions
    for (int u = 0; u < n; ++u) {
        ListIterator<Pair> li(HeadNodes[u]);
        while (li.NotNull()) {
            Pair activity = li.getData();
            int v = activity.vertex;
            int duration = activity.dur;

            // Calculate slack for the activity (u -> v)
            int earlyTime = earliestTimes[u];
            int lateTime = latestTimes[v] - duration;
            int slack = lateTime - earlyTime;

            // If slack > 0, this activity is non-critical and has potential for reduction
            if (slack > 0) {
                reductionPossible = true;
                std::cout << "(" << u << " -> " << v << ")\t\t" << slack << " units\n";
            }

            li.Next();
        }
    }

    // If no activity has potential reduction, output a message
    if (!reductionPossible) {
        std::cout << "The project length cannot be reduced by speeding any single activity.\n";
    }
    std::cout << std::endl;
}
