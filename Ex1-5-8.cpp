/**
 * Solutions to Chapter 1, Section 5, Exercise 8 of Horowitz's
 * Data Structures in C++.
 */

#include <iostream>

int iterativeFibonacci(int n) {
    int a = 0;
    int b = 1;
    int fibResult = 0;    
    if (n < 0) {
        std::cout << "Please enter a nonnegative integer." << std::endl;
        return -1;
    }
    else if (n <= 1) return n;
    else if (n > 1){
        for (int i = 2; i <= n; i++) {
            fibResult = a + b;
            a = b;
            b = fibResult;
        }
    }
    return fibResult;
}

int recursiveFibonacci(int n) {
    if (n < 0) {
        std::cout << "Please enter a nonnegative integer." << std::endl;
        return -1;
    }
    else if (n == 0) return 0;
    else if (n == 1) return 1;
    else return recursiveFibonacci(n - 1) + recursiveFibonacci(n - 2);
}

int main() {
    int n;
    std::cout << "Enter the position of the Fibonacci sequence: ";
    std::cin >> n;

    if (recursiveFibonacci(n) != -1) {
        std::cout << "The recursively calculated Fibonacci number at position "
                  << n << " is " << recursiveFibonacci(n) << std::endl;
    }

    if (iterativeFibonacci(n) != -1) {
        std::cout << "The iteratively calculated Fibonacci number at position "
                  << n << " is " << iterativeFibonacci(n) << std::endl;
    }

    return 0;
}
