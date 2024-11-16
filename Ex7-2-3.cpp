/**
 * Solutions to Chapter 7, Section 2, Exercises 2 and 3 of Horowitz, Sahni, and
 * Mehta's Fundamentals of Data Structure in C++.
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
 * 
 * 
 * Task 3:
 * Write a C++ function that implements linked-list insertion sort. What is the
 * worst-case number of comparisons made by your sort function? What is the
 * worst-case number of record moves made? How do these compare with simple
 * insertion sort?
 * 
 * Analysis of Linked List Insertion Sort
 * 
 * Worst-case number of comparisons:
 * For each node, the algorithm traverses the sorted portion of the list to find
 * the correct position. In the worst case, this requires O(k) comparisons for
 * the k-th node.
 * 
 * Summing over all nodes:
 * Total comparisons = sum(k) for k = 1 to n - 1 = n(n-1)/2 ≈ O(n^2).
 * The number of comparisons is the same as in simple insertion sort for arrays.
 * 
 * Worst-case number of record moves:
 * In a linked list, moving a record involves only updating pointers rather than
 * shifting elements. Each insertion involves updating exactly 2 pointers (the 
 * next pointers of the preceding node and the inserted node). Therefore, the 
 * total number of pointer updates (analogous to record moves) is:
 * 
 * Total moves = 2(n-1) = O(n).
 * 
 * This is significantly better than the O(n^2) record moves required by simple 
 * insertion sort for arrays.
 * 
 * Comparison with Simple Insertion Sort
 * 
 * Comparisons:
 * Both linked list insertion sort and simple insertion sort for arrays require
 * O(n^2) comparisons in the worst case.
 * 
 * Record moves:
 * Linked list insertion sort requires O(n) pointer updates in the worst case.
 * Simple insertion sort for arrays requires O(n^2) moves in the worst case
 * because elements need to be physically shifted.
 * 
 * Conclusion:
 * Linked list insertion sort is advantageous when the cost of moving records is
 * high, as it reduces the number of record moves to O(n). However, it does not
 * reduce the number of comparisons.
 * 
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

// New Node class for linked list
class Node {
public:
    Element data;
    Node* next;
    
    Node(const Element& elem) : data(elem), next(nullptr) {}
};

class LinkedList {
private:
    Node* head;
    
    // Helper function to insert a node in sorted position
    void sortedInsert(Node* newNode) {
        // If list is empty or new node should be at start
        if (head == nullptr || head->data.getKey() >= newNode->data.getKey()) {
            newNode->next = head;
            head = newNode;
            return;
        }
        
        // Find the position to insert
        Node* current = head;
        while (current->next != nullptr && 
               current->next->data.getKey() < newNode->data.getKey()) {
            current = current->next;
        }
        
        newNode->next = current->next;
        current->next = newNode;
    }

public:
    LinkedList() : head(nullptr) {}
    
    // Destructor to clean up memory
    ~LinkedList() {
        Node* current = head;
        while (current != nullptr) {
            Node* next = current->next;
            delete current;
            current = next;
        }
    }
    
    // Insert a new element
    void insert(const Element& elem) {
        Node* newNode = new Node(elem);
        sortedInsert(newNode);
    }
    
    // Convert array to linked list and sort using insertion sort
    void insertionSort(Element* arr, int size) {
        // Clear existing list if any
        Node* current = head;
        while (current != nullptr) {
            Node* next = current->next;
            delete current;
            current = next;
        }
        head = nullptr;
        
        // Process each element
        for (int i = 0; i < size; i++) {
            std::cout << "\nInserting " << arr[i].getKey() << " into sorted list" << std::endl;
            insert(arr[i]);
            printList();
        }
    }
    
    // Convert back to array
    void toArray(Element* arr) {
        Node* current = head;
        int i = 0;
        while (current != nullptr) {
            arr[i++] = current->data;
            current = current->next;
        }
    }
    
    // Print the list
    void printList() const {
        Node* current = head;
        std::cout << "Current list: ";
        while (current != nullptr) {
            std::cout << current->data.getKey() << " ";
            current = current->next;
        }
        std::cout << std::endl;
    }
};

void insert(const Element& e, Element* list, int i) {
    int j = i;
    while (j >= 0 && e.getKey() < list[j].getKey()) {
        list[j + 1] = list[j];
        j--;
    }
    list[j + 1] = e;
}

void InsertionSort(Element* list, const int n) {
    for (int i = 1; i < n; i++) {
        Element current = list[i];
        insert(current, list, i - 1);
        
        // Print current state of array after each insertion
        std::cout << "After inserting " << current.getKey() << ": ";
        for (int k = 0; k < n; k++) {
            std::cout << list[k].getKey() << " ";
        }
        std::cout << std::endl;
    }
}

// Binary search to find insertion position
int binarySearch(Element* list, int target, int low, int high) {
    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (list[mid].getKey() == target) {
            return mid + 1;  // Insert after the existing element
        } else if (list[mid].getKey() < target) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return low;  // This is the insertion point
}

void BinaryInsertionSort(Element* list, const int n) {
    for (int i = 1; i < n; i++) {
        Element current = list[i];
        int j = i - 1;
        
        // Use binary search to find the insertion position
        int insertPos = binarySearch(list, current.getKey(), 0, j);
        
        // Print state before insertion
        std::cout << "Finding position for " << current.getKey() << ": ";
        for (int k = 0; k < n; k++) {
            std::cout << list[k].getKey() << " ";
        }
        std::cout << "\nInsertion position: " << insertPos << std::endl;
        
        // Shift elements to make room for insertion
        for (int k = j; k >= insertPos; k--) {
            list[k + 1] = list[k];
        }
        
        list[insertPos] = current;
        
        // Print state after insertion
        std::cout << "After insertion: ";
        for (int k = 0; k < n; k++) {
            std::cout << list[k].getKey() << " ";
        }
        std::cout << "\n" << std::endl;
    }
}


int main() {
    const int size = 15;
    
    LinkedList list;
    
    Element F1[size] = {12, 2, 16, 26, 30, 14, 8, 28, 4, 22, 10, 20, 24, 6, 18};
    Element F2[size] = {12, 2, 16, 26, 30, 14, 8, 28, 4, 22, 10, 20, 24, 6, 18};
    Element F3[size] = {12, 2, 16, 26, 30, 14, 8, 28, 4, 22, 10, 20, 24, 6, 18};
        
    std::cout << "Original list 1 (for simple insertion sort): ";
    for (int i = 0; i < size; i++) {
        std::cout << F1[i].getKey() << " ";
    }
    std::cout << std::endl;
    
    std::cout << "Original list 2 (for binary insertion sort): ";
    for (int i = 0; i < size; i++) {
        std::cout << F2[i].getKey() << " ";
    }
    std::cout << std::endl;
    
    std::cout << "Original list 3 (for linked insertion sort): ";
    for (int i = 0; i < size; i++) {
        std::cout << F3[i].getKey() << " ";
    }
    std::cout << std::endl;
    
    std::cout << "\n\nSorting process:\n";
    std::cout << "\nSimple Insertion Sort:\n";
    InsertionSort(F1, size);
    
    std::cout << "\nBinary Insertion Sort:\n";
    BinaryInsertionSort(F2, size);
    
    std::cout << "Linked-list Insertion Sort:\n";
    list.insertionSort(F3, size);
    list.toArray(F3);
    
    std::cout << "\nFinal sorted list using simple insertion sort:\n";
    for (int i = 0; i < size; i++) {
        std::cout << F1[i].getKey() << " ";
    }
    std::cout << std::endl;
    
    std::cout << "\nFinal sorted list using binary insertion sort:\n";
    for (int i = 0; i < size; i++) {
        std::cout << F2[i].getKey() << " ";
    }
    std::cout << std::endl;
    
    std::cout << "\nFinal sorted list using linked-list insertion sort:\n";
    for (int i = 0; i < size; i++) {
        std::cout << F3[i].getKey() << " ";
    }
    std::cout << std::endl << std::endl;
    std::cout << "End of program." << std::endl;
    return 0;
}
