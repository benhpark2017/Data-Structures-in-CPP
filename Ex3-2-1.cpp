/**
 * Solutions to Chapter 3, Section 2, Exercise 1 of Horowitz's
 * Fundamentals of Data Structures in C++.
 */
 
/**************************************main.cpp****************************************/
#include <iostream>
#include "Stack.h"

int main() {
    std::cout << "Hello World" << std::endl;

    Stack<int> stack(5); // Create a stack with an initial size of 5

    try {
        // Test adding elements
        for (int i = 1; i <= 7; ++i) { // Add 7 elements, which will trigger resizing
            stack.Add(i);
            std::cout << "Added: " << i << std::endl;
        }

        // Test deleting elements
        int value;
        while (!stack.isEmpty()) {
            stack.Delete(value);
            std::cout << "Deleted: " << value << std::endl;
        }

        // Attempt to delete from an empty stack to trigger exception
        stack.Delete(value);

    } catch (const std::underflow_error& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}



/***************************************Stack.h****************************************/
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
    bool isFull();
    void Add(const KeyType &item);
    bool isEmpty();
    KeyType* Delete(KeyType&);
    void StackEmpty();
    void StackFull();
    
    ~Stack(); // Destructor to free allocated memory
};

#endif



/**************************************Stack.cpp***************************************/
#include "Stack.h"

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
inline bool Stack<KeyType>::isFull() {
    return top == MAX_SIZE - 1;
}

template <class KeyType>
inline bool Stack<KeyType>::isEmpty() {
    return top == -1;
}

template <class KeyType>
void Stack<KeyType>::Add(const KeyType& x) {
    if (isFull()) {
        StackFull(); // Increase the size of the stack if it's full
    }
    stack[++top] = x;
}

template <class KeyType>
KeyType* Stack<KeyType>::Delete(KeyType& x) {
    if (isEmpty()) {
        StackEmpty(); // Signal an empty stack
        return nullptr;
    }
    x = stack[top--];
    return &x;
}

//Solution to Chapter 3, Section 2, Exercise 1
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
void Stack<KeyType>::StackEmpty() {
    throw std::underflow_error("Stack is empty");
}

//Explicitly instantiate the template
template class Stack<int>;