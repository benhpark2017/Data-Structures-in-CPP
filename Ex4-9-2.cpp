/**
 * Solutions to Chapter 4, Section 9, Exercise 2 of Horowitz's
 * Fundamentals of Data Structures in C++.
 */

/********************************main.cpp**************************************/
#include <iostream>
#include "DoublyLinkedList.h"

int main() {
    DoublyLinkedList list(10);  // Create a list with capacity 10
    
    list.insert(1);
    list.insert(2);
    list.insert(3);
    list.insert(4);
    list.insert(5);
    
    list.traverseLeftToRight();  // This will print: Traversing left to right: 1 2 3 4 5
    list.traverseRightToLeft();  // THis will print: Traversing right to left: 5 4 3 2 1
    
    return 0;
}



/**************************DoublyLinkedList.h*****************************/
#include <cstddef>
#include <stdexcept>
#include <iostream>

class DoublyLinkedListNode {
private:
    int info;
    size_t link;

public:
    DoublyLinkedListNode() : info(0), link(0) {}
    DoublyLinkedListNode(int value) : info(value), link(0) {}

    int getValue() const { return info; }
    void setValue(int value) { info = value; }

    size_t getLink() const { return link; }
    void setLink(size_t xorLink) { link = xorLink; }

    friend class DoublyLinkedList;
};

class DoublyLinkedList {
private:
    DoublyLinkedListNode* left;
    DoublyLinkedListNode* right;
    DoublyLinkedListNode* memory;
    int size;
    int capacity;

    size_t XOR(DoublyLinkedListNode* a, DoublyLinkedListNode* b) const {
        return reinterpret_cast<size_t>(a) ^ reinterpret_cast<size_t>(b);
    }

    DoublyLinkedListNode* XORPtr(size_t a, DoublyLinkedListNode* b) const {
        return reinterpret_cast<DoublyLinkedListNode*>(a ^ reinterpret_cast<size_t>(b));
    }

public:
    DoublyLinkedList(int n) : left(nullptr), right(nullptr), size(0), capacity(n) {
        memory = new DoublyLinkedListNode[n];
    }

    ~DoublyLinkedList() {
        delete[] memory;
    }

    void insert(int value) {
        if (size >= capacity) {
            throw std::runtime_error("List is full");
        }

        DoublyLinkedListNode* newNode = &memory[size];
        newNode->setValue(value);

        if (size == 0) {
            left = right = newNode;
        } else {
            newNode->setLink(XOR(right, nullptr));
            right->setLink(XOR(XORPtr(right->getLink(), nullptr), newNode));
            right = newNode;
        }

        size++;
    }

    void traverseLeftToRight() {
        DoublyLinkedListNode* curr = left;
        DoublyLinkedListNode* prev = nullptr;
        
        std::cout << "Traversing left to right: ";
        while (curr != nullptr) {
            std::cout << curr->getValue() << " ";
            
            DoublyLinkedListNode* next = XORPtr(curr->getLink(), prev);
            prev = curr;
            curr = next;
        }
        std::cout << std::endl;
    }
    
    void traverseRightToLeft() {
        DoublyLinkedListNode* curr = right;
        DoublyLinkedListNode* next = nullptr;
        
        std::cout << "Traversing right to left: ";
        while (curr != nullptr) {
            std::cout << curr->getValue() << " ";
            
            DoublyLinkedListNode* prev = XORPtr(curr->getLink(), next);
            next = curr;
            curr = prev;
        }
        std::cout << std::endl;
    }
    
};
