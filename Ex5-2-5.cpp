/**
 * Solutions to Chapter 5, Section 2, Exercise 5 of Horowitz's
 * Fundamentals of Data Structures in C++.
 * 
 * Task 5: 
 * Write the C++ class definition for trees in the linked representation using
 * nested classes.
 */

/*********************************main.cpp*************************************/
#include "Tree.h"
#include <iostream>

int main() {
    Tree completeTree;

    // Insert nodes to match the complete tree structure in Figure 5.10
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
    std::cout << "In-order traversal of the complete binary tree: ";
    completeTree.inorder();
    std::cout << std::endl;

    return 0;
}



/**********************************Tree.h**************************************/
#ifndef TREE_H
#define TREE_H

#include <iostream>
#include <queue>

class Tree {
  private:
    class TreeNode {
      public:
        char data;  // Assuming tree holds `char` type data
        TreeNode* leftChild;
        TreeNode* rightChild;

        // Constructor
        TreeNode(char data) : data(data), leftChild(nullptr), rightChild(nullptr) {}
    };

    TreeNode* root;  // Root of the tree

    // Helper functions
    void inorder(TreeNode* node) const;
    void copyTree(TreeNode*& thisRoot, TreeNode* otherRoot);
    void deleteTree(TreeNode* node);

  public:
    Tree();                         // Constructor
    Tree(const Tree& other);        // Copy constructor
    ~Tree();                        // Destructor

    void insert(char data);         // Insert nodes
    void inorder() const;           // Inorder traversal
};

#endif // TREE_H



/*********************************Tree.cpp*************************************/
#include "Tree.h"

// Constructor
Tree::Tree() : root(nullptr) {}

// Copy constructor
Tree::Tree(const Tree& other) : root(nullptr) {
    if (other.root != nullptr) {
        copyTree(root, other.root);
    }
}

// Destructor
Tree::~Tree() {
    deleteTree(root);
}

// Insert function for complete binary tree
void Tree::insert(char data) {
    TreeNode* newNode = new TreeNode(data);
    if (root == nullptr) {
        root = newNode;
        return;
    }

    std::queue<TreeNode*> q;
    q.push(root);

    while (!q.empty()) {
        TreeNode* temp = q.front();
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
void Tree::inorder() const {
    inorder(root);
}

// Helper function for inorder traversal
void Tree::inorder(TreeNode* node) const {
    if (node) {
        inorder(node->leftChild);
        std::cout << node->data << " ";
        inorder(node->rightChild);
    }
}

// Copy helper function
void Tree::copyTree(TreeNode*& thisRoot, TreeNode* otherRoot) {
    if (otherRoot == nullptr) {
        thisRoot = nullptr;
    } else {
        thisRoot = new TreeNode(otherRoot->data);
        copyTree(thisRoot->leftChild, otherRoot->leftChild);
        copyTree(thisRoot->rightChild, otherRoot->rightChild);
    }
}

// Delete helper function
void Tree::deleteTree(TreeNode* node) {
    if (node != nullptr) {
        deleteTree(node->leftChild);
        deleteTree(node->rightChild);
        delete node;
    }
}
