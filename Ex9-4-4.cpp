/**
 * Solutions to Chapter 9, Section 4, Exercise 4 of Horowitz, Sahni, and Mehta's
 * Fundamentals of Data Structure in C++, 1st Edition.
 * 
 * Task 4:
 * Compare the performance of leftist trees and binomial heaps under the
 * assumption that the only permissible operations are insert and delete-min.
 * For this, do the following:
 * (a) Create a random list of n elements and a random sequence of insert and
 *     delete-min operations of length m. The number of delete-mins and inserts
 *     should be approximately equal. Initialize a min leftist tree and a
 *     binomial heap to contain the n elements in the first random list. Now,
 *     measure the time to perform the m operations using the min leftist tree
 *     as well as the binomial heap. Divide this time by n to get the average
 *     time per operation. Do this for n = 100, 200, 500, 1000, 2000, ..., 5000.
 *     Let m = 5000. Tabulate your computing times.
 * (b) Based on your experiments, make some statements about the relative merits
 *     of the two data structures.
 */

/**********************************main.cpp************************************/
#include <iostream>
#include <stdexcept>
#include <vector>
#include <random>
#include <chrono>

#include "MinLeftistTree.h"
#include "BinomialTree.h"

// Function to generate a random list of n elements using Mersenne Twister
Element<int>* generateRandomList(int n, std::mt19937& rng) {
    std::uniform_int_distribution<int> dist(0, 9999); // Random keys between 0 and 9999
    Element<int>* elements = new Element<int>[n];
    for (int i = 0; i < n; ++i) {
        elements[i] = Element<int>(dist(rng));
    }
    return elements;
}

// Function to generate a random sequence of m operations (Insert or DeleteMin) using Mersenne Twister
char* generateRandomOperations(int m, std::mt19937& rng) {
    std::uniform_int_distribution<int> dist(0, 1); // 0 for Insert, 1 for DeleteMin
    char* operations = new char[m];
    for (int i = 0; i < m; ++i) {
        operations[i] = (dist(rng) == 0) ? 'I' : 'D'; // 'I' for Insert, 'D' for DeleteMin
    }
    return operations;
}

// Function to measure the time taken to perform m operations on a MinHeap
double measureBinomialTreeTime(BinomialTree<int>& bTree, char* operations, int m, std::mt19937& rng) {
    std::uniform_int_distribution<int> dist(0, 9999); // Random keys between 0 and 9999
    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < m; ++i) {
        if (operations[i] == 'I') {
            bTree.Insert(Element<int>(dist(rng))); // Insert a random element
        } else {
            Element<int> minElement;
            bTree.DeleteMin(minElement); // Delete the minimum element
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::micro> duration = end - start;
    return duration.count();
}

// Function to measure the time taken to perform m operations on a MinLeftistTree
double measureMinLeftistTreeTime(MinLeftistTree<int>& tree, char* operations, int m, std::mt19937& rng) {
    std::uniform_int_distribution<int> dist(0, 9999); // Random keys between 0 and 9999
    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < m; ++i) {
        if (operations[i] == 'I') {
            tree.Insert(Element<int>(dist(rng))); // Insert a random element
        } else {
            Element<int> minElement;
            tree.DeleteMin(minElement); // Delete the minimum element
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::micro> duration = end - start;
    return duration.count();
}

int main() {
    // Seed the Mersenne Twister engine
    std::random_device rd; // True random number generator for seeding
    std::mt19937 rng(rd()); // Mersenne Twister engine

    // Parameters
    int n = 100; // Number of initial elements
    
    // Sequence lengths
    int mValues[] = {100, 200, 500, 1000, 2000, 3000, 4000, 5000, 10000};

    // Generate a random list of n elements
    Element<int>* initialElements = generateRandomList(n, rng);
    
    // Initialize MinHeap and MinLeftistTree with the initial elements
    BinomialTree<int> bTree;
    bTree.Initialize(initialElements, n);

    MinLeftistTree<int> minLeftistTree;
    minLeftistTree.Initialize(initialElements, n);

    // Test for different values of m
    for (int m : mValues) {
        // Generate a random sequence of m operations
        char* operations = generateRandomOperations(m, rng);
        
        // Measure time for MinHeap
        double bTreeTime = measureBinomialTreeTime(bTree, operations, m, rng);
        double avgBinomialTreeTime = bTreeTime / m;

        // Measure time for MinLeftistTree
        double LeftistTreeTime = measureMinLeftistTreeTime(minLeftistTree, operations, m, rng);
        double avgLeftistTreeTime = LeftistTreeTime / m;

        // Print results
        std::cout << "m = " << m << ":\n";
        std::cout << "  BinomialTree: Total Time = " << bTreeTime << " microseconds, Avg Time = " << avgBinomialTreeTime << " microseconds/op\n";
        std::cout << "  MinLeftistTree: Total Time = " << LeftistTreeTime << " microseconds, Avg Time = " << avgLeftistTreeTime << " microseconds/op\n";
        std::cout << "----------------------------------------\n";

        // Clean up
        delete[] operations;
    }
    
    return 0;
}



/*********************************Element.h************************************/
#ifndef ELEMENT_H
#define ELEMENT_H

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
    bool operator<(const Element<KeyType>& other) const { return key < other.getKey(); }
};

#endif // ELEMENT_H



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



/*********************************MinHeap.h************************************/
#ifndef MIN_HEAP_H
#define MIN_HEAP_H

#include "Element.h"

//MinHeap abstract base class
template <typename KeyType>
class MinHeap : public MinPQ<KeyType> {
private:
    Element<KeyType>* heap; // Dynamic array to store elements
    int capacity;           // Maximum capacity of the heap
    int size;               // Current size of the heap

    void heapifyDown(int index);
    void resizeHeap();

public:
    virtual ~MinHeap() {};

    virtual void Insert(const Element<KeyType>& element) = 0;
    virtual Element<KeyType>* DeleteMin(Element<KeyType>& minElement) = 0;
    bool IsEmpty() const { return size == 0; }

    // Initialize the heap with an array of elements
    void Initialize(const Element<KeyType> elements[], int numElements);
};

#endif // MIN_HEAP_H



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
    LeftistNode *LeftChild, *RightChild;
    int shortest;

  public:
    LeftistNode() : LeftChild(nullptr), RightChild(nullptr), shortest(1) {}
    LeftistNode(const Element<KeyType>& el) : data(el), LeftChild(nullptr), RightChild(nullptr), shortest(1) {}

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

    // Getter for shortest
    int getShortest() const { return shortest; }

    // Setter for shortest
    void setShortest(int s) { shortest = s; }
};

#endif // LEFTIST_NODE_H



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

/**
 * Solution to Chapter 9, Section 3, Exercise 3, Part 1.
 * A function with Θ(n) time complexity that initializes a minimum leftist tree.
 */
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
    if (!node) return nullptr;
    
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



/*******************************BinomialNode.h*********************************/
#ifndef BINOMIAL_NODE_H
#define BINOMIAL_NODE_H

#include "Element.h"

template <typename KeyType> class BinomialTree; //forward declaration

// BinomialNode definition remains unchanged
template <typename KeyType>
class BinomialNode {
    friend class BinomialTree<KeyType>;
  public:
    Element<KeyType> getData() const { return data; }
    void setData(const Element<KeyType>& el) { data = el; }
    BinomialNode<KeyType>* getChild() const { return child; }
    void setChild(BinomialNode<KeyType>* c) { child = c; }
    BinomialNode<KeyType>* getLink() const { return link; }
    void setLink(BinomialNode<KeyType>* l) { link = l; }
    int getDegree() const { return degree; }
    void setDegree(const int& d) { degree = d; }
  private:
    Element<KeyType> data;
    BinomialNode<KeyType> *child, *link;
    int degree;
};

#endif //BINOMIAL_NODE_H



/*******************************BinomialTree.h*********************************/
#ifndef BINOMIAL_TREE_H
#define BINOMIAL_TREE_H

#include "MinHeap.h"
#include "BinomialNode.h"

template <typename KeyType>
class BinomialTree : public MinHeap<KeyType> {
  public:
    void Initialize(Element<KeyType> elements[], int n);
    void PrintTree();
    BinomialTree(BinomialNode<KeyType>* init = nullptr) : min(init) {}
    void Insert(const Element<KeyType>&);
    Element<KeyType>* DeleteMin(Element<KeyType>&);
    void MinCombine(BinomialTree<KeyType>*);
  private:
    void DeletionError();
    void JoinMinTrees(BinomialNode<KeyType>* p, BinomialNode<KeyType>* q);
    void PrintSubtree(BinomialNode<KeyType>* node, BinomialNode<KeyType>* parent, int level);
    BinomialNode<KeyType>* min;
};

#include "BinomialTree.tpp"

#endif // BINOMIAL_TREE_H



/******************************BinomialNode.tpp********************************/
//Implementation file for the header file BinomialTree.h
template <typename KeyType>
void BinomialTree<KeyType>::DeletionError() {
    std::cerr << "Binomial Heap is empty.\n" << std::endl;
}

template <typename KeyType>
void BinomialTree<KeyType>::Initialize(Element<KeyType> elements[], int n) {
    min = nullptr;
    if (n <= 0) return;

    std::vector<BinomialNode<KeyType>*> trees;
    for (int i = 0; i < n; ++i) {
        BinomialNode<KeyType>* newNode = new BinomialNode<KeyType>;
        newNode->data = elements[i];
        newNode->child = nullptr;
        newNode->link = newNode;
        newNode->degree = 0;
        trees.push_back(newNode);
    }

    const int MAX_DEGREE = 32;
    std::vector<BinomialNode<KeyType>*> degreeTable(MAX_DEGREE, nullptr);
    for (auto p : trees) {
        BinomialNode<KeyType>* currentTree = p;
        int d = currentTree->degree;
        for (; degreeTable[d] != nullptr; d++) {
            BinomialNode<KeyType>* otherTree = degreeTable[d];
            degreeTable[d] = nullptr;
            if (currentTree->getData().getKey() <= otherTree->getData().getKey()) {
                JoinMinTrees(currentTree, otherTree);
            } else {
                JoinMinTrees(otherTree, currentTree);
                currentTree = otherTree;
            }
        }
        degreeTable[d] = currentTree;
    }

    std::vector<BinomialNode<KeyType>*> newRoots;
    for (int i = 0; i < MAX_DEGREE; ++i) {
        if (degreeTable[i]) newRoots.push_back(degreeTable[i]);
    }

    if (newRoots.empty()) {
        min = nullptr;
    } else {
        min = newRoots[0];
        for (size_t i = 0; i < newRoots.size() - 1; ++i) {
            newRoots[i]->link = newRoots[i + 1];
        }
        newRoots.back()->link = newRoots[0];

        BinomialNode<KeyType>* current = min;
        BinomialNode<KeyType>* newMin = min;
        do {
            if (current->getData().getKey() < newMin->getData().getKey()) {
                newMin = current;
            }
            current = current->link;
        } while (current != min);
        min = newMin;
    }
}

template <typename KeyType>
void BinomialTree<KeyType>::PrintSubtree(BinomialNode<KeyType>* node, BinomialNode<KeyType>* parent, int level) {
    if (!node) return;
    std::string indent(level * 4, ' ');
    if (level == 0) {
        std::cout << indent << "Key: " << node->getData().getKey()
                  << " (Root, Degree: " << node->degree << ")" << std::endl;
    } else {
        std::cout << indent << "Key: " << node->getData().getKey()
                  << " (Child of " << parent->getData().getKey() << ")" << std::endl;
    }
    if (node->child) {
        BinomialNode<KeyType>* child = node->child;
        do {
            PrintSubtree(child, node, level + 1);
            child = child->link;
        } while (child != node->child);
    }
}

template <typename KeyType>
void BinomialTree<KeyType>::PrintTree() {
    if (!min) {
        std::cout << "Empty binomial heap." << std::endl;
        return;
    }
    std::vector<BinomialNode<KeyType>*> rootNodes;
    BinomialNode<KeyType>* current = min;
    do {
        rootNodes.push_back(current);
        current = current->link;
    } while (current != min);

    // Sort by degree for consistent output
    for (size_t i = 0; i < rootNodes.size() - 1; i++) {
        for (size_t j = 0; j < rootNodes.size() - i - 1; j++) {
            if (rootNodes[j]->degree > rootNodes[j + 1]->degree) {
                std::swap(rootNodes[j], rootNodes[j + 1]);
            }
        }
    }

    for (auto node : rootNodes) {
        std::cout << "Tree B" << node->degree << ":" << std::endl;
        PrintSubtree(node, nullptr, 0);
        std::cout << std::endl;
    }
}

template <typename KeyType>
void BinomialTree<KeyType>::Insert(const Element<KeyType>& x) {
    BinomialNode<KeyType>* newNode = new BinomialNode<KeyType>;
    newNode->data = x;
    newNode->child = nullptr;
    newNode->link = newNode;
    newNode->degree = 0;

    BinomialTree<KeyType> tempHeap(newNode);
    MinCombine(&tempHeap);
}

template <typename KeyType>
void BinomialTree<KeyType>::JoinMinTrees(BinomialNode<KeyType>* parent, BinomialNode<KeyType>* child) {
    child->link = child; // Ensure child is a single-node circular list
    if (!parent->child) {
        parent->child = child;
    } else {
        BinomialNode<KeyType>* oldChild = parent->child;
        BinomialNode<KeyType>* lastChild = oldChild;
        while (lastChild->link != oldChild) {
            lastChild = lastChild->link;
        }
        child->link = oldChild;
        lastChild->link = child;
        parent->child = child;
    }
    parent->degree++;
}

template <typename KeyType>
Element<KeyType>* BinomialTree<KeyType>::DeleteMin(Element<KeyType>& x) {
    if (!min) 
        throw std::runtime_error("Cannot delete minimum from an empty binomial heap");

    x = min->getData();
    BinomialNode<KeyType>* oldMin = min;
    BinomialNode<KeyType>* child = min->getChild();

    if (min->link == min) {
        min = nullptr;
    } else {
        BinomialNode<KeyType>* prev = min;
        while (prev->link != min) prev = prev->link;
        prev->link = min->link;
        min = min->link;
    }

    BinomialNode<KeyType>* current = nullptr;
    if (child) {
        current = child;
        do {
            BinomialNode<KeyType>* next = current->link;
            current->link = current;
            if (min) {
                BinomialNode<KeyType>* tail = min;
                while (tail->link != min) tail = tail->link;
                tail->link = current;
                current->link = min;
            } else {
                min = current;
            }
            current = (next == child) ? nullptr : next;
        } while (current);
    }

    delete oldMin;
    if (!min) return &x;

    // Consolidate with fixed array
    const int MAX_DEGREE = 32;
    BinomialNode<KeyType>* degreeTable[MAX_DEGREE] = {nullptr};
    current = min;
    BinomialNode<KeyType>* start = min;
    do {
        BinomialNode<KeyType>* next = current->link;
        BinomialNode<KeyType>* p = current;
        int d = p->degree;
        for (; degreeTable[d] != nullptr; d++) {
            BinomialNode<KeyType>* other = degreeTable[d];
            degreeTable[d] = nullptr;
            if (p->getData().getKey() <= other->getData().getKey()) {
                JoinMinTrees(p, other);
            } else {
                JoinMinTrees(other, p);
                p = other;
            }
        }
        degreeTable[d] = p;
        current = next;
    } while (current != start && current); // Added null check

    // Rebuild list explicitly
    min = nullptr;
    BinomialNode<KeyType>* last = nullptr;
    for (int i = 0; i < MAX_DEGREE; ++i) {
        if (degreeTable[i]) {
            if (!min) {
                min = degreeTable[i];
                last = min;
            } else {
                last->link = degreeTable[i];
                last = degreeTable[i];
            }
        }
    }
    if (last) last->link = min;

    if (min) {
        current = min;
        BinomialNode<KeyType>* newMin = min;
        do {
            if (current->getData().getKey() < newMin->getData().getKey()) newMin = current;
            current = current->link;
        } while (current != min);
        min = newMin;
    }

    return &x;
}

template <typename KeyType>
void BinomialTree<KeyType>::MinCombine(BinomialTree<KeyType>* other) {
    if (!other->min) return;
    if (!min) {
        min = other->min;
        other->min = nullptr;
        return;
    }

    // Merge circular lists only
    BinomialNode<KeyType>* thisTail = min;
    while (thisTail->link != min) thisTail = thisTail->link;
    BinomialNode<KeyType>* otherTail = other->min;
    while (otherTail->link != other->min) otherTail = otherTail->link;

    thisTail->link = other->min;
    otherTail->link = min;

    // Update min
    if (other->min->getData().getKey() < min->getData().getKey()) {
        min = other->min;
    }

    other->min = nullptr;
}
