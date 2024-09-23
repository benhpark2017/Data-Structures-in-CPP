/**
 * Solutions to Chapter 5, Section 2, Exercise 4 of Horowitz's
 * Fundamentals of Data Structures in C++.
 * 
 * Task 4: 
 * Write the C++ class definition for trees in the linked representation using
 * templates.
 */

/*********************************main.cpp*************************************/
#include "Tree.h"
#include <iostream>

int main() {
    Tree<char> completeTree;

    // Insert nodes in level order to match the complete tree structure in Figure 5.10
    completeTree.insert('A'); // Root
    completeTree.insert('B'); // Level 2 - left child of A
    completeTree.insert('C'); // Level 2 - right child of A
    completeTree.insert('D'); // Level 3 - left child of B
    completeTree.insert('E'); // Level 3 - right child of B
    completeTree.insert('F'); // Level 3 - left child of C
    completeTree.insert('G'); // Level 3 - right child of C
    completeTree.insert('H'); // Level 4 - left child of D
    completeTree.insert('I'); // Level 4 - right child of D

    // Display inorder traversal to confirm tree structure
    std::cout << "Inorder Traversal of the complete binary tree: ";
    completeTree.inorder();
    std::cout << std::endl;

    return 0;
}



/**********************************Tree.h**************************************/
#ifndef TREE_H
#define TREE_H

template <typename T>
class Tree;

template <typename T>
class TreeNode {
    friend class Tree<T>;  // Tree is a friend class of TreeNode
  private:
    TreeNode* leftChild;
    T data;               // Generic data type
    TreeNode* rightChild;

  public:
    // Constructor to initialize the node
    TreeNode(const T& data) : leftChild(nullptr), data(data), rightChild(nullptr) {}
};

template <typename T>
class Tree {
  public:
    Tree();                         // Constructor
    Tree(const Tree& other);        // Copy constructor
    ~Tree();                        // Destructor

    void insert(const T& data);     // Function to insert nodes
    void inorder() const;  // Inorder traversal for testing

  private:
    TreeNode<T>* root;

    // Helper functions for copying and deleting the tree
    void copyTree(TreeNode<T>*& thisRoot, TreeNode<T>* otherRoot);
    void deleteTree(TreeNode<T>* node);

    // Traversal helper
    void inorder(TreeNode<T>* node) const;
};

#endif //Tree.h



/*********************************Tree.cpp*************************************/
#include "Tree.h"
#include <iostream>
#include <queue>

// Constructor
template <typename T>
Tree<T>::Tree() : root(nullptr) {}

// Copy constructor
template <typename T>
Tree<T>::Tree(const Tree& other) : root(nullptr) {
    if (other.root != nullptr) {
        copyTree(root, other.root);
    }
}

// Destructor
template <typename T>
Tree<T>::~Tree() {
    deleteTree(root);
}

// Insert function for complete binary tree
template <typename T>
void Tree<T>::insert(const T& data) {
    TreeNode<T>* newNode = new TreeNode<T>(data);
    if (root == nullptr) {
        root = newNode;
        return;
    }

    std::queue<TreeNode<T>*> q;
    q.push(root);

    while (!q.empty()) {
        TreeNode<T>* temp = q.front();
        q.pop();

        if (temp->leftChild == nullptr) {
            temp->leftChild = newNode;
            return;
        } else {
            q.push(temp->leftChild);
        }

        if (temp->rightChild == nullptr) {
            temp->rightChild = newNode;
            return;
        } else {
            q.push(temp->rightChild);
        }
    }
}

// Inorder traversal function
template <typename T>
void Tree<T>::inorder() const {
    inorder(root);
}

// Helper function for inorder traversal
template <typename T>
void Tree<T>::inorder(TreeNode<T>* node) const {
    if (node) {
        inorder(node->leftChild);
        std::cout << node->data << " ";
        inorder(node->rightChild);
    }
}

// Copy helper function
template <typename T>
void Tree<T>::copyTree(TreeNode<T>*& thisRoot, TreeNode<T>* otherRoot) {
    if (otherRoot == nullptr) {
        thisRoot = nullptr;
    } else {
        thisRoot = new TreeNode<T>(otherRoot->data);
        copyTree(thisRoot->leftChild, otherRoot->leftChild);
        copyTree(thisRoot->rightChild, otherRoot->rightChild);
    }
}

// Delete helper function
template <typename T>
void Tree<T>::deleteTree(TreeNode<T>* node) {
    if (node != nullptr) {
        deleteTree(node->leftChild);
        deleteTree(node->rightChild);
        delete node;
    }
}

template class Tree<char>; // Explicit instantiation of class template Tree
