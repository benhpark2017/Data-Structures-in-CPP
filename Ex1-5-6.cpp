/**
 * Solutions to Chapter 1, Section 5, Exercise 6 of Horowitz's
 * Data Structures in C++.
 */

#include <iostream>
#include <vector>

// Iterative binary search function
int binarySearch(const std::vector<int>& arr, int target) {
    int left = 0;
    int right = arr.size() - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;

        // Check if target is present at mid
        if (arr[mid] == target) {
            return mid;
        }

        // If target is greater, ignore the left half
        if (arr[mid] < target) {
            left = mid + 1;
        } 
        // If target is smaller, ignore the right half
        else {
            right = mid - 1;
        }
    }

    // Target is not present in the array
    return -1;
}

// Recursive binary search function
int binarySearchRecursive(const std::vector<int>& arr, int left, int right, int target) {
    if (left <= right) {
        int mid = left + (right - left) / 2;

        // Check if the target is present at mid
        if (arr[mid] == target) {
            return mid;
        }

        // If target is smaller, ignore the right half
        if (arr[mid] > target) {
            return binarySearchRecursive(arr, left, mid - 1, target);
        }

        // If target is larger, ignore the left half
        return binarySearchRecursive(arr, mid + 1, right, target);
    }

    // Target is not present in the array
    return -1;
}

// Wrapper function for the recursive binary search
int binarySearchRecursive(const std::vector<int>& arr, int target) {
    return binarySearchRecursive(arr, 0, arr.size() - 1, target);
}

int main() {
    std::vector<int> arr = {2, 3, 4, 10, 40};
    int target = 10;

    int result = binarySearch(arr, target);
    if (result != -1) {
        std::cout << "Element is present at index " << result << std::endl;
    } else {
        std::cout << "Element is not present in array" << std::endl;
    }

    int resultRecursive = binarySearchRecursive(arr, target);
    if (resultRecursive != -1) {
        std::cout << "Element is present at index " << resultRecursive << std::endl;
    } else {
        std::cout << "Element is not present in array" << std::endl;
    }

    return 0;
}
