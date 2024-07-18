/**
 * Solutions to Chapter 3, Section 7, Exercise 1 of Horowitz's
 * Fundamentals of Data Structures in C++.
 * 
 */

/********************************main.cpp************************************/
#include <iostream>
#include "TwoStacks.h"

int main() {
    TwoStacks<int> stacks(10);  // Create two stacks with a total capacity of 10 elements

    try {
        // Add elements to Stack 0
        stacks.Add(0, 5);
        stacks.Add(0, 10);
        stacks.Add(0, 15);

        // Add elements to Stack 1
        stacks.Add(1, 100);
        stacks.Add(1, 200);

        std::cout << "After adding elements:" << std::endl;
        stacks.DisplayAll();

        // Delete an element from each stack
        int deleted0 = stacks.Delete(0);
        int deleted1 = stacks.Delete(1);

        std::cout << "\nDeleted from Stack 0: " << deleted0 << std::endl;
        std::cout << "Deleted from Stack 1: " << deleted1 << std::endl;

        std::cout << "\nAfter deleting elements:" << std::endl;
        stacks.DisplayAll();

        // Try to add more elements
        stacks.Add(0, 20);
        stacks.Add(1, 300);

        std::cout << "\nAfter adding more elements:" << std::endl;
        stacks.DisplayAll();

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}



/******************************TwoStacks.h***********************************/
#ifndef TWOSTACKS_H
#define TWOSTACKS_H

#include <iostream>
#include <stdexcept>

template <class KeyType>
class TwoStacks {
  private:
    KeyType* M;           // The array to hold both stacks
    int m;          // Total size of the array
    int top0;       // Top of stack 0 (grows from left)
    int top1;       // Top of stack 1 (grows from right)

  public:
    TwoStacks(int size);
    ~TwoStacks();

    void Add(int stackNum, const KeyType& x);
    KeyType Delete(int stackNum);
    void DisplayAll() const;
};

#include "TwoStacks.cpp" // Include the template implementation file

#endif // TWOSTACKS_H





/*****************************TwoStacks.cpp***********************************/
#ifndef TWOSTACKS_CPP
#define TWOSTACKS_CPP
#include "TwoStacks.h"

template <class KeyType>
TwoStacks<KeyType>::TwoStacks(int size) : m(size), top0(-1), top1(size) {
    M = new KeyType[size];
}

template <class KeyType>
TwoStacks<KeyType>::~TwoStacks() {
    delete[] M;
}

template <class KeyType>
void TwoStacks<KeyType>::Add(int stackNum, const KeyType& x) {
    if (top0 + 1 == top1) {
        throw std::runtime_error("Stacks are full");
    }

    if (stackNum == 0) {
        M[++top0] = x;
    } else if (stackNum == 1) {
        M[--top1] = x;
    } else {
        throw std::invalid_argument("Invalid stack number");
    }
}

template <class KeyType>
KeyType TwoStacks<KeyType>::Delete(int stackNum) {
    if (stackNum == 0) {
        if (top0 == -1) {
            throw std::runtime_error("Stack 0 is empty");
        }
        return M[top0--];
    } else if (stackNum == 1) {
        if (top1 == m) {
            throw std::runtime_error("Stack 1 is empty");
        }
        return M[top1++];
    } else {
        throw std::invalid_argument("Invalid stack number");
    }
}

template <class KeyType>
void TwoStacks<KeyType>::DisplayAll() const {
    std::cout << "Array M contents:" << std::endl;
    for (int i = 0; i < m; ++i) {
        if (i <= top0 || i >= top1) {
            std::cout << "[" << i << "]: " << M[i];
            if (i <= top0) std::cout << " (Stack 0)";
            if (i >= top1) std::cout << " (Stack 1)";
            std::cout << std::endl;
        } else {
            std::cout << "[" << i << "]: Empty" << std::endl;
        }
    }
    std::cout << "Top of Stack 0: " << top0 << std::endl;
    std::cout << "Top of Stack 1: " << top1 << std::endl;
}

//Explicit instantiation of class template TwoStacks
template class TwoStacks<int>;

#endif