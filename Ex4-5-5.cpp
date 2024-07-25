/**
 * Solutions to Chapter 4, Section 5, Exercise 5 of Horowitz's
 * Fundamentals of Data Structures in C++.
 */

/*********************************main.cpp*************************************/
#include "LinkedStack.h"
#include <iostream>

int main() {
    LinkedStack<int> intLinkedStack;

    for (int i = 0; i < 5; i++)
        intLinkedStack.Push(i + 1);

    std::cout << "Stack output:" << std::endl;
    std::cout << intLinkedStack;

    std::cout << "Top: " << intLinkedStack.Peek() << std::endl;

    for (int i = 0; i < 5; i++)
        intLinkedStack.Pop();

    return 0;
}



/******************************LinkedStack.h**********************************/
/******************************LinkedStack.h**********************************/

#ifndef LINKEDSTACK_H
#define LINKEDSTACK_H

#include "StackNode.h"

template <class Type>
class LinkedStack {
private:
    StackNode<Type> *top;

public:
    LinkedStack() : top(nullptr) { }

    bool isEmpty() const { return top == nullptr; }

    void Push(const Type &item);

    void Pop();

    Type &Peek() const;

    template <class KeyType>
    friend ostream &operator<<(ostream &os, const LinkedStack<KeyType> &ls);
};

#include "LinkedStack.cpp"

#endif // LINKEDSTACK_H





/******************************LinkedStack.cpp*********************************/

#ifndef LINKEDSTACK_CPP
#define LINKEDSTACK_CPP

#include "LinkedStack.h"

template <class Type>
void LinkedStack<Type>::Push(const Type &item) {
    top = new StackNode<Type>(item, top);
    cout << item << " has been added" << endl;
}

template <class Type>
void LinkedStack<Type>::Pop() {
    if (isEmpty()) {
        cout << "Stack is empty, cannot pop" << endl;
        return;
    }
    StackNode<Type> *delNode = top;
    cout << top->data << " has been removed" << endl;
    top = top->link;
    delete delNode;
}

template <class Type>
Type &LinkedStack<Type>::Peek() const {
    return top->data; // Return top
}



template <class Type>
ostream &operator<<(ostream &os, const LinkedStack<Type> &ls) {
    StackNode<Type> *cur = ls.top; // To not affect Pop
    while (cur != nullptr) {
        os << cur->data;
        if (cur->link != nullptr) {
            os << "->";
        }
        cur = cur->link;
    }
    os << endl;
    return os;
}

//Explicit instantiation of the LinkedStack class template
template class LinkedStack<int>;

#endif //LINKEDSTACK_CPP


/*******************************StackNode.h***********************************/

#ifndef STACKNODE_H
#define STACKNODE_H

#include <iostream>
using namespace std;

template <class Type>
class LinkedStack;

template <class Type>
class StackNode {
    friend class LinkedStack<Type>;
private:
    Type data;
    StackNode<Type> *link;
    StackNode(Type element = 0, StackNode *next = nullptr) : data(element), link(next) { };

public:
    template <class KeyType>
    friend ostream &operator<<(ostream &os, const LinkedStack<KeyType> &ls);
};

#endif // STACKNODE_H
