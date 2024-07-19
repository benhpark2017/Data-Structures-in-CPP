/**
 * Solutions to Chapter 3, Section 7, Exercise 3 of Horowitz's
 * Fundamentals of Data Structures in C++.
 * 
 */

/********************************main.cpp************************************/
#include <iostream>
#include <stdexcept>
#include "MultipleStack.h"

// Helper function to print the status of the stack
template <typename Type>
void printStackStatus(const MultipleStack<Type>& ms) {
    ms.printArrayStatus();
    ms.printBottomStack();
    ms.printTopStack();
}

int main() {
    try {
        // Create a MultipleStack with 3 stacks and a total size of 15
        MultipleStack<int> ms(15, 3);

        // Add elements to the stacks
        std::cout << "Adding elements to the stacks:" << std::endl;
        for (int i = 0; i < 3; ++i) {
            for (int j = 1; j <= 4; ++j) {
                ms.Add(i, i*10 + j);
                std::cout << "Added " << i*10 + j << " to stack " << i << std::endl;
            }
        }

        // Print the current status of the stacks
        std::cout << "\nCurrent status of stacks after additions:" << std::endl;
        printStackStatus(ms);

        // Delete elements from the stacks
        std::cout << "\nDeleting elements from the stacks:" << std::endl;
        for (int i = 0; i < 3; ++i) {
            int x;
            ms.Delete(i, x);
            std::cout << "Deleted " << x << " from stack " << i << std::endl;
        }

        // Print the current status of the stacks
        std::cout << "\nCurrent status of stacks after deletion:" << std::endl;
        printStackStatus(ms);

        // Try to add more elements
        std::cout << "\nAdding more elements:" << std::endl;
        ms.Add(0, 100);
        ms.Add(1, 200);
        ms.Add(2, 300);
        std::cout << "Added 100 to stack 0, 200 to stack 1, and 300 to stack 2" << std::endl;

        // Print the current status of the stacks
        std::cout << "\nFinal status of stacks after adding more elements:" << std::endl;
        printStackStatus(ms);

        // Test stack full condition
        std::cout << "\nTesting stack full condition:" << std::endl;
        try {
            for (int i = 0; i < 10; ++i) {
                ms.Add(0, i + 50); // Adds 50 in the first iteration
            }
        } catch (const std::runtime_error& e) {
            std::cout << "Caught exception: " << e.what() << std::endl;
            // The above line should fail because stack 0 is now full.
        }

        // Print the current status of the stacks
        std::cout << "\nStatus of stacks after attempting to overfill stack 0:" << std::endl;
        printStackStatus(ms);

        // Test invalid stack index
        std::cout << "\nTesting invalid stack index:" << std::endl;
        try {
            ms.Add(3, 1000); // Invalid stack index
        } catch (const std::out_of_range& e) {
            std::cout << "Caught exception: " << e.what() << std::endl;
        }

    } catch (const std::exception& e) {
        std::cerr << "An error occurred: " << e.what() << std::endl;
        return 1;
    }

    // Cleanup static members
    MultipleStack<int>::CleanupStaticMembers();

    return 0;
}



/****************************MultipleStack.h*********************************/
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
    
    void StackFull(int i);
    void StackEmpty(int i);
    
public:
    MultipleStack(int totalSize, int numStacks);
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




/***************************MultipleStack.cpp********************************/
#ifndef MULTIPLESTACK_CPP
#define MULTIPLESTACK_CPP

#include "MultipleStack.h"

template <class Type>
Type* MultipleStack<Type>::M = nullptr;

template <class Type>
int MultipleStack<Type>::m = 0;

template <class Type>
MultipleStack<Type>::MultipleStack(int totalSize, int numStacks) : n(numStacks) {
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
    if (t[i] == b[i+1]) {
        StackFull(i);
    } else {
        M[++t[i]] = x;
    }
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
    x = M[t[i]--];
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
    throw std::runtime_error("Stack " + std::to_string(i) + " is full");
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

