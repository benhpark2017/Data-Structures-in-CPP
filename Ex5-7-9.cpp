/**
 * Solutions to Chapter 5, Section 7, Exercise 9 of Horowitz's
 * Fundamentals of Data Structures in C++.
 * 
 * Task 9: 
 * Notice that a binary search tree can be used to implement a priority queue.
 * a) Write a C++ class definition for a binary search tree. This class should
 *    be a publicly derived class of class MaxPQ.
 * b) Write a C++ function to delete the largest element in a binary search tree.
 *    Your function should have complexity O(h), where h is the height of the
 *    search tree. Since h is O(log n) on average, we can perform each of the
 *    priority queue operations in average time O(log n).
 * c) Compare the actual performance of heaps and binary search trees as data
 *    structures for priority queues. For this comparison, generate random
 *    sequences of insert and delete max operations and measure the total time
 *    taken for each sequence by each of these data structures.
 */
/*********************************main.cpp*************************************/

#include <iostream>
#include <chrono>
#include <vector>
#include <cstdlib>   // For random numbers
#include "MaxHeap.h" // Include your MaxHeap implementation
#include "BST.h"     // Include your BST implementation

// Helper function to generate random integers
std::vector<int> generateRandomSequence(int size, int range) {
    std::vector<int> sequence;
    for (int i = 0; i < size; ++i) {
        sequence.push_back(rand() % range);  // Generate random number in [0, range)
    }
    return sequence;
}

// Function to measure the time of Insert and DeleteMax operations on MaxHeap
template<typename KeyType>
double measureHeapPerformance(MaxHeap<KeyType>& heap, const std::vector<int>& sequence) {
    using Clock = std::chrono::high_resolution_clock;
    
    auto start = Clock::now();  // Start time
    
    for (const int& num : sequence) {
        Element<KeyType> element{num};
        heap.Insert(element);  // Insert elements
    }
    
    Element<KeyType> maxElement;
    for (int i = 0; i < sequence.size() / 2; ++i) {
        heap.DeleteMax(maxElement);  // Delete max elements
    }
    
    auto end = Clock::now();  // End time
    return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
}

// Function to measure the time of Insert and DeleteMax operations on BST
template<typename KeyType>
double measureBSTPerformance(BST<KeyType>& bst, const std::vector<int>& sequence) {
    using Clock = std::chrono::high_resolution_clock;
    
    auto start = Clock::now();  // Start time
    
    for (const int& num : sequence) {
        Element<KeyType> element{num};
        bst.Insert(element);  // Insert elements
    }
    
    Element<KeyType> maxElement;
    for (int i = 0; i < sequence.size() / 2; ++i) {
        bst.DeleteMax(maxElement);  // Delete max elements
    }
    
    auto end = Clock::now();  // End time
    return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
}

int main() {
    const int numOperations = 10000;  // Number of insert and delete operations
    const int range = 100000;  // Range of random numbers
    
    // Generate random sequence of numbers
    std::vector<int> randomSequence = generateRandomSequence(numOperations, range);

    // Initialize MaxHeap and BST
    MaxHeap<int> heap(numOperations);
    BST<int> bst;
    
    // Measure performance of MaxHeap
    double heapTime = measureHeapPerformance(heap, randomSequence);
    std::cout << "MaxHeap time: " << heapTime << " microseconds" << std::endl;

    // Measure performance of BST
    double bstTime = measureBSTPerformance(bst, randomSequence);
    std::cout << "BST time: " << bstTime << " microseconds" << std::endl;

    return 0;
}



/*********************************MaxPQ.h**************************************/
#ifndef MAX_PQ_H
#define MAX_PQ_H

enum Boolean { FALSE, TRUE };

template <typename Type>
class Element {
  public:
    Type key;
};

template <typename KeyType>
class MaxPQ {
public:
    virtual void Insert(const Element<KeyType>& item) = 0;
    virtual Element<KeyType>* DeleteMax(Element<KeyType>& x) = 0;
    virtual ~MaxPQ() {}
};

#endif // MAX_PQ_H



/*********************************MaxHeap.h************************************/
#ifndef MAX_HEAP_H
#define MAX_HEAP_H

#include "MaxPQ.h"
#include "Constants.h"

//enum Boolean {FALSE, TRUE};

template <typename KeyType>
class MaxHeap : public MaxPQ<KeyType> {
private:
    Element<KeyType>* heap;
    int n;       // current size of heap
    int MaxSize; // maximum allowed size of heap
    
public:
    MaxHeap(int sz = constants::DefaultSize);
    ~MaxHeap() override { delete[] heap; }
    
    inline Boolean isFull() { return (n == MaxSize) ? TRUE : FALSE; }
    inline Boolean isEmpty() { return (n == 0) ? TRUE : FALSE; }
    
    void Insert(const Element<KeyType>& item) override;
    Element<KeyType>* DeleteMax(Element<KeyType>& x) override;
};

#include "MaxHeap.tpp"

#endif //MIN_HEAP_H



/********************************MaxHeap.tpp***********************************/
template <typename KeyType>
MaxHeap<KeyType>::MaxHeap(int sz) {
    MaxSize = sz;
    n = 0;
    heap = new Element<KeyType>[MaxSize + 1];
}

template <typename KeyType>
void MaxHeap<KeyType>::Insert(const Element<KeyType>& x) {
    if (n == MaxSize) {
        return;
    }
    n++;
    int i = n;
    for (i = n; true; ) {
        if (i == 1) break; //at root
        if (x.key <= heap[i / 2].key) break;
        //move from parent to i
        heap[i] = heap[i / 2];
        i /= 2;
    }
    heap[i] = x;
}

template <typename KeyType>
Element<KeyType>* MaxHeap<KeyType>::DeleteMax(Element<KeyType>& x) {
    if (n == 0) {
        return nullptr;
    }
    x = heap[1];
    Element<KeyType> k = heap[n];
    n--;
    
    int i = 1;
    int j = 2;
    while (j <= n) {
        // The only changes here are reversing the comparisons
        if (j < n && heap[j].key < heap[j + 1].key) {
            j++;
        }
        if (k.key >= heap[j].key) {
            break;
        }
        heap[i] = heap[j];
        i = j;
        j *= 2;
    }
    heap[i] = k;
    return &x;
}



/***********************************BST.h*************************************/
#ifndef BST_H
#define BST_H

#include "MaxPQ.h"
#include "Constants.h"

// Forward declarations
template <typename Type> class BstNode;
template <typename Type> class BST;

template <typename Type>
class BstNode {
  private:
    BstNode* LeftChild;
    BstNode* RightChild;
    int LeftSize;
  public:
    Element<Type> data;

    BstNode(const Element<Type>& keyValue) 
        : LeftChild(nullptr), RightChild(nullptr), LeftSize(1), data(keyValue) {}
    
    // Add a constructor that takes just the key value
    BstNode(const Type& key) 
        : LeftChild(nullptr), RightChild(nullptr), LeftSize(1) {
        data.key = key;
    }

    friend class BST<Type>;
};

template <typename Type>
class BST : public MaxPQ<Type> {
  private:
    BstNode<Type>* root;

  public:
    BST() : root(nullptr) {}

    void Insert(const Element<Type>& x);
    Element<Type>* DeleteMax(Element<Type>& x);
};

#include "BST.tpp"

#endif //BST_H



/***********************************BST.tpp************************************/
#include <iostream>

template <typename Type>
void BST<Type>::Insert(const Element<Type>& x) {
    BstNode<Type>* p = root;
    BstNode<Type>* q = nullptr;

    // Traverse the tree to find the insertion point
    while (p) {
        q = p;
        if (x.key == p->data.key) {
            //std::cout << "Insertion failed. Duplicate key: " << x.key << std::endl;
            return;  // Exit if the key already exists
        }
        if (x.key < p->data.key)
            p = p->LeftChild; 
        else
            p = p->RightChild;
    }

    // Create a new node for the Element to be inserted
    BstNode<Type>* newNode = new BstNode<Type>(x);

    // If the tree is empty, the new node becomes the root
    if (!root) {
        root = newNode;
    } else {
        // Attach the new node to the parent `q`
        if (x.key < q->data.key)
            q->LeftChild = newNode;
        else
            q->RightChild = newNode;
    }

    // Insertion successful
    //std::cout << "Insertion of " << x.key << " successful." << std::endl;
}


template <typename Type>
Element<Type>* BST<Type>::DeleteMax(Element<Type>& x) {
    if (root == nullptr) {
        // Tree is empty, nothing to delete
        return nullptr; // Return nullptr when there's nothing to delete
    }

    BstNode<Type>* parent = nullptr;
    BstNode<Type>* current = root;

    // Traverse to the rightmost node (largest element)
    while (current->RightChild != nullptr) {
        parent = current;
        current = current->RightChild;
    }

    // Now `current` points to the node with the largest element
    // and `parent` is its parent (or null if `current` is root)

    // Copy the largest element data into `x`
    x = current->data;

    // If the largest node has a left child, move it up
    if (current->LeftChild != nullptr) {
        if (parent == nullptr) {
            // The root itself is the largest and has a left child
            root = current->LeftChild;
        } else {
            // Link the parent's right child to current's left child
            parent->RightChild = current->LeftChild;
        }
    } else {
        // If the largest node has no left child, just remove it
        if (parent == nullptr) {
            // The root itself is the largest and has no children
            root = nullptr;
        } else {
            // Remove the current node from its parent
            parent->RightChild = nullptr;
        }
    }

    // Free memory of the deleted node
    delete current;

    // Return the pointer to `x`
    return &x;
}



/********************************Constants.h***********************************/
#ifndef CONSTANTS_H
#define CONSTANTS_H

namespace constants {
    const int DefaultSize{100001};
};

#endif // CONSTANTS_H
