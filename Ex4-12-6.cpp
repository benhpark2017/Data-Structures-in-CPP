/**
 * Solutions to Chapter 4, Section 12, first part of Exercise 6 of Horowitz's
 * Fundamentals of Data Structures in C++.
 * 
 * Task 6a: 
 * Write an algorithm to compute the memory requirements of the homogeneous list. 
 * Use the sizeof() function to determine the memory requirements of a node in
 * your machine.
 */
 
/*********************************main.cpp*************************************/
#include "HomogeneousList.h"
#include <iostream>

int main() {
    List myList;
    myList.InsertFirst(5);
    myList.InsertFirst(10);
    myList.InsertFirst(15);
    
    std::cout << "List contents: ";
    myList.Display();
    
    // Size of Data structure
    // int id: 4 bytes
    // union: 4 bytes, because it needs to accommodate for the largest data type, float
    // Total for data: 8 bytes
    // Size of CombinedNode structure
    // data of type Data: 8 bytes (as calculated previously)
    // CombinedNode* link: 8 bytes 
    // Subtotal for Data and CombinedNode: 16 bytes/node
    // Total for three nodes: 48 bytes
    std::cout << "Memory requirements: " << myList.MemoryRequirements() << " bytes" << std::endl;
    
    return 0;
}



/****************************HomogeneousList.h********************************/
#ifndef HOMOGENEOUS_LIST_H
#define HOMOGENEOUS_LIST_H

#include <cstddef>

enum Boolean {FALSE, TRUE};

struct Data {
    int id; //id = 0, 1, or 2 if the node contains a char, int, or float respectively.
    union {
        int i;
        char c;
        float f;
    };
};

class CombinedNode {
    friend class List;
    friend class ListIterator;
  private:
    Data data;
    CombinedNode* link;
};

class List {
    friend class ListIterator;
  public:
    List() : first(nullptr) {}
    void InsertFirst(int value);
    void Display() const;
    size_t MemoryRequirements() const;
  private:
    CombinedNode* first;
};

class ListIterator {
  public:
    ListIterator(const List &l) : list(l), current(l.first) {};
    Boolean NotNull();
    Boolean NextNotNull();
    Data* First();
    Data* Next();
  private:
    const List &list; //refers to an existing List
    CombinedNode* current; //points to a node in list
};

#endif //HOMOGENEOUS_LIST_H



/***************************HomogeneousList.cpp*******************************/
#include "HomogeneousList.h"
#include <iostream>

void List::InsertFirst(int value) {
    CombinedNode* newNode = new CombinedNode;
    newNode->data.id = 1; // 1 for int
    newNode->data.i = value;
    newNode->link = first;
    first = newNode;
}

void List::Display() const {
    CombinedNode* current = first;
    while (current) {
        if (current->data.id == 1) { // int
            std::cout << current->data.i << " ";
        } else if (current->data.id == 0) { // char
            std::cout << current->data.c << " ";
        } else if (current->data.id == 2) { // float
            std::cout << current->data.f << " ";
        }
        current = current->link;
    }
    std::cout << std::endl;
}

// MemoryRequirements implementation
size_t List::MemoryRequirements() const {
    size_t totalMemory = 0;
    CombinedNode* current = first;
    while (current) {
        totalMemory += sizeof(CombinedNode); // Add size of each node
        current = current->link;
    }
    return totalMemory;
}

Boolean ListIterator::NotNull() {
    if (current) return TRUE;
    else return FALSE;
}

Boolean ListIterator::NextNotNull() {
    if (current && current->link) return TRUE;
    else return FALSE;
}

Data* ListIterator::First() {
    if (list.first) return &list.first->data;
    else return 0;
}

Data* ListIterator::Next() {
    if (current) {
        current = current->link;
        return &current->data;
    }
    else return 0;
}




/**
 * Solutions to Chapter 4, Section 12, second part of Exercise 6 of Horowitz's
 * Fundamentals of Data Structures in C++.
 * 
 * Task 6b: 
 * Write an algorithm to compute the memory requirements of the heterogeneous
 * list. Use the sizeof() function to determine the memory requirements of a 
 * node in your machine.
 */
 
/*********************************main.cpp*************************************/
#include "HeterogeneousList.h"
#include <iostream>

int main() {
    List myList;
    myList.InsertFirst(5);  // Insert an integer
    myList.InsertFirst(10); // Insert another integer
    myList.InsertFirst(15); // Insert yet another integer
    
    std::cout << "List contents: ";
    myList.Display();
    
    // Size of Data structure
    // int id: 4 bytes
    // union: 4 bytes, because it needs to accommodate for the largest data type, float
    // Total for data: 8 bytes
    // Size of Node base class: 8 bytes
    // Size of DerivedNode<int> derived class: 8 bytes
    // Subtotal for Data, Node, and DerivedNode<int>: 24 bytes/node
    // Total for three nodes: 72 bytes
    std::cout << "Memory requirements: " << myList.MemoryRequirements() << " bytes" << std::endl;
    
    return 0;
}


/*****************************HeterogeneousList.h********************************/
#ifndef HETEROGENEOUS_LIST_H
#define HETEROGENEOUS_LIST_H

#include <iostream>

enum Boolean { FALSE, TRUE };

struct Data {
    int id; // 0 for char, 1 for int, 2 for float
    union {
        int i;
        char c;
        float f;
    };
};

// Abstract base class for a node in the list
class Node {
    friend class List;
    friend class ListIterator;
  protected:
    Node* link; // Pointer to the next node
    virtual Data GetData() = 0;
    virtual void Display() = 0;
};

// Template class for a node containing specific data type
template<typename Type>
class DerivedNode : public Node {
    friend class List;
    friend class ListIterator;
  public:
    DerivedNode(Type item); // Constructor
  private:
    Type data;
    Data GetData() override; // Function to get node's data
    void Display() override; // Function to display node's data
};

// List class for managing the nodes
class List {
    friend class ListIterator;
  public:
    List(); // Constructor
    void InsertFirst(int value); // Function to insert a node at the beginning
    void Display() const; // Function to display the list
    size_t MemoryRequirements() const;
  private:
    Node* first; // Pointer to the first node in the list
};

// Iterator class for traversing the list
class ListIterator {
  public: 
    ListIterator(const List &l); // Constructor
    Data* First(); // Get the data from the first node
    Data* Next(); // Get the data from the next node
    Boolean NotNull(); // Check if current node is not null
    Boolean NextNotNull(); // Check if next node is not null
  private:
    const List &list;
    Node* current;
    Data temp;
};

#endif // HETEROGENEOUS_LIST_H


/****************************HeterogeneousList.cpp*******************************/
#include "HeterogeneousList.h"

// List constructor
List::List() : first(nullptr) {}

// Insert a new node at the beginning of the list
void List::InsertFirst(int value) {
    Node* newNode = new DerivedNode<int>(value); // Create a new node
    newNode->link = first; // Set the link to the current first node
    first = newNode; // Update the first pointer to the new node
}

// Display the contents of the list without iterators
/**
void List::Display() const {
    Node* current = first;
    while (current) {
        current->Display(); // Call the Display function of each node
        current = current->link; // Move to the next node
    }
    std::cout << std::endl;
}
*/

void List::Display() const {
    ListIterator it(*this); // Create an iterator for this list
    for (Data* data = it.First(); it.NotNull(); data = it.Next()) {
        switch (data->id) {
            case 0: // char
                std::cout << data->c << " ";
                break;
            case 1: // int
                std::cout << data->i << " ";
                break;
            case 2: // float
                std::cout << data->f << " ";
                break;
        }
    }
    std::cout << std::endl;
}

// Function that calculates memory requirement.
size_t List::MemoryRequirements() const {
    size_t totalMemory = 0;
    Node* current = first;
    while (current) {
        totalMemory += sizeof(DerivedNode<int>); // Assuming all nodes are int
        current = current->link;
    }
    return totalMemory;
}

// ListIterator constructor
ListIterator::ListIterator(const List &l) : list(l), current(l.first) {}

// Check if current node is not null
Boolean ListIterator::NotNull() {
    return current ? TRUE : FALSE;
}

// Check if the next node is not null
Boolean ListIterator::NextNotNull() {
    return (current && current->link) ? TRUE : FALSE;
}

// Get the data from the first node
Data* ListIterator::First() {
    if (list.first) {
        temp = list.first->GetData(); // Retrieve the data from the first node
        return &temp;
    }
    return nullptr;
}

// Get the data from the next node
Data* ListIterator::Next() {
    if (current) {
        current = current->link; // Move to the next node
        if (current) {
            temp = current->GetData(); // Retrieve the data
            return &temp;
        }
    }
    return nullptr;
}

// Constructor for DerivedNode
template<typename Type>
DerivedNode<Type>::DerivedNode(Type item) : data(item) {
    link = nullptr; // Initialize link to nullptr
}

// Display function for DerivedNode
template<typename Type>
void DerivedNode<Type>::Display() {
    std::cout << data << " "; // Print the data
}

// Specialization for char type
template<>
Data DerivedNode<char>::GetData() {
    Data t;
    t.id = 0;
    t.c = data; // Assign the char data
    return t;
}

// Specialization for int type
template<>
Data DerivedNode<int>::GetData() {
    Data t;
    t.id = 1;
    t.i = data; // Assign the int data
    return t;
}

// Specialization for float type
template<>
Data DerivedNode<float>::GetData() {
    Data t;
    t.id = 2;
    t.f = data; // Assign the float data
    return t;
}
