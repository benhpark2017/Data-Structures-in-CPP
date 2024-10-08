/**
 * Solutions to Chapter 5, Section 6, Exercises 2 and 3 of Horowitz's
 * Fundamentals of Data Structures in C++.
 * 
 * Task 2:
 * Write a C++ function to insert into a minimum heap. Show that the complexity
 * of your function is O(log n). 
 * 
 * Response:
 * The initial insertion takes O(1) time. The while loop adjusts the position of
 * the new element to maintain the heap property. It compares the new element
 * x.key with its parent node (heap[i/2].key).
 * If the new element's key is smaller than the parent's key (in a min-heap),
 * you swap the positions and move up the tree by halving the index (i /= 2).
 * This percolation continues until the element reaches its proper position,
 * either at the root or a point where the parent's key is smaller than x.key.
 * A complete binary tree represented by a binary heap has the height of log n,
 * and the while loop runs at most log n times as the element is percolated up
 * the tree. Therefore, the time complexity of the function is O(log n). 
 *
 * 
 * Task 3:
 * Write a C++ function to delete the smallest item from a minimum heap. Show
 * that the complexity of your function is O(log n).
 * 
 * Response:
 * The function removes the minimum element in a min-heap, which is stored in
 * heap[1]. This takes constant time, O(1), since accessing and removing the
 * minimum element where the root data is stored, is direct. 
 * After removing the root, the function replaces it with the last element in
 * the heap (heap[n]), then reduces the heap size (n--). The percolation process
 * starts by comparing this element (k) with its children to maintain the heap
 * property. In each iteration, the function finds the smaller child 
 * (if (heap[j].key > heap[j+1].key)). Then it swaps the current element (k)
 * with its smaller child if the child’s key is smaller (k.key > heap[j].key).
 * Lastly, it moves down to the next level in the heap by setting i = j and
 * updating j = 2*i. Like in the insertion function, the height of the binary
 * heap is log(n) because it is a complete binary tree. The percolation down
 * process is limited to the height of the tree, which means it runs at most 
 * logn times.
 */
/**********************************main.cpp************************************/
#include "MinHeap.h"
#include <iostream>

int main() {
    MinHeap<int> heap(10);
    
    heap.Insert(Element<int>{5});
    heap.Insert(Element<int>{10});
    heap.Insert(Element<int>{3});
    heap.Insert(Element<int>{20});
    heap.Insert(Element<int>{15});
    
    std::cout << "Deleting minimum elements from the heap:" << std::endl;
    
    Element<int> minElement;
    while (!heap.isEmpty()) {
        heap.DeleteMin(minElement);
        std::cout << "Min element: " << minElement.key << std::endl;
    }
    
    return 0;
}



/*********************************MinPQ.h*************************************/
#ifndef MIN_PQ_H
#define MIN_PQ_H

template <typename KeyType>
struct Element {
    KeyType key;
};

template <typename KeyType>
class MinPQ {
public:
    virtual void Insert(const Element<KeyType>& item) = 0;
    virtual Element<KeyType>* DeleteMin(Element<KeyType>& x) = 0;
    virtual ~MinPQ() {}
};

#endif // MIN_PQ_H



/********************************MinHeap.h************************************/
#ifndef MIN_HEAP_H
#define MIN_HEAP_H

#include "MinPQ.h"
#include "Constants.h"

enum Boolean {FALSE, TRUE};

template <typename KeyType>
class MinHeap : public MinPQ<KeyType> {
private:
    Element<KeyType>* heap;
    int n;       // current size of heap
    int MaxSize; // maximum allowed size of heap
    
public:
    MinHeap(int sz = constants::DefaultSize);
    ~MinHeap() override { delete[] heap; }
    
    inline Boolean isFull() { return (n == MaxSize) ? TRUE : FALSE; }
    inline Boolean isEmpty() { return (n == 0) ? TRUE : FALSE; }
    
    void Insert(const Element<KeyType>& item) override;
    Element<KeyType>* DeleteMin(Element<KeyType>& x) override;
};

#include "MinHeap.tpp"

#endif //MIN_HEAP_H


/*******************************MinHeap.tpp***********************************/
template <typename KeyType>
MinHeap<KeyType>::MinHeap(int sz) {
    MaxSize = sz;
    n = 0;
    heap = new Element<KeyType>[MaxSize + 1];
}

template <typename KeyType>
void MinHeap<KeyType>::Insert(const Element<KeyType>& x) {
    if (n == MaxSize) {
        return;
    }
    n++;
    int i = n;
    // The only change here is '>' becomes '<'
    while (i > 1 && x.key < heap[i/2].key) {
        heap[i] = heap[i/2];
        i /= 2;
    }
    heap[i] = x;
}

template <typename KeyType>
Element<KeyType>* MinHeap<KeyType>::DeleteMin(Element<KeyType>& x) {
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
        if (j < n && heap[j].key > heap[j+1].key) {
            j++;
        }
        if (k.key <= heap[j].key) {
            break;
        }
        heap[i] = heap[j];
        i = j;
        j *= 2;
    }
    heap[i] = k;
    return &x;
}
