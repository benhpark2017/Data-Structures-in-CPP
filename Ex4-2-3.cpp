/**
 * Solutions to Chapter 4, Section 2, Exercise 3 of Horowitz's
 * Fundamentals of Data Structures in C++.
 */

/*******************************************main.cpp***********************************************/
#include "List.h"

int main() {
    List list;
    ListNode *LN = list.getFirst();
    
    for (int i = 0; i < 10; i++) {
        LN = list.Insert(LN, i + 1);
    }
    
    std::cout << list << std::endl;
    std::cout << "Number of nodes in the current list: " << list.Length() << std::endl;
    
    list.deleteEveryOtherNode(); // Delete every other node starting with the first
    
    std::cout << "List after deleting every other node: " << list << std::endl;

    std::cout << "Number of nodes in the list after deletion: " << list.Length() << std::endl;
    
    return 0;
}


/********************************************List.h************************************************/
#ifndef LIST_H
#define LIST_H

#include <iostream>

// Forward declaration of List class
class List;

class ListNode {
private:
    int data;
    ListNode *link;

    ListNode(int element = 0, ListNode *next = NULL) : data(element), link(next) {}
    
    int getData() { return data; }

    friend class List;
    friend std::ostream &operator<<(std::ostream &os, List &l); // Required for getData() in ListNode
};

class List {
private:
    ListNode *first;

public:
    List() { first = NULL; }
    ListNode *getFirst() { return first; } // Returns the first node
    ListNode *Insert(ListNode *x, int i);  // Adds a node
    void Delete(ListNode* x);
    int Length();                          // Counts the number of nodes
    void deleteEveryOtherNode();           // Deletes every other node starting with the first
    friend std::ostream &operator<<(std::ostream &os, List &l);
};

#endif



/*******************************************List.cpp***********************************************/
#include "List.h"

ListNode *List::Insert(ListNode *x, int i) {
    if (first) { // Add between existing nodes
        ListNode *temp = new ListNode(i, x->link);
        x->link = temp; // x points to temp
        return x->link; // Return position to add the next value
    } else { // If there are no existing nodes
        first = new ListNode(i);
        return first;
    }
}

void List::Delete(ListNode* x) {
    if (first == NULL) {
        std::cout << "The linked list is empty." <<std::endl;
    }
    if (x == first) {
        first = first->link;
    } else {
        ListNode* temp = first;
        while (temp->link != x)
            temp = temp->link;
        temp->link = x->link;
    }
    delete x;
}

int List::Length() {
    int len = 0;
    ListNode *temp = first;
    while (temp != NULL) { // Until it points to NULL
        if (temp->link == NULL) {
            temp->link = (ListNode *)0; // Set the link of the last node to 0
        }
        temp = temp->link;
        len++;
    }
    return len;
}

//Solution to Chapter 4, Section 2, Exercise 3. This function iterates a while
//loop over n/2 items of the linked list, resulting in a O(n) time complexity.
void List::deleteEveryOtherNode() {
    if (first == NULL || first->link == NULL) {
        return;  // Nothing to delete if there are 0 or 1 nodes
    }

    ListNode* current = first;
    first = first->link;  // Move the head to the second node
    delete current;  // Delete the first node

    current = first;  // Start from the new first node (originally the second)

    while (current != NULL && current->link != NULL) {
        ListNode* temp = current->link;
        current->link = temp->link;
        delete temp;
        current = current->link;
    }
}

std::ostream &operator<<(std::ostream &os, List &l) {
    ListNode *temp = l.first;
    while (temp != NULL) {
        os << temp->getData();
        if (temp->link != NULL && temp->link != (ListNode *)0) // Prevent the last "->" from being printed
            os << "->";
        temp = temp->link; // Move to the next node
    }
    return os;
}
