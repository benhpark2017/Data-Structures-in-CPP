/**
 * Solutions to Chapter 4, Section 4, Exercise 6 of Horowitz's
 * Fundamentals of Data Structures in C++.
 */

/*******************************************main.cpp***********************************************/
#include "List.h"

int main() {
    CircList<int> newList;
    ListNode<int>* current = nullptr;

    for (int i = 1; i <= 10; i++) {
        current = newList.Insert(current, i);
    }
    std::cout << "\nNew list for movement demonstration: " << newList << std::endl;

    ListNode<int>* l = nullptr;
    ListNode<int>* r = newList.getFirst();

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



/***********************************List.h*******************************************/
#ifndef LIST_H
#define LIST_H

#include <iostream>
#include <limits>

enum Boolean { FALSE, TRUE };

template <class Type>
class CircList;

template <class Type>
class CircIterator;

template <class Type>
class ListNode {
    friend class CircList<Type>;
    friend class CircIterator<Type>;

    template <class T>
    friend std::ostream& operator<<(std::ostream& os, const CircList<T>& list);

  private:
    Type data;
    ListNode *link;

  public:
    ListNode(Type element = 0, ListNode *next = nullptr) : data(element), link(next) {}
    Type getData() const { return data; }  // Added this line
};

template <class Type>
class CircList {
    friend class CircIterator<Type>;

    template <class T>
    friend std::ostream& operator<<(std::ostream& os, const CircList<T>& list);

public:
    CircList();
    ~CircList();

    ListNode<Type> *getFirst() const;
    
    Type getData(ListNode<Type>* node) const;
    
    ListNode<Type> *Insert(ListNode<Type> *x, Type i);
    int Length() const;
    void Delete(ListNode<Type> *x);
    void InsertBack(const Type &item);
    void Concatenate(CircList<Type> &b);
    void Reverse();
    Type findMin() const;
    void moveRight(ListNode<Type>*& l, ListNode<Type>*& r, int n); //Solution
    void moveLeft(ListNode<Type>*& l, ListNode<Type>*& r, int n);  //Solution
    void deleteEveryOtherNode();
    void mergeSorted(CircList<Type>& other);
    
private:
    ListNode<Type> *last;
};

template <class Type>
class CircIterator {
public:
    CircIterator(const CircList<Type> &l);
    Type getData();

    Boolean NotNull() const;
    Boolean NextNotNull() const;
    Type* First();
    Type* Next();
    const ListNode<Type>* CurrentPosition() const;

private:
    const CircList<Type> &list;
    ListNode<Type> *current;
};

template <class Type>
std::ostream& operator<<(std::ostream& os, const CircList<Type>& list);

#include "List.cpp"

#endif // LIST_H



/************************************List.cpp*******************************************/
#ifndef LIST_CPP
#define LIST_CPP

#include "List.h"

template <class Type>
CircList<Type>::CircList() : last(nullptr) {}

template <class Type>
CircList<Type>::~CircList() {
    if (last == nullptr) return;
    ListNode<Type> *current = last->link;
    while (current != last) {
        ListNode<Type> *next = current->link;
        delete current;
        current = next;
    }
    delete last;
}

template <class Type>
ListNode<Type> *CircList<Type>::getFirst() const {
    return last ? last->link : nullptr;
}

template <class Type>
ListNode<Type> *CircList<Type>::Insert(ListNode<Type> *x, Type i) {
    if (last == nullptr) {
        last = new ListNode<Type>(i);
        last->link = last;
        return last;
    }
    ListNode<Type> *temp = new ListNode<Type>(i, x->link);
    x->link = temp;
    if (x == last) {
        last = temp;
    }
    return temp;
}

template <class Type>
int CircList<Type>::Length() const {
    if (last == nullptr) return 0;
    int len = 1;
    ListNode<Type> *temp = last->link;
    while (temp != last) {
        temp = temp->link;
        len++;
    }
    return len;
}

template <class Type>
void CircList<Type>::Delete(ListNode<Type> *x) {
    if (last == nullptr) {
        std::cout << "The list is empty" << std::endl;
        return;
    }
    if (last->link == last && last == x) {
        delete last;
        last = nullptr;
        return;
    }
    ListNode<Type> *temp = last->link;
    ListNode<Type> *prev = last;
    do {
        if (temp == x) {
            prev->link = temp->link;
            if (x == last) {
                last = prev;
            }
            if (x == last->link) {
                last->link = x->link;
            }
            delete x;
            return;
        }
        prev = temp;
        temp = temp->link;
    } while (temp != last->link);

    std::cout << "Node not found" << std::endl;
}

template <class Type>
void CircList<Type>::InsertBack(const Type &item) {
    if (last == nullptr) {
        last = new ListNode<Type>(item);
        last->link = last;
    } else {
        Insert(last, item);
    }
}

template <class Type>
void CircList<Type>::Concatenate(CircList<Type> &b) {
    if (b.last == nullptr) return;
    if (last == nullptr) {
        last = b.last;
    } else {
        ListNode<Type> *temp = last->link;
        last->link = b.last->link;
        b.last->link = temp;
        last = b.last;
    }
    b.last = nullptr;
}

template <class Type>
void CircList<Type>::Reverse() {
    if (last == nullptr || last->link == last) return;
    ListNode<Type> *prev = last;
    ListNode<Type> *current = last->link;
    ListNode<Type> *next;
    do {
        next = current->link;
        current->link = prev;
        prev = current;
        current = next;
    } while (current != last->link);
    last = last->link;
}

template <class Type>
Type CircList<Type>::findMin() const {
    if (last == nullptr) {
        throw std::runtime_error("The list is empty");
    }
    Type min = last->data;
    ListNode<Type> *temp = last->link;
    do {
        if (temp->data < min) {
            min = temp->data;
        }
        temp = temp->link;
    } while (temp != last->link);
    return min;
}

template <class Type>
std::ostream& operator<<(std::ostream& os, const CircList<Type>& list) {
    if (list.last == nullptr) return os;
    ListNode<Type>* current = list.last->link;
    do {
        os << current->data;
        current = current->link;
        if (current != list.last->link) {
            os << "->";
        }
    } while (current != list.last->link);
    return os;
}

template <class Type>
CircIterator<Type>::CircIterator(const CircList<Type> &l) : list(l), current(l.getFirst()) {};

template <class Type>
Type CircIterator<Type>::getData() {
    return current->data;
}

template <class Type>
Boolean CircIterator<Type>::NotNull() const {
    return current != nullptr ? TRUE : FALSE;
}

template <class Type>
Boolean CircIterator<Type>::NextNotNull() const {
    return current != nullptr && current->link != nullptr ? TRUE : FALSE;
}

template <class Type>
Type* CircIterator<Type>::First() {
    current = list.getFirst();
    return current != nullptr ? &current->data : nullptr;
}

template <class Type>
Type* CircIterator<Type>::Next() {
    if (current != nullptr) {
        current = current->link;
        return current != nullptr ? &current->data : nullptr;
    }
    return nullptr;
}

template <class Type>
const ListNode<Type>* CircIterator<Type>::CurrentPosition() const {
    return current;
}

//Solution to Chapter 4, Section 4, Exercise 6 part a).
template <class Type>
void CircList<Type>::moveRight(ListNode<Type>*& l, ListNode<Type>*& r, int n) {
    if (last == nullptr || n <= 0) return;
    
    for (int i = 0; i < n; ++i) {
        if (r == nullptr) {
            r = last->link;  // If r is null, set it to the first node
        }
        l = r;
        r = r->link;
    }
}

//Solution to Chapter 4, Section 4, Exercise 6 part b).
template <class Type>
void CircList<Type>::moveLeft(ListNode<Type>*& l, ListNode<Type>*& r, int n) {
    if (last == nullptr || n <= 0) return;

    for (int i = 0; i < n; ++i) {
        if (l == nullptr) {
            // Find the node before the first node (i.e., last)
            l = last;
        } else {
            // Find the node before l
            ListNode<Type>* temp = l;
            while (temp->link != l) {
                temp = temp->link;
            }
            l = temp;
        }
        r = l->link;
    }
}

template <class Type>
void CircList<Type>::deleteEveryOtherNode() {
    if (last == nullptr || last->link == last) {
        return;
    }

    ListNode<Type> *current = last;
    ListNode<Type> *firstNode = last->link;
    bool isFirstIteration = true;

    do {
        // Delete the node after the current one
        ListNode<Type> *temp = current->link;
        current->link = temp->link;

        // If we're deleting the last node, update last
        if (temp == last) {
            last = current;
        }

        // If we're deleting the first node, update firstNode
        if (temp == firstNode) {
            firstNode = current->link;
        }

        delete temp;

        // Move to the next node (which is the one we just linked to)
        current = current->link;

        // Break the loop if we've come full circle
        if (!isFirstIteration && current == last) {
            break;
        }
        isFirstIteration = false;

    } while (true);

    // Ensure last->link points to the new first node
    last->link = firstNode;
}

template <class Type>
void CircList<Type>::mergeSorted(CircList<Type>& other) {
    if (this->last == nullptr) {
        this->last = other.last;
        other.last = nullptr;
        return;
    }
    if (other.last == nullptr) {
        return;
    }

    ListNode<Type>* head1 = this->last->link;
    ListNode<Type>* head2 = other.last->link;
    
    // Break the circular links
    this->last->link = nullptr;
    other.last->link = nullptr;

    ListNode<Type>* merged = nullptr;
    ListNode<Type>* tail = nullptr;

    // Merge the lists
    while (head1 != nullptr && head2 != nullptr) {
        ListNode<Type>* temp;
        if (head1->data <= head2->data) {
            temp = head1;
            head1 = head1->link;
        } else {
            temp = head2;
            head2 = head2->link;
        }

        if (merged == nullptr) {
            merged = tail = temp;
        } else {
            tail->link = temp;
            tail = temp;
        }
    }

    // Attach remaining nodes
    if (head1 != nullptr) {
        tail->link = head1;
        while (tail->link != nullptr) {
            tail = tail->link;
        }
    }
    if (head2 != nullptr) {
        tail->link = head2;
        while (tail->link != nullptr) {
            tail = tail->link;
        }
    }

    // Make the list circular again
    tail->link = merged;
    this->last = tail;

    // Clear the other list
    other.last = nullptr;
}

#endif //LIST_CPP

