/**
 * Solutions to Chapter 5, Section 12, Exercises 5 and 6 of Horowitz's
 * Fundamentals of Data Structures in C++.
 * 
 * Task 5:
 * Write an algorithm to construct the binary tree with given preorder and inorder
 * sequences.
 * 
 * Task 6:
 * Write an algorithm to construct the binary tree with given inorder and postorder
 * sequences.
 */
/*********************************main.cpp*************************************/
#include "BinaryTree.h"
#include <iostream>

int main() {
    BinaryTree<int> treePI;
    BinaryTree<int> treeIP;
    
    std::vector<int> preorder = {1, 2, 4, 5, 3};
    std::vector<int> inorder = {4, 2, 5, 1, 3};
    std::vector<int> postorder = {4, 5, 2, 3, 1};
    
    treePI.buildTreePI(preorder, inorder);
    treeIP.buildTreeIP(inorder, postorder);

    std::cout << "Testing in-order traversal from preorder and inorder sequences: "
              << treePI << std::endl;
    std::cout << "Testing in-order traversal from inorder and postorder sequences: " 
              << treeIP << std::endl;

    std::cout << "Inorder traversal of the first binary tree: ";
    treePI.inorder();  // Expected output: 4 2 5 1 3
    std::cout << std::endl;
    
    std::cout << "Preorder traversal of the first binary tree: ";
    treePI.preorder();  // Expected output: 1 2 4 5 3
    std::cout << std::endl;
    
    std::cout << "Postorder traversal of the first binary tree: ";
    treePI.postorder();  // Expected output: 4 5 2 3 1
    std::cout << std::endl;


    std::cout << "Inorder traversal of the second binary tree: ";
    treeIP.inorder();  // Expected output: 4 2 5 1 3
    std::cout << std::endl;
    
    std::cout << "Preorder traversal of the second binary tree: ";
    treeIP.preorder();  // Expected output: 1 2 4 5 3
    std::cout << std::endl;
    
    std::cout << "Postorder traversal of the second binary tree: ";
    treeIP.postorder();  // Expected output: 4 5 2 3 1
    std::cout << std::endl;

    BinaryTree<int> copiedTree = treePI;  // Test copy constructor
    std::cout << "Inorder traversal of the copied binary tree: ";
    copiedTree.inorder();  // Should match the original tree's traversal
    std::cout << std::endl;

    return 0;
}



/********************************BinaryTree.h**********************************/
#ifndef BINARY_TREE_H
#define BINARY_TREE_H

#include <vector>
#include <iostream>

template <typename T>
class BinaryTree;

template <typename T>
std::ostream& operator<<(std::ostream& os, const BinaryTree<T>& tree);

template <typename T>
class BinaryTreeNode {
    friend class BinaryTree<T>;  // Tree is a friend class of TreeNode
  private:
    BinaryTreeNode* leftChild;
    T data;               // Generic data type
    BinaryTreeNode* rightChild;

  public:
    // Constructor to initialize the node
    BinaryTreeNode(const T& data) : leftChild(nullptr), data(data), rightChild(nullptr) {}
};

template <typename T>
class BinaryTree {
public:
    BinaryTree();                         // Constructor
    BinaryTree(const BinaryTree& other);  // Copy constructor
    ~BinaryTree();                        // Destructor

    void insert(const T& data);     // Function to insert nodes
    void inorder() const;           // Inorder traversal for testing
    void preorder() const;          // Preorder traversal for testing
    void postorder() const;         // Postorder traversal for testing
    
    //Function that builds a tree from preorder and inorder sequences
    void buildTreePI(const std::vector<T>& preorder, const std::vector<T>& inorder);
    
    //Function that builds a tree from inorder and postorder sequences
    void buildTreeIP(const std::vector<T>& inorder, const std::vector<T>& postorder);
    
    friend std::ostream& operator<< <T>(std::ostream& os, const BinaryTree<T>& tree);
    
private:
    BinaryTreeNode<T>* root;

    // Helper functions for copying and deleting the tree
    void copyTree(BinaryTreeNode<T>*& thisRoot, BinaryTreeNode<T>* otherRoot);
    void deleteTree(BinaryTreeNode<T>* node);

    // Traversal helper
    void inorder(BinaryTreeNode<T>* node) const;
    void preorder(BinaryTreeNode<T>* node) const;
    void postorder(BinaryTreeNode<T>* node) const;

    // Private member function to build the tree
    BinaryTreeNode<T>* buildTreeRecursivePI(const std::vector<T>& preorder, int& preIndex,
                                            const std::vector<T>& inorder, int inStart, int inEnd);
                                          
    // Private member function to build the tree                                      
    BinaryTreeNode<T>* buildTreeRecursiveIP(const std::vector<T>& inorder, int inStart, int inEnd,
                                            const std::vector<T>& postorder, int& postIndex);

    void printTree(std::ostream& os, BinaryTreeNode<T>* node) const;
};

#endif //BinaryTree.h

#include "BinaryTree.tpp"



#include <iostream>
#include <queue>

// Constructor
template <typename T>
BinaryTree<T>::BinaryTree() : root(nullptr) {}

// Copy constructor
template <typename T>
BinaryTree<T>::BinaryTree(const BinaryTree& other) : root(nullptr) {
    if (other.root != nullptr) {
        copyTree(root, other.root);
    }
}

// Destructor
template <typename T>
BinaryTree<T>::~BinaryTree() {
    deleteTree(root);
}

// Insert function for complete binary tree
template <typename T>
void BinaryTree<T>::insert(const T& data) {
    BinaryTreeNode<T>* newNode = new BinaryTreeNode<T>(data);
    if (root == nullptr) {
        root = newNode;
        return;
    }

    std::queue<BinaryTreeNode<T>*> q;
    q.push(root);

    while (!q.empty()) {
        BinaryTreeNode<T>* temp = q.front();
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
void BinaryTree<T>::inorder() const {
    inorder(root);
}

// Helper function for inorder traversal
template <typename T>
void BinaryTree<T>::inorder(BinaryTreeNode<T>* node) const {
    if (node) {
        inorder(node->leftChild);
        std::cout << node->data << " ";
        inorder(node->rightChild);
    }
}

// Public function to start preorder traversal
template <typename T>
void BinaryTree<T>::preorder() const {
    preorder(root);
}

// Private helper function for recursive preorder traversal
template <typename T>
void BinaryTree<T>::preorder(BinaryTreeNode<T>* node) const {
    if (node) {
        std::cout << node->data << " "; // Visit the current node
        preorder(node->leftChild);      // Traverse left subtree
        preorder(node->rightChild);     // Traverse right subtree
    }
}

// Public function to start postorder traversal
template <typename T>
void BinaryTree<T>::postorder() const {
    postorder(root);
}

// Private helper function for recursive postorder traversal
template <typename T>
void BinaryTree<T>::postorder(BinaryTreeNode<T>* node) const {
    if (node) {
        postorder(node->leftChild);     // Traverse left subtree
        postorder(node->rightChild);    // Traverse right subtree
        std::cout << node->data << " "; // Visit the current node
    }
}

// Implementation of buildTreeRecursivePI as a private member function
template <typename T>
BinaryTreeNode<T>* BinaryTree<T>::buildTreeRecursivePI(const std::vector<T>& preorder, int& preIndex,
                                                     const std::vector<T>& inorder, int inStart, int inEnd) {
    // Base case: no elements to construct
    if (inStart > inEnd) {
        return nullptr;
    }

    // The current root is the next element in the preorder sequence
    T rootData = preorder[preIndex++];
    BinaryTreeNode<T>* node = new BinaryTreeNode<T>(rootData);

    // Find the index of this root in the inorder sequence
    int inIndex;
    for (inIndex = inStart; inIndex <= inEnd; ++inIndex) {
        if (inorder[inIndex] == rootData) {
            break;
        }
    }

    // Recursively build the left and right subtrees
    node->leftChild = buildTreeRecursivePI(preorder, preIndex, inorder, inStart, inIndex - 1);
    node->rightChild = buildTreeRecursivePI(preorder, preIndex, inorder, inIndex + 1, inEnd);

    return node;
}

// Implementation of buildTreePI
template <typename T>
void BinaryTree<T>::buildTreePI(const std::vector<T>& preorder, const std::vector<T>& inorder) {
    int preIndex = 0;
    root = buildTreeRecursivePI(preorder, preIndex, inorder, 0, inorder.size() - 1);
}

// Implementation of buildTreeRecursiveIP as a private member function
template <typename T>
BinaryTreeNode<T>* BinaryTree<T>::buildTreeRecursiveIP(const std::vector<T>& inorder, int inStart,
                                                       int inEnd, const std::vector<T>& postorder, int& postIndex) {
    // Base case: no elements to construct
    if (inStart > inEnd) {
        return nullptr;
    }

    // The current root is the last element in the postorder sequence
    T rootData = postorder[postIndex--];
    BinaryTreeNode<T>* node = new BinaryTreeNode<T>(rootData);

    // If this node has no children, return the node
    if (inStart == inEnd) {
        return node;
    }

    // Find the index of this root in the inorder sequence
    int inIndex;
    for (inIndex = inStart; inIndex <= inEnd; ++inIndex) {
        if (inorder[inIndex] == rootData) {
            break;
        }
    }

    // Recursively build the right and left subtrees
    node->rightChild = buildTreeRecursiveIP(inorder, inIndex + 1, inEnd, postorder, postIndex);
    node->leftChild = buildTreeRecursiveIP(inorder, inStart, inIndex - 1, postorder, postIndex);

    return node;
}

// Implementation of buildTreeIP
template <typename T>
void BinaryTree<T>::buildTreeIP(const std::vector<T>& inorder, const std::vector<T>& postorder) {
    int postIndex = postorder.size() - 1;
    root = buildTreeRecursiveIP(inorder, 0, inorder.size() - 1, postorder, postIndex);
}

// Copy helper function
template <typename T>
void BinaryTree<T>::copyTree(BinaryTreeNode<T>*& thisRoot, BinaryTreeNode<T>* otherRoot) {
    if (otherRoot == nullptr) {
        thisRoot = nullptr;
    } else {
        thisRoot = new BinaryTreeNode<T>(otherRoot->data);
        copyTree(thisRoot->leftChild, otherRoot->leftChild);
        copyTree(thisRoot->rightChild, otherRoot->rightChild);
    }
}

// Delete helper function
template <typename T>
void BinaryTree<T>::deleteTree(BinaryTreeNode<T>* node) {
    if (node != nullptr) {
        deleteTree(node->leftChild);
        deleteTree(node->rightChild);
        delete node;
    }
}

// Helper function for printing the tree (in-order traversal)
template <typename T>
void BinaryTree<T>::printTree(std::ostream& os, BinaryTreeNode<T>* node) const {
    if (node) {
        printTree(os, node->leftChild);  // Print left subtree
        os << node->data << " ";  // Print the current node's data
        printTree(os, node->rightChild);  // Print right subtree
    }
}

// overloaded friend function of class BinaryTree that prints the tree.
template <typename T>
std::ostream& operator<<(std::ostream& os, const BinaryTree<T>& tree) {
    tree.printTree(os, tree.root);
    return os;
}
