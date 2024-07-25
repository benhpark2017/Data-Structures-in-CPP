/**
 * Solution to Chapter 4, Section 6, Exercise 1 of Horowitz's
 * Fundamentals of Data Structures in C++.
 */

/**********************************main.cpp************************************/
#include "SinglyLinkedList.h"

int main() {
    SinglyLinkedList<int> list;
    
    for (int i = 1; i <= 5; i++) {
        list.append(i);
    }
    
    std::cout << "Original list: " << list << std::endl;

    // Use the copy constructor
    SinglyLinkedList<int> copiedList = list;

    std::cout << "Copied list: " << copiedList << std::endl;

    return 0;
}



/****************************SinglyLinkedList.h******************************/
#ifndef SINGLYLINKEDLIST_H
#define SINGLYLINKEDLIST_H

#include <iostream>

template <class T>
class SinglyLinkedList {
private:
    struct Node {
        T data;
        Node* next;

        Node(T data) : data(data), next(nullptr) {}
    };

    Node* head;

public:
    // Default constructor
    SinglyLinkedList();

    // Copy constructor (Solution to Chapter 4, Section 6, Exercise 1)
    SinglyLinkedList(const SinglyLinkedList& other);

    // Destructor
    ~SinglyLinkedList();

    // Method to add a new node at the end of the list
    void append(T data);

    // Friend function to overload the << operator
    template <class U>
    friend std::ostream& operator<<(std::ostream& os, const SinglyLinkedList<U>& list);
};

#include "SinglyLinkedList.cpp"

#endif // SINGLYLINKEDLIST_H



/*****************************SinglyLinkedList.cpp******************************/
#ifndef SINGLYLINKEDLIST_CPP
#define SINGLYLINKEDLIST_CPP

#include "SinglyLinkedList.h"

// Default constructor
template <class T>
SinglyLinkedList<T>::SinglyLinkedList() : head(nullptr) {}

// Copy constructor
template <class T>
SinglyLinkedList<T>::SinglyLinkedList(const SinglyLinkedList& other) : head(nullptr) {
    if (other.head == nullptr) {
        head = nullptr;
    } else {
        head = new Node(other.head->data);
        Node* current = head;
        Node* otherCurrent = other.head->next;

        while (otherCurrent != nullptr) {
            current->next = new Node(otherCurrent->data);
            current = current->next;
            otherCurrent = otherCurrent->next;
        }
    }
}

// Destructor
template <class T>
SinglyLinkedList<T>::~SinglyLinkedList() {
    Node* current = head;
    while (current != nullptr) {
        Node* next = current->next;
        delete current;
        current = next;
    }
}

// Method to add a new node at the end of the list
template <class T>
void SinglyLinkedList<T>::append(T data) {
    if (head == nullptr) {
        head = new Node(data);
    } else {
        Node* current = head;
        while (current->next != nullptr) {
            current = current->next;
        }
        current->next = new Node(data);
    }
}

// Overload the << operator
template<typename T>
std::ostream& operator<<(std::ostream& os, const SinglyLinkedList<T>& list) {
    if (list.head == nullptr) {
        return os << "nullptr";
    }

    auto current = list.head;
    os << current->data;
    
    while (current->next != nullptr) {
        current = current->next;
        os << "->" << current->data;
    }

    return os;
}

#endif //SINGLYLINKEDLIST_CPP
