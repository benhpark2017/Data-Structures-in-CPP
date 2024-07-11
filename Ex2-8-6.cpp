/**
 * Solutions to Chapter 2, Section 8, Exercise 6 of Horowitz's
 * Fundamentals of Data Structure in C++.
 */

#include <iostream>
#include <cmath>
#include <stdexcept>

const int MAX_SIZE = 100; // Assuming a maximum matrix size

double getTridiagonalElement(double B[3][MAX_SIZE], int n, int i, int j) {
    // Check if indices are within bounds
    if (i < 0 || i >= n || j < 0 || j >= n) {
        throw std::out_of_range("Matrix indices out of bounds");
    }

    // Check if the element is within the tridiagonal band
    if (std::abs(i - j) > 1) {
        return 0.0; // Elements outside the band are zero
    }

    if (i == j) {
        return B[1][i];  // Main diagonal
    } else if (i > j) {
        return B[0][j];  // Lower diagonal
    } else {
        return B[2][i];  // Upper diagonal
    }
}

int main() {
    int n = 5; // Size of the matrix
    double B[3][MAX_SIZE] = {
        {1, 2, 3, 4},     // Lower diagonal
        {5, 6, 7, 8, 9},  // Main diagonal
        {10, 11, 12, 13}  // Upper diagonal
    };

    // Print the full matrix
    std::cout << "The full matrix A:" << std::endl;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            std::cout << getTridiagonalElement(B, n, i, j) << " ";
        }
        std::cout << std::endl;
    }

    // Example usage
    std::cout << "\nSome specific elements:" << std::endl;
    std::cout << "A[0][0] = " << getTridiagonalElement(B, n, 0, 0) << std::endl;
    std::cout << "A[1][2] = " << getTridiagonalElement(B, n, 1, 2) << std::endl;
    std::cout << "A[2][1] = " << getTridiagonalElement(B, n, 2, 1) << std::endl;
    std::cout << "A[3][3] = " << getTridiagonalElement(B, n, 3, 3) << std::endl;
    std::cout << "A[0][2] = " << getTridiagonalElement(B, n, 0, 2) << std::endl;

    return 0;
}