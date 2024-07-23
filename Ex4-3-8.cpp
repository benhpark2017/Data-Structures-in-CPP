/**
 * Solutions to Chapter 4, Section 3, Exercise 8, Exercises from 9 to 11 of Horowitz's
 * Fundamentals of Data Structure in C++.
 */

/************************************main.cpp*****************************************/
#include <iostream>
#include "List.h"

int main() {
    CircList<int> intCircList;
    for (int i = 1; i <= 10; i++) {
        intCircList.InsertBack(i);
    }

    std::cout << "List: " << intCircList << std::endl;

    try {
        int minValue = intCircList.findMin();
        std::cout << "Minimum value in the list: " << minValue << std::endl;
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
    
    int result = sumOfProducts(intCircList);
    std::cout << "Sum of products: " << result << std::endl;

    return 0;
}



/*********************************List.h***********************************/
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
    ListNode<Type> *Insert(ListNode<Type> *x, Type i);
    int Length() const;
    void Delete(ListNode<Type> *x);
    void InsertBack(const Type &item);
    void Concatenate(CircList<Type> &b);
    void Reverse();
    Type findMin() const;

private:
    ListNode<Type> *first;
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
Type sumOfProducts(const CircList<Type>& list);

#include "List.cpp"

#endif // LIST_H



/************************************List.cpp*******************************************/
#ifndef LIST_CPP
#define LIST_CPP

#include "List.h"

template <class Type>
CircList<Type>::CircList() : first(nullptr) {}

template <class Type>
CircList<Type>::~CircList() {
    if (first == nullptr) return;
    ListNode<Type> *current = first->link;
    while (current != first) {
        ListNode<Type> *next = current->link;
        delete current;
        current = next;
    }
    delete first;
}

template <class Type>
ListNode<Type> *CircList<Type>::getFirst() const {
    return first ? first->link : nullptr;
}

template <class Type>
ListNode<Type> *CircList<Type>::Insert(ListNode<Type> *x, Type i) {
    if (first == nullptr) {
        first = new ListNode<Type>(i);
        first->link = first;
        return first;
    }
    ListNode<Type> *temp = new ListNode<Type>(i, x->link);
    x->link = temp;
    return temp;
}

template <class Type>
int CircList<Type>::Length() const {
    if (first == nullptr) return 0;
    int len = 1;
    ListNode<Type> *temp = first->link;
    while (temp != first) {
        temp = temp->link;
        len++;
    }
    return len;
}

template <class Type>
void CircList<Type>::Delete(ListNode<Type> *x) {
    if (first == nullptr) {
        std::cout << "The list is empty" << std::endl;
        return;
    }
    if (first->link == first) {
        delete first;
        first = nullptr;
        return;
    }
    ListNode<Type> *temp = first;
    while (temp->link != x) {
        temp = temp->link;
        if (temp == first) {
            std::cout << "Node not found" << std::endl;
            return;
        }
    }
    temp->link = x->link;
    if (x == first) {
        first = x->link;
    }
    delete x;
}

template <class Type>
void CircList<Type>::InsertBack(const Type &item) {
    if (first == nullptr) {
        first = new ListNode<Type>(item);
        first->link = first;
    } else {
        ListNode<Type> *last = first;
        while (last->link != first) {
            last = last->link;
        }
        Insert(last, item);
    }
}

template <class Type>
void CircList<Type>::Concatenate(CircList<Type> &b) {
    if (b.first == nullptr) return;
    if (first == nullptr) {
        first = b.first;
    } else {
        ListNode<Type> *lastA = first;
        while (lastA->link != first) {
            lastA = lastA->link;
        }
        ListNode<Type> *lastB = b.first;
        while (lastB->link != b.first) {
            lastB = lastB->link;
        }
        lastA->link = b.first;
        lastB->link = first;
    }
    b.first = nullptr;
}

template <class Type>
void CircList<Type>::Reverse() {
    if (first == nullptr || first->link == first) return;
    ListNode<Type> *prev = first;
    ListNode<Type> *current = first->link;
    ListNode<Type> *next;
    do {
        next = current->link;
        current->link = prev;
        prev = current;
        current = next;
    } while (current != first);
    first->link = prev;
    first = prev;
}

template <class Type>
Type CircList<Type>::findMin() const {
    if (first == nullptr) {
        throw std::runtime_error("The list is empty");
    }
    Type min = first->data;
    ListNode<Type> *temp = first->link;
    do {
        if (temp->data < min) {
            min = temp->data;
        }
        temp = temp->link;
    } while (temp != first);
    return min;
}

template <class Type>
std::ostream& operator<<(std::ostream& os, const CircList<Type>& list) {
    if (list.first == nullptr) return os;
    ListNode<Type>* current = list.first;
    do {
        os << current->data;
        current = current->link;
        if (current != list.first) {
            os << "->";
        }
    } while (current != list.first);
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
Type sumOfProducts(const CircList<Type>& list) {
    if (list.Length() == 0) return 0;
    
    CircIterator<Type> iter1(list);
    CircIterator<Type> iter2(list);
    
    // Move iter2 5 positions ahead or until we wrap around
    for (int i = 0; i < 5 && iter2.NextNotNull(); ++i) {
        iter2.Next();
    }
    
    Type sum = 0;
    int count = 0;
    int listLength = list.Length();
    
    do {
        sum += iter1.getData() * iter2.getData();
        iter1.Next();
        iter2.Next();
        count++;
    } while (count < listLength / 2);
    
    return sum;
}

#endif //LIST_CPP
