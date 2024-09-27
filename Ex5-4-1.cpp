/**
 * Solutions to Chapter 5, Section 4, Exercises 1 and 2 of Horowitz's
 * Fundamentals of Data Structures in C++.
 * 
 * Task 1:
 * Write a C++ function to count the number of nodes in a binary tree, T. What
 * is its computing time?
 * (The time complexity of this recursive function is O(n). This is derived from
 * the fact that in the worst case of a skewed tree, where every node has only
 * one child, the recursion depth would be O(n).)
 *
 * Task 2:
 * Write a C++ function, SwapTree(), that takes a binary tree and swaps the left 
 * and right children of every node. An example is given in Figure 5.19.
 */ 
/***********************************main.cpp**************************************/
#include "Tree.h"
#include <iostream>

int main() {
    // Create trees with 'int' data type
    Tree<int> t1, t2, t3;
    
    // Initialize the first tree
    t1.Initialize();
    
    // Copy t1 to t2
    t2 = t1;

    // Copy t1 to t3
    t3.Initialize();
    t3.SwapTree();

    // Output the solution for Exercise 5.4.1 and 5.4.2
    std::cout << "Solutions to Chapter 5, Section 4, Exercise 1 and 2: " << std::endl;
    std::cout << std::endl;

    // Preorder traversal of t1
    std::cout << "Pre-order traversal of first tree: ";
    t1.Preorder();
    std::cout << std::endl;

    // Inorder traversal of t1
    std::cout << "In-order traversal of first tree: ";
    t1.Inorder();
    std::cout << std::endl;

    // Postorder traversal of t1
    std::cout << "Post-order traversal of first tree: ";
    t1.Postorder();
    std::cout << std::endl << std::endl;

    // Preorder traversal of t2
    std::cout << "Pre-order traversal of second tree: ";
    t2.Preorder();
    std::cout << std::endl;

    // Inorder traversal of t2
    std::cout << "In-order traversal of second tree: ";
    t2.Inorder();
    std::cout << std::endl;

    // Postorder traversal of t2
    std::cout << "Post-order traversal of second tree: ";
    t2.Postorder();
    std::cout << std::endl << std::endl;

    // Preorder traversal of t3
    std::cout << "Pre-order traversal of third tree: ";
    t3.Preorder();
    std::cout << std::endl;

    // Inorder traversal of t3
    std::cout << "In-order traversal of third tree: ";
    t3.Inorder();
    std::cout << std::endl;

    // Postorder traversal of t3
    std::cout << "Post-order traversal of third tree: ";
    t3.Postorder();
    std::cout << std::endl << std::endl;

    // Count the leaf nodes in t1
    std::cout << "Leaf nodes: " << t1.LeafCount() << std::endl;

    // Compare the two trees
    if (t1 == t2)
        std::cout << "Trees 1 and 2 are the same." << std::endl;
    else
        std::cout << "Trees 1 and 2 are not the same." << std::endl;

    // Compare the two trees
    if (t1 == t3)
        std::cout << "Trees 1 and 3 are the same." << std::endl;
    else
        std::cout << "Trees 1 and 3 are not the same." << std::endl;    

    return 0;
}


/***********************************Tree.h**************************************/
#ifndef TREE_H
#define TREE_H

#include <iostream>

template <typename T>
class Tree;

// Templated TreeNode class
template <typename T>
class TreeNode {
  public:
    TreeNode *leftChild;
    T data;
    TreeNode *rightChild;

    // Default constructor
    TreeNode();

    // Parameterized constructor
    TreeNode(TreeNode<T> *left, T temp, TreeNode<T> *right);

    // Friend class declaration
    friend class Tree<T>;
};

template <typename T>
class Tree {
  private:
    TreeNode<T> *root;

  public:
    // Default constructor
    Tree();

    // Copy constructor
    Tree(const Tree<T> &t);

    // Initialize the tree
    void Initialize();

    // Copy function for the tree
    TreeNode<T>* Copy(TreeNode<T> *origNode);

    // Traversal functions
    void Preorder();
    void Inorder();
    void Postorder();

    // Count leaf nodes
    int LeafCount();

    // Swap tree
    void SwapTree();

    // Equality operator overload
    friend bool operator==(const Tree<T> &t1, const Tree<T> &t2) {
        return Equal(t1.root, t2.root);
    }

    // Compare two nodes
    static bool Equal(TreeNode<T> *tn1, TreeNode<T> *tn2);

  private:
    // Private helper functions
    void Preorder(TreeNode<T> *currentNode);
    void Inorder(TreeNode<T> *currentNode);
    void Postorder(TreeNode<T> *currentNode);
    int LeafCount(TreeNode<T> *currentNode);
    void SwapTree(TreeNode<T>* currentNode);
};

#include "Tree.tpp"

#endif


/**********************************Tree.tpp*************************************/
#ifndef TREE_TPP
#define TREE_TPP

// TreeNode implementations
template <typename T>
TreeNode<T>::TreeNode() : leftChild(nullptr), data(T()), rightChild(nullptr) {}

template <typename T>
TreeNode<T>::TreeNode(TreeNode<T> *left, T temp, TreeNode<T> *right) :
    leftChild(left), data(temp), rightChild(right) {}

// Tree implementations
template <typename T>
Tree<T>::Tree() : root(nullptr) {}

template <typename T>
Tree<T>::Tree(const Tree<T> &t) {
    root = Copy(t.root);
}

template <typename T>
void Tree<T>::Initialize() {
    root = new TreeNode<T>(nullptr, 1, nullptr);
    
    root->leftChild = new TreeNode<T>(nullptr, 2, nullptr);
    root->rightChild = new TreeNode<T>(nullptr, 3, nullptr);

    TreeNode<T>* four = new TreeNode<T>(nullptr, 4, nullptr);
    TreeNode<T>* five = new TreeNode<T>(nullptr, 5, nullptr);
    TreeNode<T>* six = new TreeNode<T>(nullptr, 6, nullptr);
    TreeNode<T>* seven = new TreeNode<T>(nullptr, 7, nullptr);
    
    root->leftChild->leftChild = four;
    root->leftChild->rightChild = five; 
    root->rightChild->leftChild = six;
    root->rightChild->rightChild = seven;
    
    TreeNode<T>* eight = new TreeNode<T>(nullptr, 8, nullptr);
    TreeNode<T>* nine = new TreeNode<T>(nullptr, 9, nullptr);
    TreeNode<T>* ten = new TreeNode<T>(nullptr, 10, nullptr);
    TreeNode<T>* eleven = new TreeNode<T>(nullptr, 11, nullptr);
    TreeNode<T>* twelve = new TreeNode<T>(nullptr, 12, nullptr);
    TreeNode<T>* thirteen = new TreeNode<T>(nullptr, 13, nullptr);
    TreeNode<T>* fourteen = new TreeNode<T>(nullptr, 14, nullptr);
    TreeNode<T>* fifteen = new TreeNode<T>(nullptr, 15, nullptr);
    
    four->leftChild = eight;
    four->rightChild = nine; 
    five->leftChild = ten;
    five->rightChild = eleven;
    six->leftChild = twelve;
    six->rightChild = thirteen; 
    seven->leftChild = fourteen;
    seven->rightChild = fifteen;
}

template <typename T>
TreeNode<T>* Tree<T>::Copy(TreeNode<T> *origNode) {
    if (!origNode)
        return nullptr;

    TreeNode<T> *temp = new TreeNode<T>(nullptr, origNode->data, nullptr);
    temp->leftChild = Copy(origNode->leftChild);
    temp->rightChild = Copy(origNode->rightChild);

    return temp;
}

template <typename T>
void Tree<T>::Preorder() {
    Preorder(root);
}

template <typename T>
void Tree<T>::Preorder(TreeNode<T> *currentNode) {
    if (currentNode) {
        std::cout << currentNode->data << " -> ";
        Preorder(currentNode->leftChild);
        Preorder(currentNode->rightChild);
    }
}

template <typename T>
void Tree<T>::Inorder() {
    Inorder(root);
}

template <typename T>
void Tree<T>::Inorder(TreeNode<T> *currentNode) {
    if (currentNode) {
        Inorder(currentNode->leftChild);
        std::cout << currentNode->data << " -> ";
        Inorder(currentNode->rightChild);
    }
}

template <typename T>
void Tree<T>::Postorder() {
    Postorder(root);
}

template <typename T>
void Tree<T>::Postorder(TreeNode<T> *currentNode) {
    if (currentNode) {
        Postorder(currentNode->leftChild);
        Postorder(currentNode->rightChild);
        std::cout << currentNode->data << " -> ";
    }
}

template <typename T>
int Tree<T>::LeafCount() {
    return LeafCount(root);
}

template <typename T>
int Tree<T>::LeafCount(TreeNode<T> *currentNode) {
    if (!currentNode) 
        return 0;

    if (!currentNode->leftChild && !currentNode->rightChild) 
        return 1;

    return LeafCount(currentNode->leftChild) + LeafCount(currentNode->rightChild);
}

template <typename T>
bool Tree<T>::Equal(TreeNode<T> *tn1, TreeNode<T> *tn2) {
    if (!tn1 && !tn2)
        return true;
    if (tn1 && tn2 && tn1->data == tn2->data &&
        Equal(tn1->leftChild, tn2->leftChild) &&
        Equal(tn1->rightChild, tn2->rightChild))
        return true;
    return false;
}

template <typename T>
void Tree<T>::SwapTree() { 
    SwapTree(root); 
}

template <typename T>
void Tree<T>::SwapTree(TreeNode<T>* currentNode) {
    if (!currentNode)
        return;

    if (!currentNode->leftChild && !currentNode->rightChild)
        return;

    if (!currentNode->leftChild && currentNode->rightChild) {
        currentNode->leftChild = currentNode->rightChild;
        currentNode->rightChild = nullptr;
    } else if (currentNode->leftChild && !currentNode->rightChild) {
        currentNode->rightChild = currentNode->leftChild;
        currentNode->leftChild = nullptr;
    } else {
        TreeNode<T> *temp = currentNode->leftChild;
        currentNode->leftChild = currentNode->rightChild;
        currentNode->rightChild = temp;
    }

    if (currentNode->leftChild)
        SwapTree(currentNode->leftChild);

    if (currentNode->rightChild)
        SwapTree(currentNode->rightChild);
}

#endif
