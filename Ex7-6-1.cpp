/**
 * Solutions to Chapter 7, Section 6, Exercise 1 of Horowitz, Sahni, and Mehta's
 * Fundamentals of Data Structures in C++, 1st Edition.
 * 
 * Task 1:
 * Write the status of the list F = {12, 2, 16, 30, 8, 28, 4, 10, 20, 6, 18} at
 * the end of each phase of HeapSort().
 */

#include <iostream>

class Element {
  public:
    int getKey() const { return key; }
    void setKey(int k) { key = k; }
    
    Element(int k = 0) : key(k) {}
  private:
    int key;
};

void printList(Element *list, int length, const std::string& message = "") {
    if (!message.empty()) {
        std::cout << message << std::endl;
    }
    for (int i = 1; i < length; i++) {
        std::cout << list[i].getKey() << " ";
    }
    std::cout << std::endl;
}

void adjust(Element *tree, const int root, const int n) {
    Element e = tree[root];
    int k = e.getKey();
    int j;
    // First, find max of left and right children.
    for (j = 2 * root; j <= n; j *= 2) {
        if (j < n)
            if (tree[j].getKey() < tree[j + 1].getKey()) j++;
        
        // Compare max child with k. If k is max, we are done.     
        if (k >= tree[j].getKey()) break;
        tree[j / 2] = tree[j]; // move j-th record up the tree.
    }
    tree[j / 2] = e;
}

void HeapSort(Element *list, const int n) {
    // Heap Creation Phase (Heapify)
    std::cout << "\nHeap Creation Phase:\n";
    for (int i = n / 2; i >= 1; i--) {
        std::cout << "Adjusting subtree rooted at index " << i << ": ";
        adjust(list, i, n);
        printList(list, n + 1, "");
    }

    // Sorting Phase
    std::cout << "\nSorting Phase:\n";
    for (int i = n - 1; i >= 1; i--) {
        // Swap first (largest) and last elements
        std::cout << "Swap root with last unsorted element (index " << i + 1 << "): ";
        Element t = list[i + 1];
        list[i + 1] = list[1];
        list[1] = t;
        
        // Print the list after swap
        printList(list, n + 1, "After swap:");
        
        // Restore heap property for reduced heap
        std::cout << "Restore heap property: ";
        adjust(list, 1, i);
        printList(list, n + 1, "After adjustment:");
    }
}

int main() {
    const int length = 11;
    Element List[length + 1] = {0, 12, 2, 16, 30, 8, 28, 4, 10, 20, 6, 18};
    
    std::cout << "Initial List:" << std::endl;
    printList(List, length + 1);
    
    HeapSort(List, length);
    
    std::cout << "\nFinal Sorted List:" << std::endl;
    printList(List, length + 1);
    
    return 0;
}
