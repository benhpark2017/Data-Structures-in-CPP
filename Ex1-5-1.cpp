/**Solutions to Chapter 1, Section 5, Exercise 1 of Horowitz's Data Structures
 * in C++. */

#include <iostream>
#include <vector>

// For this recursive implementation of Horner's rule, the total step count is 2n + 2.
int stepCount = 0;

// Function to evaluate a polynomial using Horner's Rule
double evaluatePolynomial(int degree, const std::vector<double>& coefficients, double x) {
    stepCount++;
    // Base case: if the degree is 0, return the constant term
    if (degree == 0) {
        stepCount++;
        return coefficients[0];
    } else {
        stepCount++;
        // Recursive case: evaluate the polynomial with reduced degree
        return evaluatePolynomial(degree - 1, coefficients, x) * x + coefficients[degree];
    }
}

int main() {
    int degree;

    // Get the degree of the polynomial from the user
    std::cout << "Enter the degree of the polynomial: ";
    std::cin >> degree;

    // Ensure the degree is non-negative
    if (degree < 0) {
        std::cout << "Invalid degree. Please enter a non-negative integer.\n";
        return 1;  // Exit with an error code
    }

    // Dynamically allocate an array to store coefficients
    std::vector<double> coefficients(degree + 1);

    // Get coefficients from the user
    for (int i = degree; i >= 0; i--) {
        std::cout << "Enter the coefficient for x^" << i << ": ";
        std::cin >> coefficients[i];
    }

    double x;  // Value of x for evaluation

    // Get the value of x from the user
    std::cout << "Enter the value of x for evaluation: ";
    std::cin >> x;

    // Evaluate the polynomial using Horner's Rule
    double result = evaluatePolynomial(degree, coefficients, x);

    // Display number of step counts
    std::cout << "Number of steps taken: " << stepCount << "\n";

    // Display the result
    std::cout << "Result of the polynomial evaluation at x = " << x << " is: " << result << "\n";

    return 0;
}

