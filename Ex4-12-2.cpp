/**
 * Solutions to Chapter 4, Section 12, Exercise 2 of Horowitz's
 * Fundamentals of Data Structures in C++.
 * 
 * Task: Using iterators, write C++ code for printing all elements in the
 * heterogeneous list of Program 4.45.
 */

/***********************************main.cpp***************************************/
#include "HeterogeneousList.h"
#include <iostream>

int main() {
    List myList;
    myList.InsertFirst(5);  // Insert an integer
    myList.InsertFirst(10); // Insert another integer
    myList.InsertFirst(15); // Insert yet another integer
    
    std::cout << "List contents: ";
    myList.Display();
    
    return 0;
}

/******************************HeterogeneousList.h*********************************/
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



/*****************************HeterogeneousList.cpp********************************/
#include "HeterogeneousList.h"

// List constructor
List::List() : first(nullptr) {}

// Insert a new node at the beginning of the list
void List::InsertFirst(int value) {
    Node* newNode = new DerivedNode<int>(value); // Create a new node
    newNode->link = first; // Set the link to the current first node
    first = newNode; // Update the first pointer to the new node
}

// Display the contents of the list
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



