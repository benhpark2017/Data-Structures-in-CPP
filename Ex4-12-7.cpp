/**
 * Solutions to Chapter 4, Section 12, Exercise 7 of Horowitz's
 * Fundamentals of Data Structures in C++.
 * 
 * Task 7: 
 * Implement heterogeneous linked stacks and queues so that these data structures can
 * contain objects of type char, int, and float.
 */

/**************************************main.cpp*****************************************/
#include "HeterogeneousStack.h"
#include "HeterogeneousQueue.h"
#include <iostream>

int main() {
    HeterogeneousStack stack;
    stack.pushChar('A');
    stack.pushInt(42);
    stack.pushFloat(3.1416f);
    
    std::cout << "Stack contents:" << std::endl;
    stack.printStack();

    HeterogeneousQueue queue;
    queue.enqueueChar('B');
    queue.enqueueInt(100);
    queue.enqueueFloat(2.7183f);

    std::cout << "\nQueue contents:" << std::endl;
    queue.printQueue();

    return 0;
}



/******************************HeterogeneousStack.h**********************************/
#ifndef HETEROGENEOUS_STACK_H
#define HETEROGENEOUS_STACK_H

class HeterogeneousStack {
private:
    struct Node;
    Node* topNode;

public:
    HeterogeneousStack();
    ~HeterogeneousStack();

    void pushChar(char c);
    void pushInt(int i);
    void pushFloat(float f);
    void pop();
    void peek() const;
    bool isEmpty() const;
    void printStack() const;  // New function
};

#endif // HETEROGENEOUS_STACK_H


/*****************************HeterogeneousStack.cpp*********************************/
#include "HeterogeneousStack.h"
#include <iostream>

struct HeterogeneousStack::Node {
    enum class Type { CHAR, INT, FLOAT } type;
    union Value {
        char c;
        int i;
        float f;
        Value() {}
        ~Value() {}
    } value;
    Node* next;

    Node(char c) : type(Type::CHAR), next(nullptr) { value.c = c; }
    Node(int i) : type(Type::INT), next(nullptr) { value.i = i; }
    Node(float f) : type(Type::FLOAT), next(nullptr) { value.f = f; }
};

HeterogeneousStack::HeterogeneousStack() : topNode(nullptr) {}

HeterogeneousStack::~HeterogeneousStack() {
    while (!isEmpty()) {
        pop();
    }
}

void HeterogeneousStack::pushChar(char c) {
    Node* newNode = new Node(c);
    newNode->next = topNode;
    topNode = newNode;
}

void HeterogeneousStack::pushInt(int i) {
    Node* newNode = new Node(i);
    newNode->next = topNode;
    topNode = newNode;
}

void HeterogeneousStack::pushFloat(float f) {
    Node* newNode = new Node(f);
    newNode->next = topNode;
    topNode = newNode;
}

void HeterogeneousStack::pop() {
    if (isEmpty()) {
        std::cout << "Stack is empty" << std::endl;
        return;
    }
    Node* temp = topNode;
    topNode = topNode->next;
    delete temp;
}

void HeterogeneousStack::peek() const {
    if (isEmpty()) {
        std::cout << "Stack is empty" << std::endl;
        return;
    }
    switch (topNode->type) {
        case Node::Type::CHAR:
            std::cout << "Char: " << topNode->value.c << std::endl;
            break;
        case Node::Type::INT:
            std::cout << "Int: " << topNode->value.i << std::endl;
            break;
        case Node::Type::FLOAT:
            std::cout << "Float: " << topNode->value.f << std::endl;
            break;
    }
}

bool HeterogeneousStack::isEmpty() const {
    return topNode == nullptr;
}

void HeterogeneousStack::printStack() const {
    if (isEmpty()) {
        std::cout << "Stack is empty" << std::endl;
        return;
    }
    
    std::cout << "Stack contents (top to bottom):" << std::endl;
    Node* current = topNode;
    while (current != nullptr) {
        switch (current->type) {
            case Node::Type::CHAR:
                std::cout << "Char: " << current->value.c << std::endl;
                break;
            case Node::Type::INT:
                std::cout << "Int: " << current->value.i << std::endl;
                break;
            case Node::Type::FLOAT:
                std::cout << "Float: " << current->value.f << std::endl;
                break;
        }
        current = current->next;
    }
    std::cout << "End of stack" << std::endl;
}



/******************************HeterogeneousQueue.h**********************************/
#ifndef HETEROGENEOUS_QUEUE_H
#define HETEROGENEOUS_QUEUE_H

class HeterogeneousQueue {
private:
    struct Node;
    Node* frontNode;
    Node* rearNode;

public:
    HeterogeneousQueue();
    ~HeterogeneousQueue();

    void enqueueChar(char c);
    void enqueueInt(int i);
    void enqueueFloat(float f);
    void dequeue();
    void peek() const;
    bool isEmpty() const;
    void printQueue() const;
};

#endif // HeterogeneousQueue.h


/*****************************HeterogeneousQueue.cpp*********************************/
#include "HeterogeneousQueue.h"
#include <iostream>

struct HeterogeneousQueue::Node {
    enum class Type { CHAR, INT, FLOAT } type;
    union Value {
        char c;
        int i;
        float f;
        Value() {}
        ~Value() {}
    } value;
    Node* next;

    Node(char c) : type(Type::CHAR), next(nullptr) { value.c = c; }
    Node(int i) : type(Type::INT), next(nullptr) { value.i = i; }
    Node(float f) : type(Type::FLOAT), next(nullptr) { value.f = f; }
};

HeterogeneousQueue::HeterogeneousQueue() : frontNode(nullptr), rearNode(nullptr) {}

HeterogeneousQueue::~HeterogeneousQueue() {
    while (!isEmpty()) {
        dequeue();
    }
}

void HeterogeneousQueue::enqueueChar(char c) {
    Node* newNode = new Node(c);
    if (isEmpty()) {
        frontNode = rearNode = newNode;
    } else {
        rearNode->next = newNode;
        rearNode = newNode;
    }
}

void HeterogeneousQueue::enqueueInt(int i) {
    Node* newNode = new Node(i);
    if (isEmpty()) {
        frontNode = rearNode = newNode;
    } else {
        rearNode->next = newNode;
        rearNode = newNode;
    }
}

void HeterogeneousQueue::enqueueFloat(float f) {
    Node* newNode = new Node(f);
    if (isEmpty()) {
        frontNode = rearNode = newNode;
    } else {
        rearNode->next = newNode;
        rearNode = newNode;
    }
}

void HeterogeneousQueue::dequeue() {
    if (isEmpty()) {
        std::cout << "Queue is empty" << std::endl;
        return;
    }
    Node* temp = frontNode;
    frontNode = frontNode->next;
    if (frontNode == nullptr) {
        rearNode = nullptr;
    }
    delete temp;
}

void HeterogeneousQueue::peek() const {
    if (isEmpty()) {
        std::cout << "Queue is empty" << std::endl;
        return;
    }
    switch (frontNode->type) {
        case Node::Type::CHAR:
            std::cout << "Char: " << frontNode->value.c << std::endl;
            break;
        case Node::Type::INT:
            std::cout << "Int: " << frontNode->value.i << std::endl;
            break;
        case Node::Type::FLOAT:
            std::cout << "Float: " << frontNode->value.f << std::endl;
            break;
    }
}

bool HeterogeneousQueue::isEmpty() const {
    return frontNode == nullptr;
}

void HeterogeneousQueue::printQueue() const {
    if (isEmpty()) {
        std::cout << "Queue is empty" << std::endl;
        return;
    }
    
    std::cout << "Queue contents (front to rear):" << std::endl;
    Node* current = frontNode;
    while (current != nullptr) {
        switch (current->type) {
            case Node::Type::CHAR:
                std::cout << "Char: " << current->value.c << std::endl;
                break;
            case Node::Type::INT:
                std::cout << "Int: " << current->value.i << std::endl;
                break;
            case Node::Type::FLOAT:
                std::cout << "Float: " << current->value.f << std::endl;
                break;
        }
        current = current->next;
    }
    std::cout << "End of queue" << std::endl;
}
