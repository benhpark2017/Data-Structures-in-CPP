/**
 * Solutions to Chapter 4, Section 2, Exercises from 1 to 2 of Horowitz's
 * Fundamentals of Data Structures in C++.
 */

/*************************************main.cpp******************************************/
#include "List.h"

int main() {
    List list;
    int num;
    ListNode *LN = list.getFirst();
    
    for (int i = 0; i < 10; i++) {
        LN = list.Insert(LN, i + 1);
    }
    
    std::cout << list << std::endl;
    std::cout << "Number of nodes in the current list: " << list.Length() << std::endl;
    
    ListNode* nodeToDelete = list.getFirst();
    std::cout << "Select a node to delete from 1 to 10." << std::endl;
    std::cin >> num;
    
    for (int i = 1; i < num; i++)
        nodeToDelete = nodeToDelete->Link();
        
    list.Delete(nodeToDelete);
    std::cout << list << std::endl;
    
    return 0;
}



/**************************************List.h******************************************/
#ifndef LIST_H
#define LIST_H

#include <iostream>

//Forward declaration of List class
class List;

class ListNode {
private:
    int data;
    ListNode *link;

public:
    ListNode(int element = 0, ListNode *next = NULL) : data(element), link(next) {}
    int getData() { return data; }
    ListNode *Link(){ return link; }
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
    void Delete(ListNode *x);
    int Length();                          // Counts the number of nodes
    friend std::ostream &operator<<(std::ostream &os, List &l);
};

#endif



/*************************************List.cpp******************************************/
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

//Solution to Chapter 4, Section 2, Exercise 1
int List::Length() {
    int len = 0;
    ListNode *temp = first;
    while (temp != NULL) { // Until it points to NULL
        temp = temp->link;
        len++;
    }
    return len;
}

//Solution to Chapter 4, Section 2, Exercise 2
void List::Delete(ListNode* x) {
    if (first == nullptr) {
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

std::ostream &operator<<(std::ostream &os, List &l) {
    ListNode *temp = l.first;
    while (temp != NULL) {
        os << temp->getData();
        if (temp->link != NULL) // Prevent the last "->" from being printed
            os << "->";
        temp = temp->link; // Move to the next node
    }
    return os;
}
