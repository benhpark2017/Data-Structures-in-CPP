/**
 * Solutions to Chapter 3, Section 3, Exercise 3 of Horowitz's
 * Fundamentals of Data Structures in C++.
 */
 
/**************************************main.cpp****************************************/
#include <iostream>
#include "CircularQueue.h"

int main() {
    CircularQueue<int> queue(5);

    std::cout << "Adding elements to the queue:" << std::endl;
    queue.AddToQueue(10);
    queue.AddToQueue(20);
    queue.AddToQueue(30);
    queue.AddToQueue(40);
    queue.AddToQueue(50);

    std::cout << "Queue is full: " << (queue.isFull() ? "Yes" : "No") << std::endl;
    queue.PrintCircularQueue();

    std::cout << "Deleting an element from the queue: " << queue.DeleteFromQueue() << std::endl;
    queue.PrintCircularQueue();

    std::cout << "Queue is full: " << (queue.isFull() ? "Yes" : "No") << std::endl;

    std::cout << "Adding an element to the queue:" << std::endl;
    queue.AddToQueue(60);
    queue.PrintCircularQueue();

    std::cout << "Deleting an element from the queue: " << queue.DeleteFromQueue() << std::endl;
    queue.PrintCircularQueue();

    std::cout << "Deleting the 2nd element from the queue (0-based index):" << std::endl;
    queue.deleteKth(2);
    queue.PrintCircularQueue();

    std::cout << "Adding an element after the 1st element (0-based index):" << std::endl;
    queue.insertAfterKth(1, 70);
    queue.PrintCircularQueue();

    std::cout << "Current queue size: " << queue.size() << std::endl;
    queue.PrintCircularQueue();

    std::cout << "Deleting elements from the queue:" << std::endl;
    while (!queue.isEmpty()) {
        std::cout << queue.DeleteFromQueue() << " ";
    }
    std::cout << std::endl;

    return 0;
}





/***********************************CircularQueue.h************************************/
#ifndef CIRCULARQUEUE_H
#define CIRCULARQUEUE_H

#include <stdexcept>

template <class KeyType>
class CircularQueue {
public:
    CircularQueue(int size);
    ~CircularQueue();

    void AddToQueue(KeyType element);
    KeyType DeleteFromQueue();
    bool isEmpty() const;
    bool isFull() const;
    int size() const;

    void deleteKth(int k);
    void insertAfterKth(int k, KeyType element);
    void PrintCircularQueue() const;

private:
    int capacity;
    KeyType* c;
    int front;
    int rear;
    int count;
};

#endif // CIRCULARQUEUE_H





/**********************************CircularQueue.cpp***********************************/
#include "CircularQueue.h"
#include <iostream>

template <class KeyType>
CircularQueue<KeyType>::CircularQueue(int size) 
    : capacity(size), front(0), rear(size - 1), count(0) {
    c = new KeyType[size];
}

template <class KeyType>
CircularQueue<KeyType>::~CircularQueue() {
    delete[] c;
}

template <class KeyType>
void CircularQueue<KeyType>::AddToQueue(KeyType element) {
    if (isFull()) {
        throw std::overflow_error("Queue is full");
    }
    rear = (rear + 1) % capacity;
    c[rear] = element;
    count++;
}

template <class KeyType>
KeyType CircularQueue<KeyType>::DeleteFromQueue() {
    if (isEmpty()) {
        throw std::underflow_error("Queue is empty");
    }
    KeyType element = c[front];
    front = (front + 1) % capacity;
    count--;
    return element;
}

template <class KeyType>
bool CircularQueue<KeyType>::isEmpty() const {
    return count == 0;
}

template <class KeyType>
bool CircularQueue<KeyType>::isFull() const {
    return count == capacity;
}

template <class KeyType>
int CircularQueue<KeyType>::size() const {
    return count;
}

template <class KeyType>
void CircularQueue<KeyType>::deleteKth(int k) {
    if (k >= count) {
        throw std::out_of_range("Index out of range");
    }

    int indexToDelete = (front + k) % capacity;
    for (int i = indexToDelete; i != rear; i = (i + 1) % capacity) {
        int nextIndex = (i + 1) % capacity;
        c[i] = c[nextIndex];
    }
    
    rear = (rear - 1 + capacity) % capacity;
    count--;
}


template <class KeyType>
void CircularQueue<KeyType>::insertAfterKth(int k, KeyType element) {
    if (isFull()) {
        throw std::overflow_error("Queue is full");
    }
    if (k >= count) {
        throw std::out_of_range("Index out of range");
    }

    int indexToInsert = (front + k + 1) % capacity;
    for (int i = count; i > k + 1; --i) {
        int from = (front + i - 1) % capacity;
        int to = (from + 1) % capacity;
        c[to] = c[from];
    }
    
    c[indexToInsert] = element;
    rear = (rear + 1) % capacity;
    count++;
}

template <class KeyType>
void CircularQueue<KeyType>::PrintCircularQueue() const {
    if (isEmpty()) {
        std::cout << "Queue is empty." << std::endl;
        return;
    }

    std::cout << "Queue elements: ";
    for (int i = 0; i < count; ++i) {
        std::cout << c[(front + i) % capacity] << " ";
    }
    std::cout << std::endl;
}

// Don't forget the explicit instantiation!
template class CircularQueue<int>;
