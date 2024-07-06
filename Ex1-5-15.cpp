/**
 * Solutions to Chapter 1, Section 5, Exercise 15 of Horowitz's
 * Data Structures in C++.
 */

#include <iostream>

// Function to compute powerset using recursion
void computePowerSet(char set[], int n, char subset[], int index) {
    // If all elements are processed, print the current subset
    if (index == n) {
        std::cout << "{ ";
        for (int i = 0; i < n; ++i) {
            if (subset[i] != 0) {
                std::cout << subset[i] << " ";
            }
        }
        std::cout << "}\n";
        return;
    }

    // Include the current element in the subset
    subset[index] = set[index];
    computePowerSet(set, n, subset, index + 1);

    // Exclude the current element from the subset and move to the next element
    subset[index] = 0; // Set the current element to 0 to exclude it
    computePowerSet(set, n, subset, index + 1);
}

// Driver program to test the above function
int main() {
    char set[] = {'a', 'b', 'c'};
    int n = sizeof(set) / sizeof(set[0]);

    // Array to store the current subset
    char subset[n];

    // Initialize subset with 0s
    for (int i = 0; i < n; ++i) {
        subset[i] = 0;
    }

    // Call the recursive function to compute the powerset
    computePowerSet(set, n, subset, 0);

    return 0;
}
