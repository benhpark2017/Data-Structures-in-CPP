/**
 * Solutions to Chapter 5, Section 10, Exercise 3 of Horowitz's Fundamentals
 * of Data Structures in C++.
 * 
 * Task 3:
 * Experimentally compare the performance of SimpleUnion() and SimpleFind() with
 * WeightedUnion() and CollapsingFind(). For this, generate a random sequence of
 * union and find operations.
 */

/*********************************main.cpp*************************************/
#include "Set.h"
#include <iostream>
#include <chrono>
#include <cstdlib>
#include <cmath>

// Function to generate a random sequence of union and find operations
void generateRandomOperations(Sets& s, int numOperations, int setSize) {
    for (int i = 0; i < numOperations; i++) {
        int opType = std::rand() % 2; // 0 for union, 1 for find
        int a = std::rand() % setSize + 1; // Random element in range [1, setSize]
        int b = std::rand() % setSize + 1; // Random element in range [1, setSize]

        if (opType == 0) {
            // Perform union operation
            s.SimpleUnion(a, b);
        } else {
            // Perform find operation
            s.SimpleFind(a);
        }
    }
}

int main() {
    const int setSize = 1000;      // Number of elements in the set
    const int numOperations = 1000; // Number of random operations to perform

    Sets simpleSet(setSize);
    Sets weightedSet(setSize);

    // Time the SimpleUnion and SimpleFind operations
    auto start = std::chrono::high_resolution_clock::now();
    generateRandomOperations(simpleSet, numOperations, setSize);
    auto end = std::chrono::high_resolution_clock::now();
    auto durationSimple = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    std::cout << "Time taken for SimpleUnion and SimpleFind: " << durationSimple
              << " microseconds" << std::endl;

    // Time the WeightedUnion and CollapsingFind operations
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < numOperations; i++) {
        int opType = std::rand() % 2; // 0 for union, 1 for find
        int a = std::rand() % setSize + 1; // Random element in range [1, setSize]
        int b = std::rand() % setSize + 1; // Random element in range [1, setSize]

        if (opType == 0) {
            // Perform weighted union operation
            weightedSet.WeightedUnion(a, b);
        } else {
            // Perform collapsing find operation
            weightedSet.CollapsingFind(a);
        }
    }
    end = std::chrono::high_resolution_clock::now();
    auto durationWeighted = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    std::cout << "Time taken for WeightedUnion and CollapsingFind: " << durationWeighted 
              << " microseconds" << std::endl;

    // Compare the time taken by both approaches
    std::cout << "Difference in time: " << std::abs(durationSimple - durationWeighted)
              << " microseconds" << std::endl;

    return 0;
}



/***********************************Set.h**************************************/
#ifndef SET_H
#define SET_H

#include <iostream>

class Sets {
  private:
    int* parent; // Array to store parent information
    int n; // Number of elements in the set

  public:
    // Constructor
    Sets(int size = 10);

    // Destructor
    ~Sets() { delete[] parent; }

    // Equality operator to compare two sets
    bool operator==(const Sets& s1);

    // Simple union: make j the parent of i
    void SimpleUnion(int i, int j);

    // Simple find: find the root of i
    int SimpleFind(int i);
    
    // Weighted union: combine sets based on size
    void WeightedUnion(int i, int j);

    // Collapsing find: find the root and compress the path
    int CollapsingFind(int i);
    
    // Friend function for output
    friend std::ostream& operator<<(std::ostream& os, Sets& s);
};

#endif //SET_H



/**********************************Set.cpp*************************************/
#include "Set.h"

Sets::Sets(int size) {
    n = size;
    parent = new int[size + 1]; // [0] is not used
    for (int i = 1; i <= n; i++) {
        parent[i] = -1; // Set all elements as root (-1 means root)
    }
}

// Equality operator to compare two sets
bool Sets::operator==(const Sets& s1) {
    if (n != s1.n)
        return false;
    for (int i = 1; i <= n; i++) {
        if (parent[i] != s1.parent[i])
            return false; // If any element differs, they are not equal
    }
    return true;
}

// Simple union: make j the parent of i
void Sets::SimpleUnion(int i, int j) {
    parent[i] = j; // Assign j as the parent of i
}

// Simple find: find the root of i
int Sets::SimpleFind(int i) {
    while (parent[i] > 0)
        i = parent[i];
    return i;
}

// Weighted union: combine sets based on size
void Sets::WeightedUnion(int i, int j) {
    int temp = parent[i] + parent[j];
    if (parent[i] > parent[j]) {
        // i has fewer nodes (more negative), so make j the parent
        parent[i] = j;
        parent[j] = temp;
    } else {
        // j has fewer nodes, so make i the parent
        parent[j] = i;
        parent[i] = temp;
    }
}

int Sets::CollapsingFind(int i) {
    int r;
    //int count = 0;
    // Find the root
    for (r = i; parent[r] > 0; r = parent[r]); //find root
    // Compress the path
    while (i != r) {
        int s = parent[i];
        parent[i] = r;
        i = s;
    }
    return r;
}

// Output function to display the sets
std::ostream& operator<<(std::ostream& os, Sets& s) {
    os << "First row: Set elements\tSecond row: Parent (-1 means root)" << std::endl;
    for (int i = 1; i <= s.n; i++) {
        os << i << "\t";
    }
    os << std::endl;
    for (int i = 1; i <= s.n; i++) {
        os << s.parent[i] << "\t";
    }
    os << std::endl;
    return os;
}
