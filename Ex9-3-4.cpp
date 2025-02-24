/**
 * Solutions to Chapter 9, Section 3, Exercise 4 of Horowitz, Sahni, and Mehta's 
 * Fundamentals of Data Structures in C++, 1st Edition.
 * 
 * Task 4:
 * Write a function to delete the element in node x of a min leftist tree.
 * Assume that each node has data members LeftChild, RightChild, parent,
 * shortest, and data. The parent data member of a node points to its parent
 * in the leftist tree. What is the time complexity of your function?
 * 
 * Response to conceptual question:
 * The time complexity of the function is dominated by the level-order search
 * for the node, which is O(n). The merging and rebalancing steps of the
 * function take O(log n) for the worst-case scenario. Therefore, the overall
 * time complexity of DeleteElement function has time complexity of O(n).
 */

/*********************************main.cpp*************************************/
#include <iostream>
#include <algorithm>
#include <queue>
#include "MinLeftistTree.h"

int main() {
    MinLeftistTree<int> tree;
    Element<int> elements[] = {1, 10, 5, 20, 3, 8, 35, 40, 18, 21};
    
    tree.Initialize(elements, 10);
    std::cout << "Initial tree:" << std::endl;
    tree.PrintTree();
    
    // Test deletions
    int keysToDelete[] = {3, 5, 8, 10, 18, 21, 35, 40};
    for (int key : keysToDelete) {
        Element<int>* elementToDelete = new Element<int>(key);
        std::cout << "\nDeleting " << key << ":" << std::endl;
        tree.DeleteElement(elementToDelete);
        tree.PrintTree();
        delete elementToDelete;
    }
    
    return 0;
}



/*********************************Element.h************************************/
#ifndef ELEMENT_H
#define ELEMENT_H

// Element class definition
template <typename KeyType>
class Element {
private:
    KeyType key;

public:
    Element() {}
    Element(KeyType k) : key(k) {}

    // Getter and Setter for key
    KeyType getKey() const { return key; }
    void setKey(KeyType k) { key = k; }

    bool operator>(const Element<KeyType>& other) const { return key > other.getKey(); }
};

#endif // ELEMENT_H



/*******************************LeftistNode.h**********************************/
#ifndef LEFTIST_NODE_H
#define LEFTIST_NODE_H

#include "Element.h"

// Forward declaration of MinLeftistTree
template <typename KeyType> class MinLeftistTree;

// LeftistNode class definition
template <typename KeyType> 
class LeftistNode {
    friend class MinLeftistTree<KeyType>;

  private:
    Element<KeyType> data;
    LeftistNode *LeftChild, *RightChild, *parent;
    int shortest;

  public:
    LeftistNode() : 
        LeftChild(nullptr), RightChild(nullptr), parent(nullptr), shortest(1) {}
        
    LeftistNode(const Element<KeyType>& el) : 
        data(el), LeftChild(nullptr), RightChild(nullptr), parent(nullptr), shortest(1) {}
        
    // Getter and Setter functions for data, LeftChild, RightChild, parent, and shortest
    Element<KeyType> getData() const { return data; }

    // Setter for data
    void setData(const Element<KeyType>& el) { data = el; }

    // Getter for LeftChild
    LeftistNode* getLeftChild() const { return LeftChild; }

    // Setter for LeftChild
    void setLeftChild(LeftistNode* child) { LeftChild = child; }

    // Getter for RightChild
    LeftistNode* getRightChild() const { return RightChild; }

    // Setter for RightChild
    void setRightChild(LeftistNode* child) { RightChild = child; }

    // Getter for parent
    LeftistNode* getParent() const { return parent; }

    // Setter for parent
    void setParent(LeftistNode* p) { parent = p; }

    // Getter for shortest
    int getShortest() const { return shortest; }

    // Setter for shortest
    void setShortest(int s) { shortest = s; }
};

#endif // LEFTIST_NODE_H



/**********************************MinPQ.h*************************************/
#ifndef MIN_PQ_H
#define MIN_PQ_H

#include "Element.h"

// MinPQ abstract base class
template <typename KeyType>
class MinPQ {
  public:
    virtual void Insert(const Element<KeyType>&) = 0;
    virtual Element<KeyType>* DeleteMin(Element<KeyType>&) = 0;
    virtual ~MinPQ() {}
};

#endif // MIN_PQ_H



/*****************************MinLeftistTree.h*********************************/
#ifndef MIN_LEFTIST_TREE_H
#define MIN_LEFTIST_TREE_H

#include "MinPQ.h"
#include "LeftistNode.h"

// MinLeftistTree class implementation
template <typename KeyType>
class MinLeftistTree : public MinPQ<KeyType> {
  public:
    MinLeftistTree(LeftistNode<KeyType> *init = nullptr) : root(init) {}
    ~MinLeftistTree();
    void InitializeShortestPath(); // Driver function
    void ConvertToLeftistTree(); // Driver function
    void Insert(const Element<KeyType>&) override;
    Element<KeyType>* DeleteMin(Element<KeyType>&) override;
    void MinCombine(MinLeftistTree<KeyType>*);
    void PrintTree() const; // Helper for visualization
    bool IsEmpty() const { return root == nullptr; }
    void Initialize(const Element<KeyType>* elements, int numElements);
    void DeleteElement(Element<KeyType>* element);
  private:
    int InitializeShortestPath(LeftistNode<KeyType>*); // Workhorse function
    LeftistNode<KeyType>* ConvertToLeftistTree(LeftistNode<KeyType>*); // Workhorse function
    LeftistNode<KeyType>* MinUnion(LeftistNode<KeyType>*, LeftistNode<KeyType>*);
    void DeleteHelper(LeftistNode<KeyType>*); // For destructor
    LeftistNode<KeyType> *root;
};

#include "MinLeftistTree.tpp"

#endif // MIN_LEFTIST_TREE_H



/****************************MinLeftistTree.tpp********************************/
// Destructor implementation
template <typename KeyType>
MinLeftistTree<KeyType>::~MinLeftistTree() {
    DeleteHelper(root);
}

template <typename KeyType>
void MinLeftistTree<KeyType>::DeleteHelper(LeftistNode<KeyType>* node) {
    if (node) {
        DeleteHelper(node->getLeftChild());
        DeleteHelper(node->getRightChild());
        delete node;
    }
}

template <typename KeyType>
void MinLeftistTree<KeyType>::Initialize(const Element<KeyType> elements[], int numElements) {
    // Clear the existing tree
    DeleteHelper(root);
    root = nullptr;

    // Build the tree by combining nodes
    for (int i = 0; i < numElements; ++i) {
        LeftistNode<KeyType>* newNode = new LeftistNode<KeyType>(elements[i]);
        MinLeftistTree<KeyType> tempTree(newNode);
        if (root) {
            root = MinUnion(root, tempTree.root);
        } else {
            root = tempTree.root;
        }
        tempTree.root = nullptr; // Prevent double deletion
    }
}

// MinCombine implementation
template <typename KeyType>
void MinLeftistTree<KeyType>::MinCombine(MinLeftistTree<KeyType>* b) {
    // Combines the min leftist tree b with the given min leftist tree.
    // b is set to the empty minimum leftist tree.
    if (!root) root = b->root;
    else if (b->root) root = MinUnion(root, b->root);
    b->root = nullptr;
}

// MinUnion implementation
template <typename KeyType>
LeftistNode<KeyType>* MinLeftistTree<KeyType>::MinUnion(LeftistNode<KeyType>* a,
                                                        LeftistNode<KeyType>* b) {
    // Recursive function that combines two nonempty min leftist trees rooted
    // at a and b. The root of the resulting min leftist tree is returned.
    
    if (!a) return b;
    if (!b) return a;
    
    // Set a to be min leftist tree with smaller root.
    if (a->getData().getKey() > b->getData().getKey()) { LeftistNode<KeyType> *t = a; a = b; b = t; }
    
    // Create binary tree such that the smallest key in each subtree is in the root
    if (!a->getRightChild()) a->setRightChild(b);
    else a->setRightChild(MinUnion(a->getRightChild(), b));
    
    // Leftist Tree property
    // Interchange subtrees if needed
    if (!a->getLeftChild()) { a->setLeftChild(a->getRightChild()); a->setRightChild(nullptr); }
    else if (!a->getRightChild() || a->getLeftChild()->getShortest() < a->getRightChild()->getShortest()) {
        LeftistNode<KeyType> *t = a->getLeftChild();
        a->setLeftChild(a->getRightChild());
        a->setRightChild(t);
    }
    
    // Set 'shortest' data member
    if (!a->getRightChild()) a->setShortest(1);
    else a->setShortest(a->getRightChild()->getShortest() + 1);
    
    return a;
}

// Initialize shortest path implementation
template <typename KeyType>
void MinLeftistTree<KeyType>::InitializeShortestPath() {
    // Call recursive helper function starting from the root
    if (root) {
        InitializeShortestPath(root);
    }
}

// Helper function that recursively initializes the 'shortest' data member
template <typename KeyType>
int MinLeftistTree<KeyType>::InitializeShortestPath(LeftistNode<KeyType>* node) {
    if (!node) {
        return 0; // Null node has a path length of 0
    }
    
    // Recursively calculate shortest paths for left and right subtrees
    int leftShortest = InitializeShortestPath(node->getLeftChild());
    int rightShortest = InitializeShortestPath(node->getRightChild());
    
    // For leftist trees, the shortest path is always on the right side
    // Set the shortest path value for this node
    if (!node->getRightChild()) {
        node->setShortest(1); // If there's no right child, shortest path is 1
    } else {
        // Otherwise, it's the right child's shortest path plus 1
        node->setShortest(rightShortest + 1);
    }
    
    return node->getShortest();
}

// Convert to leftist tree implementation
template <typename KeyType>
void MinLeftistTree<KeyType>::ConvertToLeftistTree() {
    // Start conversion from the root
    if (root) {
        root = ConvertToLeftistTree(root);
    }
}

template <typename KeyType>
LeftistNode<KeyType>* MinLeftistTree<KeyType>::ConvertToLeftistTree(LeftistNode<KeyType>* node) {
    if (!node) {
        return nullptr;
    }
    
    // First, recursively convert left and right subtrees
    node->setLeftChild(ConvertToLeftistTree(node->getLeftChild()));
    node->setRightChild(ConvertToLeftistTree(node->getRightChild()));
    
    // Calculate shortest paths
    int leftShortest = (node->getLeftChild()) ? node->getLeftChild()->getShortest() : 0;
    int rightShortest = (node->getRightChild()) ? node->getRightChild()->getShortest() : 0;
    
    // If the leftist property is violated, swap the children
    if ((node->getLeftChild() == nullptr && node->getRightChild() != nullptr) || 
        (node->getLeftChild() != nullptr && node->getRightChild() != nullptr && 
         leftShortest < rightShortest)) {
        LeftistNode<KeyType>* temp = node->getLeftChild();
        node->setLeftChild(node->getRightChild());
        node->setRightChild(temp);
    }
    
    // Update the shortest path value for this node
    if (!node->getRightChild()) {
        node->setShortest(1);
    } else {
        node->setShortest(node->getRightChild()->getShortest() + 1);
    }
    
    return node;
}

// Insert implementation
template <typename KeyType>
void MinLeftistTree<KeyType>::Insert(const Element<KeyType>& element) {
    // Create a new leftist node with the given element
    LeftistNode<KeyType>* newNode = new LeftistNode<KeyType>(element);
    
    // Create a temporary leftist tree with the new node as root
    MinLeftistTree<KeyType> tempTree(newNode);
    
    // Combine the current tree with the temporary tree
    MinCombine(&tempTree);
}

// DeleteMin implementation
template <typename KeyType>
Element<KeyType>* MinLeftistTree<KeyType>::DeleteMin(Element<KeyType>& minElement) {
    // Check if the tree is empty
    if (!root) {
        return nullptr;  // Cannot delete from an empty tree
    }
    
    // Store the minimum element (at the root)
    minElement = root->getData();
    
    // Save pointers to the left and right subtrees
    LeftistNode<KeyType>* leftTree = root->getLeftChild();
    LeftistNode<KeyType>* rightTree = root->getRightChild();
    
    // Delete the root node
    delete root;
    
    // If one of the subtrees is empty, the result is the other subtree
    if (!leftTree) {
        root = rightTree;
    } else if (!rightTree) {
        root = leftTree;
    } else {
        // Combine the left and right subtrees using MinUnion
        root = MinUnion(leftTree, rightTree);
    }
    
    return &minElement;
}

template <typename KeyType>
void MinLeftistTree<KeyType>::DeleteElement(Element<KeyType>* element) {
    if (!element || !root) return;

    // Step 1: Find the node containing the element using level-order traversal
    LeftistNode<KeyType>* nodeToDelete = nullptr;
    std::queue<LeftistNode<KeyType>*> q;
    q.push(root);

    while (!q.empty() && !nodeToDelete) {
        LeftistNode<KeyType>* current = q.front();
        q.pop();

        if (current->getData().getKey() == element->getKey()) {
            nodeToDelete = current;
        } else {
            if (current->getLeftChild()) {
                current->getLeftChild()->setParent(current);  // Ensure parent links are set
                q.push(current->getLeftChild());
            }
            if (current->getRightChild()) {
                current->getRightChild()->setParent(current);  // Ensure parent links are set
                q.push(current->getRightChild());
            }
        }
    }

    if (!nodeToDelete) return;  // Element not found

    // Step 2: If node is root, handle specially
    if (nodeToDelete == root) {
        LeftistNode<KeyType>* leftChild = root->getLeftChild();
        LeftistNode<KeyType>* rightChild = root->getRightChild();
        
        delete root;
        
        if (!leftChild) {
            root = rightChild;
            if (root) root->setParent(nullptr);
        } else if (!rightChild) {
            root = leftChild;
            if (root) root->setParent(nullptr);
        } else {
            root = MinUnion(leftChild, rightChild);
            root->setParent(nullptr);
        }
        return;
    }

    // Step 3: For non-root nodes
    LeftistNode<KeyType>* parent = nodeToDelete->getParent();
    LeftistNode<KeyType>* leftChild = nodeToDelete->getLeftChild();
    LeftistNode<KeyType>* rightChild = nodeToDelete->getRightChild();

    // Determine which child of parent is being deleted
    bool isLeftChild = (parent->getLeftChild() == nodeToDelete);

    // Merge children of the node being deleted
    LeftistNode<KeyType>* mergedChildren = nullptr;
    if (leftChild && rightChild) {
        mergedChildren = MinUnion(leftChild, rightChild);
        if (mergedChildren) mergedChildren->setParent(parent);
    } else {
        mergedChildren = leftChild ? leftChild : rightChild;
        if (mergedChildren) mergedChildren->setParent(parent);
    }

    // Update parent's child pointer
    if (isLeftChild) {
        parent->setLeftChild(mergedChildren);
    } else {
        parent->setRightChild(mergedChildren);
    }

    // Delete the node
    delete nodeToDelete;

    // Ensure the tree maintains leftist property
    while (parent) {
        // Check and maintain leftist property
        LeftistNode<KeyType>* parentLeft = parent->getLeftChild();
        LeftistNode<KeyType>* parentRight = parent->getRightChild();
        
        int leftShortest = parentLeft ? parentLeft->getShortest() : 0;
        int rightShortest = parentRight ? parentRight->getShortest() : 0;
        
        // Swap if leftist property is violated
        if (!parentLeft || (parentRight && leftShortest < rightShortest)) {
            parent->setLeftChild(parentRight);
            parent->setRightChild(parentLeft);
            if (parentLeft) parentLeft->setParent(parent);
            if (parentRight) parentRight->setParent(parent);
        }
        
        // Update shortest path
        parent->setShortest(1 + (parent->getRightChild() ? parent->getRightChild()->getShortest() : 0));
        
        parent = parent->getParent();
    }
}

template <typename KeyType>
void MinLeftistTree<KeyType>::PrintTree() const {
    if (root) {
        std::cout << "root: " << root->getData().getKey() << "(" << root->getShortest() << ")" << std::endl;

        // Initialize arrays for current level and parents
        int currentLevelSize = 1;
        LeftistNode<KeyType>** currentLevel = new LeftistNode<KeyType>*[currentLevelSize];
        currentLevel[0] = root;

        LeftistNode<KeyType>** parents = new LeftistNode<KeyType>*[currentLevelSize];
        parents[0] = nullptr;

        while (currentLevelSize > 0) {
            // Calculate the size of the next level
            int nextLevelSize = 0;
            for (int i = 0; i < currentLevelSize; i++) {
                if (currentLevel[i]->getLeftChild()) nextLevelSize++;
                if (currentLevel[i]->getRightChild()) nextLevelSize++;
            }

            // Allocate arrays for the next level and new parents
            LeftistNode<KeyType>** nextLevel = new LeftistNode<KeyType>*[nextLevelSize];
            LeftistNode<KeyType>** newParents = new LeftistNode<KeyType>*[nextLevelSize];

            // Populate the next level and new parents arrays
            int index = 0;
            for (int i = 0; i < currentLevelSize; i++) {
                LeftistNode<KeyType>* node = currentLevel[i];
                LeftistNode<KeyType>* parent = parents[i];

                if (node->getLeftChild()) {
                    nextLevel[index] = node->getLeftChild();
                    newParents[index] = node;
                    index++;
                }
                if (node->getRightChild()) {
                    nextLevel[index] = node->getRightChild();
                    newParents[index] = node;
                    index++;
                }
            }

            // Print the next level
            if (nextLevelSize > 0) {
                for (int i = 0; i < nextLevelSize; i++) {
                    LeftistNode<KeyType>* node = nextLevel[i];
                    LeftistNode<KeyType>* parent = newParents[i];

                    std::cout << node->getData().getKey() << "(" << node->getShortest() << ")";
                    if (parent) {
                        std::cout << "<under " << parent->getData().getKey() << ">";
                    }

                    if (i < nextLevelSize - 1) {
                        std::cout << " ";
                    }
                }
                std::cout << std::endl;
            }

            // Clean up the current level and parents arrays
            delete[] currentLevel;
            delete[] parents;

            // Update for the next iteration
            currentLevel = nextLevel;
            parents = newParents;
            currentLevelSize = nextLevelSize;
        }

        // Clean up the final arrays
        delete[] currentLevel;
        delete[] parents;
    } else {
        std::cout << "Empty tree" << std::endl;
    }
}
