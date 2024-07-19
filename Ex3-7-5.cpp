/**
 * Solutions to Chapter 3, Section 7, Exercise 5 of Horowitz's
 * Fundamentals of Data Structures in C++.
 * 
 */

/********************************main.cpp************************************/
#include <iostream>
#include <stdexcept>
#include "MultipleStack.h"

int main() {
    try {
        MultipleStack<int> ms(10, 2);

        // Fill both stacks
        std::cout << "Filling both stacks:" << std::endl;
        for (int i = 0; i < 5; ++i) {
            ms.Add(0, i); // Stack 0: 0 1 2 3 4
            ms.Add(1, i + 5); // Stack 1: 5 6 7 8 9
        }
        ms.printStackStatus();

        // Remove one element from stack 0 to create space
        std::cout << "\nRemoving one element from stack 0:" << std::endl;
        int temp;
        ms.Delete(0, temp); // Stack 0: 0 1 2 3
        ms.printStackStatus();

        // Add element to stack 1 causing right shift
        std::cout << "\nAdding element to stack 1 (causing right shift):" << std::endl;
        ms.Add(1, 10); // Stack 1: 5 6 7 8 9 10 (shifted to the right)
        ms.printStackStatus();

    } catch (const std::exception& e) {
        std::cerr << "An error occurred: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}




/****************************MultipleStack.h*********************************/
#ifndef MULTIPLESTACK_H
#define MULTIPLESTACK_H

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

template <class Type>
class MultipleStack {
private:
    Type* M;  // Available memory
    int* b;   // Bottom pointers
    int* t;   // Top pointers
    int n;    // Number of stacks
    int m;    // Total size of memory

public:
    MultipleStack(int totalSize, int numStacks); // May be causing problems
    ~MultipleStack();

    void printStackStatus() const;
    void Add(const int i, const Type& x); // Please do not change this function
    Type* Delete(const int i, Type& x); // Please do not change this function

private:
    void StackFull(int i); //May be causing problems
    void StackEmpty(int i);
};

#include "MultipleStack.cpp"
#endif // MULTIPLESTACK_H




/***************************MultipleStack.cpp********************************/
#ifndef MULTIPLESTACK_CPP
#define MULTIPLESTACK_CPP

#include "MultipleStack.h"

template <class Type>
MultipleStack<Type>::MultipleStack(int totalSize, int numStacks) : n(numStacks), m(totalSize) {
    if (n <= 0 || m <= 0) {
        throw std::invalid_argument("Invalid stack or memory size");
    }

    M = new Type[m];
    b = new int[n + 1];
    t = new int[n];

    int segmentSize = m / n;
    for (int i = 0; i < n; ++i) {
        b[i] = t[i] = segmentSize * i - 1;
    }
    b[n] = m - 1;  // Set the boundary for the last stack
}

template <class Type>
MultipleStack<Type>::~MultipleStack() {
    delete[] M;
    delete[] b;
    delete[] t;
}

template <class Type>
void MultipleStack<Type>::printStackStatus() const {
    std::cout << "Memory contents: ";
    for (int i = 0; i < m; ++i) {
        std::cout << M[i] << " ";
    }
    std::cout << std::endl;
    std::cout << "Bottom pointers: ";
    for (int i = 0; i <= n; ++i) {
        std::cout << b[i] << " ";
    }
    std::cout << std::endl;
    std::cout << "Top pointers: ";
    for (int i = 0; i < n; ++i) {
        std::cout << t[i] << " ";
    }
    std::cout << std::endl;
}

template <class Type>
void MultipleStack<Type>::Add(const int i, const Type& x) {
    std::cout << "Adding " << x << " to stack " << i << std::endl;
    if (i < 0 || i >= n) {
        throw std::out_of_range("Invalid stack index");
    }

    if (t[i] == b[i + 1]) {
        StackFull(i);
    }
    M[++t[i]] = x;
}

template <class Type>
Type* MultipleStack<Type>::Delete(const int i, Type& x) {
    if (i < 0 || i >= n) {
        throw std::out_of_range("Invalid stack index");
    }

    if (t[i] == b[i]) {
        StackEmpty(i);
        return nullptr;
    }
    x = M[t[i]];
    M[t[i]--] = 0;
    return &x;
}

template <class Type>
void MultipleStack<Type>::StackFull(int i) {
    // Check if there's space at the end of the memory
    if (t[n - 1] < m - 1) {
        // Shift all elements in stack i and to its right by one position
        for (int k = n - 1; k > i; k--) {
            for (int p = t[k]; p > b[k]; p--) {
                M[p + 1] = M[p];
            }
            t[k]++;
            b[k + 1]++;
        }
        b[i + 1]++;
    }
    // Check if there's space at the beginning of the memory
    else if (b[0] < 0) {
        // Shift all elements in stacks 1 to i to the left by one position
        for (int k = 1; k <= i; k++) {
            for (int p = b[k]; p <= t[k]; p++) {
                M[p] = M[p + 1];
            }
            b[k]--;
            t[k]--;
        }
    }
    else {
        // If we can't shift in either direction, the stack is truly full
        throw std::runtime_error("Stack " + std::to_string(i) + " is full");
    }
}

template <class Type>
void MultipleStack<Type>::StackEmpty(int i) {
    throw std::runtime_error("Stack " + std::to_string(i) + " is empty");
}

#endif // MULTIPLESTACK_CPP
