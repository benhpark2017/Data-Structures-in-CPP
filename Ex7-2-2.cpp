/**
 * Solutions to Chapter 7, Section 2, Exercise 2 of Horowitz, Sahni, and Mehta's
 * Fundamentals of Data Structure in C++.
 * 
 * Task 2:
 * Write a C++ function that implements binary insertion sort. What is the
 * worst-case number of comparisons made by your sort function? What is the
 * worst-case number of record moves made? How do these compare with simple
 * insertion sort?
 * 
 * Analysis of Binary Insertion Sort
 * 
 * Worst-case number of comparisons:
 * 
 * For each element in the array, binary search is performed to find its correct
 * position.
 * 
 * Binary search takes at most ceil(log2(k)) comparisons to position the k-th 
 * element in the sorted portion of the array.
 * 
 * Summing over all elements:
 * Total comparisons = sum(ceil(log2(k))) for k = 1 to n-1 ≈ n * log2(n) - O(n).
 * This is fewer comparisons than simple insertion sort, which may require up to
 * (n^2)/2 in the worst case.
 * 
 * Worst-case number of record moves:
 * In binary insertion sort, after finding the correct position via binary
 * search, all elements greater than the key need to be shifted by one position
 * to the right. This operation requires O(k) moves for the k-th element.
 * 
 * Summing over all elements:
 * Total moves = sum(k) for k = 1 to n-1 ≈ (n^2)/2.
 * This is the same as simple insertion sort because both methods involve moving
 * records in an identical manner.
 * 
 * Comparison with Simple Insertion Sort
 * Comparisons:
 * Binary insertion sort performs O(n log n) comparisons, whereas simple
 * insertion sort performs O(n^2) in the worst case.
 * 
 * Record moves:
 * Both methods require O(n^2) moves in the worst case because shifting elements
 * is the same for both algorithms.
 * 
 * Conclusion:
 * Binary insertion sort reduces the number of comparisons but does not improve
 * the number of record moves compared to simple insertion sort.
 */

/**********************************main.cpp************************************/

#include <vector>
#include <iostream>

// Function to perform binary search in a sorted subarray
int binarySearch(const std::vector<int>& arr, int item, int low, int high) {
    while (low < high) {
        int mid = low + (high - low) / 2;
        if (item >= arr[mid])
            low = mid + 1;
        else
            high = mid;
    }
    return low;
}

// Function to perform binary insertion sort
void binaryInsertionSort(std::vector<int>& arr) {
    int n = arr.size();
    for (int i = 1; i < n; ++i) {
        int key = arr[i];
        int pos = binarySearch(arr, key, 0, i);

        // Move elements to make room for the key
        for (int j = i; j > pos; --j) {
            arr[j] = arr[j - 1];
        }
        arr[pos] = key;
    }
}

// Helper function to print an array
void printArray(const std::vector<int>& arr) {
    for (int val : arr) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
}

// Example usage
int main() {
    std::vector<int> arr = {37, 23, 0, 17, -5, 7, 12, 92, 31};
    std::cout << "Original array: ";
    printArray(arr);

    binaryInsertionSort(arr);

    std::cout << "Sorted array: ";
    printArray(arr);

    return 0;
}
