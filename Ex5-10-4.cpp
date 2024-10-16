/**
 * Solutions to Chapter 5, Section 10, Exercises 4(a), 4(d), 5, and 6 of 
 * Horowitz's Fundamentals of Data Structures in C++.
 * 
 * 
 * Task 4(a):
 * Write a function HeightUnion() that uses the height rule for union operations
 * instead of the weighting rule. The rule is defined as follows:
 * 
 * Definition [Height Rule]: If the height of the tree i is less than that of
 * tree j, then make j the parent of i, otherwise make i the parent of j.
 * 
 * Your function must run in O(1) time and should maintain the height of each
 * tree as a negative number in the parent field of the root.
 * 
 * 
 * Task 4(d):
 * Experiment with functions WeightedUnion() and HeightUnion() to determine
 * which one produces better results when used in conjunction with function
 * CollapsingFind().
 * 
 * 
 * Task 5(a):
 * Write a function SplittingFind() that uses path splitting for the find
 * operations instead of path collapsing. This is defined below:
 * 
 * Definition [Path splitting]: In path splitting, the parent pointer in
 * each node (except the root and its child) on the path from i to the root
 * is changed to point to the node's grandparent.
 * 
 * Note that when path splitting is used, a single pass from i to the root
 * suffices.
 * 
 * Task 5(b):
 * Experiment with functions CollapsingFind() and SplittingFind() to determine
 * which produces better results when used in conjunction with function
 * WeightedUnion().
 * 
 * 
 * Task 6(a):
 * Write a function HalvingFind() that uses path halving for the find operations
 * instead of path collapsing? The path halving is defined below:
 * 
 * Definition [Path halving]: In path halving, the parent pointer of every other
 * node (except the root and its child) on the path from i to the root is 
 * changed to point to the node's grandparent.
 * 
 * Note that path halving, like path splitting, can be implemented with a single
 * pass from i to the root. However, in path halving, only half as many pointers
 * are changed as in path splitting.
 * 
 * 
 * Task 6(b):
 * Experiment with functions CollapsingFind() and HalvingFind() to determine
 * which one produces better results when used in conjunction with function
 * WeightedUnion().
 * 
 */

/*********************************main.cpp*************************************/
#include "Set.h"
#include <iostream>
#include <chrono>
#include <cstdlib>
#include <cmath>

int main() {
    // This setSize number must be sufficiently large to accommodate all
    // 1000 union and find operations
    const int setSize = 3000;       // Number of elements in the set
                                    
    const int numOperations = 1000; // Number of random operations to perform
    
    Sets heightSet(setSize);
    Sets weightedSet(setSize);
    Sets splittingSet(setSize);
    Sets halvingSet(setSize);
    
    /********TASK 4********/
    // Time the HeightUnion and CollapsingFind operations
    auto start = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < numOperations; i++) {
        int opType = rand() % 2; // 0 for union, 1 for find
        int a = rand() % setSize + 1; // Random element in range [1, setSize]
        int b = rand() % setSize + 1; // Random element in range [1, setSize]

        if (opType == 0) {
            // Perform height union operation
            heightSet.HeightUnion(a, b);
        } else {
            // Perform collapsing find operation
            heightSet.CollapsingFind(a);
        }
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto durationHeight
             = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    std::cout << "Time taken for HeightUnion and CollapsingFind: " << durationHeight 
              << " microseconds\n" << std::endl;
    
    // Time the WeightedUnion and CollapsingFind operations
    start = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < numOperations; i++) {
        int opType = rand() % 2; // 0 for union, 1 for find
        int a = rand() % setSize + 1; // Random element in range [1, setSize]
        int b = rand() % setSize + 1; // Random element in range [1, setSize]

        if (opType == 0) {
            // Perform weighted union operation
            weightedSet.WeightedUnion(a, b);
        } else {
            // Perform collapsing find operation
            weightedSet.CollapsingFind(a);
        }
    }
    
    end = std::chrono::high_resolution_clock::now();
    auto durationWeighted
        = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    std::cout << "Time taken for WeightedUnion and CollapsingFind: " 
              << durationWeighted << " microseconds\n" << std::endl;

    // Compare the time taken by both approaches
    std::cout << "Difference in time in trials 1 and 2: " 
              << std::abs(durationWeighted - durationHeight)
              << " milliseconds\n" << std::endl;
    /*****END OF TASK 4*****/


    /********TASK 5********/          
    // Time the WeightedUnion() and SplittingFind() operations
    start = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < numOperations; i++) {
        int opType = rand() % 2; // 0 for union, 1 for find
        int a = rand() % setSize + 1; // Random element in range [1, setSize]
        int b = rand() % setSize + 1; // Random element in range [1, setSize]

        if (opType == 0) {
            // Perform weighted union operation
            splittingSet.WeightedUnion(a, b);
        } else {
            // Perform collapsing find operation
            splittingSet.SplittingFind(a);
        }
    }
    
    end = std::chrono::high_resolution_clock::now();
    auto durationSplitting
        = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    std::cout << "Time taken for WeightedUnion and SplittingFind: " 
              << durationSplitting << " microseconds\n" << std::endl;  
  
    // Compare the time taken by both approaches
    std::cout << "Difference in time in trials 2 and 3: " 
              << std::abs(durationSplitting - durationWeighted)
              << " milliseconds\n" << std::endl;
    /*****END OF TASK 5*****/


    /********TASK 6*********/
    // Time the WeightedUnion() and HalvingFind() operations
    start = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < numOperations; i++) {
        int opType = rand() % 2; // 0 for union, 1 for find
        int a = rand() % setSize + 1; // Random element in range [1, setSize]
        int b = rand() % setSize + 1; // Random element in range [1, setSize]

        if (opType == 0) {
            // Perform weighted union operation
            halvingSet.WeightedUnion(a, b);
        } else {
            // Perform collapsing find operation
            halvingSet.HalvingFind(a);
        }
    }
    
    end = std::chrono::high_resolution_clock::now();
    auto durationHalving
        = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    std::cout << "Time taken for WeightedUnion and HalvingFind: " 
              << durationHalving << " microseconds\n" << std::endl;  

    // Compare the time taken by both approaches
    std::cout << "Difference in time in trials 2 and 4: " 
              << std::abs(durationHalving - durationWeighted)
              << " milliseconds\n" << std::endl;
    /*****END OF TASK 6*****/
    
    std::cout << std::endl << "End of program." << std::endl;
    
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
    Sets(int size);

    // Destructor
    ~Sets() { delete[] parent; }

    // Equality operator to compare two sets
    bool operator==(const Sets& s1);

    // Simple union: make j the parent of i
    void SimpleUnion(int i, int j);
    
    // Weighted union: combine sets based on size
    void WeightedUnion(int i, int j);
    
    // Height-based union: combine two sets based on their heights
    void HeightUnion(int i, int j);
    
    // Simple find: find the root of i
    int SimpleFind(int i);    
    
    // Collapsing find: find the root and compress the path
    int CollapsingFind(int i);
    
    // Splitting find: find the root of specified element and flatten
    int SplittingFind(int i);
    
    // Halving find: find the root of specified element and halve the path
    int HalvingFind(int i);
    
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

// Height-based union: combine trees based on their height
void Sets::HeightUnion(int i, int j) {
    // If the heights are the same, make i the new root and increase its height by 1
    if (parent[i] == parent[j]) {
        parent[j] = i; // Make i the parent of j
        parent[i]--;   // Increment the height of i's tree by 1
    }
    // If the height of i's tree is less than j's tree, make j the parent
    else if (parent[i] > parent[j]) {
        parent[i] = j; // Make j the parent of i
    }
    // Otherwise, make i the parent
    else {
        parent[j] = i; // Make i the parent of j
    }
}

// Simple find: find the root of i
int Sets::SimpleFind(int i) {
    while (parent[i] > 0)
        i = parent[i];
    return i;
}

int Sets::CollapsingFind(int i) {
    int r;
    //int count = 0;
    // Find the root
    for (r = i; parent[r] >= 0; r = parent[r]); //find root
    // Compress the path
    while (i != r) {
        int s = parent[i];
        parent[i] = r;
        i = s;
    }
    return r;
}

// Find operation that uses path splitting.
int Sets::SplittingFind(int i) {
    // Variable to hold the original root
    int root = i;

    // Find the root while adjusting pointers for path splitting
    while (parent[root] >= 0) {
        root = parent[root];  // Move to the parent
    }

    // Path splitting: point each node to its grandparent
    int current = i;
    while (current != root) {
        int grandparent = parent[current];  // Save the grandparent
        parent[current] = root;             // Change parent to root
        current = grandparent;               // Move to grandparent
    }

    return root;  // Return the root of the set
}

// Find the root and optimize tree by halving the paths for nodes along the way
int Sets::HalvingFind(int i) {
    while (parent[i] >= 0) { // While i is not the root
        if (parent[parent[i]] >= 0) { // If i's parent is not the root
            parent[i] = parent[parent[i]]; // Set i's parent to i's grandparent
        }
        i = parent[i]; // Move up to the parent
    }
    return i; // Return the root
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
