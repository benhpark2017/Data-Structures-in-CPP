/*********************************main.cpp*************************************/
#include "X2.h"
#include <iostream>

int main() {
    X2<int> list(3);  // Initialize with a maximum size of 3

    list.addFront(10);
    list.addBack(20);
    list.addFront(30);

    try {
        list.addBack(40);  // This should throw an exception
    } catch (const std::out_of_range& e) {
        std::cerr << e.what() << std::endl;  // Outputs "List is full"
    }

    std::cout << list.removeFront() << std::endl;  // Outputs 30
    std::cout << list.removeFront() << std::endl;  // Outputs 10
    std::cout << list.removeFront() << std::endl;  // Outputs 20

    return 0;
}

/**********************************Node.h**************************************/
#ifndef NODE_H
#define NODE_H

template <typename Type>
class Node {
public:
    Type data;
    Node<Type>* next;
    Node<Type>* prev;
    Node(Type val);
};

#include "Node.cpp"

#endif // NODE_H

/*********************************Node.cpp*************************************/
#ifndef NODE_CPP
#define NODE_CPP

#include "Node.h"

template <typename Type>
Node<Type>::Node(Type val) : data(val), next(nullptr), prev(nullptr) {}

#endif // NODE_CPP

/***********************************X2.h***************************************/
#ifndef X2_H
#define X2_H

#include "Node.h"
#include <stdexcept>

template <typename Type>
class X2 {
private:
    Node<Type>* head;
    Node<Type>* tail;
    size_t size;
    size_t maxSize;

public:
    X2(size_t maxSize = SIZE_MAX);
    ~X2();

    void addFront(Type val);
    void addBack(Type val);
    Type removeFront();
    bool isEmpty() const;
    bool isFull() const;
};

#include "X2.cpp"

#endif // X2_H

/**********************************X2.cpp*************************************/
#ifndef X2_CPP
#define X2_CPP

#include "X2.h"

// Constructor with maximum size limit
template <typename Type>
X2<Type>::X2(size_t maxSize) : head(nullptr), tail(nullptr), size(0), maxSize(maxSize) {}

// Destructor to clean up memory
template <typename Type>
X2<Type>::~X2() {
    while (!isEmpty()) {
        removeFront();
    }
}

// Add to the front of the list
template <typename Type>
void X2<Type>::addFront(Type val) {
    if (isFull()) {
        throw std::out_of_range("List is full");
    }
    Node<Type>* newNode = new Node<Type>(val);
    if (isEmpty()) {
        head = tail = newNode;
    } else {
        newNode->next = head;
        head->prev = newNode;
        head = newNode;
    }
    size++;
}

// Add to the back of the list
template <typename Type>
void X2<Type>::addBack(Type val) {
    if (isFull()) {
        throw std::out_of_range("List is full");
    }
    Node<Type>* newNode = new Node<Type>(val);
    if (isEmpty()) {
        head = tail = newNode;
    } else {
        newNode->prev = tail;
        tail->next = newNode;
        tail = newNode;
    }
    size++;
}

// Remove from the front of the list
template <typename Type>
Type X2<Type>::removeFront() {
    if (isEmpty()) {
        throw std::out_of_range("List is empty");
    }
    Node<Type>* temp = head;
    Type val = head->data;
    head = head->next;
    if (head) {
        head->prev = nullptr;
    } else {
        tail = nullptr;
    }
    delete temp;
    size--;
    return val;
}

// Initial condition: Check if the list is empty
template <typename Type>
bool X2<Type>::isEmpty() const {
    return head == nullptr;
}

// Boundary condition: Check if the list is full
template <typename Type>
bool X2<Type>::isFull() const {
    return size >= maxSize;
}

#endif //X2_CPP
