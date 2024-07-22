/**
 * Solutions to Chapter 4, Section 2, Exercise 4 of Horowitz's
 * Fundamentals of Data Structures in C++.
 */

/*******************************************main.cpp***********************************************/
#include "List.h"
#include <iostream>

int main(void) {
    List x, y, z;
    ListNode *LN1 = x.getFirst();
    ListNode *LN2 = y.getFirst();

    for (int i = 0; i < 5; i++)
        LN1 = x.Insert(LN1, i + 1);

    std::cout << "x: " << x << std::endl;

    for (int i = 0; i < 5; i++)
        LN2 = y.Insert(LN2, (i + 2) * 2);

    std::cout << "y: " << y << std::endl;

    Merge(x, y, z);

    std::cout << "z: " << z << std::endl;

    return 0;
}



/********************************************List.h************************************************/
#ifndef LIST_H
#define LIST_H

#include <iostream>

//Forward declaration of the List class.
class List;

class ListNode {
private:
    int data;
    ListNode *link;

    ListNode(int element = 0, ListNode *next = NULL) : data(element), link(next) {}

    int getData() { return data; }
    ListNode *Link() { return link; }

    friend class List;
    friend std::ostream &operator<<(std::ostream &os, List &l);
};

class List {
private:
    ListNode *first;

public:
    List() { first = NULL; }
    ListNode *getFirst() { return first; } // Returns the first node
    int getData() { return first->data; }
    ListNode *Insert(ListNode *x, int i);  // Adds a node
    int Length();                          // Counts the number of nodes
    void Delete(ListNode *x);              // Deletes a specified node
    void deleteEveryOtherNode();                // Deletes every other node starting with the first
    friend std::ostream &operator<<(std::ostream &os, List &l);
    friend void Merge(List &x, List &y, List &z);
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

int List::Length() {
    int len = 0;
    ListNode *temp = first;
    while (temp != NULL) { // Until it points to NULL
        temp = temp->link;
        len++;
    }
    return len;
}

void List::Delete(ListNode *x) {
    if (first == NULL) {
        std::cout << "The list is empty" << std::endl;
        return;
    }
    if (x == first) {
        first = first->link; // Point to the next node
    } else {
        ListNode *temp = first;
        while (temp->link != x) {
            temp = temp->link; // Find the node before x
        }
        temp->link = x->link; // Bypass x
    }
    delete x;
}

//An alternate representation of the function that deletes every other node
//of a linked list.
//See also the solution to Chapter 4, Section 2, Exercise 3.
void List::deleteEveryOtherNode() {
    if (first == NULL) {
        std::cout << "The list is empty" << std::endl;
        return;
    } else {
        ListNode *temp = first;
        Delete(temp);
        temp = first; // first now points to what was the second node
        while (temp != NULL && temp->link != NULL) {
            Delete(temp->link);
            temp = temp->link; // Move to the next node
        }
    }
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

void Merge(List &x, List &y, List &z) {
    ListNode *ListNodePtr = z.getFirst();
    int index1 = 0, index2 = 0;
    int length1 = x.Length(); // Store the length since it reduces as we Delete nodes
    int length2 = y.Length();
    while (index1 < length1 && index2 < length2) {
        ListNodePtr = z.Insert(ListNodePtr, x.getData());
        index1++;
        x.Delete(x.getFirst());
        ListNodePtr = z.Insert(ListNodePtr, y.getData());
        index2++;
        y.Delete(y.getFirst());
    }
    if (index1 < length1) {
        while (index1 < x.Length()) {
            ListNodePtr = z.Insert(ListNodePtr, x.getData());
            index1++;
            x.Delete(x.getFirst());
        }
    } else if (index2 < length2) {
        while (index2 < y.Length()) {
            ListNodePtr = z.Insert(ListNodePtr, y.getData());
            index2++;
            y.Delete(y.getFirst());
        }
    }
}
