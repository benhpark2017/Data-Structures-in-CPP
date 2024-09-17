


/***********************************main.cpp***************************************/
#include "HeterogeneousList.h"
#include <iostream>

int main() {
    List myList;
    myList.InsertFirst(5);       // Insert an integer
    myList.InsertFirst(3.14159f);   // Insert a float
    myList.InsertFirst('A');     // Insert a char
    myList.InsertFirst(2.71828f);   // Insert another float

    myList.Display();

    std::cout << "Sum of floats: " << myList.SumFloats() << std::endl; 
    // Should output the sum of 3.14 + 2.71

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
    
    void InsertFirst(char value);
    void InsertFirst(int value); // Function to insert a node at the beginning
    void InsertFirst(float value);
    
    float SumFloats() const;
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

// Insert a new character node at the beginning of the list
void List::InsertFirst(char value) {
    Node* newNode = new DerivedNode<char>(value); // Create a char node
    newNode->link = first;
    first = newNode;
}

// Insert a new integer node at the beginning of the list
void List::InsertFirst(int value) {
    Node* newNode = new DerivedNode<int>(value); // Create a new node
    newNode->link = first; // Set the link to the current first node
    first = newNode; // Update the first pointer to the new node
}

// Insert a new floating point number node at the beginning of the list
void List::InsertFirst(float value) {
    Node* newNode = new DerivedNode<float>(value); // Create a float node
    newNode->link = first;
    first = newNode;
}

float List::SumFloats() const {
    float sum = 0.0f;
    ListIterator it(*this); // Create an iterator for this list

    // Traverse the list
    for (Data* data = it.First(); it.NotNull(); data = it.Next()) {
        if (data->id == 2) { // Check if it's a float
            sum += data->f;
        }
    }
    return sum;
}

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
