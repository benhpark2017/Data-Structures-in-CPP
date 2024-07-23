/**
 * Solutions to Chapter 4, Section 3, Exercise 1 and 2 of Horowitz's
 * Fundamentals of Data Structures in C++.
 */

/********************************main.cpp**********************************/
#include "List.h"

int main() {
    List<int> intList;
    ListNode<int> *ListPtr = intList.getFirst();

    for (int i = 0; i < 10; i++)
        intList.InsertBack(i + 11);
    
    std::cout << "List Output: " << intList << std::endl;
    
    std::cout << std::endl;

    try {
        int minValue = intList.findMin();
        std::cout << "Minimum value in the list: " << minValue << std::endl;
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}



/*********************************List.h***********************************/
#ifndef LIST_H
#define LIST_H

#include <iostream>
#include <limits> // For std::numeric_limits

enum Boolean { FALSE, TRUE };

template <class Type>
class List;

template <class Type>
class ListIterator;

template <class Type>
class ListNode {
    friend class List<Type>;
    friend class ListIterator<Type>;
    
    // Add this line to declare operator<< as a friend of ListNode
    template <class T>
    friend std::ostream& operator<<(std::ostream& os, const List<T>& list);

private:
    Type data;
    ListNode *link;

public:
    ListNode(Type element = 0, ListNode *next = nullptr) : data(element), link(next) {}
};

template <class Type>
class List {
    friend class ListIterator<Type>;
    
    template <class T>
    friend std::ostream& operator<<(std::ostream& os, const List<T>& list);
public:
    List();
    ~List();

    ListNode<Type> *getFirst() const;
    ListNode<Type> *Insert(ListNode<Type> *x, Type i);
    int Length() const;
    void Delete(ListNode<Type> *x);
    void InsertBack(const Type &item);
    void Concatenate(List<Type> &b);
    void Reverse();
    Type findMin() const;

private:
    ListNode<Type> *first;
    ListNode<Type> *last;
};

template <class Type>
class ListIterator {
public:
    ListIterator(const List<Type> &l);
    Type getData();
    
    Boolean NotNull() const;
    Boolean NextNotNull() const;
    Type* First();
    Type* Next();

private:
    const List<Type> &list;
    ListNode<Type> *current;
};

// Declaration of the operator<< function
template <class Type>
std::ostream& operator<<(std::ostream& os, const List<Type>& list);

#include "List.cpp"

#endif // LIST_H



/********************************List.cpp**********************************/

#ifndef LIST_CPP
#define LIST_CPP

#include "List.h"

template <class Type>
List<Type>::List() : first(nullptr), last(nullptr) {}

template <class Type>
List<Type>::~List() {
    ListNode<Type> *current = first;
    while (current != nullptr) {
        ListNode<Type> *next = current->link;
        delete current;
        current = next;
    }
}

template <class Type>
ListNode<Type> *List<Type>::getFirst() const {
    return first;
}

template <class Type>
ListNode<Type> *List<Type>::Insert(ListNode<Type> *x, Type i) {
    if (first) {
        ListNode<Type> *temp = new ListNode<Type>(i, x->link);
        x->link = temp;
        if (x == last) {
            last = temp;
        }
        return temp;
    } else {
        first = last = new ListNode<Type>(i);
        return first;
    }
}

template <class Type>
int List<Type>::Length() const {
    int len = 0;
    ListNode<Type> *temp = first;
    while (temp != nullptr) {
        temp = temp->link;
        len++;
    }
    return len;
}

template <class Type>
void List<Type>::Delete(ListNode<Type> *x) {
    if (first == nullptr) {
        std::cout << "The list is empty" << std::endl;
        return;
    }
    if (x == first) {
        first = first->link;
        if (first == nullptr) {
            last = nullptr;
        }
    } else {
        ListNode<Type> *temp = first;
        while (temp->link != x) {
            temp = temp->link;
        }
        temp->link = x->link;
        if (x == last) {
            last = temp;
        }
    }
    delete x;
}

template <class Type>
void List<Type>::InsertBack(const Type &item) {
    if (first) {
        last->link = new ListNode<Type>(item);
        last = last->link;
    } else {
        first = last = new ListNode<Type>(item);
    }
}

template <class Type>
void List<Type>::Concatenate(List<Type> &b) {
    if (first) {
        last->link = b.first;
        if (b.last) {
            last = b.last;
        }
    } else {
        first = b.first;
        last = b.last;
    }
    b.first = b.last = nullptr;
}

template <class Type>
void List<Type>::Reverse() {
    ListNode<Type> *current = first, *previous = nullptr;
    last = first;
    while (current) {
        ListNode<Type> *next = current->link;
        current->link = previous;
        previous = current;
        current = next;
    }
    first = previous;
}

template <class Type>
Type List<Type>::findMin() const {
    if (first == nullptr) {
        throw std::runtime_error("The list is empty");
    }

    Type min = first->data;
    ListNode<Type> *temp = first->link;

    while (temp != nullptr) {
        if (temp->data < min) {
            min = temp->data;
        }
        temp = temp->link;
    }

    return min;
}

// Definition of the operator<< function
template <class Type>
std::ostream& operator<<(std::ostream& os, const List<Type>& list) {
    ListNode<Type>* current = list.first;
    while (current != nullptr) {
        os << current->data;
        if (current->link != nullptr) {
            os << "->";
        }
        current = current->link;
    }
    return os;
}

// ListIterator implementations
template <class Type>
ListIterator<Type>::ListIterator(const List<Type> &l) : list(l), current(l.first) {}

template <class Type>
Type ListIterator<Type>::getData() {
    return current->data;
}

template <class Type>
Boolean ListIterator<Type>::NotNull() const {
    return current != nullptr ? TRUE : FALSE;
}

template <class Type>
Boolean ListIterator<Type>::NextNotNull() const {
    return current != nullptr && current->link != nullptr ? TRUE : FALSE;
}

template <class Type>
Type* ListIterator<Type>::First() {
    current = list.first;
    return current != nullptr ? &current->data : nullptr;
}

template <class Type>
Type* ListIterator<Type>::Next() {
    if (current != nullptr) {
        current = current->link;
        return current != nullptr ? &current->data : nullptr;
    }
    return nullptr;
}

#endif //LIST_CPP
