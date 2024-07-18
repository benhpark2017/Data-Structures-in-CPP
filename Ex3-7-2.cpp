/**
 * Solutions to Chapter 3, Section 7, Exercise 2 of Horowitz's
 * Fundamentals of Data Structures in C++.
 * 
 */

/********************************main.cpp************************************/
#include <iostream>
#include <stdexcept>
#include "StackAndQueue.h"

int main() {
    StackAndQueue<int> sq(10);  // Create a StackAndQueue with a capacity of 10 elements

    try {
        // Stack operations
        sq.stackPush(5);
        sq.stackPush(10);
        sq.stackPush(15);

        // Queue operations
        sq.queueEnqueue(100);
        sq.queueEnqueue(200);
        sq.queueEnqueue(300);

        std::cout << "After pushing to stack and enqueueing to queue:" << std::endl;
        sq.display();

        // Pop from stack and dequeue from queue
        int stackItem = sq.stackPop();
        int queueItem = sq.queueDequeue();

        std::cout << "\nPopped from stack: " << stackItem << std::endl;
        std::cout << "Dequeued from queue: " << queueItem << std::endl;

        std::cout << "\nAfter popping from stack and dequeuing from queue:" << std::endl;
        sq.display();

        // Add more elements
        sq.stackPush(20);
        sq.queueEnqueue(400);

        std::cout << "\nAfter adding more elements:" << std::endl;
        sq.display();

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}



/****************************StackAndQueue.h*********************************/
#ifndef STACKANDQUEUE_H
#define STACKANDQUEUE_H

#include <stdexcept>
#include <iostream>

template <class KeyType>
class StackAndQueue {
private:
    KeyType* M;           // The array to hold both stack and queue
    int n;                // Total size of the array
    int stackTop;         // Top of stack (grows from left)
    int queueFront;       // Front of queue
    int queueRear;        // Rear of queue (grows from right)
    int queueSize;        // Current size of the queue

public:
    StackAndQueue(int size);
    ~StackAndQueue();

    // Stack operations
    void stackPush(const KeyType& x);
    KeyType stackPop();

    // Queue operations
    void queueEnqueue(const KeyType& x);
    KeyType queueDequeue();

    // Display function to show the contents of the array
    void display() const;
};

#include "StackAndQueue.cpp"

#endif // STACKANDQUEUE_H




/***************************StackAndQueue.cpp********************************/
#ifndef STACKANDQUEUE_CPP
#define STACKANDQUEUE_CPP

#include "StackAndQueue.h"

template <class KeyType>
StackAndQueue<KeyType>::StackAndQueue(int size)
                      : n(size), stackTop(-1), queueFront(size), 
                        queueRear(size - 1), queueSize(0) {
    M = new KeyType[size];
}

template <class KeyType>
StackAndQueue<KeyType>::~StackAndQueue() {
    delete[] M;
}

// Stack operations
template <class KeyType>
void StackAndQueue<KeyType>::stackPush(const KeyType& x) {
    if (stackTop + 1 == queueFront) {
        throw std::runtime_error("Array is full");
    }
    M[++stackTop] = x;
}

template <class KeyType>
KeyType StackAndQueue<KeyType>::stackPop() {
    if (stackTop == -1) {
        throw std::runtime_error("Stack is empty");
    }
    return M[stackTop--];
}

// Queue operations
template <class KeyType>
void StackAndQueue<KeyType>::queueEnqueue(const KeyType& x) {
    if (stackTop + 1 == queueFront) {
        throw std::runtime_error("Array is full");
    }
    if (queueSize == 0) {
        queueFront = queueRear = n - 1;
    } else {
        queueRear = (queueRear - 1 + n) % n;
    }
    M[queueRear] = x;
    queueSize++;
}

template <class KeyType>
KeyType StackAndQueue<KeyType>::queueDequeue() {
    if (queueSize == 0) {
        throw std::runtime_error("Queue is empty");
    }
    KeyType x = M[queueFront];
    if (queueSize == 1) {
        queueFront = queueRear = n;
    } else {
        queueFront = (queueFront - 1 + n) % n;
    }
    queueSize--;
    return x;
}

// Display function to show the contents of the array
template <class KeyType>
void StackAndQueue<KeyType>::display() const {
    std::cout << "Array contents:" << std::endl;
    for (int i = 0; i < n; ++i) {
        std::cout << "[" << i << "]: ";
        if (i <= stackTop) {
            std::cout << M[i] << " (Stack)";
        } else if (queueSize > 0 && 
                  ((queueRear <= queueFront && (i >= queueRear && i <= queueFront)) ||
                    (queueRear > queueFront && (i >= queueRear || i <= queueFront)))) {
            std::cout << M[i] << " (Queue)";
        } else {
            std::cout << "Empty";
        }
        std::cout << std::endl;
    }
    std::cout << "Stack top: " << stackTop << std::endl;
    std::cout << "Queue front: " << queueFront << ", Queue rear: "
                                 << queueRear << std::endl;
    std::cout << "Queue size: " << queueSize << std::endl;
}

template class StackAndQueue<int>;

#endif // STACKANDQUEUE_CPP