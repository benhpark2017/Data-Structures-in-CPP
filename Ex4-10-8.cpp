/**
 * Solutions to Chapter 4, Section 10, Exercises 8 of Horowitz's
 * Fundamentals of Data Structures in C++.
 * 
 * Assignment:
 * Devise a function that produces the list representation of an arbitrary
 * list, given its linear form as a string of atoms, commas, blanks, and
 * parentheses.
 * 
 */

/***********************************main.cpp**************************************/
#include <iostream>
#include "GenList.h"

int main() {
    // Create the list A = (a (b, c))
    GenList* listA = new GenList();
    listA->addAtomic('a');

    GenList* sublist = new GenList();
    sublist->addAtomic('b');
    sublist->addAtomic('c');

    listA->addSublist(sublist);

    std::cout << "List A: ";
    listA->print();
    std::cout << std::endl;

    // Create the list B = (A, A, ())
    GenList* listB = new GenList();
    listB->addSublist(listA);  // Reuse list A
    listB->addSublist(listA);  // Reuse list A again
    listB->addSublist(new GenList());  // Empty sublist

    std::cout << "List B: ";
    listB->print();
    std::cout << std::endl;

    // Test the parseFromString function
    std::string input = "(a,(b,c))";
    GenList* parsedList = GenList::parseFromString(input);

    std::cout << "Parsed List: ";
    parsedList->print();
    std::cout << std::endl;

    std::cout << "Detailed Parsed List:" << std::endl;
    parsedList->printDetailed();

    // Clean up
    delete listB;
    delete listA;
    delete sublist;
    delete parsedList;

    return 0;
}



/**********************************GenList.h**************************************/
#ifndef GENLIST_H
#define GENLIST_H

#include <iostream>

// Forward declaration
class GenListNode;

class GenList {
private:
    GenListNode* head;

    // Helper function for deep copy
    GenListNode* copyList(GenListNode* source);

    // Helper function for destruction
    void destroyList(GenListNode* node);
    
    void printNode(const GenListNode* node, int& nodeCount) const;

public:
    // Constructor for an empty list
    GenList();

    // Copy constructor
    GenList(const GenList& other);

    // Destructor
    ~GenList();

    // Add an atomic value to the list
    void addAtomic(const char& value);

    // Add a sublist to the list
    void addSublist(GenList* sublist);

    // Print the list
    void print() const;

    // Increment reference count (used by sublists)
    void incrementRef();

    // Decrement reference count (used by sublists)
    void decrementRef();
    
    void printDetailed() const;
    
    void printNodeDetailed(const GenListNode* node, int& nodeCount, int depth) const;
    
    // Parse a string representation of a list and return a GenList object
    static GenList* parseFromString(const std::string& input);
};

#include "GenListNode.h"
#include "GenList.tpp"

#endif // GENLIST_H


/**********************************GenList.tpp**************************************/
#ifndef GENLIST_TPP
#define GENLIST_TPP

#include "GenList.h"
#include <iostream>
#include <stack>
#include <string>

GenList::GenList() : head(nullptr) {}

GenList::GenList(const GenList& other) : head(copyList(other.head)) {}

GenList::~GenList() {
    destroyList(head);
}

void GenList::addAtomic(const char& value) {
    GenListNode* newNode = new GenListNode(value);
    newNode->next = nullptr;
    if (!head) {
        head = newNode;
    } else {
        GenListNode* current = head;
        while (current->next) {
            current = current->next;
        }
        current->next = newNode;
    }
}

void GenList::addSublist(GenList* sublist) {
    GenListNode* newNode = new GenListNode(sublist);
    newNode->next = nullptr;
    if (!head) {
        head = newNode;
    } else {
        GenListNode* current = head;
        while (current->next) {
            current = current->next;
        }
        current->next = newNode;
    }
}

void GenList::print() const {
    std::cout << "(";
    GenListNode* current = head;
    while (current) {
        if (current->isAtomic) {
            std::cout << current->data.atomic;
        } else {
            current->data.sublist->print();
        }
        if (current->next) {
            std::cout << ", ";
        }
        current = current->next;
    }
    std::cout << ")";
}

void GenList::incrementRef() {
    if (head) {
        head->incrementRef();
    }
}

void GenList::decrementRef() {
    if (head) {
        head->decrementRef();
    }
}

GenListNode* GenList::copyList(GenListNode* source) {
    if (!source) {
        return nullptr;
    }

    GenListNode* newNode;
    if (source->isAtomic) {
        newNode = new GenListNode(source->data.atomic);
    } else {
        newNode = new GenListNode(new GenList(*source->data.sublist));
    }

    newNode->next = copyList(source->next);
    return newNode;
}

void GenList::destroyList(GenListNode* node) {
    while (node) {
        GenListNode* next = node->next;
        node->decrementRef();
        node = next;
    }
}

void GenList::printDetailed() const {
    int nodeCount = 0;
    printNodeDetailed(head, nodeCount, 0);
}

void GenList::printNodeDetailed(const GenListNode* node, int& nodeCount, int depth) const {
    while (node) {
        nodeCount++;
        /** Adds indentation to show depth of the generalized list
        for (int i = 0; i < depth; i++) {
            std::cout << "  ";
        }
        */
        std::cout << "Node " << nodeCount << ": ";
        
        if (node->isAtomic) {
            std::cout << "tag = f; data = " << node->data.atomic << "; ";
        } else {
            std::cout << "tag = t; dlink; ";
        }

        if (node->next) {
            std::cout << "link;" << std::endl;
        } else {
            std::cout << "link = 0;" << std::endl;
        }

        if (!node->isAtomic) {
            printNodeDetailed(node->data.sublist->head, nodeCount, depth + 1);
        }

        node = node->next;
    }
}

void GenList::printNode(const GenListNode* node, int& nodeCount) const {
    std::cout << "Node " << nodeCount << ": ";
    
    if (node->isAtomic) {
        std::cout << "tag = f; data = " << node->data.atomic << "; ";
    } else {
        std::cout << "tag = t; dlink; ";
    }

    if (node->next) {
        std::cout << "link;" << std::endl;
    } else {
        std::cout << "link = 0;" << std::endl;
    }

    if (!node->isAtomic) {
        GenListNode* sublistNode = node->data.sublist->head;
        while (sublistNode) {
            nodeCount++;
            printNode(sublistNode, nodeCount);
            sublistNode = sublistNode->next;
        }
    }

    nodeCount++;
}

GenList* GenList::parseFromString(const std::string& input) {
    std::stack<GenList*> listStack;
    GenList* currentList = nullptr;
    std::string currentAtom;

    for (char c : input) {
        if (c == '(') {
            GenList* newList = new GenList();
            if (currentList) {
                listStack.push(currentList);
            }
            currentList = newList;
        } else if (c == ')') {
            if (!currentAtom.empty()) {
                currentList->addAtomic(currentAtom[0]);
                currentAtom.clear();
            }
            if (!listStack.empty()) {
                GenList* parentList = listStack.top();
                listStack.pop();
                parentList->addSublist(currentList);
                currentList = parentList;
            }
        } else if (c == ',' || c == ' ') {
            if (!currentAtom.empty()) {
                currentList->addAtomic(currentAtom[0]);
                currentAtom.clear();
            }
        } else {
            currentAtom += c;
        }
    }

    return currentList;
}

#endif // GENLIST_TPP


/********************************GenListNode.h***********************************/
#ifndef GENLISTNODE_H
#define GENLISTNODE_H

// Forward declaration
class GenList;

class GenListNode {
public:
    union Data {
        char atomic;
        GenList* sublist;
        Data() {}
        ~Data() {}
    };

    bool isAtomic;  // true if atomic, false if sublist
    Data data;
    GenListNode* next;
    int refCount;

    // Constructor for atomic data
    GenListNode(const char& value);

    // Constructor for sublist
    GenListNode(GenList* sublist);

    // Destructor
    ~GenListNode();

    // Increment the reference count
    void incrementRef();

    // Decrement the reference count
    void decrementRef();

private:
    // Private copy constructor and assignment operator to prevent copying
    GenListNode(const GenListNode&);
    GenListNode& operator=(const GenListNode&);
};

#include "GenListNode.tpp"

#endif // GENLISTNODE_H


/*******************************GenListNode.tpp**********************************/
#ifndef GENLISTNODE_TPP
#define GENLISTNODE_TPP

#include "GenListNode.h"

GenListNode::GenListNode(const char& value) : isAtomic(true), next(nullptr), refCount(1) {
    data.atomic = value;
}

GenListNode::GenListNode(GenList* sublist) : isAtomic(false), next(nullptr), refCount(1) {
    data.sublist = sublist;
    if (sublist) {
        sublist->incrementRef();
    }
}

GenListNode::~GenListNode() {
    if (!isAtomic && data.sublist) {
        data.sublist->decrementRef();
    }
}

void GenListNode::incrementRef() {
    ++refCount;
}

void GenListNode::decrementRef() {
    --refCount;
    if (refCount == 0) {
        delete this;
    }
}

#endif // GENLISTNODE_TPP
