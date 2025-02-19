/**
 * Solutions to Chapter 9, Section 1, Exercises 1 through 5 of Horowitz, Sahni,
 * and Mehta's Fundamentals of Data Structures in C++, 1st Edition.
 * 
 * Task 1:
 * Write the function MinMaxHeap<KeyType>::VerifyMin() defined in connection
 * with insertion into a min-max heap.
 * 
 * Task 2:
 * Write the function MinMaxHeap<KeyType>::level(i), which determines whether
 * node i of a min-max heap is on a min or a max level.
 * 
 * Task 3:
 * Write the function MinMaxHeap<KeyType>::MinChildGrandChild(i), which returns
 * the child or grandchild of node i of a min-max heap that has the smallest
 * key. You may assume that i has at least one child.
 * 
 * Task 4:
 * Write the function MinMaxHeap<KeyType>::DeleteMax() to delete the element
 * with maximum key in a min-max heap. Your function should run in O(log(n)) 
 * time for a min-max heap with n elements.
 * 
 * Task 5:
 * Write a function to initialize a min-max heap with n elements. Do this using
 * a series of adjust steps as used in the initialization of a max heap
 * (see Chapter 5). Show that your function takes O(n) time rather than the
 * O(n*log(n)) time that would be taken if initialization were done by
 * performing n insertions to an initially empty heap.
 * 
 * Note: There is a mistake in void MinMaxHeap<KeyType>::Insert(const Element
 * <KeyType>& x). The break statement is missing in the else statement for case
 * MAX: after the function call VerifyMin(n, x). The Insert() function found in 
 * this code is newly written to reflect this erratum.
 */

/*********************************main.cpp*************************************/

#include <iostream>
#include <stdexcept>
#include <algorithm>

#include "MinMaxHeap.h"

int main() {
    // Create four min-max heaps each of which has capacity of 20 
    MinMaxHeap<int> heapFirst(20), heapSecond(20), heapThird(20), heapFourth(20); 

    Element<int> elements[] = {
        {7}, {70}, {40}, {30}, {9}, {10}, {15}, {45}, {50}, {30}, {20}, {12}
    };

    Element<int> e13 = {5};
    Element<int> e14 = {80};

    heapFirst.Initialize(elements, 12); // Initialize the heap with 12 elements
    heapSecond.Initialize(elements, 12); // Initialize the heap with 12 elements
    heapThird.Initialize(elements, 12); // Initialize the heap with 12 elements
    heapFourth.Initialize(elements, 12); // Initialize the heap with 12 elements
    
    std::cout << "Test 1: Testing insertions and initialization of heaps." << std::endl;
    // Now the heap is properly initialized and satisfies the min-max heap properties
    heapFirst.printHeap();
    
    heapFirst.Insert(e13);
    heapFirst.printHeap();
    
    heapSecond.Insert(e14);
    heapSecond.printHeap();

    std::cout << "Test 2: Testing deletion of minimum element." << std::endl;
    heapThird.DeleteMin(elements[0]);
    heapThird.printHeap();

    std::cout << "Test 3: Testing re-insertion." << std::endl;
    heapFourth.DeleteMin(elements[0]);
    heapFourth.DeleteMin(elements[11]); // Remove element with key value 12.
    heapFourth.Insert(elements[11]); // Re-insert element with key value 12.
    heapFourth.printHeap();

    return 0;
}



/********************************Constants.h***********************************/

#ifndef CONSTANTS_H
#define CONSTANTS_H

namespace constants {
    static const int DefaultHeapSize = 64;
};

#endif //Constants.h



/*******************************MinMaxHeap.h***********************************/

#ifndef MIN_MAX_HEAP_H
#define MIN_MAX_HEAP_H

#include "DEPQ.h"
#include "Constants.h"

template <typename KeyType>
class MinMaxHeap : public DEPQ<KeyType> {

  public:
    MinMaxHeap(const int sz = constants::DefaultHeapSize);
    ~MinMaxHeap();
    void Insert(const Element<KeyType>&);
    Element<KeyType>* DeleteMax(Element<KeyType>&);
    Element<KeyType>* DeleteMin(Element<KeyType>&);
    void Initialize(Element<KeyType>* input, int size);
    void printHeap();
    
  private:
    Element<KeyType> *h;
    int n;
    int MaxSize;
    void VerifyMax(int i, const Element<KeyType>& x);
    void VerifyMin(int i, const Element<KeyType>& x);
    int MinChildGrandChild(int i);
    void MinMaxFull();
    void MinMaxEmpty();
    int level(int i);
};

#include "MinMaxHeap.tpp"

#endif //MinMaxHeap.h



/******************************MinMaxHeap.tpp**********************************/

template <typename KeyType>
MinMaxHeap<KeyType>::MinMaxHeap(const int sz) : MaxSize(sz), n(0) {
    h = new Element<KeyType>[MaxSize + 1]; // h[0] is not used.
}

template <typename KeyType>
MinMaxHeap<KeyType>::~MinMaxHeap() {
    delete[] h;  // Deallocate the dynamically allocated array
}

template <typename KeyType>
void MinMaxHeap<KeyType>::MinMaxFull() {
    throw std::overflow_error("MinMaxHeap is full");
}

template <typename KeyType>
void MinMaxHeap<KeyType>::MinMaxEmpty() {
    throw std::underflow_error("MinMaxHeap is empty");
}

template <typename KeyType>
void MinMaxHeap<KeyType>::Insert(const Element<KeyType>& x) {
    if (n == MaxSize) {MinMaxFull(); return; }
    n++;
    int p = n/2; // p is the parent of the new node
    
    if (!p) {h[1] = x; return;}  // If it's the first element
    
    h[n] = x;  // Place x initially at the last position
    
    switch (level(p)) {
        case 0:  // Parent is at min level
            if (x.key < h[p].key) {  // x should be at min level
                h[n] = h[p];
                VerifyMin(p, x);
            } else {  // x should be at max level
                VerifyMax(n, x);
            }
            break;
        case 1:  // Parent is at max level
            if (x.key > h[p].key) {  // x should be at max level
                h[n] = h[p];
                VerifyMax(p, x);
            } else {  // x should be at min level
                VerifyMin(n, x);
            }
            break;
    }
}

// Follows max nodes from the max node i to the root and inserts x at its proper
// place.
template <typename KeyType>
void MinMaxHeap<KeyType>::VerifyMax(int i, const Element<KeyType>& x) {
    for (int gp = i / 4; // grandparent of i
         gp && (x.key > h[gp].key);
         gp /= 4) {
        h[i] = h[gp];
        i = gp;
    }
    h[i] = x;
}

// Follows min nodes from the min node i to the root and inserts x at its proper
// place.
template <typename KeyType>
void MinMaxHeap<KeyType>::VerifyMin(int i, const Element<KeyType>& x) {
    for (int gp = i / 4; // grandparent of i
         gp && (x.key < h[gp].key);
         gp /= 4) {
        h[i] = h[gp];
        i = gp;
    }
    h[i] = x;
}

template <typename KeyType>
Element<KeyType>* MinMaxHeap<KeyType>::DeleteMax(Element<KeyType>& y) {
    if (!n) { MinMaxEmpty(); return 0; }
    
    // Find the maximum element (either root's children or root itself)
    int maxIndex = 1;
    if (n >= 2 && h[2].key > h[maxIndex].key) {
        maxIndex = 2;
    }
    if (n >= 3 && h[3].key > h[maxIndex].key) {
        maxIndex = 3;
    }
    
    // Save the maximum element and replace it with the last element
    y = h[maxIndex];
    Element<KeyType> x = h[n--];
    
    // Re-insert x into the heap
    int i = maxIndex;
    int j = n / 2;
    
    while (i <= j) {
        int k = MinChildGrandChild(i);
        if (x.key >= h[k].key) break;
        else {
            h[i] = h[k];
            if (k <= 2 * i + 1) break;
            else {
                int p = k / 2;
                if (x.key > h[p].key) {
                    Element<KeyType> t = h[p];
                    h[p] = x;
                    x = t;
                }
            }
            i = k;
        }
    }
    h[i] = x;
    return &y;
}

// Deletes and returns an element with minimum key from the min-max heap.
template <typename KeyType>
Element<KeyType>* MinMaxHeap<KeyType>::DeleteMin(Element<KeyType>& y) {
    if (!n) {MinMaxEmpty(); return 0;}
    // Save root and last element; update heap size
    y = h[1];
    Element<KeyType> x = h[n--];
    
    // Initialize for re-insertion of x
    int i = 1, j = n / 2;
    
    // Finds place to insert x
    while (i <= j) {
        int k = MinChildGrandChild(i);
        if (x.key <= h[k].key) break;
        else {
            h[i] = h[k];
            if (k <= 2*i + 1) break;
            else {
                int p = k / 2;
                if (x.key > h[p].key) { 
                    Element<KeyType> t = h[p];
                    h[p] = x;
                    x = t;
                }
            } // end of if (k <= 2*i + 1)
            i = k;
        } // if (x.key <= h[k].key)
    } // end of while
    h[i] = x;
    return &y;
} // end of DeleteMin()

template <typename KeyType>
int MinMaxHeap<KeyType>::level(int i) {
    int level = 0;
    while (i > 1) {
        i /= 2;
        level++;
    }
    return level % 2; // 0 for min level, 1 for max level
}

template <typename KeyType>
int MinMaxHeap<KeyType>::MinChildGrandChild(int i) {
    int minIndex = i;
    int leftChild = 2 * i;
    int rightChild = 2 * i + 1;
    
    // Check children
    if (leftChild <= n && h[leftChild].key < h[minIndex].key) {
        minIndex = leftChild;
    }
    if (rightChild <= n && h[rightChild].key < h[minIndex].key) {
        minIndex = rightChild;
    }
    
    // Check grandchildren
    int leftGrandChild = 2 * leftChild;
    int rightGrandChild = 2 * leftChild + 1;
    
    if (leftGrandChild <= n && h[leftGrandChild].key < h[minIndex].key) {
        minIndex = leftGrandChild;
    }
    if (rightGrandChild <= n && h[rightGrandChild].key < h[minIndex].key) {
        minIndex = rightGrandChild;
    }
    
    return minIndex;
}

template <typename KeyType>
void MinMaxHeap<KeyType>::Initialize(Element<KeyType>* input, int size) {
    // Check if size exceeds MaxSize
    if (size > MaxSize) {
        MinMaxFull();
        return;
    }
    
    // Copy input array to heap
    for (int i = 1; i <= size; i++) {
        h[i] = input[i-1];
    }
    n = size;
    
    // Adjust from bottom up
    for (int i = n/2; i >= 1; i--) {
        Element<KeyType> temp = h[i];
        int j = i;
        bool done = false;
        
        // Determine if we're on a min or max level
        int lvl = level(i);
        
        while (!done && 2*j <= n) {
            int k = 2*j;  // Left child
            
            // Find the largest/smallest among children and grandchildren
            if (k < n && ((lvl == 0 && h[k].key > h[k+1].key) ||
                         (lvl == 1 && h[k].key < h[k+1].key))) {
                k++;
            }
            
            // Check grandchildren if they exist
            int gc = 4*j;  // First grandchild
            for (int g = gc; g <= std::min(gc + 3, n); g++) {
                if ((lvl == 0 && h[g].key < h[k].key) ||
                    (lvl == 1 && h[g].key > h[k].key)) {
                    k = g;
                }
            }
            
            // Compare with temp and adjust accordingly
            if (k <= 2*j+1) {  // k is a child
                if ((lvl == 0 && temp.key > h[k].key) ||
                    (lvl == 1 && temp.key < h[k].key)) {
                    h[j] = h[k];
                    j = k;
                } else {
                    done = true;
                }
            } else {  // k is a grandchild
                if ((lvl == 0 && temp.key > h[k].key) ||
                    (lvl == 1 && temp.key < h[k].key)) {
                    h[j] = h[k];
                    if ((lvl == 0 && temp.key > h[k/2].key) ||
                        (lvl == 1 && temp.key < h[k/2].key)) {
                        Element<KeyType> t = h[k/2];
                        h[k/2] = temp;
                        temp = t;
                    }
                    j = k;
                } else {
                    done = true;
                }
            }
        }
        
        if (!done) {
            h[j] = temp;
        }
    }
}

template <typename KeyType>
void MinMaxHeap<KeyType>::printHeap() {
    if (n == 0) {
        std::cout << "Heap is empty" << std::endl;
        return;
    }

    // Print root (level 0)
    std::cout << "min(root): " << h[1].key << std::endl;

    // For each level
    int level = 1;
    int start = 2;  // Start index of current level
    
    while (start <= n) {
        int end = std::min(2 * start - 1, n);  // End index of current level
        
        // Print based on whether it's a min or max level
        if (level % 2 == 1) {  // max level
            std::cout << "max: ";
        } else {  // min level
            std::cout << "min: ";
        }
        
        // Print each node at this level with its parent
        for (int i = start; i <= end; i++) {
            std::cout << h[i].key << "(under " << h[i/2].key << ")";
            if (i < end) {
                std::cout << ", ";
            }
        }
        std::cout << std::endl;
        
        // Move to next level
        start *= 2;
        level++;
    }
    std::cout << std::endl;
}



/**********************************DEPQ.h**************************************/

#ifndef DEPQ_H
#define DEPQ_H

template <typename KeyType>
struct Element {
    KeyType key;
};

template <typename KeyType>
class DEPQ {
    virtual void Insert(const Element<KeyType>&) = 0;
    virtual Element<KeyType>* DeleteMax(Element<KeyType>&) = 0;
    virtual Element<KeyType>* DeleteMin(Element<KeyType>&) = 0;
};

#endif //DEPQ.h
