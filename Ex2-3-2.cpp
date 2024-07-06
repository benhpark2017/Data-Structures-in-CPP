/**
 * Solutions to Chapter 2, Section 3, Exercise 2 of Horowitz's
 * Data Structures in C++.
 */

#include <iostream>

int compareOrderedLists(const int* A, int n, const int* B, int m) {
    // Compare elements of A and B
    for (int i = 0; i < std::min(n, m); ++i) {
        if (A[i] < B[i]) {
            return -1; // A is less than B
        } else if (A[i] > B[i]) {
            return 1; // A is greater than B
        }
    }
    
    // If all compared elements are equal, compare lengths
    if (n < m) {
        return -1; // A is less than B because A is shorter
    } else if (n > m) {
        return 1; // A is greater than B because A is longer
    } else {
        return 0; // A is equal to B
    }
}

int main() {
    const int A[] = {1, 2, 3};
    const int B[] = {1, 2, 3, 4};
    
    int n = sizeof(A) / sizeof(A[0]);
    int m = sizeof(B) / sizeof(B[0]);
    
    int result = compareOrderedLists(A, n, B, m);
    
    if (result == -1) {
        std::cout << "A is less than B" << std::endl;
    } else if (result == 0) {
        std::cout << "A is equal to B" << std::endl;
    } else {
        std::cout << "A is greater than B" << std::endl;
    }
    
    return 0;
}
