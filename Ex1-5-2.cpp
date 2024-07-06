/**Solutions to Chapter 1, Section 5, Exercise 2 of Horowitz's 
 * Data Structures in C++.
 */

#include <iostream>
#include <vector>

// Function to print truth values for n variables
void prepareTruthValues(std::vector<char>& values, int n, int index) {
    if (index == n) {
        // Print the current permutation of truth values
        for (int i = 0; i < n; i++) {
            std::cout << values[i] << ' ';
        }
        std::cout << std::endl;
        return;
    }

    // Set the current variable to 'T' and use recursion
    values[index] = 'T';
    prepareTruthValues(values, n, index + 1);

    // Set the current variable to 'F' and use recursion
    values[index] = 'F';
    prepareTruthValues(values, n, index + 1);
}

// Function to initialize and call the recursive helper function
void printTruthValues(int n) {
    std::vector<char> values(n);
    prepareTruthValues(values, n, 0);
}

int main() {
    int n;

    // Get the number of variables from the user
    std::cout << "Enter the number of variables (n): ";
    std::cin >> n;

    // Validate input
    if (n <= 0) {
        std::cout << "Number of variables should be greater than 0.\n";
        return 1;
    }

    // Call the function to print truth values
    printTruthValues(n);

    return 0;
}

