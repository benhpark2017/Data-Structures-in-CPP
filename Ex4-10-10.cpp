/**
 * Solutions to Chapter 4, Section 10, Exercise 10 of Horowitz's Fundamentals
 * of Data Structures in C++.
 * 
 * Task 10:
 * [Wilczynski] Following the conventions of LISP, assume nodes with two data
 * members: HEAD and TAIL. If A = ((a(bc))), then HEAD(A) = (a(bc)), TAIL(A) =
 * NIL, HEAD(HEAD(A)) = a, and TAIL(HEAD(A)) = (bc). CONS(A, B) gets a new node
 * T, stores A in its HEAD, B in its TAIL, and returns T. B must always be a
 * list. If L = a and M = (bc), then CONS(L, M) = (abc) and CONS(M, M) = 
 * ((bc)bc). Three other useful functions are: ATOM(X) which is true if X is an
 * atom else false; NIL(X) which is true if X is NIL else false; EQUAL(X, Y)
 * which is true if X and Y are the same atoms or equivalent lists, else false.
 * (a) Give a sequence of HEAD and TAIL operations for extracting 'a' from the
 *     lists: ((cat)), ((a)), ((mart)), and (((cb)a)).
 * 
 *     Soln) There is none.
 *           HEAD -> HEAD
 *           There is none.
 *           HEAD -> TAIL
 * 
 * (b) Write recursive functions for the standard LISP functions: COPY, REVERSE,
 *     and APPEND.
 * (c) Implement this LISP subsystem. Store atoms in an array and write
 *     functions MakeGenList and GenListPrint for input and output of lists.
 */

#include "GenList.h"
#include <iostream>
#include <cassert>

int main() {

    GenList genList; // Create an instance of GenList

    // Test parseList and cons
    GenListNode* atom1 = genList.parseList("a");
    GenListNode* atom2 = genList.parseList("b");
    GenListNode* result1 = genList.cons(atom1, atom2);
    std::cout << "Result of cons(atom1, atom2): ";
    genList.print(result1); // Print result1

    // Test parseList with a list
    GenListNode* list1 = genList.parseList("(xy)");
    GenListNode* atom3 = genList.parseList("z");
    GenListNode* result2 = genList.cons(list1, atom3);
    std::cout << "\nResult of cons(list1, atom3): ";
    genList.print(result2); // Print result2


    // Test copy
    GenListNode* original1 = genList.parseList("(a b c)");
    GenListNode* copied1 = genList.copy(original1);
    std::cout << "\nOriginal list1: ";
    genList.print(original1);
    std::cout << "\nCopied list1: ";
    genList.print(copied1);


    // Test reverse
    GenListNode* list2 = genList.parseList("(a (bc) d e (fgh))");
    GenListNode* reversed1 = genList.reverse(list2);
    std::cout << "\nOriginal list2: ";
    genList.print(list2);
    std::cout << "\nReversed list2: ";
    genList.print(reversed1);

    // Test append
    GenListNode* list3 = genList.parseList("(a b)");
    GenListNode* list4 = genList.parseList("(c d)");
    std::cout << "\nOriginal list3: ";
    genList.print(list3);
    std::cout << "\nOriginal list4 to be appended to list3: ";
    genList.print(list4);
    GenListNode* appended1 = genList.append(list3, list4);
    std::cout << "\nAppended list3 and list4: ";
    genList.print(appended1);

    return 0;
}


/*******************************GenListNode.h**********************************/
#ifndef GENLISTNODE_H
#define GENLISTNODE_H

#include <string>

class GenList; // Forward declaration

class GenListNode {
public:
    GenListNode* head;
    GenListNode* tail;
    int atomIndex;

    GenListNode(GenListNode* h = nullptr, GenListNode* t = nullptr, int idx = -1);
    bool isAtom() const;
    bool isNil() const;
    ~GenListNode();
};

#endif // GENLISTNODE_H



/******************************GenListNode.cpp*********************************/
#include "GenListNode.h"
#include <iostream>

GenListNode::GenListNode(GenListNode* h, GenListNode* t, int idx)
    : head(h), tail(t), atomIndex(idx) {}

bool GenListNode::isAtom() const {
    return head == nullptr && tail == nullptr && atomIndex >= 0;
}

bool GenListNode::isNil() const {
    return this == nullptr;
}

GenListNode::~GenListNode() {
    delete head;
    delete tail;
}



/*********************************GenList.h************************************/
#ifndef GENLIST_H
#define GENLIST_H

#include "GenListNode.h"
#include <string>
#include <vector>

class GenList {
  private:
    static const int MAX_ATOMS = 1000; // Moved MAX_ATOMS here
    std::vector<std::string> atoms; // Moved atoms to be a non-static member
    int atomCount = 0; // Moved atomCount to be a non-static member
    
    // Atom storage management methods
    int addAtom(const std::string& value);
    bool isAtomStorageFull() const;
    const std::string& getAtom(int index) const;

    GenListNode* parseListRecursive(std::string& input, size_t& pos);
    std::string getAtomValue(const GenListNode* node) const;

  public:
    GenList(); // Constructor to initialize atom storage if needed
    
    GenListNode* createAtom(const std::string& value);
    GenListNode* cons(GenListNode* A, GenListNode* B);
    GenListNode* copy(const GenListNode* list);
    GenListNode* reverse(const GenListNode* list);
    GenListNode* append(const GenListNode* list1, const GenListNode* list2);
    
    GenListNode* parseList(const std::string& input);
    void print(const GenListNode* list, bool isOuter = true) const;
    void deleteList(GenListNode* list);

};

#endif // GENLIST_H



/********************************GenList.cpp***********************************/
#include "GenList.h"
#include <iostream>
#include <cctype>

// Constructor to initialize atom storage
GenList::GenList() : atomCount(0) {
    atoms.reserve(MAX_ATOMS);
}

/** 
 * A function that takes in the string value to store as an atom
 * into the function and returns a pointer to a new GenListNode
 * containing the atom, or nullptr if the atom storage is full.
 */
GenListNode* GenList::createAtom(const std::string& value) {
    int atomIndex = addAtom(value);
    if (atomIndex != -1) {
        return new GenListNode(nullptr, nullptr, atomIndex);
    }
    std::cerr << "Error: Failed to create atom '" << value << "' - storage full\n";
    return nullptr;
}

// Atom storage management methods
int GenList::addAtom(const std::string& value) {
    if (isAtomStorageFull()) {
        return -1;
    }

    // Check if atom already exists
    for (int i = 0; i < atomCount; ++i) {
        if (atoms[i] == value) {
            return i;
        }
    }

    // Add new atom
    atoms.push_back(value);
    return atomCount++;
}

bool GenList::isAtomStorageFull() const {
    return atomCount >= MAX_ATOMS;
}

const std::string& GenList::getAtom(int index) const {
    static const std::string empty;
    if (index >= 0 && index < atomCount) {
        return atoms[index];
    }
    return empty;
}

std::string GenList::getAtomValue(const GenListNode* node) const {
    if (node && node->isAtom() && node->atomIndex >= 0 &&
        node->atomIndex < atomCount) {
        return atoms[node->atomIndex];
    }
    return "";
}

// Core list operations
GenListNode* GenList::cons(GenListNode* A, GenListNode* B) {
    return new GenListNode(A, B);
}

GenListNode* GenList::copy(const GenListNode* list) {
    if (list == nullptr) return nullptr;
    if (list->isAtom()) {
        return new GenListNode(nullptr, nullptr, list->atomIndex);
    }
    return new GenListNode(copy(list->head), copy(list->tail), -1);
}

GenListNode* GenList::reverse(const GenListNode* list) {
    if (list == nullptr || list->isAtom()) {
        return copy(list);
    }
    GenListNode* reversedList = nullptr;
    const GenListNode* current = list;

    while (current != nullptr) {
        GenListNode* headCopy = current->head->isAtom() ?
                                copy(current->head) : reverse(current->head);
        reversedList = cons(headCopy, reversedList);
        current = current->tail;
    }
    return reversedList;
}

GenListNode* GenList::append(const GenListNode* list1, const GenListNode* list2) {
    if (list1 == nullptr) return const_cast<GenListNode*>(list2);
    if (list1->isAtom()) {
        return new GenListNode(nullptr, const_cast<GenListNode*>(list2), list1->atomIndex);
    }
    return new GenListNode(list1->head, append(list1->tail, list2), -1);
}

// List parsing and printing
GenListNode* GenList::parseList(const std::string& input) {
    size_t pos = 0;
    std::string str = input;
    return parseListRecursive(str, pos);
}

GenListNode* GenList::parseListRecursive(std::string& input, size_t& pos) {
    if (pos >= input.length()) return nullptr;

    // Skip whitespace
    while (pos < input.length() && std::isspace(input[pos])) pos++;
    if (pos >= input.length()) return nullptr;

    // Handle opening parenthesis for sublist
    if (input[pos] == '(') {
        pos++; // Skip opening parenthesis
        GenListNode* result = nullptr;
        GenListNode* current = nullptr;

        // Handle empty list "()", which is treated as NIL
        while (pos < input.length() && std::isspace(input[pos])) pos++;
        if (pos < input.length() && input[pos] == ')') {
            pos++; // Skip closing parenthesis
            return nullptr; // Return NIL for an empty list
        }

        // Parse elements inside the list
        while (pos < input.length()) {
            // Skip whitespace
            while (pos < input.length() && std::isspace(input[pos])) pos++;
            if (pos >= input.length()) break;

            // Check for closing parenthesis
            if (input[pos] == ')') {
                pos++; // Skip closing parenthesis
                return result; // Return the parsed list
            }

            // Parse the next element recursively
            GenListNode* element = parseListRecursive(input, pos);

            // Add the element to the list
            if (result == nullptr) {
                result = new GenListNode(element, nullptr, -1);
                current = result;
            } else {
                current->tail = new GenListNode(element, nullptr, -1);
                current = current->tail;
            }
        }
    } else {
        // Parse atom
        size_t start = pos;
        while (pos < input.length() && !std::isspace(input[pos]) &&
               input[pos] != '(' && input[pos] != ')') {
            pos++;
        }
        std::string atomValue = input.substr(start, pos - start);
        int atomIndex = addAtom(atomValue);
        if (atomIndex == -1) {
            std::cerr << "Error: Failed to add atom '" << atomValue << "'\n";
            return nullptr;
        }
        return new GenListNode(nullptr, nullptr, atomIndex);
    }
    return nullptr;
}

void GenList::print(const GenListNode* list, bool isOuter) const {
    if (list == nullptr) {
        std::cout << "()";
        return;
    }
    if (list->isAtom()) {
        std::cout << getAtomValue(list);
    } else {
        if (isOuter) std::cout << "(";
        print(list->head, true);
        if (list->tail != nullptr) {
            std::cout << " ";
            print(list->tail, false);
        }
        if (isOuter) std::cout << ")";
    }
}

void GenList::deleteList(GenListNode* list) {
    if (list == nullptr) return;
    deleteList(list->head);
    deleteList(list->tail);
    delete list;
}
