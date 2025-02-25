/**
 * Solutions to Chapter 9, Section 3, Exercise 6 of Horowitz, Sahni, and Mehta's 
 * Fundamentals of Data Structures in C++, 1st Edition.
 * 
 * Task 6: Skewed Heap
 * A skewed heap is a minimum tree that supports the minimum leftist tree
 * operations: Insert, DeleteMin, and Combine in amortized time of O(log n) per 
 * operation. As in the case of min leftist trees, insertions and deletions are 
 * performed using the Combine operation, which is carried out by following the 
 * rightmost paths in the two heaps being combined. However, unlike minimum 
 * leftist trees, the left and right subtrees of all nodes (except the last) on 
 * the rightmost path in the resulting heap are interchanged.
 * (a) Write insert, delete-min, and combine functions for skewed heaps.
 * (b) Compare the running times of these with those for the same operations on
 *     a min leftist tree. Use random sequences of insert, delete-min, and
 *     combine operations.
 * 
 * Response to conceptual questions.
 * (b) Skewed heaps are slightly faster than minimum leftist tree across all 
 *     values of m, due to cost amortization.
 */

/*********************************main.cpp*************************************/
#include <iostream>
#include <queue>
#include <algorithm>
#include <random>
#include <chrono>
#include "SkewedHeap.h"
#include "MinLeftistTree.h"

// Function to generate a random list of n elements using Mersenne Twister
Element<int>* generateRandomList(int n, std::mt19937& rng) {
    std::uniform_int_distribution<int> dist(0, 999); // Random keys between 0 and 999
    Element<int>* elements = new Element<int>[n];
    for (int i = 0; i < n; ++i) {
        elements[i] = Element<int>(dist(rng));
    }
    return elements;
}

// Function to generate a random sequence of m operations (Insert, DeleteMin, Combine) using Mersenne Twister
char* generateRandomOperations(int m, std::mt19937& rng) {
    std::uniform_int_distribution<int> dist(0, 2); // 0 for Insert, 1 for DeleteMin, 2 for Combine
    char* operations = new char[m];
    for (int i = 0; i < m; ++i) {
        int op = dist(rng);
        if (op == 0) {
            operations[i] = 'I'; // 'I' for Insert
        } else if (op == 1) {
            operations[i] = 'D'; // 'D' for DeleteMin
        } else {
            operations[i] = 'C'; // 'C' for Combine
        }
    }
    return operations;
}

// Function to measure the time taken to perform m operations on a Skewed Heap
double measureSkewedHeapTime(SkewedHeap<int>& skewedHeap, 
                             char* operations,
                             int m, 
                             std::mt19937& rng) {
    std::uniform_int_distribution<int> dist(0, 999); // Random keys between 0 and 999
    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < m; ++i) {
        if (operations[i] == 'I') {
            skewedHeap.Insert(Element<int>(dist(rng))); // Insert a random element
        } else if (operations[i] == 'D') {
            Element<int> minElement;
            skewedHeap.DeleteMin(minElement); // Delete the minimum element
        } else {
            SkewedHeap<int> tempHeap;
            tempHeap.Insert(Element<int>(dist(rng)));
            skewedHeap.Combine(&tempHeap);
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::micro> duration = end - start;
    return duration.count();
}

// Function to measure the time taken to perform m operations on a MinLeftistTree
double measureMinLeftistTreeTime(MinLeftistTree<int>& tree, 
                                 char* operations, 
                                 int m, 
                                 std::mt19937& rng) {
    std::uniform_int_distribution<int> dist(0, 9999); // Random keys between 0 and 999
    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < m; ++i) {
        if (operations[i] == 'I') {
            tree.Insert(Element<int>(dist(rng))); // Insert a random element
        } else if (operations[i] == 'D') {
            Element<int> minElement;
            tree.DeleteMin(minElement); // Delete the minimum element
        } else {
            MinLeftistTree<int> tempTree;
            tempTree.Insert(Element<int>(dist(rng)));
            tree.MinCombine(&tempTree);            
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::micro> duration = end - start;
    return duration.count();
}


int main() {
    std::cout << "==========TEST 1: Tests for correctness.==========" << std::endl;
    
    SkewedHeap<int> heap;
    Element<int> elements[] = {1, 10, 5, 20, 3, 8, 35, 40, 18, 21};

    heap.Initialize(elements, 10);
    std::cout << "Initial heap:" << std::endl;
    heap.PrintTree();

    // Test deletions
    int keysToDelete[] = {3, 5, 8, 10, 18, 21, 35, 40};
    for (int key : keysToDelete) {
        Element<int>* elementToDelete = new Element<int>(key);
        std::cout << "\nDeleting " << key << ":" << std::endl;
        heap.DeleteElement(elementToDelete);
        heap.PrintTree();
        delete elementToDelete;
    }

    std::cout << "\n\n==========TEST 2: Timing experiments.==========" << std::endl;
    
    // Seed the Mersenne Twister engine
    std::random_device rd; // True random number generator for seeding
    std::mt19937 rng(rd()); // Mersenne Twister engine

    // Parameters
    int n = 100; // Number of initial elements
    
    // Sequence lengths
    int mValues[] = {100, 200, 500, 1000, 2000, 3000, 4000, 5000, 10000};

    // Generate a random list of n elements
    Element<int>* initialElements = generateRandomList(n, rng);

    SkewedHeap<int> skewedHeap;
    skewedHeap.Initialize(initialElements, n);

    MinLeftistTree<int> minLeftistTree;
    minLeftistTree.Initialize(initialElements, n);

    // Test for different values of m
    for (int m : mValues) {
        // Generate a random sequence of m operations
        char* operations = generateRandomOperations(m, rng);
        
        // Measure time for MinHeap
        double skewedHeapTime = measureSkewedHeapTime(skewedHeap, operations, m, rng);
        double avgSkewedHeapTime = skewedHeapTime / m;

        // Measure time for MinLeftistTree
        double minLeftistTreeTime = measureMinLeftistTreeTime(minLeftistTree, operations, m, rng);
        double avgMinLeftistTreeTime = minLeftistTreeTime / m;

        // Print results
        std::cout << "m = " << m << ":\n";
        std::cout << "  Skewed Heap: Total Time = " << skewedHeapTime << " microseconds, Avg Time = " << avgSkewedHeapTime << " microseconds/op\n";
        std::cout << "  Min Leftist Tree: Total Time = " << minLeftistTreeTime << " microseconds, Avg Time = " << avgMinLeftistTreeTime << " microseconds/op\n";
        std::cout << "-----------------------------------------------\n";

        // Clean up
        delete[] operations;
    }

    // Clean up
    delete[] initialElements;

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

    // Getter for key
    KeyType getKey() const { return key; }

    // Setter for key
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
        
    // Getter for data
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



/*******************************SkewedHeap.h***********************************/
#ifndef SKEWED_HEAP_H
#define SKEWED_HEAP_H

#include "MinPQ.h"
#include "LeftistNode.h"

template <typename KeyType>
class SkewedHeap : public MinPQ<KeyType> {
  public:
    SkewedHeap(LeftistNode<KeyType> *init = nullptr) : root(init) {}
    ~SkewedHeap();
    void Insert(const Element<KeyType>&) override;
    Element<KeyType>* DeleteMin(Element<KeyType>&) override;
    void Combine(SkewedHeap<KeyType>*);
    void PrintTree() const; // Helper for visualization
    bool IsEmpty() const { return root == nullptr; }
    void Initialize(const Element<KeyType>* elements, int numElements);
    void DeleteElement(Element<KeyType>* element);

  private:
    LeftistNode<KeyType>* Merge(LeftistNode<KeyType>*, LeftistNode<KeyType>*);
    void DeleteHelper(LeftistNode<KeyType>*); // For destructor
    LeftistNode<KeyType> *root;
};

#include "SkewedHeap.tpp"

#endif // SKEWED_HEAP_H



/******************************SkewedHeap.tpp**********************************/
// Destructor implementation
template <typename KeyType>
SkewedHeap<KeyType>::~SkewedHeap() {
    DeleteHelper(root);
}

template <typename KeyType>
void SkewedHeap<KeyType>::DeleteHelper(LeftistNode<KeyType>* node) {
    if (node) {
        DeleteHelper(node->getLeftChild());
        DeleteHelper(node->getRightChild());
        delete node;
    }
}

// Initialize the heap with an array of elements
template <typename KeyType>
void SkewedHeap<KeyType>::Initialize(const Element<KeyType> elements[], int numElements) {
    // Clear the existing heap
    DeleteHelper(root);
    root = nullptr;

    // Insert each element into the heap
    for (int i = 0; i < numElements; ++i) {
        Insert(elements[i]);
    }
}

// Merge function for skewed heaps
template <typename KeyType>
LeftistNode<KeyType>* SkewedHeap<KeyType>::Merge(LeftistNode<KeyType>* a, LeftistNode<KeyType>* b) {
    if (!a) return b;
    if (!b) return a;

    // Ensure 'a' is the smaller root
    if (a->getData().getKey() > b->getData().getKey()) {
        std::swap(a, b);
    }

    // Recursively merge the right subtree of 'a' with 'b'
    a->setRightChild(Merge(a->getRightChild(), b));

    // Swap the left and right subtrees to maintain the skewed property
    LeftistNode<KeyType>* temp = a->getLeftChild();
    a->setLeftChild(a->getRightChild());
    a->setRightChild(temp);
    
    //std::swap(a->getLeftChild(), a->getRightChild());

    return a;
}

// Insert implementation
template <typename KeyType>
void SkewedHeap<KeyType>::Insert(const Element<KeyType>& element) {
    // Create a new node with the given element
    LeftistNode<KeyType>* newNode = new LeftistNode<KeyType>(element);

    // Merge the new node with the existing heap
    root = Merge(root, newNode);
}

// DeleteMin implementation
template <typename KeyType>
Element<KeyType>* SkewedHeap<KeyType>::DeleteMin(Element<KeyType>& minElement) {
    if (!root) {
        return nullptr; // Cannot delete from an empty heap
    }

    // Store the minimum element (at the root)
    minElement = root->getData();

    // Save pointers to the left and right subtrees
    LeftistNode<KeyType>* leftChild = root->getLeftChild();
    LeftistNode<KeyType>* rightChild = root->getRightChild();

    // Delete the root node
    delete root;

    // Merge the left and right subtrees
    root = Merge(leftChild, rightChild);

    return &minElement;
}

// Combine implementation
template <typename KeyType>
void SkewedHeap<KeyType>::Combine(SkewedHeap<KeyType>* other) {
    if (!root) {
        root = other->root;
    } else if (other->root) {
        root = Merge(root, other->root);
    }
    other->root = nullptr; // Prevent double deletion
}

// DeleteElement implementation
template <typename KeyType>
void SkewedHeap<KeyType>::DeleteElement(Element<KeyType>* element) {
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
            if (current->getLeftChild()) q.push(current->getLeftChild());
            if (current->getRightChild()) q.push(current->getRightChild());
        }
    }

    if (!nodeToDelete) return; // Element not found

    // Step 2: Extract the node and merge its children
    LeftistNode<KeyType>* leftChild = nodeToDelete->getLeftChild();
    LeftistNode<KeyType>* rightChild = nodeToDelete->getRightChild();

    // Merge the children
    LeftistNode<KeyType>* mergedChildren = Merge(leftChild, rightChild);

    // Step 3: Replace the node with its merged children
    if (nodeToDelete == root) {
        root = mergedChildren;
    } else {
        // Find the parent of the node to delete
        LeftistNode<KeyType>* parent = nullptr;
        q.push(root);
        while (!q.empty()) {
            LeftistNode<KeyType>* current = q.front();
            q.pop();

            if (current->getLeftChild() == nodeToDelete || current->getRightChild() == nodeToDelete) {
                parent = current;
                break;
            }

            if (current->getLeftChild()) q.push(current->getLeftChild());
            if (current->getRightChild()) q.push(current->getRightChild());
        }

        if (parent) {
            if (parent->getLeftChild() == nodeToDelete) {
                parent->setLeftChild(mergedChildren);
            } else {
                parent->setRightChild(mergedChildren);
            }
        }
    }

    // Delete the node
    delete nodeToDelete;
}

// PrintTree implementation
template <typename KeyType>
void SkewedHeap<KeyType>::PrintTree() const {
    if (root) {
        // Print the root
        std::cout << "root: " << root->getData().getKey() << std::endl;

        // Use a queue for level-order traversal
        std::queue<LeftistNode<KeyType>*> q;
        q.push(root);

        while (!q.empty()) {
            int levelSize = q.size();
            for (int i = 0; i < levelSize; ++i) {
                LeftistNode<KeyType>* current = q.front();
                q.pop();

                // Print left child
                if (current->getLeftChild()) {
                    std::cout << current->getLeftChild()->getData().getKey() 
                              << "<under " << current->getData().getKey() << "> ";
                    q.push(current->getLeftChild());
                }

                // Print right child
                if (current->getRightChild()) {
                    std::cout << current->getRightChild()->getData().getKey() 
                              << "<under " << current->getData().getKey() << "> ";
                    q.push(current->getRightChild());
                }
            }
            std::cout << std::endl; // Move to the next line after printing a level
        }
    } else {
        std::cout << "Empty heap" << std::endl;
    }
}



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
  private:
    int InitializeShortestPath(LeftistNode<KeyType>*); // Workhorse function
    LeftistNode<KeyType>* ConvertToLeftistTree(LeftistNode<KeyType>*); // Workhorse function
    LeftistNode<KeyType>* MinUnion(LeftistNode<KeyType>*, LeftistNode<KeyType>*);
    void DeleteHelper(LeftistNode<KeyType>*); // For destructor
    LeftistNode<KeyType> *root;
};

#include "MinLeftistTree.tpp"

#endif // MIN_LEFTIST_TREE_H



/*****************************MinLeftistTree.tpp*******************************/
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
