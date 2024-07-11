/**
 * Solutions to Chapter 2, Section 8, Exercise 1 of Horowitz's
 * Fundamentals of Data Structure in C++.
 */

#include <iostream>
#include <cstdlib>

void Reverse(int *a, int length) {
    for (int i = 0; i < length/2; i++) {
        int temp = a[i];
        a[i] = a[length - i - 1]; 
        a[length - i - 1] = temp; 
    }
}

int main() {
    int list[] = {44, 93, 53, 98, 100, 28, 75, 36, 33, 87};
    int size = sizeof(list) / sizeof(int);
    
    std::cout << "The array is initially "; 
    for (int i = 0; i < size; i++) 
        std::cout << list[i] << " ";
    std::cout << std::endl;
    
    Reverse(list, size);
    std::cout << "The reversed array is now ";
    for (int i = 0; i < size; i++) 
        std::cout << list[i] << " ";
    std::cout << std::endl;
}