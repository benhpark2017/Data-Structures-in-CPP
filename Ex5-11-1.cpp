/**
 * Solutions to Chapter 5, Section 11, Exercises 1, 2, and 3 of 
 * Horowitz's Fundamentals of Data Structures in C++.
 * 
 * Task 1:
 * Enlarge the object-oriented system of this section by adding classes
 * MinPQ, MinHeap, and LoserTree.
 * 
 * Task 2:
 * Modify the object-oriented system of this section by using in-order
 * iterator classes to perform the inorder traversal.
 * 
 * Task 3:
 * Add a function GetSize() that returns the number of elements in a 
 * tree. This function must be defined for each tree ADT defined in the
 * object-oriented system of this section.
 * 
 */

/**********************************main.cpp************************************/
#include "BinaryTree.h"
#include <iostream>
#include <sstream>
#include <cassert>
#include <string>

// Custom BinaryTree implementation for testing
class TestBinaryTree : public BinaryTree {
  public:
    TestBinaryTree() {
        root = new BinaryTreeNode;
        root->data = 4;
        root->leftChild = new BinaryTreeNode;
        root->leftChild->data = 2;
        root->rightChild = new BinaryTreeNode;
        root->rightChild->data = 6;
        root->leftChild->leftChild = new BinaryTreeNode;
        root->leftChild->leftChild->data = 1;
        root->leftChild->rightChild = new BinaryTreeNode;
        root->leftChild->rightChild->data = 3;
        root->rightChild->leftChild = new BinaryTreeNode;
        root->rightChild->leftChild->data = 5;
        root->rightChild->rightChild = new BinaryTreeNode;
        root->rightChild->rightChild->data = 7;
    }

    ~TestBinaryTree() {
        // Note: In a real implementation, you should properly delete all nodes to avoid memory leaks
    }
};

int main() {
    TestBinaryTree tree;
    inOrderIterator iterator(tree);
    
    // Use a stringstream to capture the output
    std::stringstream output;
    int* value;
    while ((value = iterator.Next()) != nullptr) {
        output << *value << " ";
        std::cout << *value << " ";
    }
    std::cout << std::endl;
    
    // Get the output as a string and remove the trailing space
    std::string result = output.str();
    if (!result.empty() && result.back() == ' ') {
        result.pop_back();
    }
    
    // Define the expected output
    const std::string expected = "1 2 3 4 5 6 7";
    
    // Use cassert to check if the output matches the expected result
    assert(result == expected && "In-order traversal output does not match expected result");
    
    std::cout << "In-order traversal test passed successfully!" << std::endl;
    
    return 0;
}



/********************************BinaryTree.h**********************************/
#ifndef BINARY_TREE_H
#define BINARY_TREE_H

#include "Constants.h"
#include <iostream>
#include <stack>

enum Boolean {FALSE, TRUE};

class MaxPQ {
  public:
    virtual void Insert(const int) = 0;
    virtual int* DeleteMax(int& ) = 0;
};

class MinPQ {
  public:
    virtual void Insert(const int item) = 0;
    virtual int* DeleteMin(int& x) = 0;
    virtual ~MinPQ() {};
};

class SearchStruct {
  public:
    virtual void Insert(const int ) = 0;
    virtual int* Delete(int& ) = 0;
    virtual Boolean Search(const int ) = 0;
};

class BinaryTree {
  protected:
    class BinaryTreeNode {
      public:
        BinaryTreeNode *leftChild;
        BinaryTreeNode *rightChild;
        int data;
        
        BinaryTreeNode() : leftChild(nullptr), rightChild(nullptr), data(0) {}
        BinaryTreeNode(int value) : leftChild(nullptr), rightChild(nullptr), data(value) {}
    };

  public:
    virtual void inOrder();
    BinaryTree() : root(nullptr) {}
    virtual ~BinaryTree() {}  // Add a virtual destructor
    
    // Change the return type to Node*
    BinaryTreeNode* getRoot() const { return root; }
    virtual int GetSize() const;

  protected:
    BinaryTreeNode *root;
    virtual void inOrder(BinaryTreeNode* );
    virtual int GetSize(BinaryTreeNode* node) const;
    
    
    friend class inOrderIterator;
};


class inOrderIterator {
  public:
    int* Next();
    inOrderIterator(BinaryTree& tree) : t(tree) { CurrentNode = t.root; }

  private:
    BinaryTree& t;
    std::stack<BinaryTree::BinaryTreeNode*> s;
    BinaryTree::BinaryTreeNode* CurrentNode;
};

class CompleteBinaryTree : public BinaryTree {
  public:
    virtual void inOrder(); //driver
    CompleteBinaryTree(int sz = Constants::defaultSize) : MaxSize(sz), n(0) {
        tree = new int[MaxSize + 1];
    };
    
    virtual int GetSize() const override { return n; } 
    // n is the number of nodes in the tree
    
  protected:
    int* tree;
    int MaxSize;
    int n;
    virtual void inOrder(int ); //hides BinaryTree::inOrder(BinaryTreeNode* )
};

class MaxHeap : public MaxPQ, public CompleteBinaryTree {
//multiple inheritance
  private:
    int* heap;
    int n;
    int MaxSize;
  public:
    MaxHeap(int sz = Constants::defaultSize) : CompleteBinaryTree(sz) {};
    
    // n is the number of nodes in the heap
    virtual int GetSize() const { return n; }
    
    virtual void Insert(const int );
    virtual int* DeleteMax(int& );
};

class MinHeap : public MinPQ, public CompleteBinaryTree { 	 
  private: 	 
    int* heap; 	 
    int n; 	     // current size of heap 	 
    int MaxSize; // maximum allowed size of heap 	 
  public: 	 
    MinHeap(int sz = Constants::defaultSize); 	 
    ~MinHeap() { delete[] heap; } 	 
	inline Boolean isFull() { return (n == MaxSize) ? TRUE : FALSE; } 	 
	inline Boolean isEmpty() { return (n == 0) ? TRUE : FALSE; } 	 

    // n is the number of nodes in the heap
    virtual int GetSize() const { return n; }
    
	virtual void Insert(const int& item); 	 
	virtual int* DeleteMin(int& x); 	 
}; 		 

class WinnerTree : public CompleteBinaryTree {
  public:
    WinnerTree(int sz = Constants::defaultSize) : CompleteBinaryTree(sz) {};
    virtual int GetSize() const override { return CompleteBinaryTree::GetSize(); }
};

class LoserTree : public CompleteBinaryTree {
  public:
    LoserTree(int sz = Constants::defaultSize) : CompleteBinaryTree(sz) {};
    virtual int GetSize() const override { return CompleteBinaryTree::GetSize(); }
};

class BST : public SearchStruct, public BinaryTree {
//multiple inheritance
  public:
    BST() {root = 0; };
    virtual void Insert(const int );
    virtual int* Delete(int& );
    virtual Boolean Search(const int );
    virtual int GetSize() const override { return BinaryTree::GetSize(); }
};

#endif //BINARY_TREE_H



/*******************************BinaryTree.cpp*********************************/
#include "BinaryTree.h"

void BinaryTree::inOrder() { inOrder(root); }

void BinaryTree::inOrder(BinaryTreeNode* CurrentNode) {
    if (CurrentNode) {
        inOrder(CurrentNode->leftChild);
        std::cout << CurrentNode->data;
        inOrder(CurrentNode->rightChild);
    }
}

void CompleteBinaryTree::inOrder() { inOrder(1); }

void CompleteBinaryTree::inOrder(int current) {
    if (current <= n) {
        inOrder(2 * current);
        std::cout << tree[current];
        inOrder(2 * current + 1);
    }
}

int* inOrderIterator::Next() {
    while (CurrentNode) {
        s.push(CurrentNode);
        CurrentNode = CurrentNode->leftChild;
    }
    if (!s.empty()) {
        CurrentNode = s.top();
        s.pop();
        int* temp = &(CurrentNode->data);
        CurrentNode = CurrentNode->rightChild;
        return temp;
    }
    return nullptr;
}

int BinaryTree::GetSize() const {
    return GetSize(root);
}

int BinaryTree::GetSize(BinaryTreeNode* node) const {
    if (node == nullptr) return 0;
    return 1 + GetSize(node->leftChild) + GetSize(node->rightChild);
}



/*********************************Constants.h**********************************/
#ifndef CONSTANTS_H
#define CONSTANTS_H

namespace Constants {
    const int defaultSize = 100;
}

#endif //CONSTANTS_H
