/**Solutions to Chapter 1, Section 5, Exercise 4 of Horowitz's
 * Data Structures in C++.
 */

#include <iostream>

void searchArray(int a[], int n, int x, int &j) {
    // Initialize j to -1, indicating x is not found
    j = -1;
    
    // Loop through the array to search for x
    for (int i = 0; i < n; i++) {
        if (a[i] == x) {
            j = i; // Set j to the index of x if found
            break; // Exit the loop once x is found
        }
    }
}

int main() {
    int n, x, j;
    
    // Get the size of the array from the user
    std::cout << "Enter the size of the array: ";
    std::cin >> n;
    
    // Create an array of size n
    int a[n];
    
    // Get the elements of the array from the user
    std::cout << "Enter the elements of the array:\n";
    for (int i = 0; i < n; i++) {
        std::cin >> a[i];
    }
    
    // Get the element to search for from the user
    std::cout << "Enter the element to search for: ";
    std::cin >> x;
    
    // Search for the element in the array
    searchArray(a, n, x, j);
    
    // Print the result
    if (j != -1) {
        std::cout << "Element " << x << " found at index " << j << ".\n";
    } else {
        std::cout << "Element " << x << " not found in the array.\n";
    }
    
    return 0;
}

