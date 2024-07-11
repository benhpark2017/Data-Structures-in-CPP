/**
 * Solutions to Chapter 2, Section 8, Exercise 8 of Horowitz's
 * Fundamentals of Data Structures in C++.
 */
 
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <cstdlib>
#include <ctime>

// The value function remains the same
int value(int n, int a, int b, int i, int j, const int* c) {
    if (i < 0 || i >= n || j < 0 || j >= n) {
        throw std::out_of_range("Matrix indices out of bounds");
    }

    int k = j - i + a - 1;
    
    if (k < 0 || k >= a + b - 1) {
        return 0;  // Outside the band, so value is zero
    }

    int index = i * (a + b - 1) + k;
    
    if (index < 0 || index >= n * (a + b - 1)) {
        throw std::out_of_range("Array index out of bounds");
    }

    return c[index];
}

int main() {
    const int n = 5;  // 5x5 matrix
    const int a = 1;  // 0 diagonals below main diagonal
    const int b = 2;  // 1 diagonal above main diagonal

    // Seed the random number generator
    srand(time(0));

    // Calculate the correct size for array c
    const int c_size = n * (a + b - 1) - (a + b - 2);

    // Create and populate the band array
    int c[c_size];
    for (int i = 0; i < c_size; ++i) {
        c[i] = rand() % 50 + 1;  // Random integer from 1 to 50
    }

    // Print the band array
    std::cout << "Band array c:" << std::endl;
    for (int i = 0; i < c_size; ++i) {
        std::cout << std::setw(4) << c[i] << " ";
    }
    std::cout << std::endl << std::endl;

    // Print the full matrix
    std::cout << "Full matrix:" << std::endl;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            std::cout << std::setw(4) << value(n, a, b, i, j, c) << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}