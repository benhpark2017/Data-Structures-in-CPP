/**
 * Solutions to Chapter 4, Section 2, Exercise 6 of Horowitz's
 * Fundamentals of Data Structures in C++.
 */

/*******************************************main.cpp***********************************************/
#include "List.h"

int main() {
    List newList;
    ListNode* current = nullptr;

    for (int i = 1; i <= 10; i++) {
        current = newList.Insert(current, i);
    }
    std::cout << "\nNew list for movement demonstration: " << newList << std::endl;

    ListNode* l = nullptr;
    ListNode* r = newList.getFirst();

    std::cout << "Moving right by 4 nodes:" << std::endl;
    newList.moveRight(l, r, 4);
    std::cout << "l points to: " << (l ? l->getData() : 0) << std::endl;
    std::cout << "r points to: " << (r ? r->getData() : 0) << std::endl;

    std::cout << "Moving left by 2 nodes:" << std::endl;
    newList.moveLeft(l, r, 2);
    std::cout << "l points to: " << (l ? l->getData() : 0) << std::endl;
    std::cout << "r points to: " << (r ? r->getData() : 0) << std::endl;
    
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
    
public:
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
    void moveRight(ListNode*& l, ListNode*& r, int n);
    void moveLeft(ListNode*& l, ListNode*& r, int n);
    
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

void List::moveRight(ListNode*& l, ListNode*& r, int n) {
    while (n > 0 && r != nullptr) {
        ListNode* temp = r->link;
        r->link = l;
        l = r;
        r = temp;
        n--;
    }
    
    if (r == nullptr) {
        // We've reached the end of the list
        while (l != nullptr && l->link != r) {
            ListNode* temp = l->link;
            l->link = r;
            r = l;
            l = temp;
        }
    }
    
    // Update first if necessary
    if (l == nullptr) {
        first = r;
    }
}

void List::moveLeft(ListNode*& l, ListNode*& r, int n) {
    while (n > 0 && l != nullptr) {
        ListNode* temp = l->link;
        l->link = r;
        r = l;
        l = temp;
        n--;
    }
    
    if (l == nullptr) {
        // We've reached the start of the list
        while (r != nullptr && r->link != l) {
            ListNode* temp = r->link;
            r->link = l;
            l = r;
            r = temp;
        }
    }
    
    // Update first if necessary
    if (l == nullptr) {
        first = r;
    }
}
