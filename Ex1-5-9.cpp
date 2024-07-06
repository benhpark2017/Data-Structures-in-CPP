/**
 * Solutions to Chapter 1, Section 5, Exercise 9 and 10 of Horowitz's
 * Data Structures in C++.
 */

#include <iostream>

int recursiveBinomialCoeff(int n, int m) {
	if (m == 0 || m == n) return 1;
	else return recursiveBinomialCoeff(n - 1, m) + recursiveBinomialCoeff(n - 1, m - 1);
}

int iterativeBinomialCoeff(int n, int m) {
	int numerator = 1;
	int firstTerm = 1;
	int secondTerm = 1;
	int result;
	
	for (int i = 1; i <= n; i++) {
	    numerator *= i;
	}
	
	for (int j = 1; j <= m; j++) {
	    firstTerm *= j;
	}
	
	for (int k = 1; k <= n - m; k++) {
	    secondTerm *= k;
	}
	
	result = numerator / (firstTerm * secondTerm);
	
	return result;
}

int main() {
    int n = 0, m = 0;
    std::cout << "Please enter the first number to choose from. " << std::endl;
    std::cin >> n;
    std::cout<< "Please enter the second number for choosing." << std::endl;
    std::cin >> m;
    
    std::cout << "The iterative Binomial Coefficient is " 
              << iterativeBinomialCoeff(n, m) << std::endl;
    std::cout << "The recursive Binomial Coefficient is " 
              << recursiveBinomialCoeff(n, m) << std::endl;              
              
}