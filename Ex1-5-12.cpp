/**
 * Solutions to Chapter 1, Section 5, Exercise 12 of Horowitz's
 * Data Structures in C++.
 */

#include <iostream>
#include <vector>

// Example function f. Modify this function as needed.
int f(int x) {
    return x % 5;  // For example purposes, f(x) = x % 5
}

int main() {
    int n;
    std::cout << "Enter the value of n: ";
    std::cin >> n;

    // Assume the function f(x) outputs values in the range [0, maxRange - 1]
    const int maxRange = 5;
    std::vector<int> seen(maxRange, -1);  // Initialize with -1 (indicating unseen)

    for (int i = 1; i <= n; ++i) {
        int value = f(i);

        // Check if this value has already been encountered
        if (seen[value] != -1) {
            std::cout << "Found two distinct inputs a and b such that f(a) = f(b):\n";
            std::cout << "a = " << seen[value] << ", b = " << i << ", f(a) = f(b) = " << value << std::endl;
            return 0;
        }

        // Mark this value as seen with the current index
        seen[value] = i;
    }

    std::cout << "No two distinct inputs found such that f(a) = f(b)." << std::endl;
    return 0;
}