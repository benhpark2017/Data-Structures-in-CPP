/**
 * Solutions to Chapter 1, Section 5, Exercise 7 of Horowitz's
 * Data Structures in C++.
 */

#include <iostream>

int recursiveFactorial(int n) {
    if (n < 0) return -1;
    else if (n == 0 || n == 1) return 1;
    else return n * recursiveFactorial(n - 1);
}

int iterativeFactorial(int n) {
    int factorial = 1;
    for (int i = 1; i <= n; i++)
        factorial *= i;
    return factorial;
}

int main() {
    int number;
    int fact = 1;
    std::cout << "Enter a number: ";
    std::cin >> number;
	std::cout << std::endl;
    std::cout << "Factorial "<< number << " is " 
              << recursiveFactorial(number) << std::endl;
    std::cout << "Factorial "<< number << " is " 
              << iterativeFactorial(number) << std::endl;
}