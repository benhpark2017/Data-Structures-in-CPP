/**
 * Solutions to Chapter 5, Section 7, Exercise 2 of Horowitz's
 * Fundamentals of Data Structures in C++.
 * 
 * Task 2: 
 * Write a program to start with an initially empty binary search tree and make
 * n random insertions. Use a uniform random number generator to obtain the values
 * to be inserted. Measure the height of the resulting binary search tree and
 * divide this height by log_2(n). Do this for 100, 500, 1000, 2000, 3000, ...,
 * 10000. Plot the ratio height / log_2(n) as a function of n. The ratio should
 * be approximately constant (around 2). Verify that this is so.
 */
/***********************************main.cpp***************************************/
#include <iostream>
#include <random>
#include <cmath>
#include "BST.h"

int main() {
    std::mt19937 mt(12345);  // Mersenne Twister RNG with a seed for reproducibility
    std::uniform_int_distribution<int> dist(1, 100000);  // Uniform distribution for random numbers
    std::cout << "n\tHeight/log2(n)\n";

    for (int n = 100; n <= 10500; n += 500) {
        BST<int> bst;
        
        // Insert n random values into the binary search tree
        for (int i = 0; i < n; ++i) {
            int value = dist(mt);
            bst.insert(value);
        }

        // Get the height of the binary search tree
        int height = bst.getHeight();
        double heightByLogN = height / std::log2(n);

        // Output the result
        std::cout << n << "\t" << heightByLogN << "\n";

        // Clear the tree for the next iteration
        bst.clear(bst.getRoot());
    }

    std::cout << "End of program." << std::endl;
    return 0;
}


/************************************BST.h****************************************/
#ifndef BST_H
#define BST_H

template <typename Type>
class BstNode {
public:
    Type key;
    BstNode* left;
    BstNode* right;
    BstNode(Type key);
};

template <typename Type>
class BST {
private:
    BstNode<Type>* root;

public:
    BST();
    void insert(Type key);
    int getHeight();
    void clear(BstNode<Type>* node);
    BstNode<Type>* getRoot() { return root; }

private:
    BstNode<Type>* insert(BstNode<Type>* node, Type key);
    int getHeight(BstNode<Type>* node);
};

#include "BST.tpp"
#endif // BST_H



/***********************************BST.cpp***************************************/
#include <algorithm>

template <typename Type>
BstNode<Type>::BstNode(Type key) : key(key), left(nullptr), right(nullptr) {}

template <typename Type>
BST<Type>::BST() : root(nullptr) {}

template <typename Type>
void BST<Type>::insert(Type key) {
    root = insert(root, key);
}

template <typename Type>
BstNode<Type>* BST<Type>::insert(BstNode<Type>* node, Type key) {
    if (!node) return new BstNode<Type>(key);
    if (key < node->key)
        node->left = insert(node->left, key);
    else if (key > node->key)
        node->right = insert(node->right, key);
    return node;
}

template <typename Type>
int BST<Type>::getHeight() {
    return getHeight(root);
}

template <typename Type>
int BST<Type>::getHeight(BstNode<Type>* node) {
    if (!node) return 0;
    return 1 + std::max(getHeight(node->left), getHeight(node->right));
}

template <typename Type>
void BST<Type>::clear(BstNode<Type>* node) {
    if (!node) return;
    clear(node->left);
    clear(node->right);
    delete node;
}
