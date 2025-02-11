/**
 * Solutions to Chapter 9, Section 2, Exercise 4 of Horowitz, Sahni, and Mehta's 
 * Fundamentals of Data Structures in C++, 1st Edition.
 * 
 * Task 4:
 * Write the functions to perform all double-ended priority queue operations, 
 * both for a min-max heap and for a deap.
 * 
 * (a) Use suitable test data to verify the correctness of your functions.
 * (b) Create a random list of n elements and a random sequence of insert,
 *     delete-min, and delete-max operations of length m. The latter sequence
 *     is created such that the probability of an insert is approximately 0.5,
 *     whereas that of each type of delete is approximately 0.25. Initialize
 *     a min-max heap and a deap to contain the n elements in the first random
 *     list. Now, measure the time to perform the m operations using the
 *     min-max heap as well as the deap. Divide this time by m to get the
 *     average time per operation. Do this for m = 100, 500, 1000, 2000, ... ,
 *     5000. Let m be 5000. Tabulate your computing times.
 * (c) Based on your experiments, what can you say about the relative merits of
 *     the two double-ended priority queue schemes?
 * 
 * Response to (c):
 * The average operation times for MinMaxHeap were consistently faster across 
 * all sizes of randomized lists of elements that were inserted and deleted. In
 * most cases, the operation times for MinMaxHeap was faster than those for
 * deaps by roughly a factor of 2. This suggests that limited to this case only,
 * the min-max heap is more efficient than the deap structure. Interestingly, 
 * neither of the two data structures show degradation in performance as the
 * sizes of the random sequence increase. This suggests that both of these 
 * structures maintain an O(log(n)) complexity for all insert, delete-min, and 
 * delete-max operations.
 * 
 */

/*********************************main.cpp*************************************/

#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <cmath>

#include <chrono> // For timed experiments
#include <random> // For Mersenne Twister randomizer engine
#include <vector> // For randomization of operations and elements

#include "MinMaxHeap.h"
#include "Deap.h"

// Function to generate a random list of n elements
std::vector<Element<int>> generateRandomElements(int n) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 10000); // Adjust range as needed

    std::vector<Element<int>> elements(n);
    for (int i = 0; i < n; ++i) {
        elements[i].key = dis(gen);
    }
    return elements;
}

// Function to generate a random sequence of operations
std::vector<int> generateRandomOperations(int m) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    std::vector<int> operations(m);
    for (int i = 0; i < m; ++i) {
        double prob = dis(gen);
        if (prob < 0.5) {
            operations[i] = 0; // Insert
        } else if (prob < 0.75) {
            operations[i] = 1; // DeleteMin
        } else {
            operations[i] = 2; // DeleteMax
        }
    }
    return operations;
}

int main() {
    // Create four min-max heaps each of which has capacity of 20 
    MinMaxHeap<int> heapFirst(20), heapSecond(20), heapThird(20), heapFourth(20); 

    Element<int> elementMinMax[] = {
        {7}, {70}, {40}, {30}, {9}, {10}, {15}, {45}, {50}, {30}, {20}, {12}
    };

    Element<int> e13 = {5};
    Element<int> e14 = {80};

    heapFirst.Initialize(elementMinMax, 12); // Initialize the heap with 12 elements
    heapSecond.Initialize(elementMinMax, 12); // Initialize the heap with 12 elements
    heapThird.Initialize(elementMinMax, 12); // Initialize the heap with 12 elements
    heapFourth.Initialize(elementMinMax, 12); // Initialize the heap with 12 elements
    
    std::cout << "TEST 1: Testing Correctness of Min-Max Heap Functions.\n" << std::endl;
    
    std::cout << "Test 1-1: Testing insertions and initialization of heaps." << std::endl;
    // Now the heap is properly initialized and satisfies the min-max heap properties
    heapFirst.printHeap();
    
    heapFirst.Insert(e13);
    heapFirst.printHeap();
    
    heapSecond.Insert(e14);
    heapSecond.printHeap();

    std::cout << "Test 1-2: Testing deletion of minimum element." << std::endl;
    heapThird.DeleteMin(elementMinMax[0]);
    heapThird.printHeap();

    std::cout << "Test 1-3: Testing re-insertion." << std::endl;
    heapFourth.DeleteMin(elementMinMax[0]);
    heapFourth.DeleteMin(elementMinMax[11]); // Remove element with key value 12.
    heapFourth.Insert(elementMinMax[11]); // Re-insert element with key value 12.
    heapFourth.printHeap();
    std::cout << std::endl;

    std::cout << "TEST 2: Testing Correctness of Deap Functions.\n" << std::endl;
    
    Deap<int> deapTest(20);
    
    Element<int> elementDeap[] = {
        {5}, {45}, 
        {10}, {8}, {25}, {40}, 
        {15}, {19}, {9}, {30}, {20}
    };

    Element<int> e1 = {4};
    Element<int> e2 = {30};

    std::cout << "Test 2-1: Initialization of deap with 12 elements.\n";
    deapTest.Initialize(elementDeap, 11);
    deapTest.printDeap();
    
    std::cout << "Test 2-2: Insertion of a minimum element with key 4.\n";
    deapTest.Insert(e1);
    deapTest.printDeap();
    
    std::cout << "Test 2-3: Deletion of minimum heap element with key 4.\n";
    deapTest.DeleteMin(e1);
    deapTest.printDeap();
    
    std::cout << "Test 2-4: Re-insertion of element into deap with key 30.\n";
    deapTest.Insert(e2);
    deapTest.printDeap();

    std::cout << "TEST 3: Comparison of Timed Experiments.\n";
    std::cout << "----------------------------------------\n";
    
    const int m = 5000;
    std::vector<int> n_values = {100, 500, 1000, 2000, 3000, 4000, 5000};

    for (int n : n_values) {
        // Generate random elements and operations
        auto elements = generateRandomElements(n);
        auto operations = generateRandomOperations(m);

        // Initialize MinMaxHeap and Deap
        MinMaxHeap<int> minMaxHeap;
        Deap<int> deap;

        minMaxHeap.Initialize(elements.data(), n);
        deap.Initialize(elements.data(), n);

        // Measure time for MinMaxHeap
        auto start = std::chrono::high_resolution_clock::now();
        for (int op : operations) {
            Element<int> temp;
            if (op == 0) {
                temp.key = rand() % 10000;
                minMaxHeap.Insert(temp);
            } else if (op == 1) {
                minMaxHeap.DeleteMin(temp);
            } else {
                minMaxHeap.DeleteMax(temp);
            }
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        double avgTimeMinMaxHeap = static_cast<double>(elapsed.count()) / m;

        // Measure time for Deap
        start = std::chrono::high_resolution_clock::now();
        for (int op : operations) {
            Element<int> temp;
            if (op == 0) {
                temp.key = rand() % 10000;
                deap.Insert(temp);
            } else if (op == 1) {
                deap.DeleteMin(temp);
            } else {
                deap.DeleteMax(temp);
            }
        }
        end = std::chrono::high_resolution_clock::now();
        elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        double avgTimeDeap = static_cast<double>(elapsed.count()) / m;

        // Output results
        std::cout << "n = " << n << ", m = " << m << ":\n";
        std::cout << "MinMaxHeap average time per operation: " << avgTimeMinMaxHeap << " microseconds\n";
        std::cout << "Deap average time per operation: " << avgTimeDeap << " microseconds\n";
        std::cout << "----------------------------------------\n";
    }
    
    return 0;
}



/********************************Constants.h***********************************/

#ifndef CONSTANTS_H
#define CONSTANTS_H

namespace constants {
    static const int DefaultHeapSize = 10000;
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
    void VerifyMax(int i, const Element<KeyType>& x);
    void VerifyMin(int i, const Element<KeyType>& x);
    void Insert(const Element<KeyType>&);
    Element<KeyType>* DeleteMax(Element<KeyType>&);
    Element<KeyType>* DeleteMin(Element<KeyType>&);
    int MinChildGrandChild(int i);
    void MinMaxFull();
    void MinMaxEmpty();
    int level(int i);
    void Initialize(Element<KeyType>* input, int size);
    void printHeap();
    
  private:
    Element<KeyType> *h;
    int n;
    int MaxSize;
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



/**********************************Deap.h**************************************/
#ifndef DEAP_H
#define DEAP_H

#include "DEPQ.h"
#include "Constants.h"

template <typename KeyType>
class Deap : public DEPQ<KeyType> {
  public:
    Deap(const int sz = constants::DefaultHeapSize);
    ~Deap();
    void Insert(const Element<KeyType>& x);
    Element<KeyType>* DeleteMax(Element<KeyType>& x);
    Element<KeyType>* DeleteMin(Element<KeyType>& x);
    
    void DeapFull();
    void DeapEmpty();
    bool MaxHeap(int p);
    int MinPartner(int p);
    int MaxPartner(int p);
    void MinInsert(int pos, const Element<KeyType>& x);
    void MaxInsert(int pos, const Element<KeyType>& x);
    void Initialize(const Element<KeyType>* input, int size);
    void printDeap();
    
  private:
    Element<KeyType> *d;
    int n;          // Current size of Deap d[]
    int MaxSize;    // Maximum allowable size of Deap d[]
};

#include "Deap.tpp"

#endif // Deap.h



/*********************************Deap.tpp*************************************/

template <typename KeyType>
Deap<KeyType>::Deap(const int sz) : MaxSize(sz), n(0) {
    d = new Element<KeyType>[MaxSize + 2]; // d[0] and d[1] are not used.
}

template <typename KeyType>
Deap<KeyType>::~Deap() {
    delete[] d;  // Deallocate the dynamically allocated array
}

template <typename KeyType>
void Deap<KeyType>::DeapFull() {
    if (n == MaxSize) throw std::overflow_error("Deap is full.\n");
}

template <typename KeyType>
void Deap<KeyType>::DeapEmpty() {
    if (!n) throw std::underflow_error("Deap is empty.\n");
}

template <typename KeyType>
int Deap<KeyType>::MaxPartner(int p) {
    int expon = static_cast<int>(floor((std::log2(p)))) - 1;
    return (p + (1 << expon)) / 2;
}

template <typename KeyType>
int Deap<KeyType>::MinPartner(int p) {
    int expon = static_cast<int>(floor((std::log2(p)))) - 1;
    return p - (1 << expon);
}

template <typename KeyType>
void Deap<KeyType>::Insert(const Element<KeyType>& x) {
    if (n == MaxSize) { DeapFull(); return; } // Check if the deap is full
    n++; // Increase the size of the deap
    if (n == 1) { d[2] = x; return; } // Insert into an empty deap

    int p = n + 1; // p is the new last position of the deap
    if (MaxHeap(p)) {
        int i = MinPartner(p);
        if (x.key < d[i].key) {
            d[p] = d[i];
            MinInsert(i, x);
        } else {
            MaxInsert(p, x);
        }
    } else {
        int i = MaxPartner(p);
        if (x.key > d[i].key) {
            d[p] = d[i];
            MaxInsert(i, x);
        } else {
            MinInsert(p, x);
        }
    }
}

template <typename KeyType>
bool Deap<KeyType>::MaxHeap(int p) {
    // A position p is in the max heap if it is in the right half of the level
    // We can determine this by comparing p with its level's midpoint
    if (p <= 1) return false;
    int level = static_cast<int>(floor(std::log2(p)));
    int levelStart = 1 << level;
    int levelMidpoint = levelStart + (1 << (level - 1));
    return p >= levelMidpoint;
}

template <typename KeyType>
void Deap<KeyType>::MinInsert(int pos, const Element<KeyType>& x) {
    while (pos > 2) {  // pos 2 is the root of min heap
        int parent = pos / 2;
        if (x.key >= d[parent].key) break;
        d[pos] = d[parent];
        pos = parent;
    }
    d[pos] = x;
}

template <typename KeyType>
void Deap<KeyType>::MaxInsert(int pos, const Element<KeyType>& x) {
    while (pos > 3) {  // pos 3 is the root of max heap
        int parent = pos / 2;
        if (x.key <= d[parent].key) break;
        d[pos] = d[parent];
        pos = parent;
    }
    d[pos] = x;
}

template <typename KeyType>
Element<KeyType>* Deap<KeyType>::DeleteMax(Element<KeyType>& x) {
    if (!n) { DeapEmpty(); return nullptr; }
    
    // Find the maximum element (root of max heap)
    int maxRoot = (n >= 3) ? 3 : 2;
    x = d[maxRoot];
    
    // Get the last element
    Element<KeyType> last = d[n + 1];
    n--;
    
    if (n < 2) return &x;  // If deap is now empty or has one element
    
    // Rebuild max heap
    int pos = maxRoot;
    while (2 * pos <= n) {  // While pos has children
        int child = 2 * pos;
        if (child + 1 <= n && d[child + 1].key > d[child].key) {
            child++;  // Choose larger child
        }
        
        if (last.key >= d[child].key) break;
        
        d[pos] = d[child];
        pos = child;
    }
    
    d[pos] = last;
    
    // Check and maintain deap property with min partner
    int minPartnerPos = MinPartner(pos);
    if (minPartnerPos >= 2 && d[pos].key < d[minPartnerPos].key) {
        Element<KeyType> temp = d[pos];
        d[pos] = d[minPartnerPos];
        d[minPartnerPos] = temp;
    }
    
    return &x;
}

template <typename KeyType>
Element<KeyType>* Deap<KeyType>::DeleteMin(Element<KeyType>& x) {
    if (!n) { DeapEmpty(); return nullptr; }
    x = d[2];  // Save minimum element to return
    int p = n + 1;  // Last position in deap
    Element<KeyType> t = d[p]; 
    n--;  // Reduce size
    
    if (n < 2) return &x;  // If deap becomes empty or has one element
    
    // Follow the path down the min heap, moving elements up as we go
    int i = 2;  // Start at min heap root
    while (2 * i <= n) {  // While i has a child
        int j = 2 * i;  // Left child
        // Let j be the child with smaller key
        if (j + 1 <= n && d[j + 1].key < d[j].key) {
            j++;  // Right child is smaller
        }
        d[i] = d[j];
        i = j;
    }
    
    // Do a deap insertion of t at position i
    if (MaxHeap(i)) {
        int minp = MinPartner(i);
        if (t.key < d[minp].key) {
            d[i] = d[minp];
            MinInsert(minp, t);
        } else {
            MaxInsert(i, t);
        }
    } else {
        MinInsert(i, t);
    }
    return &x;
}

template <typename KeyType>
void Deap<KeyType>::Initialize(const Element<KeyType>* input, int size) {
    if (size > MaxSize) {
        throw std::overflow_error("Input size exceeds maximum deap size");
    }
    
    // Copy input elements starting at index 2 (indices 0 and 1 unused)
    for (int i = 0; i < size; i++) {
        d[i + 2] = input[i];
    }
    n = size;
    
    // Start from the last non-leaf level of both min and max heaps
    // and work up to the roots
    int lastLevel = static_cast<int>(floor(std::log2(n)));
    int lastLevelStart = 1 << lastLevel;
    
    // First balance min-max partnerships at each position
    for (int i = lastLevelStart; i <= n + 1; i++) {
        if (MaxHeap(i)) {
            int minp = MinPartner(i);
            if (minp >= 2 && d[i].key < d[minp].key) {
                std::swap(d[i], d[minp]);
            }
        }
    }
    
    // Then heapify both min and max heaps from bottom up
    for (int i = (n + 1) / 2; i >= 2; i--) {
        // Get the initial position
        Element<KeyType> temp = d[i];
        int parent = i;
        
        if (MaxHeap(i)) {
            // Max heap portion
            while (2 * parent <= n + 1) {
                int child = 2 * parent;
                if (child + 1 <= n + 1 && d[child + 1].key > d[child].key) {
                    child++;
                }
                if (d[child].key <= temp.key) break;
                d[parent] = d[child];
                parent = child;
            }
            d[parent] = temp;
            
            // Check min partner after settling
            int minp = MinPartner(parent);
            if (minp >= 2 && d[parent].key < d[minp].key) {
                std::swap(d[parent], d[minp]);
            }
        } else {
            // Min heap portion
            while (2 * parent <= n + 1) {
                int child = 2 * parent;
                if (child + 1 <= n + 1 && d[child + 1].key < d[child].key) {
                    child++;
                }
                if (d[child].key >= temp.key) break;
                d[parent] = d[child];
                parent = child;
            }
            d[parent] = temp;
            
            // Check max partner after settling
            int maxp = MaxPartner(parent);
            if (maxp >= 3 && d[parent].key > d[maxp].key) {
                std::swap(d[parent], d[maxp]);
            }
        }
    }
}

template <typename KeyType>
void Deap<KeyType>::printDeap() {
    if (n == 0) {
        std::cout << "Empty deap" << std::endl;
        return;
    }

    // Calculate the total number of levels
    int levels = static_cast<int>(floor(std::log2(n))) + 1;
    
    // Print complete data set by levels
    std::cout << "Data set:" << std::endl;
    for (int level = 1; level < levels; level++) {
        int start = 1 << level;  // First node at this level
        int end = std::min((1 << (level + 1)) - 1, n + 1);  // Last node at this level
        
        std::cout << "Level " << level << ": ";
        for (int i = start; i <= end; i++) {
            std::cout << d[i].key;
            if (i < end) std::cout << ", ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
    
    // Print Min heap
    std::cout << "Min heap:" << std::endl;
    std::cout << "root: " << d[2].key << std::endl;
    
    // For each level of min heap
    for (int level = 2; level < levels; level++) {
        bool firstInLevel = true;
        int start = 1 << level;
        int end = std::min((1 << (level + 1)) - 1, n + 1);
        
        for (int i = start; i <= end; i++) {
            if (!MaxHeap(i)) {
                if (firstInLevel) {
                    firstInLevel = false;
                } else {
                    std::cout << ", ";
                }
                int parent = i / 2;
                std::cout << d[i].key << "(under " << d[parent].key << ")";
            }
        }
        if (!firstInLevel) std::cout << std::endl;
    }
    std::cout << std::endl;
    
    // Print Max heap
    std::cout << "Max heap:" << std::endl;
    if (n >= 3) {
        std::cout << "root: " << d[3].key << std::endl;
        
        // For each level of max heap
        for (int level = 2; level < levels; level++) {
            bool firstInLevel = true;
            int start = 1 << level;
            int end = std::min((1 << (level + 1)) - 1, n + 1);
            
            for (int i = start; i <= end; i++) {
                if (MaxHeap(i)) {
                    if (firstInLevel) {
                        firstInLevel = false;
                    } else {
                        std::cout << ", ";
                    }
                    int parent = i / 2;
                    std::cout << d[i].key << "(under " << d[parent].key << ")";
                }
            }
            if (!firstInLevel) std::cout << std::endl;
        }
    }
    std::cout << std::endl;
}
