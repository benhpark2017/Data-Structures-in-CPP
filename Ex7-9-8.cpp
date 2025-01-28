/**
 * Solutions to Chapter 7, Section 9, Exercise 8 of Horowitz, Sahni, and Mehta's
 * Fundamentals of Data Structures in C++, 1st Edition.* 
 * 
 * Task 8:
 * (a) Write a C++ function for bubble sort.
 * (b) What is the worst-case complexity of your function?
 * (c) How much time does your function take on a sorted array of records?
 * (d) How much time does your function take on an array of records that are in
 *     the reverse of sorted order?
 * 
 * Responses to conceptual problems
 * (b) O(n^2).
 * (c) O(n).
 * (d) The reverse-sorted array presents the worst-case time for input data.
 *     Therefore, the time complexity of bubble sort in this case is O(n^2).
 */
 
#include <iostream>

// Function to perform Bubble Sort
void bubbleSort(int arr[], int n) {
    for (int i = 0; i < n-1; i++) {
        // Flag to check if any swapping happened
        bool swapped = false;
        
        // Last i elements are already sorted, so no need to check them
        for (int j = 0; j < n-i-1; j++) {
            if (arr[j] > arr[j+1]) {
                // Swap arr[j] and arr[j+1]
                int temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
                
                swapped = true;
            }
        }
        
        // If no elements were swapped, the array is already sorted
        if (!swapped) {
            break;
        }
    }
}

// Function to print an array
void printArray(int arr[], int size) {
    for (int i = 0; i < size; i++) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;
}

int main() {
    int arr[] = {64, 34, 25, 12, 22, 11, 90};
    int n = sizeof(arr)/sizeof(arr[0]);
    
    std::cout << "Original array: ";
    printArray(arr, n);
    
    bubbleSort(arr, n);
    
    std::cout << "Sorted array: ";
    printArray(arr, n);
    
    return 0;
}
