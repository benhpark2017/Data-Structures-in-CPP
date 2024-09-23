/**
 * Solutions to Chapter 5, Section 1, Exercise 3 of Horowitz's
 * Fundamentals of Data Structures in C++.
 * 
 * Task 3 (Programming Project):
 * Write the C++ class definition for trees using the list representation
 * described in this section. Write C++ functions that:
 * (a) [operator>>()]: accept a tree represented as a parenthesized list   
 *     as input and create the generalized list representation of the tree
 * (b) [copy constructor]: initialize a tree with another tree
 *     represented as a generalized list
 * (c) [operator==()]: test for equality between two trees represented as
 *     generalized lists
 * (d) [destructor]: erase a tree represented as a generalized list
 * (e) [operator<<()]: output a tree in its parenthesized list notation
 * Test the correctness of your functions with suitable data.
 */

/**************************************main.cpp*******************************************/
#include "Tree.h"
#include <iostream>

int main() {
    Tree tree;

    // Input the tree structure
    std::cout << "Enter tree structure (e.g., A(B(E(K,L),F),C(G),D(H(M),I,J))):" << std::endl;
    std::cin >> tree;

    // Output the tree in parenthesized list notation
    std::cout << "\nTree structure in parenthesized list format:" << std::endl;
    std::cout << tree << std::endl;

    // Test the printNode() function by manually printing nodes in tree-like structure
    std::cout << "\nTesting printNode() function:\n";
    tree.printNode(tree.getRoot());  // Call printNode with root of the tree

    // Test the copy constructor by copying tree onto newTree
    Tree newTree = tree;
    
    // Output the copied newTree
    std::cout << "\nCopied Tree (newTree):" << std::endl;
    std::cout << newTree << std::endl;
    
    // Check if trees are equal
    if (tree == newTree) {
        std::cout << "The trees are equal." << std::endl;
    } else {
        std::cout << "The trees are not equal." << std::endl;
    }

    return 0;
}



/***************************************Tree.h********************************************/
#ifndef TREE_H
#define TREE_H

#include <iostream>
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

    TreeNode* copyTreeNode(const TreeNode* node);
    
    // Merged delete function
    void deleteTree(TreeNode* node);

public:
    Tree();
    Tree(const Tree& other);
    ~Tree();

    void buildFromString(const std::string& s);

    // Operator overloads
    friend std::istream& operator>>(std::istream& is, Tree& tree);
    friend std::ostream& operator<<(std::ostream& os, const Tree& tree);
    
    void writeTree(std::ostream& os, TreeNode* node) const;
    
    // Equality operator
    bool operator==(const Tree& other) const;
    bool operatorEquals(TreeNode* node1, TreeNode* node2) const;

    // Print tree in tree-like structure
    void printTree() const;
    void printNode(TreeNode* node, std::string prefix = "", bool isLast = true) const;

    TreeNode* getRoot() const { return root; }
};

#endif // TREE_H



/**************************************Tree.cpp*******************************************/
#include "Tree.h"
#include <stack>
#include <cctype>

// Constructor for TreeNode
TreeNode::TreeNode(char d) : data(d), firstChild(nullptr), nextSibling(nullptr) {}

// Tree default constructor
Tree::Tree() : root(nullptr) {}

// Tree copy constructor
Tree::Tree(const Tree& other) {
    root = (other.root) ? copyTreeNode(other.root) : nullptr;
}

// Tree destructor
Tree::~Tree() {
    deleteTree(root);  // Calls the merged delete function
}

// Recursive function to delete the tree
void Tree::deleteTree(TreeNode* node) {
    if (!node) return;
    deleteTree(node->firstChild);
    deleteTree(node->nextSibling);
    delete node;
}

// Helper function to recursively copy nodes of a tree
TreeNode* Tree::copyTreeNode(const TreeNode* node) {
    if (!node) return nullptr;

    // Create a new node by copying the current node's data
    TreeNode* newNode = new TreeNode(node->data);

    // Recursively copy the first child
    newNode->firstChild = copyTreeNode(node->firstChild);

    // Recursively copy the next sibling
    newNode->nextSibling = copyTreeNode(node->nextSibling);

    return newNode;
}

// Equality operator merged with equality check
bool Tree::operator==(const Tree& other) const {
    return operatorEquals(this->root, other.root);  // Compare the roots of both trees
}

// Helper function for equality check
bool Tree::operatorEquals(TreeNode* node1, TreeNode* node2) const {
    // If both nodes are null, they are equal
    if (node1 == nullptr && node2 == nullptr) {
        return true;
    }
    // If one is null and the other is not, they are not equal
    if (node1 == nullptr || node2 == nullptr) {
        return false;
    }
    // If the data in the current nodes are different, they are not equal
    if (node1->data != node2->data) {
        return false;
    }
    // Recursively check the firstChild and nextSibling
    return operatorEquals(node1->firstChild, node2->firstChild) &&
           operatorEquals(node1->nextSibling, node2->nextSibling);
}

// Helper function to print the tree nodes in a tree-like structure
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

// Function to write the tree to an output stream in parenthesized list notation
void Tree::writeTree(std::ostream& os, TreeNode* node) const {
    if (!node) return;

    os << node->data;

    if (node->firstChild) {
        os << "(";
        TreeNode* child = node->firstChild;
        while (child) {
            writeTree(os, child);
            if (child->nextSibling) {
                os << ",";
            }
            child = child->nextSibling;
        }
        os << ")";
    }
}

// Overload of operator>> to input tree from parenthesized list
std::istream& operator>>(std::istream& is, Tree& tree) {
    std::string input;
    std::getline(is, input);  // Read entire line as input
    tree.buildFromString(input);
    return is;
}

// Overload of operator<< to output tree in parenthesized list notation
std::ostream& operator<<(std::ostream& os, const Tree& tree) {
    if (!tree.root) {
        os << "Empty tree";
    } else {
        tree.writeTree(os, tree.root);
    }
    return os;
}

// Build tree from parenthesized list
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



