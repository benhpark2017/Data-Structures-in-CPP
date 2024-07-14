/**
 * Solutions to Chapter 3, Section 3, Exercise 2 of Horowitz's
 * Fundamentals of Data Structures in C++.
 */
 
/**************************************main.cpp****************************************/
#include <iostream>
#include "Deque.h"

int main() {
    Deque<int> deque;

    deque.addBack(1);
    deque.addBack(2);
    deque.addFront(0);
    deque.printDeque(); // Output: 0 1 2

    deque.deleteFront();
    deque.printDeque(); // Output: 1 2

    deque.addFront(-1);
    deque.printDeque(); // Output: -1 1 2

    deque.deleteBack();
    deque.printDeque(); // Output: -1 1

    std::cout << "Front element: " << deque.getFront() << std::endl; // Output: -1
    std::cout << "Back element: " << deque.getBack() << std::endl;   // Output: 1

    return 0;
}




/***************************************Deque.h****************************************/
#ifndef DEQUE_H
#define DEQUE_H

#include <iostream>
#include <stdexcept>

#include <stdexcept>

template <class KeyType>
class Deque {
  private:
    KeyType* deque;
    int capacity;
    int front;
    int back;
    int size;

    // Helper function to increase capacity
    void resize();

  public:
    // Constructor
    Deque(int initialCapacity = 10);

    // Destructor
    ~Deque();

    // Add an element at the front
    void addFront(const KeyType& element);

    // Add an element at the back
    void addBack(const KeyType& element);

    // Delete an element from the front
    void deleteFront();

    // Delete an element from the back
    void deleteBack();

    // Get the front element
    KeyType getFront() const;

    // Get the back element
    KeyType getBack() const;

    // Check if the deque is empty
    bool isEmpty() const;

    // Get the size of the deque
    int getSize() const;

    // Print the elements of the deque (for debugging)
    void printDeque() const;
};

#endif




/**************************************Deque.cpp***************************************/
#include "Deque.h"
#include <iostream>

// Constructor
template <class KeyType>
Deque<KeyType>::Deque(int initialCapacity)
    : capacity(initialCapacity), front(0), back(0), size(0) {
    deque = new KeyType[capacity];
}

// Destructor
template <class KeyType>
Deque<KeyType>::~Deque() {
    delete[] deque;
}

// Helper function to increase capacity
template <class KeyType>
void Deque<KeyType>::resize() {
    int newCapacity = capacity * 2;
    KeyType* newDeque = new KeyType[newCapacity];

    // Copy old deque to new array
    for (int i = 0; i < size; ++i) {
        newDeque[i] = deque[(front + i) % capacity];
    }

    delete[] deque;
    deque = newDeque;
    capacity = newCapacity;
    front = 0;
    back = size;
}

// Add an element at the front
template <class KeyType>
void Deque<KeyType>::addFront(const KeyType& element) {
    if (size == capacity) {
        resize();
    }
    front = (front - 1 + capacity) % capacity;
    deque[front] = element;
    ++size;
}

// Add an element at the back
template <class KeyType>
void Deque<KeyType>::addBack(const KeyType& element) {
    if (size == capacity) {
        resize();
    }
    deque[back] = element;
    back = (back + 1) % capacity;
    ++size;
}

// Delete an element from the front
template <class KeyType>
void Deque<KeyType>::deleteFront() {
    if (isEmpty()) {
        throw std::underflow_error("Deque is empty");
    }
    front = (front + 1) % capacity;
    --size;
}

// Delete an element from the back
template <class KeyType>
void Deque<KeyType>::deleteBack() {
    if (isEmpty()) {
        throw std::underflow_error("Deque is empty");
    }
    back = (back - 1 + capacity) % capacity;
    --size;
}

// Get the front element
template <class KeyType>
KeyType Deque<KeyType>::getFront() const {
    if (isEmpty()) {
        throw std::underflow_error("Deque is empty");
    }
    return deque[front];
}

// Get the back element
template <class KeyType>
KeyType Deque<KeyType>::getBack() const {
    if (isEmpty()) {
        throw std::underflow_error("Deque is empty");
    }
    return deque[(back - 1 + capacity) % capacity];
}

// Check if the deque is empty
template <class KeyType>
bool Deque<KeyType>::isEmpty() const {
    return size == 0;
}

// Get the size of the deque
template <class KeyType>
int Deque<KeyType>::getSize() const {
    return size;
}

// Print the elements of the deque (for debugging)
template <class KeyType>
void Deque<KeyType>::printDeque() const {
    for (int i = 0; i < size; ++i) {
        std::cout << deque[(front + i) % capacity] << " ";
    }
    std::cout << std::endl;
}
    
template class Deque<int>;