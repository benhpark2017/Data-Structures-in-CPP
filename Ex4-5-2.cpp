/**
 * Solutions to Chapter 4, Section 5, Exercise 2 of Horowitz's
 * Fundamentals of Data Structure in C++.
 */

/*************************************main.cpp******************************************/

#include <iostream>
#include "Queue.h"

int main() {
    // Create a queue with a capacity of 5
    Queue<int> queue(5);

    // Test isEmpty on an empty queue
    std::cout << "Is the queue empty? " << (queue.isEmpty() ? "Yes" : "No") << std::endl;

    // Test enqueue operation
    std::cout << "Enqueuing elements 1, 2, 3, 4, 5 into the queue." << std::endl;
    for (int i = 1; i <= 5; i++) {
        queue.enqueue(i);
    }

    // Test isFull on a full queue
    std::cout << "Is the queue full? " << (queue.isFull() ? "Yes" : "No") << std::endl;

    // Test peekFront and peekRear
    std::cout << "Front element is: " << queue.peekFront() << std::endl;
    std::cout << "Rear element is: " << queue.peekRear() << std::endl;

    // Test dequeue operation
    std::cout << "Dequeuing elements from the queue:" << std::endl;
    while (!queue.isEmpty()) {
        std::cout << queue.dequeue() << " ";
    }
    std::cout << std::endl;

    // Test isEmpty on an empty queue after dequeuing all elements
    std::cout << "Is the queue empty? " << (queue.isEmpty() ? "Yes" : "No") << std::endl;

    // Test queue expansion (if supported) by enqueuing more elements
    std::cout << "Enqueuing elements 6, 7, 8, 9, 10 into the queue." << std::endl;
    for (int i = 1; i <= 5; i++) {
        queue.enqueue(i + 5);
    }

    // Test peekFront and peekRear after re-enqueuing elements
    std::cout << "Front element is: " << queue.peekFront() << std::endl;
    std::cout << "Rear element is: " << queue.peekRear() << std::endl;

    // Test dequeue operation again
    std::cout << "Dequeuing elements from the queue:" << std::endl;
    while (!queue.isEmpty()) {
        std::cout << queue.dequeue() << " ";
    }
    std::cout << std::endl;

    // Test exception handling by attempting to dequeue from an empty queue
    try {
        std::cout << "Attempting to dequeue from an empty queue:" << std::endl;
        queue.dequeue();
    } catch (const std::underflow_error &e) {
        std::cout << e.what() << std::endl;
    }

    // Test exception handling by attempting to peek front and rear in an empty queue
    try {
        std::cout << "Attempting to peek front in an empty queue:" << std::endl;
        queue.peekFront();
    } catch (const std::underflow_error &e) {
        std::cout << e.what() << std::endl;
    }

    try {
        std::cout << "Attempting to peek rear in an empty queue:" << std::endl;
        queue.peekRear();
    } catch (const std::underflow_error &e) {
        std::cout << e.what() << std::endl;
    }

    return 0;
}



/*********************************Queue.h*************************************/
#ifndef QUEUE_H
#define QUEUE_H

#include <stdexcept>

template <class KeyType>
class Queue {
    
  private:
    int front;
    int rear;
    int size;
    int capacity;
    KeyType *queue;
    
  public:
    Queue(int maxQueueSize);
    ~Queue();
    
    bool isEmpty() const;
    bool isFull() const;
    
    void enqueue(const KeyType &item);
    KeyType dequeue();
    
    KeyType peekFront() const;
    KeyType peekRear() const;
};

#endif



/********************************Queue.cpp************************************/
#include "Queue.h"

template <class KeyType>
Queue<KeyType>::Queue(int maxQueueSize) 
            : capacity(maxQueueSize), size(0), front(0), rear(maxQueueSize - 1) {
    queue = new KeyType[capacity];
}

template <class KeyType>
Queue<KeyType>::~Queue() {
    delete[] queue;
}

template <class KeyType>
bool Queue<KeyType>::isEmpty() const {
    return size == 0;
}

template <class KeyType>
bool Queue<KeyType>::isFull() const {
    return size == capacity;
}

template <class KeyType>
void Queue<KeyType>::enqueue(const KeyType &item) {
    if (isFull()) {
        throw std::overflow_error("Queue is full");
    }
    rear = (rear + 1) % capacity;
    queue[rear] = item;
    size++;
}

template <class KeyType>
KeyType Queue<KeyType>::dequeue() {
    if (isEmpty()) {
        throw std::underflow_error("Queue is empty");
    }
    KeyType item = queue[front];
    front = (front + 1) % capacity;
    size--;
    return item;
}

template <class KeyType>
KeyType Queue<KeyType>::peekFront() const {
    if (isEmpty()) {
        throw std::underflow_error("Queue is empty");
    }
    return queue[front];
}

template <class KeyType>
KeyType Queue<KeyType>::peekRear() const {
    if (isEmpty()) {
        throw std::underflow_error("Queue is empty");
    }
    return queue[rear];
}

// Explicitly instantiate the template for int
template class Queue<int>;
