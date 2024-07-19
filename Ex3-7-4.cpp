/**
 * Solutions to Chapter 3, Section 7, Exercise 4 of Horowitz's
 * Fundamentals of Data Structures in C++.
 * 
 */

/********************************main.cpp************************************/
#include <iostream>
#include <stdexcept>
#include "MultipleStack.h"

void printStackStatus(const MultipleStack<int>& ms, int stackIndex) {
    std::cout << "Stack " << stackIndex << " status:" << std::endl;
    try {
        std::vector<int> stackElements = ms.getStackStatus(stackIndex);
        for (int elem : stackElements) {
            std::cout << elem << " ";
        }
    } catch (const std::runtime_error&) {
        // Stack is empty
    }
    std::cout << std::endl;
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

        std::cout << "\nCurrent status of stacks:" << std::endl;
        for (int i = 0; i < 3; ++i) {
            printStackStatus(ms, i);
        }

        // Delete elements from the stacks
        std::cout << "\nDeleting elements from the stacks:" << std::endl;
        for (int i = 0; i < 3; ++i) {
            int x;
            ms.Delete(i, x);
            std::cout << "Deleted " << x << " from stack " << i << std::endl;
        }

        std::cout << "\nCurrent status of stacks after deletion:" << std::endl;
        for (int i = 0; i < 3; ++i) {
            printStackStatus(ms, i);
        }

        // Try to add more elements
        std::cout << "\nAdding more elements:" << std::endl;
        ms.Add(0, 100);
        ms.Add(1, 200);
        ms.Add(2, 300);
        std::cout << "Added 100 to stack 0, 200 to stack 1, and 300 to stack 2" << std::endl;

        std::cout << "\nFinal status of stacks:" << std::endl;
        for (int i = 0; i < 3; ++i) {
            printStackStatus(ms, i);
        }

        // Test stack full condition
        std::cout << "\nTesting stack full condition:" << std::endl;
        try {
            for (int i = 0; i < 3; ++i) {
                ms.Add(i, i + 50); //Adds 50 in the first iteration
                printStackStatus(ms, i);
            }
            ms.Add(0, 1000); //PROBLEM: Trying to add 100 to stack 0 but no error message appears.
            //AN ERROR MESSAGE SHOULD APPEAR IN THIS FUNCTION CALL.
        } catch (const std::runtime_error& e) {
            std::cout << "Caught exception: " << e.what() << std::endl;
            //The above line should fail because stack 0 is now full.
        }

        // Test invalid stack index
        std::cout << "\nTesting invalid stack index:" << std::endl;
        try {
            ms.Add(3, 1000); 
            /**Stack 3 is nonexistent; the compiler returns an error
             * message of "Invalid stack index."
             */
        } catch (const std::out_of_range& e) {
            std::cout << "Caught exception: " << e.what() << std::endl;
        }

    } catch (const std::exception& e) {
        std::cerr << "An error occurred: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}




/****************************MultipleStack.h*********************************/
#ifndef MULTIPLESTACK_H
#define MULTIPLESTACK_H

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
    MultipleStack(int totalSize, int numStacks);
    ~MultipleStack();

    void Add(const int i, const Type& x);
    Type* Delete(const int i, Type& x);
    std::vector<Type> getStackStatus(int i) const;

private:
    void StackFull(int i);
    void StackEmpty(int i);
};

#include "MultipleStack.cpp"

#endif // MULTIPLESTACK_H



/***************************MultipleStack.cpp********************************/
#ifndef MULTIPLESTACK_CPP
#define MULTIPLESTACK_CPP

#include "MultipleStack.h"
#include <vector>

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
void MultipleStack<Type>::Add(const int i, const Type& x) {
    if (i < 0 || i >= n) {
        throw std::out_of_range("Invalid stack index");
    }

    if (t[i] == b[i + 1]) {
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
std::vector<Type> MultipleStack<Type>::getStackStatus(int i) const {
    if (i < 0 || i >= n) {
        throw std::out_of_range("Invalid stack index");
    }

    std::vector<Type> stackElements;
    for (int j = b[i] + 1; j <= t[i]; ++j) {
        stackElements.push_back(M[j]);
    }
    return stackElements;
}

template <class Type>
void MultipleStack<Type>::StackFull(int i) {
    // Case 1: Look for space to the right
    for (int j = i + 1; j < n; ++j) {
        if (t[j] < b[j + 1]) {
            // Found space, move stacks to the right
            for (int k = j; k > i; --k) {
                t[k]++;
                M[t[k]] = M[t[k] - 1];
            }
            t[i]++;
            return;
        }
    }

    // Case 2: Look for space to the left
    for (int j = i - 1; j >= 0; --j) {
        if (t[j] < b[j + 1]) {
            // Found space, move stacks to the left
            for (int k = j + 1; k <= i; ++k) {
                t[k]--;
                M[t[k]] = M[t[k] + 1];
            }
            return;
        }
    }

    // Case 3: No space found
    throw std::runtime_error("All stacks are full. Cannot add more elements.");
}

template <class Type>
void MultipleStack<Type>::StackEmpty(int i) {
    throw std::runtime_error("Stack " + std::to_string(i) + " is empty");
}

#endif // MULTIPLESTACK_CPP
