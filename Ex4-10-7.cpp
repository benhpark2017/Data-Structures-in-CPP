/***************************************main.cpp*********************************************/
#include <iostream>
#include "GenList.h"

int main() {
    // Create the list A = (a (b, c))
    GenList<char>* listA = new GenList<char>();
    listA->addAtomic('a');

    GenList<char>* sublist = new GenList<char>();
    sublist->addAtomic('b');
    sublist->addAtomic('c');

    listA->addSublist(sublist);

    std::cout << "List A: ";
    listA->print();
    std::cout << std::endl;
    
    // Invert the list A
    GenList<char>* invertedListA = listA->invert();

    std::cout << "Inverted List A: ";
    invertedListA->print();
    std::cout << std::endl;

    // Create the list B = (A, A, ())
    GenList<char>* listB = new GenList<char>();
    listB->addSublist(listA);  // Reuse list A
    listB->addSublist(listA);  // Reuse list A again
    listB->addSublist(new GenList<char>());  // Empty sublist

    std::cout << "List B: ";
    listB->print();
    std::cout << std::endl;
    
    // Invert the list B
    GenList<char>* invertedListB = listB->invert();

    std::cout << "Inverted List B: ";
    invertedListB->print();
    std::cout << std::endl;

    // Clean up
    delete listB;
    delete listA;
    delete sublist;
    delete invertedListA;
    delete invertedListB;

    return 0;
}



/*************************************GenList.h*******************************************/
#ifndef GENLIST_H
#define GENLIST_H

#include "GenListNode.h"

template <typename T>
class GenList {
private:
    GenListNode<T>* head;

    // Helper function for deep copy
    GenListNode<T>* copyList(GenListNode<T>* source);

    // Helper function for destruction
    void destroyList(GenListNode<T>* node);

public:
    // Constructor for an empty list
    GenList();

    // Copy constructor
    GenList(const GenList<T>& other);

    // Destructor
    ~GenList();
    
    // Function that inverts a generalized list
    GenList<T>* invert() const;
    
    // Helper function to invert a generalized list
    void reverse();

    // Add an atomic value to the list
    void addAtomic(const T& value);

    // Add a sublist to the list
    void addSublist(GenList<T>* sublist);

    // Print the list
    void print() const;

    // Increment reference count (used by sublists)
    void incrementRef();

    // Decrement reference count (used by sublists)
    void decrementRef();
};

#include "GenList.tpp"

#endif // GENLIST_H



/************************************GenList.tpp******************************************/
#ifndef GENLIST_TPP
#define GENLIST_TPP

#include <iostream>

template <typename T>
GenList<T>::GenList() : head(nullptr) {}

template <typename T>
GenList<T>::GenList(const GenList<T>& other) : head(copyList(other.head)) {}

template <typename T>
GenList<T>::~GenList() {
    destroyList(head);
}

template <typename T>
GenList<T>* GenList<T>::invert() const {
    GenList<T>* invertedList = new GenList<T>();  // Create a new empty list to hold the inverted result
    GenListNode<T>* current = head;  // Start from the current list's head
    
    // First, recursively invert sublists and store them in reverse order
    while (current) {
        if (current->isAtomic) {
            invertedList->addAtomic(current->data.atomic);  // Add atomic element
        } else {
            // Recursively invert the sublist and add the inverted sublist
            GenList<T>* invertedSublist = current->data.sublist->invert();
            invertedList->addSublist(invertedSublist);
        }
        current = current->next;
    }

    // Reverse the entire list after adding the elements
    invertedList->reverse();
    return invertedList;
}

// Reverse the list (helper function)
template <typename T>
void GenList<T>::reverse() {
    GenListNode<T>* prev = nullptr;
    GenListNode<T>* current = head;
    GenListNode<T>* next = nullptr;

    while (current) {
        next = current->next;  // Store next node
        current->next = prev;  // Reverse current node's pointer
        prev = current;        // Move pointers forward
        current = next;
    }
    head = prev;  // Update head to the new front
}

template <typename T>
void GenList<T>::addAtomic(const T& value) {
    GenListNode<T>* newNode = new GenListNode<T>(value);
    newNode->next = nullptr;
    if (!head) {
        head = newNode;
    } else {
        GenListNode<T>* current = head;
        while (current->next) {
            current = current->next;
        }
        current->next = newNode;
    }
}

template <typename T>
void GenList<T>::addSublist(GenList<T>* sublist) {
    GenListNode<T>* newNode = new GenListNode<T>(sublist);
    newNode->next = nullptr;
    if (!head) {
        head = newNode;
    } else {
        GenListNode<T>* current = head;
        while (current->next) {
            current = current->next;
        }
        current->next = newNode;
    }
}

template <typename T>
void GenList<T>::print() const {
    std::cout << "(";
    GenListNode<T>* current = head;
    while (current) {
        if (current->isAtomic) {
            std::cout << current->data.atomic;
        } else {
            current->data.sublist->print();
        }
        if (current->next) {
            std::cout << ", ";
        }
        current = current->next;
    }
    std::cout << ")";
}

template <typename T>
void GenList<T>::incrementRef() {
    if (head) {
        head->incrementRef();
    }
}

template <typename T>
void GenList<T>::decrementRef() {
    if (head) {
        head->decrementRef();
    }
}

template <typename T>
GenListNode<T>* GenList<T>::copyList(GenListNode<T>* source) {
    if (!source) {
        return nullptr;
    }

    GenListNode<T>* newNode;
    if (source->isAtomic) {
        newNode = new GenListNode<T>(source->data.atomic);
    } else {
        newNode = new GenListNode<T>(new GenList<T>(*source->data.sublist));
    }

    newNode->next = copyList(source->next);
    return newNode;
}

template <typename T>
void GenList<T>::destroyList(GenListNode<T>* node) {
    while (node) {
        GenListNode<T>* next = node->next;
        node->decrementRef();
        node = next;
    }
}

#endif // GENLIST_TPP



/***********************************GenListNode.h*****************************************/

#ifndef GENLISTNODE_H
#define GENLISTNODE_H

#include <iostream>

// Forward declaration
template <typename T>
class GenList;

template <typename T>
class GenListNode {
public:
    union Data {
        T atomic;
        GenList<T>* sublist;
        Data() {}
        ~Data() {}
    };

    bool isAtomic;  // true if atomic, false if sublist
    Data data;
    GenListNode* next;
    int refCount;

    // Constructor for atomic data
    GenListNode(const T& value);

    // Constructor for sublist
    GenListNode(GenList<T>* sublist);

    // Destructor
    ~GenListNode();

    // Increment the reference count
    void incrementRef();

    // Decrement the reference count
    void decrementRef();

private:
    // Private copy constructor and assignment operator to prevent copying
    GenListNode(const GenListNode&);
    GenListNode& operator=(const GenListNode&);
};

#include "GenListNode.tpp"

#endif // GENLISTNODE_H



/**********************************GenListNode.tpp***************************************/
#ifndef GENLISTNODE_TPP
#define GENLISTNODE_TPP

template <typename T>
GenListNode<T>::GenListNode(const T& value) : isAtomic(true), next(nullptr), refCount(1) {
    data.atomic = value;
}

template <typename T>
GenListNode<T>::GenListNode(GenList<T>* sublist) : isAtomic(false), next(nullptr), refCount(1) {
    data.sublist = sublist;
    if (sublist) {
        sublist->incrementRef();
    }
}

template <typename T>
GenListNode<T>::~GenListNode() {
    if (!isAtomic && data.sublist) {
        data.sublist->decrementRef();
    }
}

template <typename T>
void GenListNode<T>::incrementRef() {
    ++refCount;
}

template <typename T>
void GenListNode<T>::decrementRef() {
    --refCount;
    if (refCount == 0) {
        delete this;
    }
}

#endif // GENLISTNODE_TPP
