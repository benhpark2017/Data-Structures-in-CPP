/**
 * Solutions to Chapter 4, Section 2, Exercise 5 of Horowitz's
 * Fundamentals of Data Structures in C++.
 */

/*********************************main.cpp*************************************/

#include <iostream>
#include "List.h"

int main() {
    List x, y, z;
    ListNode *ListPtr1 = x.getFirst();
    ListNode *ListPtr2 = y.getFirst();

    for (int i = 0; i < 5; i++) {
        ListPtr1 = x.Insert(ListPtr1, 15 - i);
    }
    std::cout << "Original x: " << x << std::endl;
    x.ascendingSort();
    std::cout << "Sorted x: " << x << std::endl;

    for (int i = 0; i < 4; i++) {
        ListPtr2 = y.Insert(ListPtr2, 20 - (3 * i));
    }
    std::cout << "Original y: " << y << std::endl;
    y.ascendingSort();
    std::cout << "Sorted y: " << y << std::endl;

    Merge(x, y, z);
    std::cout << "Merged z: " << z << std::endl;

    return 0;
}


/**********************************List.h**************************************/

#ifndef LIST_H
#define LIST_H

#include <iostream>

//Forward declaration of the List class.
class List;

class ListNode {
private:
    int data;
    ListNode *link;

public:
    ListNode(int element = 0, ListNode *next = nullptr)
        : data(element), link(next) {}
        
    int getData() const { return data; }
    ListNode *Link() const { return link;}

    friend class List;
    friend std::ostream &operator<<(std::ostream &os, const List &l);
};

class List {
private:
    ListNode *first;

public:
    List() : first(nullptr) {}

    ListNode *getFirst() const { return first; }

    int getData() const {
        return first ? first->data : 0; // Handle empty list case
    }

    ListNode *Insert(ListNode *x, int i);

    int Length() const;

    void Delete(ListNode *x);

    void deleteEveryOtherNode();

    void ascendingSort();

    friend std::ostream &operator<<(std::ostream &os, const List &l);
    
    friend void Merge(List &x, List &y, List &z);
};

#endif // LIST_H


/*********************************List.cpp*************************************/

#include "List.h"

// Insert a new node with value `i` after node `x`
ListNode *List::Insert(ListNode *x, int i) {
    if (first) {
        ListNode *temp = new ListNode(i, x->link);
        x->link = temp;
        return x->link;
    } else {
        first = new ListNode(i);
        return first;
    }
}

// Calculate the length of the list
int List::Length() const {
    int len = 0;
    ListNode *temp = first;
    while (temp != nullptr) {
        temp = temp->link;
        len++;
    }
    return len;
}

// Delete the specified node `x`
void List::Delete(ListNode *x) {
    if (first == nullptr) {
        std::cout << "The list is empty" << std::endl;
        return;
    }

    if (x == first) {
        first = first->link;
    } else {
        ListNode *temp = first;
        while (temp->link != x) {
            temp = temp->link;
        }
        temp->link = x->link;
    }

    delete x;
}

// Delete every other node starting with the first node
void List::deleteEveryOtherNode() {
    if (first == nullptr) {
        std::cout << "The list is empty" << std::endl;
        return;
    } else {
        ListNode *temp = first;
        Delete(temp);
        temp = first;

        while (temp != nullptr && temp->link != nullptr) {
            if (temp->link->link == nullptr) {
                delete temp->link;
                temp->link = nullptr;
                break;
            }
            Delete(temp->link);
            temp = temp->link;
        }
    }
}

// Sort the list using bubble sort
void List::ascendingSort() {
    if (first == nullptr) {
        std::cout << "The list is empty" << std::endl;
        return;
    } else {
        ListNode *End = nullptr;
        ListNode *current;
        ListNode *next;
        ListNode *previous;

        while (first->link != End) {
            current = first;
            next = first->link;
            previous = nullptr;

            while (next != End) {
                if (current->data > next->data) {
                    if (previous) {
                        previous->link = next;
                    } else {
                        first = next;
                    }
                    current->link = next->link;
                    next->link = current;
                    previous = next;
                    next = current->link;
                } else {
                    previous = current;
                    current = next;
                    next = next->link;
                }
            }
            End = current;
        }
    }
}

// Output the list to the stream
std::ostream &operator<<(std::ostream &os, const List &l) {
    ListNode *temp = l.first;
    while (temp != nullptr) {
        os << temp->getData();
        if (temp->link != nullptr) {
            os << "->";
        }
        temp = temp->link;
    }
    return os;
}

// Merge two lists `x` and `y` into list `z`
void Merge(List &x, List &y, List &z) {
    ListNode *ListNodePtr = z.getFirst();
    int index1 = 0, index2 = 0;
    int length1 = x.Length();
    int length2 = y.Length();

    while (index1 < length1 && index2 < length2) {
        ListNodePtr = z.Insert(ListNodePtr, x.getData());
        index1++;
        x.Delete(x.getFirst());

        ListNodePtr = z.Insert(ListNodePtr, y.getData());
        index2++;
        y.Delete(y.getFirst());
    }

    while (index1 < length1) {
        ListNodePtr = z.Insert(ListNodePtr, x.getData());
        index1++;
        x.Delete(x.getFirst());
    }

    while (index2 < length2) {
        ListNodePtr = z.Insert(ListNodePtr, y.getData());
        index2++;
        y.Delete(y.getFirst());
    }

    z.ascendingSort();
}
