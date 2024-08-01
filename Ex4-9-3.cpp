/**
 * Solutions to Chapter 4, Section 9, Exercise 3 of Horowitz's
 * Fundamentals of Data Structures in C++.
 */
 
/***********************************main.cpp***********************************/
#include <iostream>
#include "DCList.h"

int main() {
    
    DCList<int> intList;

    // Insert elements
    std::cout << "Inserting elements 1 to 10 at the back of the list:\n";
    for (int i = 1; i <= 10; i++) {
        intList.InsertBack(i);
    }
    std::cout << "List: " << intList << std::endl;

    // Insert at front
    std::cout << "\nInserting 0 at the front:\n";
    intList.InsertFront(0);
    std::cout << "List: " << intList << std::endl;

    // Length
    std::cout << "\nLength of the list: " << intList.Length() << std::endl;

    // Find minimum
    int minValue = intList.findMin();
    std::cout << "Minimum value in the list: " << minValue << std::endl;
    
    // Remove an element
    std::cout << "\nRemoving the second element:\n";
    DCListIterator<int> iter(intList);
    iter.Next();  // Move to the second element
    intList.Remove(iter.CurrentPosition());
    std::cout << "List after removal: " << intList << std::endl;

    // Test circular nature
    std::cout << "\nTesting circular nature (printing 15 elements):\n";
    DCListIterator<int> circularIt(intList);
    for (int i = 0; i < 15; ++i) {
        std::cout << circularIt.getData() << " ";
        circularIt.Next();
    }
    std::cout << std::endl;
    
    //Traversing backwards.
    std::cout << "\nTesting circular nature (printing 15 elements):\n";
    for (int i = 0; i < 15; ++i) {
        std::cout << circularIt.getData() << " ";
        circularIt.Prev();
    }
    std::cout << std::endl;
    
    return 0;
}



/***********************************DCList.h***********************************/
#ifndef DCLIST_H
#define DCLIST_H

#include <iostream>

template <class Type>
class DCList;

template <class Type>
class DCListIterator;

template <class Type>
class DCListNode {
    friend class DCList<Type>;
    friend class DCListIterator<Type>;
    
    template <class T>
    friend std::ostream& operator<<(std::ostream& os, const DCList<T>& list);

private:
    Type data;
    DCListNode *next;
    DCListNode *prev;

public:
    DCListNode(Type element = Type(), DCListNode *n = nullptr, DCListNode *p = nullptr) 
        : data(element), next(n), prev(p) {}
};

template <class Type>
class DCList {
    friend class DCListIterator<Type>;
    
    template <class T>
    friend std::ostream& operator<<(std::ostream& os, const DCList<T>& list);

public:
    DCList();
    ~DCList();

    bool isEmpty() const;
    void InsertFront(const Type &item);
    void InsertBack(const Type &item);
    void Insert(DCListNode<Type>* p, DCListNode<Type>* x);
    void Remove(DCListNode<Type>* pos);
    DCListNode<Type>* getHead() const;
    int Length() const;
    Type findMin() const;

private:
    DCListNode<Type> *head;  // Head node
};

template <class Type>
class DCListIterator {
public:
    //DCListIterator(const DCList<Type> &l);
    DCListIterator(const DCList<Type> &l); //copy constructor
    Type getData() const;
    
    bool NotNull() const;
    bool NextNotNull() const;
    bool PrevNotNull() const;
    DCListNode<Type>* First();
    DCListNode<Type>* Next();
    DCListNode<Type>* Prev();
    DCListNode<Type>* CurrentPosition() const;
    
private:
    const DCList<Type> &list;
    DCListNode<Type> *current;
};

// Declaration of the operator<< function
template <class Type>
std::ostream& operator<<(std::ostream& os, const DCList<Type>& list);

#include "DCList.tpp"

#endif // DCLIST_H



/*********************************DCList.tpp***********************************/
#include <stdexcept>

template <class Type>
DCList<Type>::DCList() {
    head = new DCListNode<Type>();
    head->next = head->prev = head;  // Circular structure
}

template <class Type>
DCList<Type>::~DCList() {
    while (!isEmpty()) {
        Remove(head->next);
    }
    delete head;
}

template <class Type>
bool DCList<Type>::isEmpty() const {
    return head->next == head;
}

template <class Type>
void DCList<Type>::InsertFront(const Type &item) {
    DCListNode<Type>* newNode = new DCListNode<Type>(item);
    Insert(newNode, head->next);
}

template <class Type>
void DCList<Type>::InsertBack(const Type &item) {
    DCListNode<Type>* newNode = new DCListNode<Type>(item);
    Insert(newNode, head);  // Insert the new node before the head node
}

template <class Type>
void DCList<Type>::Insert(DCListNode<Type>* newNode, DCListNode<Type>* x) {
    if (x == nullptr || newNode == nullptr) {
        throw std::invalid_argument("Nodes cannot be null.");
    }
    newNode->next = x;
    newNode->prev = x->prev;
    x->prev->next = newNode;
    x->prev = newNode;
}

template <class Type>
void DCList<Type>::Remove(DCListNode<Type>* pos) {
    if (pos == head) {
        throw std::runtime_error("Cannot remove head node");
    }
    pos->prev->next = pos->next;
    pos->next->prev = pos->prev;
    delete pos;
}

template <class Type>
DCListNode<Type>* DCList<Type>::getHead() const {
    return head;
}

template <class Type>
int DCList<Type>::Length() const {
    int count = 0;
    DCListNode<Type>* current = head->next;
    while (current != head) {
        count++;
        current = current->next;
    }
    return count;
}

template <class Type>
Type DCList<Type>::findMin() const {
    if (isEmpty()) {
        throw std::runtime_error("List is empty");
    }
    Type min = head->next->data;
    DCListNode<Type>* current = head->next->next;
    while (current != head) {
        if (current->data < min) {
            min = current->data;
        }
        current = current->next;
    }
    return min;
}

// Definition of the operator<< function
template <class Type>
std::ostream& operator<<(std::ostream& os, const DCList<Type>& list) {
    DCListNode<Type>* current = list.head->next;
    if (current == list.head) {
        return os << "Empty List";
    }
    while (current != list.head) {
        os << current->data;
        if (current->next != list.head) {
            os << " <-> ";
        }
        current = current->next;
    }
    return os;
}

// DCListIterator implementations
template <class Type>
DCListIterator<Type>::DCListIterator(const DCList<Type> &l) : list(l), current(l.head->next) {}

template <class Type>
Type DCListIterator<Type>::getData() const {
    return current->data;
}

template <class Type>
bool DCListIterator<Type>::NotNull() const {
    return current != list.head;
}

template <class Type>
bool DCListIterator<Type>::NextNotNull() const {
    return current->next != list.head;
}

template <class Type>
bool DCListIterator<Type>::PrevNotNull() const {
    return current->prev != list.head;
}

template <class Type>
DCListNode<Type>* DCListIterator<Type>::First() {
    current = list.head->next;
    return current;
}

template <class Type>
DCListNode<Type>* DCListIterator<Type>::Next() {
    if (current == nullptr) throw std::runtime_error("Iterator not initialized");
    current = current->next;
    if (current == list.head) current = current->next;  // Skip the dummy head node
    return current;
}

template <class Type>
DCListNode<Type>* DCListIterator<Type>::Prev() {
    if (current == nullptr) throw std::runtime_error("Iterator not initialized");
    current = current->prev;
    if (current == list.head) current = current->prev;  // Skip the dummy head node
    return current;
}

template <class Type>
DCListNode<Type>* DCListIterator<Type>::CurrentPosition() const {
    return current;
}
