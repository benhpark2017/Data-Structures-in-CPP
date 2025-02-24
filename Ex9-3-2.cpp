/**
 * Solutions to Chapter 9, Section 3, Exercises 2 and 3 of Horowitz, Sahni, and
 * Mehta's Fundamentals of Data Structures in C++, 1st Edition.
 * 
 * Task 2:
 * Compare the performance of leftist trees and min heaps under the assumption
 * that the only operations to be performed are insert and delete-min. For this,
 * do the following:
 * (a) The probability of an insert or delete-min operation is approximately 
 *     0.5. Initialize a min leftist tree and a min heap to contain the n
 *     elements in the first random list. Now, measure the time to perform the m
 *     operations using the min leftist tree as well as the min heap. Divide 
 *     this time by m to get the average time per operation. Do this for m = 
 *     100, 200, 500, 1000, 2000, 3000, 4000, and 5000. Let m = 5000. Tabulate 
 *     your computing times.
 * (b) Based on your experiments, make some statements about the relative merits
 *     of the two priority-queue schemes.
 * 
 * Task 3:
 * Write a function to initialize a min leftist tree with n elements. Assume
 * that the node structure is the same as that used in the text. Your function
 * must run in Θ(n) time. Show that this is the case. Can you think of a way
 * to do this initialization in Θ(n) time such that the resulting min leftist
 * tree is also a complete binary tree?
 */



/*********************************main.cpp*************************************/
#include <iostream>
#include <algorithm>
#include <random>
#include <chrono>
#include <vector>

#include "MinLeftistTree.h"
#include "MinHeap.h"

// Function to generate a random list of n elements using Mersenne Twister
Element<int>* generateRandomList(int n, std::mt19937& rng) {
    std::uniform_int_distribution<int> dist(0, 999); // Random keys between 0 and 999
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
double measureMinHeapTime(MinHeap<int>& heap, char* operations, int m, std::mt19937& rng) {
    std::uniform_int_distribution<int> dist(0, 999); // Random keys between 0 and 999
    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < m; ++i) {
        if (operations[i] == 'I') {
            heap.Insert(Element<int>(dist(rng))); // Insert a random element
        } else {
            Element<int> minElement;
            heap.DeleteMin(minElement); // Delete the minimum element
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::micro> duration = end - start;
    return duration.count();
}

// Function to measure the time taken to perform m operations on a MinLeftistTree
double measureMinLeftistTreeTime(MinLeftistTree<int>& tree, char* operations, int m, std::mt19937& rng) {
    std::uniform_int_distribution<int> dist(0, 999); // Random keys between 0 and 999
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
    MinHeap<int> minHeap;
    minHeap.Initialize(initialElements, n);

    MinLeftistTree<int> minLeftistTree;
    minLeftistTree.Initialize(initialElements, n);

    // Test for different values of m
    for (int m : mValues) {
        // Generate a random sequence of m operations
        char* operations = generateRandomOperations(m, rng);
        
        // Measure time for MinHeap
        double heapTime = measureMinHeapTime(minHeap, operations, m, rng);
        double avgHeapTime = heapTime / m;

        // Measure time for MinLeftistTree
        double treeTime = measureMinLeftistTreeTime(minLeftistTree, operations, m, rng);
        double avgTreeTime = treeTime / m;

        // Print results
        std::cout << "m = " << m << ":\n";
        std::cout << "  MinHeap: Total Time = " << heapTime << " microseconds, Avg Time = " << avgHeapTime << " microseconds/op\n";
        std::cout << "  MinLeftistTree: Total Time = " << treeTime << " microseconds, Avg Time = " << avgTreeTime << " microseconds/op\n";
        std::cout << "----------------------------------------\n";

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

/**
 * Solution to Chapter 9, Section 3, Exercise 3, Part 2.
 * A Θ(n)-complex function that initializes a minimum leftist tree that is also
 * a complete binary tree.
 */
/**
template <typename KeyType>
void MinLeftistTree<KeyType>::Initialize(const Element<KeyType> elements[], int numElements) {
    // Clear existing tree
    DeleteHelper(root);
    root = nullptr;
    
    if (numElements == 0) return;
    
    // Step 1: Create an array of single-node trees
    LeftistNode<KeyType>** currentLevel = new LeftistNode<KeyType>*[numElements];
    for (int i = 0; i < numElements; i++) {
        currentLevel[i] = new LeftistNode<KeyType>(elements[i]);
    }
    
    int currentSize = numElements;
    
    // Step 2: Combine pairs of trees bottom-up
    while (currentSize > 1) {
        // Calculate size of next level
        int nextSize = (currentSize + 1) / 2;
        LeftistNode<KeyType>** nextLevel = new LeftistNode<KeyType>*[nextSize];
        
        // Combine pairs of trees at current level
        int j = 0;
        for (int i = 0; i < currentSize - 1; i += 2) {
            nextLevel[j++] = MinUnion(currentLevel[i], currentLevel[i + 1]);
        }
        
        // Handle odd number of nodes
        if (currentSize % 2 == 1) {
            nextLevel[j] = currentLevel[currentSize - 1];
        }
        
        // Delete old level array, but not the nodes themselves.
        delete[] currentLevel;
        
        // Update for next iteration
        currentLevel = nextLevel;
        currentSize = nextSize;
    }
    
    // Set the root to the final merged tree
    if (currentSize > 0) {
        root = currentLevel[0];
    }
    
    // Clean up the final array
    delete[] currentLevel;
}
*/

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



/**********************************MinHeap.h***********************************/
#ifndef MIN_HEAP_H
#define MIN_HEAP_H

#include "MinPQ.h"

template <typename KeyType>
class MinHeap : public MinPQ<KeyType> {
private:
    Element<KeyType>* heap; // Dynamic array to store elements
    int capacity;           // Maximum capacity of the heap
    int size;               // Current size of the heap

    void heapifyDown(int index);
    void resizeHeap();

public:
    MinHeap(int initialCapacity = 10);
    ~MinHeap();

    void Insert(const Element<KeyType>& element) override;
    Element<KeyType>* DeleteMin(Element<KeyType>& minElement) override;
    bool IsEmpty() const { return size == 0; }

    // Initialize the heap with an array of elements
    void Initialize(const Element<KeyType> elements[], int numElements);

    void PrintHeap() const;
};

#include "MinHeap.tpp"

#endif // MIN_HEAP_H



/**********************************MinHeap.tpp*********************************/
template <typename KeyType>
MinHeap<KeyType>::MinHeap(int initialCapacity) : capacity(initialCapacity), size(0) {
    heap = new Element<KeyType>[capacity];
}

template <typename KeyType>
MinHeap<KeyType>::~MinHeap() {
    delete[] heap;
}

template <typename KeyType>
void MinHeap<KeyType>::resizeHeap() {
    int newCapacity = capacity * 2;
    Element<KeyType>* newHeap = new Element<KeyType>[newCapacity];

    // Copy elements to the new heap
    for (int i = 0; i < size; ++i) {
        newHeap[i] = heap[i];
    }

    // Delete the old heap array
    delete[] heap;

    // Update heap and capacity
    heap = newHeap;
    capacity = newCapacity;
}

template <typename KeyType>
void MinHeap<KeyType>::Initialize(const Element<KeyType> elements[], int numElements) {
    // Clear the existing heap
    delete[] heap;
    size = 0;

    // Allocate memory for the new heap
    capacity = numElements;
    heap = new Element<KeyType>[capacity];

    // Copy elements into the heap
    for (int i = 0; i < numElements; ++i)
        heap[size++] = elements[i];

    // Build the heap using bottom-up heap construction
    for (int i = (size / 2) - 1; i >= 0; --i)
        heapifyDown(i);
}

template <typename KeyType>
void MinHeap<KeyType>::heapifyDown(int index) {
    while (true) {
        int leftChild = 2 * index + 1;
        int rightChild = 2 * index + 2;
        int smallest = index;

        if (leftChild < size && heap[leftChild].getKey() < heap[smallest].getKey())
            smallest = leftChild;
            
        if (rightChild < size && heap[rightChild].getKey() < heap[smallest].getKey())
            smallest = rightChild;

        if (smallest != index) {
            std::swap(heap[index], heap[smallest]);
            index = smallest;
        } else break;
    }
}

template <typename KeyType>
void MinHeap<KeyType>::Insert(const Element<KeyType>& element) {
    if (size == capacity)
        resizeHeap();

    heap[size++] = element;
    int index = size - 1;
    while (index > 0) {
        int parentIndex = (index - 1) / 2;
        if (heap[index].getKey() < heap[parentIndex].getKey()) {
            std::swap(heap[index], heap[parentIndex]);
            index = parentIndex;
        } else break;
    }
}

template <typename KeyType>
Element<KeyType>* MinHeap<KeyType>::DeleteMin(Element<KeyType>& minElement) {
    if (IsEmpty()) return nullptr;
    minElement = heap[0];
    heap[0] = heap[--size];
    if (!IsEmpty()) heapifyDown(0);

    return &minElement;
}

template <typename KeyType>
void MinHeap<KeyType>::PrintHeap() const {
    if (size == 0) {
        std::cout << "Heap is empty" << std::endl;
        return;
    }

    // Print the root
    std::cout << "root: " << heap[0].getKey() << std::endl;

    // Initialize arrays for the current level and parents
    int currentLevelSize = 1;
    int* currentLevelIndices = new int[currentLevelSize];
    currentLevelIndices[0] = 0; // Root is at index 0

    int* parentIndices = new int[currentLevelSize];
    parentIndices[0] = -1; // Root has no parent

    while (currentLevelSize > 0) {
        // Calculate the size of the next level
        int nextLevelSize = 0;
        for (int i = 0; i < currentLevelSize; i++) {
            int leftChild = 2 * currentLevelIndices[i] + 1;
            int rightChild = 2 * currentLevelIndices[i] + 2;

            if (leftChild < size) nextLevelSize++;
            if (rightChild < size) nextLevelSize++;
        }

        // Allocate arrays for the next level and parent indices
        int* nextLevelIndices = new int[nextLevelSize];
        int* nextParentIndices = new int[nextLevelSize];

        // Populate the next level and parent indices arrays
        int index = 0;
        for (int i = 0; i < currentLevelSize; i++) {
            int currentIndex = currentLevelIndices[i];
            int leftChild = 2 * currentIndex + 1;
            int rightChild = 2 * currentIndex + 2;

            if (leftChild < size) {
                nextLevelIndices[index] = leftChild;
                nextParentIndices[index] = currentIndex;
                index++;
            }
            if (rightChild < size) {
                nextLevelIndices[index] = rightChild;
                nextParentIndices[index] = currentIndex;
                index++;
            }
        }

        // Print the next level
        if (nextLevelSize > 0) {
            for (int i = 0; i < nextLevelSize; i++) {
                int nodeIndex = nextLevelIndices[i];
                int parentIndex = nextParentIndices[i];

                std::cout << heap[nodeIndex].getKey();
                if (parentIndex != -1) {
                    std::cout << "<under " << heap[parentIndex].getKey() << ">";
                }

                if (i < nextLevelSize - 1) {
                    std::cout << " ";
                }
            }
            std::cout << std::endl;
        }

        // Clean up the current level and parent indices arrays
        delete[] currentLevelIndices;
        delete[] parentIndices;

        // Update for the next iteration
        currentLevelIndices = nextLevelIndices;
        parentIndices = nextParentIndices;
        currentLevelSize = nextLevelSize;
    }

    // Clean up the final arrays
    delete[] currentLevelIndices;
    delete[] parentIndices;
}
