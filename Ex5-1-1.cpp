/**
 * Solutions to Chapter 5, Section 1, Exercise 1 of Horowitz's
 * Fundamentals of Data Structures in C++.
 * 
 * Task 1: 
 * Write a function that reads in a tree represented as a list and creates its
 * internal representation using nodes with three fields: tag, data, and link.
 */

/**************************************main.cpp*******************************************/
#include "Tree.h"
#include <iostream>

int main() {
    Tree tree;
    std::string input;

    std::cout << "Enter tree structure (e.g., A(B(E(K,L),F),C(G),D(H(M),I,J))):" << std::endl;
    std::getline(std::cin, input);

    tree.buildFromString(input);

    std::cout << "\nTree structure:" << std::endl;
    tree.printTree();

    return 0;
}



/***************************************Tree.h********************************************/
#ifndef TREE_H
#define TREE_H

#include <string>

struct TreeNode {
    char data;
    TreeNode* firstChild;
    TreeNode* nextSibling;
    TreeNode(char d);
};

class Tree {
private:
    TreeNode* root;

    void deleteTree(TreeNode* node);
    void printNode(TreeNode* node, std::string prefix = "", bool isLast = true) const;

public:
    Tree();
    ~Tree();

    void buildFromString(const std::string& s); //Chapter 5, Section 1, Exercise 1
    void printTree() const;
};

#endif // TREE_H



/**************************************Tree.cpp*******************************************/
#include "Tree.h"
#include <iostream>
#include <stack>
#include <cctype>

TreeNode::TreeNode(char d) : data(d), firstChild(nullptr), nextSibling(nullptr) {}

Tree::Tree() : root(nullptr) {}

Tree::~Tree() {
    deleteTree(root);
}

void Tree::deleteTree(TreeNode* node) {
    if (!node) return;
    deleteTree(node->firstChild);
    deleteTree(node->nextSibling);
    delete node;
}

void Tree::printNode(TreeNode* node, std::string prefix, bool isLast) const {
    if (!node) return;

    std::cout << prefix;
    std::cout << (isLast ? "└── " : "├── ");
    std::cout << node->data << std::endl;

    prefix += isLast ? "    " : "│   ";

    TreeNode* child = node->firstChild;
    while (child) {
        printNode(child, prefix, child->nextSibling == nullptr);
        child = child->nextSibling;
    }
}

//Solutions to Chapter 5, Section 1, Exercise 1
void Tree::buildFromString(const std::string& s) {
    std::stack<TreeNode*> stack;
    TreeNode* current = nullptr;

    for (char c : s) {
        if (std::isalpha(c)) {  // If it's an alphabetic character, create a new node
            TreeNode* newNode = new TreeNode(c);
            if (!root) {
                root = newNode;  // Set root if it's the first node
            } else if (current) {
                if (!current->firstChild) {
                    current->firstChild = newNode;  // Add as the first child
                } else {
                    TreeNode* sibling = current->firstChild;
                    while (sibling->nextSibling) {
                        sibling = sibling->nextSibling;  // Traverse to the last sibling
                    }
                    sibling->nextSibling = newNode;  // Add as the next sibling
                }
            }
            current = newNode;  // Move current to the new node
        } else if (c == '(') {
            stack.push(current);  // Save the current node (parent) to the stack
        } else if (c == ',') {
            if (!stack.empty()) {
                current = stack.top();  // Move back to the parent node to add a sibling
            }
        } else if (c == ')') {
            if (!stack.empty()) {
                current = stack.top();  // Go back to the parent node after processing children
                stack.pop();  // Pop the parent node from the stack
            }
        }
    }
}

void Tree::printTree() const {
    if (!root) {
        std::cout << "Empty tree" << std::endl;
        return;
    }
    printNode(root);
}
