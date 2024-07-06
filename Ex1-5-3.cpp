/**Solutions to Chapter 1, Section 5, Exercise 3 of Horowitz's
 * Data Structures in C++.
 */

#include <iostream>

// Function to swap two integers
void swap(int &a, int &b) {
    int temp = a;
    a = b;
    b = temp;
}

// Recursive function to sort three integers
void sortThree(int &x, int &y, int &z) {
    if (x > y) swap(x, y);
    if (y > z) swap(y, z);
    if (x > y) swap(x, y);
}

// Main function
int main() {
    int x, y, z;
    
    // Get the values from the user
    std::cout << "Enter three integers: ";
    std::cin >> x >> y >> z;
    
    // Sort the values
    sortThree(x, y, z);
    
    // Print the sorted values
    std::cout << x << " " << y << " " << z << std::endl;
    
    return 0;
}

