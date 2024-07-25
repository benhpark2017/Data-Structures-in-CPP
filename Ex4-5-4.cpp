/**
 * Solutions to Chapter 4, Section 5, Exercise 4 of Horowitz's
 * Fundamentals of Data Structures in C++.
 */

/********************************main.cpp**************************************/
#include <iostream>
#include "Queue.h"

int main() {
    Queue<int> q;

    q.enqueue(10);
    q.enqueue(20);
    q.enqueue(30);

    q.displayQueue();  // Output: 10 20 30

    std::cout << "Dequeued: " << q.dequeue() << std::endl;  // Output: Dequeued: 10
    q.displayQueue();  // Output: 20 30

    std::cout << "Dequeued: " << q.dequeue() << std::endl;  // Output: Dequeued: 20
    q.displayQueue();  // Output: 30

    std::cout << "Dequeued: " << q.dequeue() << std::endl;  // Output: Dequeued: 30
    q.displayQueue();  // Output: Queue is empty

    return 0;
}



/*********************************Node.h***************************************/
#ifndef NODE_H
#define NODE_H

template <class Type>
class Node {
    // Declare CircLinkList as a friend class to allow access to private members
    template <class T>
    friend class CircLinkList;
    
  private:
    Type data;
    Node* next;

    // Constructor is private to be accessed only by friend classes
    Node(Type value) : data(value), next(nullptr) {}

  public:
    Type getData() const {
        return data;
    }

    Node* getNext() const {
        return next;
    }

    void setNext(Node* nextNode) {
        next = nextNode;
    }
};

#endif // NODE_H



/*****************************CircLinkList.h***********************************/
#ifndef CIRCLINKLIST_H
#define CIRCLINKLIST_H

#include "Node.h"
#include <stdexcept>
#include <iostream>

template <class Type>
class CircLinkList {
private:
    Node<Type>* tail;

public:
    CircLinkList();
    ~CircLinkList();
    
    bool isEmpty() const;
    void addNode(Type value);
    Type removeNode();
    void displayList() const;
};

#endif // CIRCLINKLIST_H



/****************************CircLinkList.cpp**********************************/
#include "CircLinkList.h"

template <class Type>
CircLinkList<Type>::CircLinkList() : tail(nullptr) {}

template <class Type>
CircLinkList<Type>::~CircLinkList() {
    while (!isEmpty()) {
        removeNode();
    }
}

template <class Type>
bool CircLinkList<Type>::isEmpty() const {
    return tail == nullptr;
}

template <class Type>
void CircLinkList<Type>::addNode(Type value) {
    Node<Type>* newNode = new Node<Type>(value);
    
    if (isEmpty()) {
        newNode->setNext(newNode);
        tail = newNode;
    } else {
        newNode->setNext(tail->getNext());
        tail->setNext(newNode);
        tail = newNode;
    }
}

template <class Type>
Type CircLinkList<Type>::removeNode() {
    if (isEmpty()) {
        throw std::out_of_range("Queue is empty");
    }

    Node<Type>* frontNode = tail->getNext();
    Type value = frontNode->getData();
    
    if (tail == frontNode) {
        tail = nullptr;
    } else {
        tail->setNext(frontNode->getNext());
    }

    delete frontNode;
    return value;
}

template <class Type>
void CircLinkList<Type>::displayList() const {
    if (isEmpty()) {
        std::cout << "Queue is empty" << std::endl;
        return;
    }

    Node<Type>* temp = tail->getNext();
    do {
        std::cout << temp->getData() << " ";
        temp = temp->getNext();
    } while (temp != tail->getNext());
    std::cout << std::endl;
}

// Explicit template instantiation
template class CircLinkList<int>;



/********************************Queue.h**************************************/
#ifndef QUEUE_H
#define QUEUE_H

#include "CircLinkList.h"

template <class Type>
class Queue {
private:
    CircLinkList<Type> list;

public:
    Queue();
    bool isEmpty() const;
    void enqueue(Type value);
    Type dequeue();
    void displayQueue() const;
};

#endif // QUEUE_H



/********************************Queue.cpp*************************************/
#include "Queue.h"

template <class Type>
Queue<Type>::Queue() {}

template <class Type>
bool Queue<Type>::isEmpty() const {
    return list.isEmpty();
}

template <class Type>
void Queue<Type>::enqueue(Type value) {
    list.addNode(value);
}

template <class Type>
Type Queue<Type>::dequeue() {
    return list.removeNode();
}

template <class Type>
void Queue<Type>::displayQueue() const {
    list.displayList();
}

// Explicit template instantiation
template class Queue<int>;
