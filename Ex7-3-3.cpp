/**
 * Solutions to Chapter 7, Section 3, Exercises 1, 2(c) and 3 of Horowitz, 
 * Sahni, and Mehta's Fundamentals of Data Structure in C++.
 * 
 * Task 1:
 * Write the status of the list F = {12, 2, 16, 30, 8, 28, 4, 10, 20, 6, 18} at
 * the end of each phase of QuickSort.
 * Response: This has been implemented by the RecursiveQuickSortStatus()
 * function in Test 2 of the program suite.
 * 
 * Task 2(c):
 * Why is list[left].key <= list.[right + 1].key required in the function 
 * RecursiveQuickSort()?
 * Response:
 * The given inequality is necessary to ensure that elements are correctly
 * partitioned relative to the pivot, and the recursive calls can continue to
 * divide and sort the subarrays properly.
 * 
 * Task 3:
 * (a) Write a nonrecursive version of RecursiveQuickSort() incorporating the
 *     median-of-three rule to determine the pivot key.
 * (b) Show that this function takes O(n*log(n)) time on an already sorted list.
 */

#include <iostream>
#include <stack>
#include <utility>

class Element {
  public:
    int getKey() const { return key; }
    void setKey(int k) { key = k; }
    Element(int k = 0) : key(k) {}
  private:
    int key;
};

void printList(Element *list, int size) {
    static int count = 0;
    std::cout << "After phase " << count << ": ";
    for (int i = 0; i < size; i++) {  // Always print the entire array
        std::cout << list[i].getKey() << " ";
    }
    count++;
    std::cout << std::endl;
}

void RecursiveQuickSort(Element *list, const int left, const int right) {
    if (left < right) {
        int i = left;
        int j = right + 1;
        int pivot = list[left].getKey();
        
        do {
            do i++; while (list[i].getKey() < pivot);
            do j--; while (list[j].getKey() > pivot);
            if (i < j) {
                Element temp = list[i];
                list[i] = list[j];
                list[j] = temp;
            }
        } while (i < j);
        Element temp = list[left];
        list[left] = list[j];
        list[j] = temp;
        
        RecursiveQuickSort(list, left, j - 1);
        RecursiveQuickSort(list, j + 1, right);
    }
}

void RecursiveQuickSortStatus(Element *list, const int left, const int right, int size) {
    if (left < right) {
        int i = left;
        int j = right + 1;
        int pivot = list[left].getKey();
        do {
            //comment out this line if necessary. This line shows the list after
            //each recursive phase.
            printList(list, size);
            do i++; while (list[i].getKey() < pivot);
            do j--; while (list[j].getKey() > pivot);
            if (i < j) {
                Element temp = list[i];
                list[i] = list[j];
                list[j] = temp;
            }
        } while (i < j);
        Element temp = list[left];
        list[left] = list[j];
        list[j] = temp;
        
        RecursiveQuickSortStatus(list, left, j - 1, size);
        RecursiveQuickSortStatus(list, j + 1, right, size);
    }
}

void swap(Element &a, Element &b) {
    Element temp = a;
    a = b;
    b = temp;
}

// Function to partition the array around the pivot
int partition(Element *list, int left, int right) {
    // Select the pivot using the median-of-three rule
    int mid = left + (right - left) / 2;
    int pivot = list[left].getKey();
    
    // Find the median of list[left], list[mid], list[right]
    if (list[mid].getKey() < list[left].getKey()) swap(list[left], list[mid]);
    if (list[right].getKey() < list[left].getKey()) swap(list[left], list[right]);
    if (list[right].getKey() < list[mid].getKey()) swap(list[mid], list[right]);
    
    // Set the pivot to be the middle element after median-of-three
    pivot = list[mid].getKey();
    
    // Move the pivot to the right for partitioning
    swap(list[mid], list[right]);
    
    int i = left - 1;
    for (int j = left; j < right; j++) {
        if (list[j].getKey() <= pivot) {
            i++;
            swap(list[i], list[j]);
        }
    }
    // Swap pivot into correct position
    swap(list[i + 1], list[right]);
    return i + 1;
}

// Iterative QuickSort using the median-of-three pivot rule
void iterativeQuickSort(Element *list, int left, int right) {
    std::stack<std::pair<int, int>> stack;
    stack.push({left, right});
    
    while (!stack.empty()) {
        int l = stack.top().first;
        int r = stack.top().second;
        stack.pop();
        
        if (l < r) {
            // Partition the array and get the pivot index
            int pivotIndex = partition(list, l, r);
            
            // Push the subarrays onto the stack for further sorting
            stack.push({l, pivotIndex - 1});
            stack.push({pivotIndex + 1, r});
        }
    }
}


int main() {

    const int size = 10;
    const int length = 11;
    std::cout << "Example as printed in p.406 of the textbook.\n";
    Element F1[size] = {26, 5, 37, 1, 61, 11, 59, 15, 48, 19};
    Element F2[size] = {26, 5, 37, 1, 61, 11, 59, 15, 48, 19};
    Element F3[length] = {12, 2, 16, 30, 8, 28, 4, 10, 20, 6, 18};
    
/******************************************************************************/
    std::cout << "Test 1: Iterative vs Recursive Quick Sort."<< std::endl;
    std::cout << "Original list before recursive quick sort: ";
    for (int i = 0; i < size; i++) {
        std::cout << F1[i].getKey() << " ";
    }
    std::cout << std::endl;
    
    std::cout << "Original list before iterative quick sort: ";
    for (int i = 0; i < size; i++) {
        std::cout << F2[i].getKey() << " ";
    }
    std::cout << std::endl;
    
    RecursiveQuickSort(F1, 0, size - 1);
    iterativeQuickSort(F2, 0, size - 1);
    
    std::cout << "Sorted list using recursive quick sort: ";
    for (int i = 0; i < size; i++) {
        std::cout << F1[i].getKey() << " ";
    }
    std::cout << std::endl;
    
    std::cout << "Sorted list using iterative quick sort: ";
    for (int i = 0; i < size; i++) {
        std::cout << F2[i].getKey() << " ";
    }
    std::cout << std::endl;
    std::cout << std::endl;
    
/******************************************************************************/
    std::cout << "Test 2: Recursive Quick Sort with Status Check." << std::endl;
    
    std::cout << "Original list before quick sort: ";
    for (int i = 0; i < length; i++) {
        std::cout << F3[i].getKey() << " ";
    }
    std::cout << std::endl;
    
    RecursiveQuickSortStatus(F3, 0, length - 1, length);
    
    std::cout << "Sorted list using recursive quick sort: ";
    for (int i = 0; i < length; i++) {
        std::cout << F3[i].getKey() << " ";
    }
    std::cout << std::endl;
    
    std::cout << "End of program." << std::endl;
    return 0;
}
