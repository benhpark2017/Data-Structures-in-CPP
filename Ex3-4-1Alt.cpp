/**
 * Solutions to Chapter 3, Section 4, Exercise 1 of Horowitz's
 * Fundamentals of Data Structures in C++.
 * This version of code does not use .tpp files, but instead uses
 * .cpp format files for implementation.
 */
 
/************************************main.cpp*************************************/
#include "Stack.h"
#include <iostream>
#include <string>

int main() {
    // Test Bag class
    Bag<int> intBag;
    std::cout << "Testing Bag with int type:" << std::endl;

    // Add elements to Bag
    intBag.Add(1);
    intBag.Add(2);
    intBag.Add(3);
    std::cout << "Bag after adding elements: " << intBag;

    // Test Contains
    std::cout << "Bag contains 2: " << intBag.Contains(2) << std::endl;
    std::cout << "Bag contains 5: " << intBag.Contains(5) << std::endl;

    // Test Size
    std::cout << "Bag size: " << intBag.Size() << std::endl;

    // Test isFull and isEmpty
    std::cout << "Bag is full: " << intBag.isFull() << std::endl;
    std::cout << "Bag is empty: " << intBag.isEmpty() << std::endl;

    // Test Delete
    int* deletedItem = intBag.Delete(2);
    if (deletedItem) {
        std::cout << "Deleted item: " << *deletedItem << std::endl;
    } else {
        std::cout << "Item not found in Bag" << std::endl;
    }
    std::cout << "Bag after deletion: " << intBag;

    // Test Stack class
    Stack<int> intStack;
    std::cout << "\nTesting Stack with int type:" << std::endl;

    // Add elements to Stack
    intStack.AddToStack(10);
    intStack.AddToStack(20);
    intStack.AddToStack(30);
    std::cout << "Stack after adding elements: " << intStack;

    // Test PeekAtTop
    try {
        std::cout << "Top of Stack: " << intStack.PeekAtTop() << std::endl;
    } catch (const std::out_of_range& e) {
        std::cerr << e.what() << std::endl;
    }

    // Test isFull and isEmpty for Stack
    std::cout << "Stack is full: " << intStack.isFull() << std::endl;
    std::cout << "Stack is empty: " << intStack.isEmpty() << std::endl;

    // Test Delete in Stack
    int topItem;
    int* topPtr = intStack.Delete(topItem);
    if (topPtr) {
        std::cout << "Deleted top item from Stack: " << *topPtr << std::endl;
    } else {
        std::cout << "Stack is empty, no item to delete" << std::endl;
    }
    std::cout << "Stack after deletion: " << intStack;

    return 0;
}




/*************************************Bag.h**************************************/
#ifndef BAG_H
#define BAG_H

#include <iostream>

const int DEFAULT_SIZE = 10;

template <class KeyType>
class Bag {
public:
    Bag(int MAX_SIZE = DEFAULT_SIZE); // Constructor
    virtual ~Bag(); // Destructor

    virtual void Add(const KeyType& item);
    virtual int* Delete(const KeyType& item);
    virtual bool Contains(const KeyType& item) const;
    virtual bool isFull() const;
    virtual bool isEmpty() const;
    virtual int Size() const;

    // Friend function declaration
    template <class K>
    friend std::ostream& operator<<(std::ostream& os, const Bag<K>& bag);

protected:
    virtual void Full() const;
    virtual void Empty() const;

    KeyType* array; // Array of items
    int itemCount; // Current count of items in the bag
    int MAX_SIZE; // Capacity
};

#include "Bag.cpp"

#endif





/************************************Bag.cpp*************************************/
#ifndef BAG_CPP
#define BAG_CPP

#include "Bag.h"
#include <stdexcept>
#include <algorithm>

template <class KeyType>
Bag<KeyType>::Bag(int MAX_SIZE) {
    this->array = new KeyType[MAX_SIZE];
    this->itemCount = 0;
    this->MAX_SIZE = MAX_SIZE;
}

template <class KeyType>
Bag<KeyType>::~Bag() {
    delete[] array;
}

template <class KeyType>
void Bag<KeyType>::Add(const KeyType& item) {
    if (isFull()) {
        throw std::overflow_error("Bag is full");
    }
    array[itemCount++] = item;
}

template <class KeyType>
int* Bag<KeyType>::Delete(const KeyType& item) {
    for (int i = 0; i < itemCount; ++i) {
        if (array[i] == item) {
            std::swap(array[i], array[--itemCount]);
            return &array[itemCount];
        }
    }
    return nullptr;
}

template <class KeyType>
bool Bag<KeyType>::Contains(const KeyType& item) const {
    for (int i = 0; i < itemCount; ++i) {
        if (array[i] == item) {
            return true;
        }
    }
    return false;
}

template <class KeyType>
bool Bag<KeyType>::isFull() const {
    return itemCount == MAX_SIZE;
}

template <class KeyType>
bool Bag<KeyType>::isEmpty() const {
    return itemCount == 0;
}

template <class KeyType>
int Bag<KeyType>::Size() const {
    return itemCount;
}

template <class KeyType>
void Bag<KeyType>::Full() const {
    if (isFull()) std::cout << "The bag is full." << std::endl;
}

template <class KeyType>
void Bag<KeyType>::Empty() const {
    if (isEmpty()) std::cout << "The bag is empty." << std::endl;
}

// Non-member friend function definition
template <class KeyType>
std::ostream& operator<<(std::ostream& os, const Bag<KeyType>& bag) {
    for (int i = 0; i < bag.itemCount; ++i) {
        os << bag.array[i] << " ";
    }
    os << std::endl;
    return os;
}

//Explicit instantiation for desired types
template class Bag<int>;

#endif //BAG_CPP




/************************************Stack.h**************************************/
#ifndef STACK_H
#define STACK_H

#include "Bag.h"

template <class KeyType>
class Stack : public Bag<KeyType> {
public:
    Stack(int MAX_SIZE = DEFAULT_SIZE);
    ~Stack();

    void AddToStack(const KeyType& item);
    int* Delete(int& item); // Delete element from stack 
    KeyType PeekAtTop() const;
};

#include "Stack.cpp"

#endif // STACK_H




/***********************************Stack.cpp*************************************/
#ifndef STACK_CPP
#define STACK_CPP

#include "Stack.h"
#include <stdexcept>

template <class KeyType>
Stack<KeyType>::Stack(int MAX_SIZE) : Bag<KeyType>(MAX_SIZE) {}

template <class KeyType>
Stack<KeyType>::~Stack() {}

template <class KeyType>
void Stack<KeyType>::AddToStack(const KeyType& item) {
    this->Add(item); // Use the inherited Add method from Bag
}

template <class KeyType>
int* Stack<KeyType>::Delete(int& item) {
    if (this->isEmpty()) {
        this->Empty();
        return nullptr;
    }
    item = this->array[--this->itemCount];
    return &item;
}

template <class KeyType>
KeyType Stack<KeyType>::PeekAtTop() const {
    if (this->isEmpty()) {
        throw std::out_of_range("Stack is empty");
    }
    return this->array[this->itemCount - 1];
}

//Explicit instantiation for desired types
template class Stack<int>;

#endif //STACK_CPP