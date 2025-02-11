/**
 * Solutions to Chapter 9, Section 2, Exercises 1 through 3 of Horowitz, Sahni,
 * and Mehta's Fundamentals of Data Structures in C++, 1st Edition.
 * 
 * Task 1:
 * Complete function Deap::Insert() by writing all the functions it uses. Test
 * the insertion function by running it on a computer. Generate your own test
 * data.
 * 
 * Task 2:
 * Refine function Deap::DeleteMin() into a C++ function. Test the correctness
 * of your function by running it on a computer using test data of your choice.
 * 
 * Task 3:
 * Write a function to initialize a deap with n elements. Your function must run
 * in O(n) time. Show that your function actually has this running time. (Hint:
 * Proceed as in the initialization of a min heap or a max heap.)
 * 
 */

/*********************************main.cpp*************************************/

#include <iostream>
#include <stdexcept>
#include <cmath>

#include "Deap.h"

int main() {
    Deap<int> deap(20);
    
    Element<int> elements[] = {
        {5}, {45}, 
        {10}, {8}, {25}, {40}, 
        {15}, {19}, {9}, {30}, {20}
    };

    Element<int> e1 = {4};
    Element<int> e2 = {30};

    std::cout << "Test 1: Initialization of deap with 12 elements.\n";
    deap.Initialize(elements, 11);
    deap.printDeap();
    
    std::cout << "Test 2: Insertion of a minimum element with key 4.\n";
    deap.Insert(e1);
    deap.printDeap();
    
    std::cout << "Test 3: Deletion of minimum heap element with key 4.\n";
    deap.DeleteMin(e1);
    deap.printDeap();
    
    std::cout << "Test 4: Re-insertion of element into deap with key 30.\n";
    deap.Insert(e2);
    deap.printDeap();
    
    return 0;
}



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



/********************************Constants.h***********************************/

#ifndef CONSTANTS_H
#define CONSTANTS_H

namespace constants {
    static const int DefaultHeapSize = 64;
}

#endif // Constants.h
