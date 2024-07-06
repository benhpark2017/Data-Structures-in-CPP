/**
 * Solutions to Chapter 1, Section 5, Exercise 13 of Horowitz's
 * Data Structures in C++.
 */

#include <iostream>

// Function to calculate the sum of proper divisors of n
int sumOfDivisors(int n) {
    int sum = 0;
    for (int i = 1; i <= n / 2; ++i) {
        if (n % i == 0) {
            sum += i;
        }
    }
    return sum;
}

int main() {
    int n;
    std::cout << "Enter a positive integer: ";
    std::cin >> n;

    if (n <= 0) {
        std::cout << "The input must be a positive integer." << std::endl;
        return 1;
    }

    int sum = sumOfDivisors(n);

    if (sum == n) {
        std::cout << n << " is the sum of all its proper divisors." << std::endl;
    } else {
        std::cout << n << " is not the sum of all its proper divisors." << std::endl;
    }

    return 0;
}