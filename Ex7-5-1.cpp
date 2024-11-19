/**
 * Solutions to Chapter 7, Section 5, Exercise 1 of Horowitz, Sahni, and Mehta's
 * Fundamentals of Data Structure in C++.
 * Task 1:
 * Write a function to shift the records (x1, x2, ..., xn) circularly to the right
 * by p, 0 <= p <= n, positions. Your function should have time complexity O(n)
 * and space complexity O(1). (Hint: Use three calls to a segment reversal 
 * function.)
 */

#include <iostream>

class Element {
  public:
    int getKey() const { return key; }
    void setKey(int k) { key = k; }
    
    Element(int k = 0) : key(k), link(0) {}
    
    friend int ListMerge(Element *list, const int start1, const int start2);
    friend int recursiveMergeSort(Element *list, const int left, const int right);
    friend void printLinkedList(Element *list, int start);
    
  private:
    int key;
    int link;
};

// Reverses the elements in the range [start, end]
void reverseSegment(Element* arr, int start, int end) {
    while (start < end) {
        // Swap elements at start and end
        int tempKey = arr[start].getKey();
        arr[start].setKey(arr[end].getKey());
        arr[end].setKey(tempKey);
        start++;
        end--;
    }
}

// Shifts array circularly to the right by p positions
void circularShiftRight(Element* arr, int n, int p) {
    if (n <= 1 || p == 0) return;  // No shift needed
    
    // Normalize p to be within array bounds
    p = p % n;
    if (p == 0) return;  // No shift needed after normalization
    
    // Example: For array [1,2,3,4,5] with p=2
    // 1. Reverse whole array: [5,4,3,2,1]
    // 2. Reverse first p elements: [4,5,3,2,1]
    // 3. Reverse remaining n-p elements: [4,5,1,2,3]
    
    // Step 1: Reverse the entire array
    reverseSegment(arr, 0, n - 1);
    
    // Step 2: Reverse first p elements
    reverseSegment(arr, 0, p - 1);
    
    // Step 3: Reverse remaining n-p elements
    reverseSegment(arr, p, n - 1);
}

// Helper function to print the array
void printArray(Element* arr, int n) {
    std::cout << "Array: ";
    for (int i = 0; i < n; i++) {
        std::cout << arr[i].getKey() << " ";
    }
    std::cout << std::endl;
}

// Test the implementation
int main() {
    const int size = 11;
    Element arr[size] = {12, 2, 16, 30, 8, 28, 4, 10, 20, 6, 18};
    
    std::cout << "Original ";
    printArray(arr, size);
    
    // Test with different shift amounts
    int shiftsToTest[] = {3, 5, size, size+2};
    for (int p : shiftsToTest) {
        Element testArr[size];
        // Copy original array
        for (int i = 0; i < size; i++) {
            testArr[i] = arr[i];
        }
        
        circularShiftRight(testArr, size, p);
        std::cout << "After " << p << " position shift: ";
        printArray(testArr, size);
    }
    
    return 0;
}
