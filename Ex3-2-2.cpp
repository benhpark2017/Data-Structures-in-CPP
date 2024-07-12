/**
 * Solutions to Chapter 3, Section 2, Exercise 2 of Horowitz's
 * Fundamentals of Data Structures in C++.
 */
 
/**************************************main.cpp****************************************/
#include "RailroadSwitcher.h"

int main() {
    int n;
    std::cout << "Enter the number of cars: ";
    std::cin >> n;

    RailroadSwitcher switcher(n);
    switcher.getAllPermutations();

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
    KeyType* stack;
    int MAX_SIZE;

public:
    Stack(int MAX_STACK_SIZE);
    ~Stack();
    
    bool isFull() const;
    bool isEmpty() const;
    void push(const KeyType& item);
    KeyType pop();
    KeyType peek() const;
    int size() const;
};

// Implementation

template <class KeyType>
Stack<KeyType>::Stack(int MAX_STACK_SIZE) : MAX_SIZE(MAX_STACK_SIZE), top(-1) {
    stack = new KeyType[MAX_SIZE];
}

template <class KeyType>
Stack<KeyType>::~Stack() {
    delete[] stack;
}

template <class KeyType>
bool Stack<KeyType>::isFull() const {
    return top == MAX_SIZE - 1;
}

template <class KeyType>
bool Stack<KeyType>::isEmpty() const {
    return top == -1;
}

template <class KeyType>
void Stack<KeyType>::push(const KeyType& item) {
    if (isFull()) {
        throw std::overflow_error("Stack is full");
    }
    stack[++top] = item;
}

template <class KeyType>
KeyType Stack<KeyType>::pop() {
    if (isEmpty()) {
        throw std::underflow_error("Stack is empty");
    }
    return stack[top--];
}

template <class KeyType>
KeyType Stack<KeyType>::peek() const {
    if (isEmpty()) {
        throw std::underflow_error("Stack is empty");
    }
    return stack[top];
}

template <class KeyType>
int Stack<KeyType>::size() const {
    return top + 1;
}

#endif



/********************************RailroadSwitcher.h**********************************/
// RailroadSwitcher.h
#ifndef RAILROAD_SWITCHER_H
#define RAILROAD_SWITCHER_H

#include "Stack.h"
#include <iostream>

class RailroadSwitcher {
private:
    Stack<int> vertical;
    int* current;
    int* remaining;
    int n;
    int currentSize;
    int remainingSize;

    void generatePermutations(int& count);
    void printPermutation() const;

public:
    RailroadSwitcher(int numCars);
    ~RailroadSwitcher();
    void getAllPermutations();
};

// Implementation

RailroadSwitcher::RailroadSwitcher(int numCars) 
    : vertical(numCars), n(numCars), currentSize(0), remainingSize(numCars) {
    current = new int[n];
    remaining = new int[n];
    for (int i = 0; i < n; ++i) {
        remaining[i] = n - i;
    }
}

RailroadSwitcher::~RailroadSwitcher() {
    delete[] current;
    delete[] remaining;
}

void RailroadSwitcher::printPermutation() const {
    for (int i = 0; i < currentSize; ++i) {
        std::cout << current[i] << " ";
    }
    std::cout << std::endl;
}

void RailroadSwitcher::generatePermutations(int& count) {
    if (currentSize == n) {
        printPermutation();
        count++;
        return;
    }

    // Move from remaining to vertical
    if (remainingSize > 0 && vertical.size() < n) {
        int car = remaining[remainingSize - 1];
        vertical.push(car);
        remainingSize--;
        generatePermutations(count);
        remainingSize++;
        remaining[remainingSize - 1] = vertical.pop();
    }

    // Move from vertical to current
    if (!vertical.isEmpty()) {
        int car = vertical.pop();
        current[currentSize++] = car;
        generatePermutations(count);
        currentSize--;
        vertical.push(current[currentSize]);
    }
}

void RailroadSwitcher::getAllPermutations() {
    int count = 0;
    std::cout << "Permutations:" << std::endl;
    generatePermutations(count);
    std::cout << "Number of possible permutations: " << count << std::endl;
}

#endif