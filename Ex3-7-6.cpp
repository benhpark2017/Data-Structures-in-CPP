/**
 * Solutions to Chapter 3, Section 7, Exercise 6 of Horowitz's
 * Fundamentals of Data Structures in C++.
 * 
 */

/**********************************main.cpp****************************************/
#include <iostream>
#include <stdexcept>
#include "MultipleStack.h"

int main() {
    try {
        int c;
        std::cout << "Please specify the number of free spaces:" << std::endl;
        std::cin >> c;
        MultipleStack<int> M(10, 2, c);

        // Fill both stacks
        std::cout << "Filling both stacks:" << std::endl;
        for (int i = 0; i < 5; ++i) {
            M.Add(0, i); // Stack 0: 0 1 2 3 4
        }
        M.printArrayStatus();
        
        std::cout << "Filling both stacks:" << std::endl;
        for (int i = 0; i < 5; ++i) {
            M.Add(1, i + 5); // Stack 1: 5 6 7 8 9
        }
        M.printArrayStatus();

        // Remove one element from stack 0 to create space
        std::cout << "\nRemoving one element from stack 0:" << std::endl;
        int temp;
        M.Delete(0, temp); // Stack 0: 0 1 2 3
        M.printArrayStatus();

        // Add element to stack 1 causing right shift
        std::cout << "\nAdding element to stack 1 (causing right shift):" << std::endl;
        M.Add(1, 10); // Stack 1: 5 6 7 8 9 10 (shifted to the right)
        M.printArrayStatus();

    } catch (const std::exception& e) {
        std::cerr << "An error occurred: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}



/*******************************MultipleStack.h************************************/
#ifndef MULTIPLESTACK_H
#define MULTIPLESTACK_H

#include <iostream>
#include <cmath>
#include <stdexcept>

template <class Type>
class MultipleStack {
private:
    static Type* M;  // Static array to store all stacks
    int* b;          // Array to store bottom indices
    int* t;          // Array to store top indices
    static int m;    // Total size of memory
    int n;           // Number of stacks
    int c;
    
    void StackFull(int i);
    void StackEmpty(int i);
    
public:
    MultipleStack(int totalSize, int numStacks, int minSpaces);
    ~MultipleStack();

    void Add(const int i, const Type& x);
    Type* Delete(const int i, Type& x);

    // Print functions
    void printArrayStatus() const;
    void printBottomStack() const;
    void printTopStack() const;

    static void CleanupStaticMembers();
};

// Include template definitions
#include "MultipleStack.cpp"

#endif // MULTIPLESTACK_H



/******************************MultipleStack.cpp***********************************/
#ifndef MULTIPLESTACK_CPP
#define MULTIPLESTACK_CPP

#include "MultipleStack.h"

template <class Type>
Type* MultipleStack<Type>::M = nullptr;

template <class Type>
int MultipleStack<Type>::m = 0;

template <class Type>
MultipleStack<Type>::MultipleStack(int totalSize, int numStacks, int minSpaces) 
    : n(numStacks), c(minSpaces) {
    if (n <= 0 || totalSize <= 0) {
        throw std::invalid_argument("Invalid stack configuration");
    }

    if (M == nullptr) {
        m = totalSize;
        M = new Type[m];
    } else if (totalSize != m) {
        throw std::invalid_argument("Total size must match the existing static array size");
    }

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
    delete[] b;
    delete[] t;
}

template <class Type>
void MultipleStack<Type>::Add(const int i, const Type& x) {
    if (i < 0 || i >= n) {
        throw std::out_of_range("Invalid stack index");
    }

    // Check if adding this element would leave fewer than c spaces
    int totalOccupiedSpaces = 0;
    for (int j = 0; j < n; ++j) {
        totalOccupiedSpaces += t[j] - b[j];
    }

    if (m - totalOccupiedSpaces <= c) {
        throw std::runtime_error("Stack is full. Fewer than " + std::to_string(c) + " spaces available.");
    }

    if (t[i] == b[i+1]) {
        StackFull(i);
    }
    t[i]++;
    M[t[i]] = x;
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
void MultipleStack<Type>::CleanupStaticMembers() {
    delete[] M;
    M = nullptr;
    m = 0;
}

template <class Type>
void MultipleStack<Type>::StackFull(int i) {
    int totalOccupiedSpaces = 0;
    for (int j = 0; j < n; ++j) {
        totalOccupiedSpaces += t[j] - b[j];
    }

    int availableSpaces = m - totalOccupiedSpaces;

    // Check if available spaces are less than c
    if (availableSpaces < c) {
        throw std::runtime_error("Stack is full. Fewer than " + std::to_string(c) + " spaces available.");
    }

    // Case 1: Look for space to the right
    for (int j = i + 1; j < n; ++j) {
        if (t[j] < b[j+1]) {
            // Found space, move stacks to the right
            for (int k = t[j]; k > t[i]; k--) {
                M[k+1] = M[k];
            }
            for (int k = j; k > i; k--) {
                t[k]++;
            }
            t[i]++;
            return;
        }
    }

    // Case 2: Look for space to the left
    for (int j = i - 1; j >= 0; --j) {
        if (t[j] < b[j+1]) {
            // Found space, move stacks to the left
            for (int k = b[j+1] + 1; k <= t[i]; k++) {
                M[k-1] = M[k];
            }
            for (int k = j + 1; k <= i; k++) {
                b[k]--;
                t[k]--;
            }
            return;
        }
    }

    // This point should not be reached due to the earlier check,
    // but we'll keep it as a fallback
    throw std::runtime_error("All stacks are full. No space available.");
}

template <class Type>
void MultipleStack<Type>::StackEmpty(int i) {
    throw std::runtime_error("Stack " + std::to_string(i) + " is empty");
}

template <class Type>
void MultipleStack<Type>::printArrayStatus() const {
    std::cout << "Array M: ";
    for (int i = 0; i < m; ++i) {
        std::cout << M[i] << " ";
    }
    std::cout << std::endl;
}

template <class Type>
void MultipleStack<Type>::printBottomStack() const {
    std::cout << "Bottom indices (b): ";
    for (int i = 0; i <= n; ++i) {
        std::cout << b[i] << " ";
    }
    std::cout << std::endl;
}

template <class Type>
void MultipleStack<Type>::printTopStack() const {
    std::cout << "Top indices (t): ";
    for (int i = 0; i < n; ++i) {
        std::cout << t[i] << " ";
    }
    std::cout << std::endl;
}

#endif //MULTIPLESTACK_CPP
