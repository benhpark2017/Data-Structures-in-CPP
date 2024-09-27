/**
 * Solutions to Chapter 5, Section 4, Exercises 3 to 6 of Horowitz's
 * Fundamentals of Data Structures in C++.
 * 
 * Task 3 (Programming Project):
 * Devise an external representation for the formulas in propositional calculus.
 * Write a function that reads such a formula and creates its binary tree
 * representation. What is the complexity of your function?
 * Answer:
 * The time complexity of the parseFormula() member function of class SatTree is
 * O(n). The space requirement of parseFormula() is also O(n).
 *
 * Task 4:
 * What is the computing time for PostOrderEval()?
 * Answer:
 * The time complexity of PostOrderEval() is O(n) in the worst case, where the
 * tree is a skewed binary tree, and O(log n) in average time, as seen in 
 * balanced trees.
 * 
 * Task 5:
 * Write a recursive algorithm for erasing all nodes in a binary tree ~SatTree
 * (Destructor).
 * 
 * Task 6:
 * Write a recursive algorithm for assigning one tree to another, named
 * operator=(). 
 */

#include "SatTree.h"
#include <iostream>
#include <string>

int main() {
    SatTree satTreeFirst, satTreeSecond;
    std::string formula;

    // Prompt user for input
    std::cout << "Enter a propositional logic expression." << std::endl;
    std::cout << "(use T for true, F for false, ! for NOT, & for AND, | for OR):" << std::endl;
    std::cout << "For example: !(T & F) | T" << std::endl;
    std::getline(std::cin, formula);

    // Build tree from formula
    satTreeFirst.buildTreeFromFormula(formula);
    
    std::cout << "Copying the first tree to the second tree." << std::endl;
    satTreeSecond = satTreeFirst;

    if (satTreeFirst == satTreeSecond) {
        std::cout << "The two trees are equal." << std::endl;
    } else {
        std::cout << "The two trees are not equal." << std::endl;
    }

    // Evaluate the tree
    satTreeFirst.PostOrderEval();

    // Output the result
    std::cout << "Result: ";
    satTreeFirst.rootvalue();
    std::cout << std::endl;
        
    std::cout << "End of program.\n" << std::endl;

    return 0;
}



/***********************SatTree.h**************************/
#ifndef SAT_TREE_H
#define SAT_TREE_H

#include <iostream>
#include <string>
#include <vector>

enum TypesOfData { LogicalNot, LogicalAnd, LogicalOr, LogicalTrue, LogicalFalse };
enum Boolean { FALSE, TRUE };

class SatNode {
    friend class SatTree;
    friend int equal(SatNode* a, SatNode* b);
private:
    SatNode* LeftChild;
    TypesOfData data;
    Boolean value;
    SatNode* RightChild;
};

class SatTree {
    friend int operator==(const SatTree &s, const SatTree &t);
    
public:
    SatTree();                      // Default constructor
    SatTree(const SatTree& s);
    ~SatTree();
    
    SatTree& operator=(const SatTree& other);
    
    // Method to build a tree from a formula string
    void buildTreeFromFormula(const std::string& formula);

    // Method to evaluate the tree using postorder traversal
    void PostOrderEval();
    
    // Output root value
    void rootvalue() { std::cout << (root->value == TRUE ? "TRUE" : "FALSE"); }

private:
    SatNode* root;
    
    // Helper methods for copy constructor and destructor
    SatNode* copy(SatNode* orignode);
    void deleteTree(SatNode* node);
    
    void PostOrderEval(SatNode* s);
    
    SatNode* createNode(char token);

    // Parse formula and build tree
    SatNode* parseFormula(const std::string& formula);
};

// Function to determine operator precedence (NOT part of SatTree)
int precedence(char op);

#endif // SAT_TREE_H



/***************************SatTree.cpp******************************/
#include "SatTree.h"
#include <stack>
#include <iostream>
#include <cctype>

int equal(SatNode* a, SatNode* b) {
    if ((!a) && (!b)) return 1;
    if ((a && b)
        && (a->data == b->data)
        && equal(a->LeftChild, b->LeftChild)
        && equal(a->RightChild, b->RightChild)) return 1;
    return 0;
}

int operator==(const SatTree &s, const SatTree &t) {
    return equal(s.root, t.root);
}


SatTree::SatTree() : root(nullptr) {  // Default constructor
    // Optionally, add any initialization logic here
}

// Copy constructor to copy a tree
SatTree::SatTree(const SatTree& s) {
    root = copy(s.root);
}

// Destructor to clean up the tree
// Solution to Chapter 5, Section 4, Exercise 5
SatTree::~SatTree() {
    deleteTree(root);
}

// Solution to Chapter 5, Section 4, Exercise 6
SatTree& SatTree::operator=(const SatTree& other) {
    if (this != &other) { // Check for self-assignment
        deleteTree(root); // Clear the existing tree
        root = copy(other.root); // Copy the new tree
    }
    return *this;
}

// Helper function for destructor
void SatTree::deleteTree(SatNode* node) {
    if (node) {
        deleteTree(node->LeftChild);
        deleteTree(node->RightChild);
        delete node;
    }
}

// Recursive copy function
SatNode* SatTree::copy(SatNode* orignode) {
    if (orignode) {
        SatNode* temp = new SatNode;
        temp->data = orignode->data;
        temp->value = orignode->value;
        temp->LeftChild = copy(orignode->LeftChild);
        temp->RightChild = copy(orignode->RightChild);
        return temp;
    } else {
        return nullptr;
    }
}

// Postorder evaluation of the tree
void SatTree::PostOrderEval() {
    PostOrderEval(root);
}

void SatTree::PostOrderEval(SatNode* s) {
    if (s) {
        PostOrderEval(s->LeftChild);
        PostOrderEval(s->RightChild);
        switch (s->data) {
            case LogicalNot:
                s->value = (s->RightChild->value == TRUE) ? FALSE : TRUE;
                break;
            case LogicalAnd:
                s->value = (s->LeftChild->value == TRUE && s->RightChild->value == TRUE) ? TRUE : FALSE;
                break;
            case LogicalOr:
                s->value = (s->LeftChild->value == TRUE || s->RightChild->value == TRUE) ? TRUE : FALSE;
                break;
            case LogicalTrue:
                s->value = TRUE;
                break;
            case LogicalFalse:
                s->value = FALSE;
        }
    }
}

// Helper function to determine operator precedence
int precedence(char op) {
    if (op == '!') return 3;
    if (op == '&') return 2;
    if (op == '|') return 1;
    return 0;
}

SatNode* SatTree::createNode(char token) {
    SatNode* node = new SatNode();
    if (token == '!') node->data = LogicalNot;
    else if (token == '&') node->data = LogicalAnd;
    else if (token == '|') node->data = LogicalOr;
    else if (token == 'T') node->data = LogicalTrue;
    else if (token == 'F') node->data = LogicalFalse;
    return node;
}

SatNode* SatTree::parseFormula(const std::string& formula) {
    std::stack<SatNode*> nodes;
    std::stack<char> operators;

    for (size_t i = 0; i < formula.length(); ++i) {
        char token = formula[i];

        if (isspace(token)) continue;

        if (token == 'T' || token == 'F') {
            nodes.push(createNode(token));
        } else if (token == '(') {
            operators.push(token);
        } else if (token == ')') {
            while (!operators.empty() && operators.top() != '(') {
                if (nodes.size() < 2 && operators.top() != '!') {
                    std::cerr << "Error: Not enough operands for operator " << operators.top() << std::endl;
                    return nullptr;
                }
                char op = operators.top();
                operators.pop();

                SatNode* node = createNode(op);

                if (op == '!') {
                    if (nodes.empty()) {
                        std::cerr << "Error: Not enough operands for NOT operator" << std::endl;
                        return nullptr;
                    }
                    node->RightChild = nodes.top();
                    nodes.pop();
                } else {
                    node->RightChild = nodes.top();
                    nodes.pop();
                    node->LeftChild = nodes.top();
                    nodes.pop();
                }
                nodes.push(node);
            }
            if (operators.empty()) {
                std::cerr << "Error: Mismatched parentheses" << std::endl;
                return nullptr;
            }
            operators.pop();  // Remove the '('
        } else if (token == '!' || token == '&' || token == '|') {
            while (!operators.empty() && precedence(operators.top()) >= precedence(token)) {
                if (nodes.size() < 2 && operators.top() != '!') {
                    std::cerr << "Error: Not enough operands for operator " << operators.top() << std::endl;
                    return nullptr;
                }
                char op = operators.top();
                operators.pop();

                SatNode* node = createNode(op);

                if (op == '!') {
                    if (nodes.empty()) {
                        std::cerr << "Error: Not enough operands for NOT operator" << std::endl;
                        return nullptr;
                    }
                    node->RightChild = nodes.top();
                    nodes.pop();
                } else {
                    node->RightChild = nodes.top();
                    nodes.pop();
                    node->LeftChild = nodes.top();
                    nodes.pop();
                }
                nodes.push(node);
            }
            operators.push(token);
        } else {
            std::cerr << "Error: Invalid token '" << token << "'" << std::endl;
            return nullptr;
        }
    }

    while (!operators.empty()) {
        if (nodes.size() < 2 && operators.top() != '!') {
            std::cerr << "Error: Not enough operands for operator " << operators.top() << std::endl;
            return nullptr;
        }
        char op = operators.top();
        operators.pop();

        SatNode* node = createNode(op);

        if (op == '!') {
            if (nodes.empty()) {
                std::cerr << "Error: Not enough operands for NOT operator" << std::endl;
                return nullptr;
            }
            node->RightChild = nodes.top();
            nodes.pop();
        } else {
            node->RightChild = nodes.top();
            nodes.pop();
            node->LeftChild = nodes.top();
            nodes.pop();
        }
        nodes.push(node);
    }

    if (nodes.size() != 1) {
        std::cerr << "Error: Invalid formula" << std::endl;
        return nullptr;
    }

    return nodes.top();
}

// Function to build tree from a formula
void SatTree::buildTreeFromFormula(const std::string& formula) {
    root = parseFormula(formula);
    if (root == nullptr) {
        std::cerr << "Failed to parse formula" << std::endl;
    }
}
