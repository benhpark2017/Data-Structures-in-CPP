/**
 * Solutions to Chapter 4, Section 4, Exercises from 1 to 4 of Horowitz's
 * Fundamentals of Data Structure in C++.
 */

/************************************main.cpp*****************************************/
#include <iostream>
#include "List.h"

int main() {
    CircList<int> intCircList1;
    CircList<int> intCircList2;

    // Insert elements into the circular list #1
    for (int i = 1; i <= 20; i++) {
        intCircList1.InsertBack(i);
    }

    // Insert elements into the circular list #2
    for (int i = 1; i <= 10; i++) {
        intCircList2.InsertBack(2*i - 1);
    }

    // Print the list
    std::cout << "Circular List 1: " << intCircList1 << std::endl;
    std::cout << "Circular List 2: " << intCircList2 << std::endl;

    // Find the minimum value in the list
    try {
        int minValue = intCircList1.findMin();
        std::cout << "Minimum value in the list: " << minValue << std::endl;
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }

    // Print the list
    std::cout << "List before deleting every other node: " << intCircList1 << std::endl;

    // Delete every other node
    intCircList1.deleteEveryOtherNode();

    std::cout << "List after deleting every other node: " << intCircList1 << std::endl;

    // Delete a specific node (for example, delete the node with value 5)
    CircIterator<int> iter(intCircList1);
    while (iter.NotNull() && iter.getData() != 6) {
        iter.Next();
    }

    if (iter.NotNull()) {
        intCircList1.Delete(const_cast<ListNode<int>*>(iter.CurrentPosition()));
        std::cout << "After deleting 6, List: " << intCircList1 << std::endl;
    } else {
        std::cout << "Node with value 6 not found" << std::endl;
    }


    CircList<int> intCircListResult = mergeLists(intCircList1, intCircList2);
    std::cout << "Merged Circular List: " << intCircListResult << std::endl;
    
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
    ListNode<Type> *getLast() const;
    
    Type getData(ListNode<Type>* node) const;
    
    ListNode<Type> *Insert(ListNode<Type> *x, Type i);
    int Length() const;
    void Delete(ListNode<Type> *x);
    void InsertBack(const Type &item);
    void Concatenate(CircList<Type> &b);
    void Reverse();
    Type findMin() const;
    void deleteEveryOtherNode();
    CircList<Type> mergeLists(CircList<Type>& x, CircList<Type>& y);
    
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

template <class Type>
CircList<Type> mergeLists(CircList<Type>& x, CircList<Type>& y);

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
ListNode<Type> *CircList<Type>::getLast() const {
    return last;
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
CircList<Type> mergeLists(CircList<Type>& x, CircList<Type>& y) {
    CircList<Type> z;
    
    if (x.Length() == 0) {
        z = y;
        y = CircList<Type>();
        return z;
    }
    if (y.Length() == 0) {
        z = x;
        x = CircList<Type>();
        return z;
    }

    CircIterator<Type> xIter(x);
    CircIterator<Type> yIter(y);
    int xLength = x.Length();
    int yLength = y.Length();
    int xCount = 0, yCount = 0;

    while (xCount < xLength || yCount < yLength) {
        if (xCount < xLength) {
            z.InsertBack(xIter.getData());
            xIter.Next();
            xCount++;
        }
        if (yCount < yLength) {
            z.InsertBack(yIter.getData());
            yIter.Next();
            yCount++;
        }
    }

    // Clear original lists
    x = CircList<Type>();
    y = CircList<Type>();

    return z;
}

#endif //LIST_CPP
