/**
 * Solutions to Chapter 3, Section 3, Exercise 1 of Horowitz's
 * Fundamentals of Data Structures in C++.
 */
 
/**************************************main.cpp****************************************/
#include <iostream>
#include "CircularQueue.h"

int main() {
    
    CircularQueue<int> intCircularQueue(10);
    
    std::cout << "Add to circular queue: " << std::endl;
    for (int i = 0; i < 10; i++)
        intCircularQueue.AddToQueue(i + 1);
    
    std::cout << "Current front: " << intCircularQueue.Front() << std::endl;
    std::cout << "Current rear: " << intCircularQueue.Rear() << std::endl;
    std::cout << "Current last operation: ";
    intCircularQueue.LastOperation();
    std::cout << std::endl;
    
    std::cout << "Deleting two elements from the circular queue." << std::endl;
    intCircularQueue.DeleteFromQueue();
    std::cout << "Deleted one element. Current last operation: ";
    intCircularQueue.LastOperation();
    std::cout << std::endl;
    
    std::cout << "Current front: " << intCircularQueue.Front() << std::endl;
    std::cout << "Current rear: " << intCircularQueue.Rear() << std::endl;

    intCircularQueue.DeleteFromQueue();
    std::cout << "Deleted another element. Current last operation: ";
    intCircularQueue.LastOperation();
    std::cout << std::endl;
    
    std::cout << "Current front: " << intCircularQueue.Front() << std::endl;
    std::cout << "Current rear: " << intCircularQueue.Rear() << std::endl;

    std::cout << "Adding more elements to the circular queue to test wrap-around." << std::endl;
    for (int i = 10; i < 15; i++) {
        intCircularQueue.AddToQueue(i + 1);
        std::cout << "Added: " << i + 1 << std::endl;
    }

    std::cout << "Current front: " << intCircularQueue.Front() << std::endl;
    std::cout << "Current rear: " << intCircularQueue.Rear() << std::endl;
    std::cout << "Current last operation: ";
    intCircularQueue.LastOperation();
    std::cout << std::endl;

    std::cout << "Deleting all elements from the circular queue." << std::endl;
    while (!intCircularQueue.isEmpty()) {
        intCircularQueue.DeleteFromQueue();
        std::cout << "Deleted an element. Current last operation: " << std::endl;
        intCircularQueue.LastOperation();
        std::cout<< std::endl;
        if (!intCircularQueue.isEmpty()) {
            std::cout << "Current front: " << intCircularQueue.Front() << std::endl;
            std::cout << "Current rear: " << intCircularQueue.Rear() << std::endl;
        }
    }
    
    return 0;
}



/***************************************CircularQueue.h****************************************/
#ifndef CIRCULARQUEUE_H
#define CIRCULARQUEUE_H

template <class KeyType>
class CircularQueue {
  private:
    int front;
    int rear;
    KeyType* circularQueue;
    int MAX_SIZE;
    int lastOperation;

  public:
    CircularQueue(int MAX_QUEUE_SIZE = 10);
    bool isFull() const;
    void AddToQueue(const KeyType& item);
    bool isEmpty() const;
    KeyType& Front() const;
    KeyType& Rear() const;
    void DeleteFromQueue();
    void LastOperation() const;
};

#endif



/************************************CircularQueue.cpp***************************************/
#include "CircularQueue.h"
#include <stdexcept>
#include <iostream>

template <class KeyType>
CircularQueue<KeyType>::CircularQueue(int MAX_QUEUE_SIZE) : MAX_SIZE(MAX_QUEUE_SIZE) {
    if (MAX_SIZE < 1) throw std::invalid_argument("Queue Capacity must be greater than zero");
    circularQueue = new KeyType[MAX_SIZE];
    front = rear = 0;
    lastOperation = 0;
}

template <class KeyType>
bool CircularQueue<KeyType>::isFull() const {
    return (rear + 1) % MAX_SIZE == front;
}

template <class KeyType>
bool CircularQueue<KeyType>::isEmpty() const {
    return (front == rear && lastOperation == 2);
}

template <class KeyType>
KeyType& CircularQueue<KeyType>::Front() const {
    if (isEmpty()) throw std::runtime_error("Queue is empty. No front element.");
    return circularQueue[(front + 1) % MAX_SIZE];
}

template <class KeyType>
KeyType& CircularQueue<KeyType>::Rear() const {
    if (isEmpty()) throw std::runtime_error("Queue is empty. No rear element.");
    return circularQueue[rear];
}

template <class KeyType>
void CircularQueue<KeyType>::AddToQueue(const KeyType& item) {
    if (isFull()) {
        KeyType* newQueue = new KeyType[2 * MAX_SIZE];
        int start = (front + 1) % MAX_SIZE;

        if (start < 2) {
            for (int i = 0; i < MAX_SIZE - 1; ++i) {
                newQueue[i] = circularQueue[start + i];
            }
        } else {
            int j = 0;
            for (int i = start; i < MAX_SIZE; ++i, ++j) {
                newQueue[j] = circularQueue[i];
            }
            for (int i = 0; i <= rear; ++i, ++j) {
                newQueue[j] = circularQueue[i];
            }
        }

        front = 2 * MAX_SIZE - 1;
        rear = MAX_SIZE - 2;
        MAX_SIZE *= 2;
        delete[] circularQueue;
        circularQueue = newQueue;
    }
    rear = (rear + 1) % MAX_SIZE;
    circularQueue[rear] = item;
    lastOperation = 1;
}

template <class KeyType>
void CircularQueue<KeyType>::DeleteFromQueue() {
    if (isEmpty()) throw std::runtime_error("Cannot perform deletion because queue is empty.");
    lastOperation = 2;
    front = (front + 1) % MAX_SIZE;
    circularQueue[front].~KeyType(); // destructor for KeyType
}

template <class KeyType>
void CircularQueue<KeyType>::LastOperation() const {
    if (lastOperation == 1) std::cout << "Add to queue." << std::endl;
    else if (lastOperation == 2) std::cout << "Delete from queue." << std::endl;
    else std::cout << "No operation." << std::endl;
}

// Instantiate class template CircularQueue
template class CircularQueue<int>;