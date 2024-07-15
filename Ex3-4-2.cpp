/**
 * Solutions to Chapter 3, Section 4, Exercise 2 of Horowitz's
 * Fundamentals of Data Structures in C++.
 * This version of code does not use .tpp files, but instead uses
 * .cpp format files for implementation.
 */
 
/************************************main.cpp*************************************/
#include <iostream>
#include "Queue.h"

int main() {
    Queue<int> intQueue;
    for (int i = 0; i < 10; i++) {
        intQueue.Push(i + 1);
    }
    std::cout << "Current Queue:" << std::endl;
    std::cout << intQueue << std::endl;

    for (int i = 0; i < 5; i++) {
        std::cout << i + 1 << "th Dequeue: " << intQueue.Dequeue() << std::endl;
    }
    std::cout << "Queue after 5 Dequeues:" << std::endl;
    std::cout << intQueue << std::endl;

    return 0;
}




/*************************************Bag.h**************************************/
#ifndef BAG_H
#define BAG_H

#include <iostream>

template <class KeyType>
class Bag {
protected:
    KeyType *array;
    int capacity;
    int top;

public:
    Bag(int bagCapacity = 10);
    virtual ~Bag();
    virtual void Push(const KeyType &item);
    virtual void Del();
    virtual bool IsEmpty();
    virtual bool IsFull();
    bool Contains(const KeyType &item) const;
    int Size() const;
    template <class K>
    friend std::ostream& operator<<(std::ostream &os, Bag<K> &b);
};

#include "Bag.cpp"

#endif // BAG_H




/************************************Bag.cpp*************************************/
#ifndef BAG_CPP
#define BAG_CPP

#include "Bag.h"

template <class KeyType>
Bag<KeyType>::Bag(int bagCapacity) : capacity(bagCapacity), top(-1) {
    array = new KeyType[capacity];
}

template <class KeyType>
Bag<KeyType>::~Bag() {
    delete[] array;
}

template <class KeyType>
void Bag<KeyType>::Push(const KeyType &item) {
    if (IsFull()) {
        std::cout << "Bag is full. Cannot Push more items." << std::endl;
    } else {
        array[++top] = item;
    }
}

template <class KeyType>
void Bag<KeyType>::Del() {
    if (IsEmpty()) {
        std::cout << "Bag is empty." << std::endl;
    } else {
        for (int i = (top / 2); i < top; i++) {
            array[i] = array[i + 1]; // Shift elements left
        }
        top--;
    }
}

template <class KeyType>
bool Bag<KeyType>::IsEmpty() {
    return top == -1;
}

template <class KeyType>
bool Bag<KeyType>::IsFull() {
    return top >= capacity - 1;
}

template <class KeyType>
bool Bag<KeyType>::Contains(const KeyType &item) const {
    for (int i = 0; i <= top; i++) {
        if (array[i] == item) {
            return true;
        }
    }
    return false;
}

template <class KeyType>
int Bag<KeyType>::Size() const {
    return top + 1;
}

template <class KeyType>
std::ostream& operator<<(std::ostream &os, Bag<KeyType> &b) {
    for (int i = 0; i <= b.top; i++) {
        os << b.array[i] << " ";
    }
    os << std::endl;
    return os;
}

//Explicit instantiation of the Bag class template
template class Bag<int>;

#endif // BAG_CPP




/************************************Queue.h**************************************/
#ifndef QUEUE_H
#define QUEUE_H

#include "Bag.h"

template <class KeyType>
class Queue : public Bag<KeyType> {
protected:
    int front;

public:
    Queue(int queueCapacity = 10);
    ~Queue();
    KeyType Dequeue();
    KeyType& Front() const;
    KeyType& Rear() const;
    bool IsEmpty() const;
    template <class T>
    friend std::ostream& operator<<(std::ostream &os, Queue<T> &q);
};

#include "Queue.cpp"

#endif // QUEUE_H




/***********************************Queue.cpp*************************************/
#ifndef QUEUE_CPP
#define QUEUE_CPP

#include "Queue.h"

template <class KeyType>
Queue<KeyType>::Queue(int queueCapacity) 
                 : Bag<KeyType>(queueCapacity), front(0) {}

template <class KeyType>
Queue<KeyType>::~Queue() {
    // Destructor for Queue. Bag's destructor is called automatically.
}

template <class KeyType>
KeyType Queue<KeyType>::Dequeue() {
    if (IsEmpty()) {
        std::cout << "Queue is empty." << std::endl;
        return KeyType(); // Return default-constructed KeyType
    }
    KeyType del = this->array[front++];
    return del;
}

template <class KeyType>
KeyType& Queue<KeyType>::Front() const {
    if (IsEmpty()) {
        std::cout << "Queue is empty." << std::endl;
        return this->array[0]; // Dummy return value. Ideally, throw an exception.
    }
    return this->array[front];
}

template <class KeyType>
KeyType& Queue<KeyType>::Rear() const {
    if (IsEmpty()) {
        std::cout << "Queue is empty." << std::endl;
        return this->array[0]; // Dummy return value. Ideally, throw an exception.
    }
    return this->array[this->top];
}

template <class KeyType>
bool Queue<KeyType>::IsEmpty() const {
    return this->top == front - 1;
}

template <class KeyType>
std::ostream& operator<<(std::ostream &os, Queue<KeyType> &q) {
    for (int i = q.front; i <= q.top; i++) {
        os << q.array[i] << " ";
    }
    os << std::endl;
    return os;
}

//Explicit instantiation of the Queue class template
template class Queue<int>;

#endif // QUEUE_CPP