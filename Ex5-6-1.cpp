/**
 * Solutions to Chapter 5, Section 6, Exercises 1 and 4 of Horowitz's
 * Fundamentals of Data Structures in C++.
 * 
 * Tasks 1 and 4:
 * Compare the run-time performance of max heaps with that of unordered and
 * ordered linear lists as a representation for priority queues. For this
 * comparison, program the max heap insertion and deletion algorithms, as well
 * as algorithms to perform these tasks on unordered and ordered linear lists
 * that are maintained as sequential lists in a one-dimensional array.
 * Generate a random sequence of n values and insert these into the priority
 * queue. Next, perform a random sequence of m inserts and deletes starting with
 * the initial queue of n values. This sequence is to be generated so that the
 * next operation in the sequence has an equal chance of being either an insert
 * or a delete. Care should be taken so that the sequence does not cause the
 * priority queue to become empty at any time. Measure the time taken for the
 * sequence of m operations using both a max heap and an unordered list. 
 * Divide the total time by m and plot the times as a function of n. Do this for
 * n = 100, 500, 1000, 2000, 3000, and 4000. Set m to be 1000. Make some
 * qualitative statements about the relative performance of the two 
 * representations for a priority queue.
 */

/**********************************main.cpp************************************/

#include "PerformanceTester.h"

int main() {
    PerformanceTester tester;
    tester.runTests();
    return 0;
}


/******************************PriorityQueue.h*********************************/
#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

template <typename T>
class PriorityQueue {
protected:
    T* arr;
    int capacity;
    int insertCount;
    int deleteCount;
    
public:
    int size;
    
    PriorityQueue(int cap) : capacity(cap), size(0), insertCount(0), deleteCount(0) {
        arr = new T[capacity];
    }
    
    virtual ~PriorityQueue() {
        delete[] arr;
    }
    
    bool isFull() const { return size == capacity; }
    bool isEmpty() const { return size == 0; }
    
    virtual void insert(T value) = 0;
    virtual T deleteMax() = 0;
    
    struct OperationCount {
        int insertions;
        int deletions;
    };
    
    OperationCount getOperationCount() const {
        return {insertCount, deleteCount};
    }
    
    void resetOperationCount() {
        insertCount = 0;
        deleteCount = 0;
    }
};

#endif // PRIORITY_QUEUE_H



/*********************************MaxHeap.h************************************/
#ifndef MAX_HEAP_PQ_H
#define MAX_HEAP_PQ_H

#include "PriorityQueue.h"

template <typename T>
class MaxHeapPQ : public PriorityQueue<T> {
public:
    MaxHeapPQ(int cap) : PriorityQueue<T>(cap) {}
    
    void insert(T value) override {
        if (this->isFull()) return;
        
        this->insertCount++;
        
        this->size++;
        int i = this->size - 1;
        
        while (i > 0 && value > this->arr[(i - 1) / 2]) {
            this->arr[i] = this->arr[(i - 1) / 2];
            i = (i - 1) / 2;
        }
        
        this->arr[i] = value;
    }
    
    T deleteMax() override {
        if (this->isEmpty()) return T();
        
        this->deleteCount++;
        
        T maxElement = this->arr[0];
        T lastElement = this->arr[this->size - 1];
        this->size--;
        
        int i = 0;
        while (i * 2 + 1 < this->size) {
            int maxChild = i * 2 + 1;
            if (maxChild + 1 < this->size && 
                this->arr[maxChild + 1] > this->arr[maxChild])
                maxChild++;
                
            if (lastElement >= this->arr[maxChild])
                break;
                
            this->arr[i] = this->arr[maxChild];
            i = maxChild;
        }
        
        this->arr[i] = lastElement;
        return maxElement;
    }
};

#endif // MAX_HEAP_PQ_H



/*******************************BTMaxHeapPQ.h**********************************/
#ifndef BT_MAX_HEAP_H
#define BT_MAX_HEAP_H

#include "PriorityQueue.h"

template <typename T>
class BTMaxHeapPQ : public PriorityQueue<T> {
private:
    int binarySearch(const T& item, int low, int high) {
        if (low > high) {
            return low;
        }
    
        int mid = low + (high - low) / 2;
    
        if (this->arr[mid] == item) {
            return mid;
        }
        else if (this->arr[mid] < item) {
            return binarySearch(item, mid + 1, high);
        }
        else {
            return binarySearch(item, low, mid - 1);
        }
    }

public:
    BTMaxHeapPQ(int cap) : PriorityQueue<T>(cap) {}
    
    void insert(T value) override {
        if (this->isFull()) return;
        this->insertCount++;
        BinarySearchInsert(value);
    }
    
    void BinarySearchInsert(T value) {
        if (this->isFull()) return;
        
        int pos = binarySearch(value, 0, this->size - 1);
        
        for (int i = this->size; i > pos; --i) {
            this->arr[i] = this->arr[i - 1];
        }
        
        this->arr[pos] = value;
        this->size++;
        
        int currentIndex = pos;
        while (currentIndex > 0) {
            int parentIndex = (currentIndex - 1) / 2;
            
            if (this->arr[currentIndex] > this->arr[parentIndex]) {
                T temp = this->arr[currentIndex];
                this->arr[currentIndex] = this->arr[parentIndex];
                this->arr[parentIndex] = temp;
                currentIndex = parentIndex;
            }
            else {
                break;
            }
        }
    }
    
    T deleteMax() override {
        if (this->isEmpty()) return T();
        
        this->deleteCount++;
        
        T maxElement = this->arr[0];
        T lastElement = this->arr[this->size - 1];
        this->size--;
        
        int i = 0;
        while (i * 2 + 1 < this->size) {
            int maxChild = i * 2 + 1;
            if (maxChild + 1 < this->size && 
                this->arr[maxChild + 1] > this->arr[maxChild])
                maxChild++;
                
            if (lastElement >= this->arr[maxChild])
                break;
                
            this->arr[i] = this->arr[maxChild];
            i = maxChild;
        }
        
        this->arr[i] = lastElement;
        return maxElement;
    }
};

#endif // BT_MAX_HEAP_H



/*****************************UnorderedListPQ.h********************************/
#ifndef UNORDERED_LIST_PQ_H
#define UNORDERED_LIST_PQ_H

#include "PriorityQueue.h"

template <typename T>
class UnorderedListPQ : public PriorityQueue<T> {
public:
    UnorderedListPQ(int cap) : PriorityQueue<T>(cap) {}
    
    void insert(T value) override {
        if (this->isFull()) return;
        this->insertCount++;
        this->arr[this->size++] = value;
    }
    
    T deleteMax() override {
        if (this->isEmpty()) return T();
        this->deleteCount++;
        int maxIndex = 0;
        for (int i = 1; i < this->size; i++) {
            if (this->arr[i] > this->arr[maxIndex])
                maxIndex = i;
        }
        
        T maxElement = this->arr[maxIndex];
        this->arr[maxIndex] = this->arr[this->size - 1];
        this->size--;
        
        return maxElement;
    }
};

#endif // UNORDERED_LIST_PQ_H



/******************************OrderedListPQ.h*********************************/
#ifndef ORDERED_LIST_PQ_H
#define ORDERED_LIST_PQ_H

#include "PriorityQueue.h"

template <typename T>
class OrderedListPQ : public PriorityQueue<T> {
public:
    OrderedListPQ(int cap) : PriorityQueue<T>(cap) {}
    
    void insert(T value) override {
        if (this->isFull()) return;
        this->insertCount++;
        int i;
        for (i = this->size - 1; i >= 0 && this->arr[i] < value; i--) {
            this->arr[i + 1] = this->arr[i];
        }
        this->arr[i + 1] = value;
        this->size++;
    }
    
    T deleteMax() override {
        if (this->isEmpty()) return T();
        this->deleteCount++;
        return this->arr[--this->size];
    }
};

#endif // ORDERED_LIST_PQ_H



/****************************PerformanceTester.h*******************************/
#ifndef PERFORMANCE_TESTER_H
#define PERFORMANCE_TESTER_H

#include "PriorityQueue.h"
#include <chrono>

class PerformanceTester {
private:
    template <typename PQ>
    struct TestResult {
        double time;
        typename PQ::OperationCount opCount;
    };

    template <typename PQ>
    TestResult<PQ> testMixedOperations(PQ& pq, int n, int m);

public:
    void runTests();
};

#include "PerformanceTester.tpp"

#endif // PERFORMANCE_TESTER_H



/***************************PerformanceTester.tpp******************************/
#include "MaxHeapPQ.h"
#include "BTMaxHeapPQ.h"
#include "UnorderedListPQ.h"
#include "OrderedListPQ.h"
#include <iostream>
#include <iomanip>
#include <random>

template <typename PQ>
typename PerformanceTester::TestResult<PQ> PerformanceTester::testMixedOperations(PQ& pq, int n, int m) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> valueDis(1, 1000000);
    std::uniform_real_distribution<> operationDis(0.0, 1.0);

    pq.resetOperationCount();

    for (int i = 0; i < n; i++) {
        pq.insert(valueDis(gen));
    }

    auto start = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < m; i++) {
        bool shouldInsert = operationDis(gen) < 0.5;
        
        if (shouldInsert) {
            if (!pq.isFull()) {
                pq.insert(valueDis(gen));
            }
        } else {
            if (!pq.isEmpty() && pq.size > n/2) {
                pq.deleteMax();
            } else {
                if (!pq.isFull()) {
                    pq.insert(valueDis(gen));
                }
            }
        }
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    double duration = std::chrono::duration<double, std::milli>(end - start).count();
    
    return {duration, pq.getOperationCount()};
}

void PerformanceTester::runTests() {
    const int sizes[] = {100, 500, 1000, 2000, 3000, 4000};
    const int M = 1000;
    
    std::cout << std::setw(5) << "n" 
              << std::setw(8) << "m"
              << std::setw(15) << "Data Structure"
              << std::setw(15) << "Time (ms)"
              << std::setw(12) << "Inserts"
              << std::setw(12) << "Deletes" << std::endl;
    std::cout << std::string(70, '-') << std::endl;
    
    for (int n : sizes) {
        MaxHeapPQ<int> maxHeap(n * 2);
        BTMaxHeapPQ<int> btMaxHeap(n * 2);
        UnorderedListPQ<int> unorderedList(n * 2);
        OrderedListPQ<int> orderedList(n * 2);
        
        auto heapResult = testMixedOperations(maxHeap, n, M);
        auto btHeapResult = testMixedOperations(btMaxHeap, n, M);
        auto unorderedResult = testMixedOperations(unorderedList, n, M);
        auto orderedResult = testMixedOperations(orderedList, n, M);
        
        std::cout << std::setw(5) << n 
                  << std::setw(8) << M
                  << std::setw(15) << "MaxHeap"
                  << std::setw(15) << std::fixed << std::setprecision(3) << heapResult.time
                  << std::setw(12) << heapResult.opCount.insertions
                  << std::setw(12) << heapResult.opCount.deletions << std::endl;
                  
        std::cout << std::setw(5) << n 
                  << std::setw(8) << M
                  << std::setw(15) << "BTMaxHeap"
                  << std::setw(15) << btHeapResult.time
                  << std::setw(12) << btHeapResult.opCount.insertions
                  << std::setw(12) << btHeapResult.opCount.deletions << std::endl;
                  
        std::cout << std::setw(5) << n 
                  << std::setw(8) << M
                  << std::setw(15) << "Unordered"
                  << std::setw(15) << unorderedResult.time
                  << std::setw(12) << unorderedResult.opCount.insertions
                  << std::setw(12) << unorderedResult.opCount.deletions << std::endl;
                  
        std::cout << std::setw(5) << n 
                  << std::setw(8) << M
                  << std::setw(15) << "Ordered"
                  << std::setw(15) << orderedResult.time
                  << std::setw(12) << orderedResult.opCount.insertions
                  << std::setw(12) << orderedResult.opCount.deletions << std::endl;
                  
        std::cout << std::string(70, '-') << std::endl;
    }
}
