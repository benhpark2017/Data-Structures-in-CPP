/**
 * Solutions to Chapter 5, Section 1, Exercises 1 and 2 of Horowitz's
 * Fundamentals of Data Structures in C++.
 * 
 * Task 1: 
 * Write a function that reads in a tree represented as a list and creates its
 * internal representation using nodes with three fields: tag, data, and link.
 *
 * Task 2:
 * Write a function that reverses the process in Task 1 and takes a pointer to
 * a tree and prints out its list representation.
 */

/**************************************main.cpp*******************************************/
#include "Tree.h"
#include <iostream>

int main() {
    Tree tree1, tree2;
    std::string input;
    int choice;

    std::cout << "Choose tree construction method:\n";
    std::cout << "1. Enter tree structure as string\n";
    std::cout << "2. Construct tree interactively\n";
    std::cin >> choice;
    std::cin.ignore();  // Ignore leftover newline after entering choice

    if (choice == 1) {
        std::cout << "Enter tree structure (e.g., A(B(E(K,L),F),C(G),D(H(M),I,J))):" << std::endl;
        std::getline(std::cin, input);
        tree1.buildFromString(input);
        std::cout << "\nTree 1 structure:" << std::endl;
        tree1.printTree();
    } else if (choice == 2) {
        tree2.constructFromUserInput();
        std::cout << "\nTree 2 structure:" << std::endl;
        tree2.printTree();
    } else {
        std::cout << "Invalid choice." << std::endl;
    }

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
    void constructFromUserInput();
    void printTree() const;
};

#endif // TREE_H



/**************************************Tree.cpp*******************************************/
#include "Tree.h"
#include <iostream>
#include <stack>
#include <queue>
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

void Tree::constructFromUserInput() {
    std::cout << "Constructing a tree interactively.\n";
    char data;
    std::queue<TreeNode*> nodeQueue;
    
    std::cout << "Enter root node data: ";
    std::cin >> data;
    root = new TreeNode(data);  // Create the root node
    nodeQueue.push(root);       // Add root node to queue

    while (!nodeQueue.empty()) {
        TreeNode* current = nodeQueue.front();
        nodeQueue.pop();

        char firstChild, nextSibling;
        std::cout << "Enter first child of " << current->data << " (or '.' for no child): ";
        std::cin >> firstChild;
        if (firstChild != '.') {
            TreeNode* childNode = new TreeNode(firstChild);
            current->firstChild = childNode;
            nodeQueue.push(childNode);
        }

        TreeNode* sibling = current->firstChild;
        while (sibling) {
            std::cout << "Enter next sibling of " << sibling->data << " (or '.' for no sibling): ";
            std::cin >> nextSibling;
            if (nextSibling == '.') break;

            TreeNode* siblingNode = new TreeNode(nextSibling);
            sibling->nextSibling = siblingNode;
            nodeQueue.push(siblingNode);

            sibling = siblingNode;  // Move to next sibling for further input
        }
    }

    std::cout << "Tree construction complete.\n";
}
