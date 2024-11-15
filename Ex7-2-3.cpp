/**
 * Solutions to Chapter 7, Section 2, Exercise 3 of Horowitz, Sahni, and Mehta's
 * Fundamentals of Data Structure in C++.
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
 */

/**********************************main.cpp************************************/

#include <iostream>
#include <vector>

// Definition of a singly linked list node
struct Node {
    int data;
    Node* next;
    Node(int value) : data(value), next(nullptr) {}
};

// Function to insert a node into the sorted part of the list
Node* sortedInsert(Node* sorted, Node* newNode) {
    if (!sorted || newNode->data < sorted->data) {
        newNode->next = sorted;
        return newNode;
    }
    
    Node* current = sorted;
    while (current->next && current->next->data < newNode->data) {
        current = current->next;
    }
    newNode->next = current->next;
    current->next = newNode;
    return sorted;
}

// Function to perform insertion sort on a linked list
Node* insertionSortLinkedList(Node* head) {
    Node* sorted = nullptr;
    Node* current = head;

    while (current) {
        Node* next = current->next; // Save the next node
        sorted = sortedInsert(sorted, current); // Insert current node into sorted list
        current = next; // Move to the next node
    }
    return sorted;
}

// Helper function to create a linked list from a vector of integers
Node* createLinkedList(const std::vector<int>& values) {
    Node* head = nullptr;
    Node* tail = nullptr;

    for (int value : values) {
        Node* newNode = new Node(value);
        if (!head) {
            head = tail = newNode; // Initialize the head and tail
        } else {
            tail->next = newNode; // Append to the end
            tail = newNode;
        }
    }

    return head;
}

// Helper function to print a linked list
void printList(Node* head) {
    while (head) {
        std::cout << head->data << " ";
        head = head->next;
    }
    std::cout << std::endl;
}

// Example usage
int main() {
    // Populate linked list using a vector
    std::vector<int> values = {37, 23, 0, 17, -5, 7, 12, 92, 31};
    Node* head = createLinkedList(values);

    std::cout << "Original list: ";
    printList(head);
    
    // Sort the list
    head = insertionSortLinkedList(head);
    
    std::cout << "Sorted list: ";
    printList(head);
    
    return 0;
}
