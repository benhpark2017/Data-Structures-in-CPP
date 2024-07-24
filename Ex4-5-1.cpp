/**
 * Solutions to Chapter 4, Section 5, Exercise 1 of Horowitz's
 * Fundamentals of Data Structure in C++.
 */

/*************************************main.cpp******************************************/

#include <iostream>
#include "Stack.h"

int main() {
    // Create a stack with a maximum size of 5
    Stack<int> stack(5);

    // Test isEmpty on an empty stack
    std::cout << "Is the stack empty? " << (stack.isEmpty() ? "Yes" : "No") << std::endl;

    // Push elements into the stack
    std::cout << "Pushing elements 1, 2, 3, 4, 5 into the stack." << std::endl;
    
    for (int i = 1; i <= 5; i++) {
        stack.Push(i);
    }

    // Test isFull on a full stack
    std::cout << "Is the stack full? " << (stack.isFull() ? "Yes" : "No") << std::endl;

    // Peek at the top element
    std::cout << "Top element is: " << stack.Peek() << std::endl;

    // Pop elements from the stack
    std::cout << "Popping elements from the stack:" << std::endl;
    while (!stack.isEmpty()) {
        std::cout << stack.Pop() << " ";
    }
    std::cout << std::endl;

    // Test isEmpty on an empty stack after popping all elements
    std::cout << "Is the stack empty? " << (stack.isEmpty() ? "Yes" : "No") << std::endl;

    // Test stack expansion by pushing more elements than initial capacity
    std::cout << "Pushing elements 6, 7, 8, 9, 10, 11 into the stack to test expansion." << std::endl;
    for (int j = 1; j <= 6; j++) {
        stack.Push(j + 5);
    }
    
    // Peek at the top element after expansion
    std::cout << "Top element is: " << stack.Peek() << std::endl;

    // Pop elements from the stack after expansion
    std::cout << "Popping elements from the stack after expansion:" << std::endl;
    while (!stack.isEmpty()) {
        std::cout << stack.Pop() << " ";
    }
    
    std::cout << std::endl;

    return 0;
}



/*************************************Stack.h******************************************/
#ifndef STACK_H
#define STACK_H

#include <stdexcept>

template <class KeyType>
class Stack {
    
  private:
    int top;
    KeyType *stack;
    int MAX_SIZE;
    
  public:
    Stack(int MAX_STACK_SIZE);
    bool isFull() const;
    void Push(const KeyType &item);
    bool isEmpty() const;
    KeyType Pop();
    KeyType Peek() const;
    void StackEmpty() const;
    void StackFull();
    
    ~Stack(); // Destructor to free allocated memory
};

#endif



/**************************************Stack.cpp***************************************/
#include "Stack.h"
#include <iostream>

template <class KeyType>
Stack<KeyType>::Stack(int MAX_STACK_SIZE) : MAX_SIZE(MAX_STACK_SIZE) {
    stack = new KeyType[MAX_SIZE];
    top = -1;
}

template <class KeyType>
Stack<KeyType>::~Stack() {
    delete[] stack;
}

template <class KeyType>
inline bool Stack<KeyType>::isFull() const {
    return top == MAX_SIZE - 1;
}

template <class KeyType>
inline bool Stack<KeyType>::isEmpty() const {
    return top == -1;
}

template <class KeyType>
void Stack<KeyType>::Push(const KeyType& x) {
    if (isFull()) {
        StackFull(); // Increase the size of the stack if it's full
        std::cout << "Stack is full. Stack expansion triggered." << std::endl;
    }
    stack[++top] = x;
}

template <class KeyType>
KeyType Stack<KeyType>::Pop() {
    if (isEmpty()) {
        StackEmpty(); // Signal an empty stack
        throw std::underflow_error("Stack is empty");
    }
    return stack[top--];
}

template <class KeyType>
KeyType Stack<KeyType>::Peek() const {
    if (isEmpty()) {
        StackEmpty(); // Signal an empty stack
        throw std::underflow_error("Stack is empty");
    }
    return stack[top];
}

template <class KeyType>
void Stack<KeyType>::StackFull() {
    // Allocate a new array with twice the size of the current stack
    int new_size = MAX_SIZE * 2;
    KeyType* new_stack = new KeyType[new_size];
    
    // Copy elements from the old stack to the new stack
    for (int i = 0; i <= top; ++i) {
        new_stack[i] = stack[i];
    }
    
    // Delete the old stack to free up the memory
    delete[] stack;
    
    // Update the stack pointer to point to the new stack
    stack = new_stack;
    
    // Update the MAX_SIZE to the new size
    MAX_SIZE = new_size;
}

template <class KeyType>
void Stack<KeyType>::StackEmpty() const {
    throw std::underflow_error("Stack is empty");
}

// Explicitly instantiate the template for int
template class Stack<int>;
