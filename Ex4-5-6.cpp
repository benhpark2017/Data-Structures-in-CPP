/**
 * Solutions to Chapter 4, Section 5, Exercise 6 of Horowitz's
 * Fundamentals of Data Structures in C++.
 */

/*********************************main.cpp*************************************/
#include "LinkedQueue.h"
#include <iostream>

int main() {
    LinkedQueue<int> intLinkedQueue;

    for (int i = 0; i < 5; i++) {
        intLinkedQueue.Enqueue(i + 1);
    }

    std::cout << "Queue output" << std::endl;
    std::cout << intLinkedQueue;
    std::cout << "Front: " << intLinkedQueue.Front() << std::endl;
    std::cout << "Rear: " << intLinkedQueue.Rear() << std::endl;

    for (int i = 0; i < 5; i++) {
        intLinkedQueue.Dequeue();
    }

    return 0;
}



/******************************LinkedQueue.h***********************************/
#ifndef LINKEDQUEUE_H
#define LINKEDQUEUE_H

#include "QueueNode.h"
#include <iostream>

template <class Type>
class LinkedQueue {
private:
    QueueNode<Type>* front;
    QueueNode<Type>* rear;

public:
    LinkedQueue() : front(NULL), rear(NULL) {}

    bool isEmpty();

    Type& Front();

    Type& Rear();

    void Enqueue(const Type& item);

    void Dequeue();

    template <class KeyType>
    friend std::ostream& operator<<(std::ostream& os, LinkedQueue<KeyType>& lq);
};

#include "LinkedQueue.cpp"

#endif // LINKEDQUEUE_H




/*****************************LinkedQueue.cpp**********************************/
#ifndef LINKEDQUEUE_CPP
#define LINKEDQUEUE_CPP

#include "LinkedQueue.h"

template <class Type>
bool LinkedQueue<Type>::isEmpty() {
    return (front == NULL && rear == NULL);
}

template <class Type>
Type& LinkedQueue<Type>::Front() {
    return front->data;
}

template <class Type>
Type& LinkedQueue<Type>::Rear() {
    return rear->data;
}

template <class Type>
void LinkedQueue<Type>::Enqueue(const Type& item) {
    if (isEmpty()) {
        std::cout << item << " added" << std::endl;
        front = rear = new QueueNode<Type>(item, 0); // Empty queue
    } else {
        std::cout << item << " added" << std::endl;
        rear = rear->link = new QueueNode<Type>(item, 0); // Insert at rear
    }
}

template <class Type>
void LinkedQueue<Type>::Dequeue() {
    if (isEmpty()) {
        std::cout << "Queue is empty. Cannot dequeue further." << std::endl;
        return;
    }

    QueueNode<Type>* delNode = front;
    std::cout << front->data << " removed" << std::endl;
    front = front->link;
    delete delNode;
}

template <class KeyType>
std::ostream& operator<<(std::ostream& os, LinkedQueue<KeyType>& lq) {
    QueueNode<KeyType>* cur = lq.front;
    while (true) {
        if (cur == lq.rear) {
            os << cur->data << " ";
            break;
        }
        os << cur->data << "->";
        cur = cur->link;
    }
    os << std::endl;
    return os;
}


//Explicit instantiation for the LinkedQueue class template
template class LinkedQueue<int>;

// Explicit instantiation for int type, since it's used in main
template std::ostream& operator<<(std::ostream& os, LinkedQueue<int>& lq);

#endif //LINKEDQUEUE_CPP




/********************************QueueNode.h************************************/
#ifndef QUEUENODE_H
#define QUEUENODE_H

#include <iostream>

template <class Type>
class LinkedQueue;

template <class Type>
class QueueNode {
    friend class LinkedQueue<Type>;
    
  private:
    Type data;
    QueueNode<Type>* link;
    QueueNode(Type element = 0, QueueNode* next = NULL) : data(element), link(next) {}

  public:
    template <class KeyType>
    friend std::ostream& operator<<(std::ostream& os, LinkedQueue<KeyType>& lq);
};

#endif // QUEUENODE_H
