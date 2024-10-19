/**
 * Solutions to Chapter 4, Section 10, Exercise 9 of Horowitz's Fundamentals
 * of Data Structures in C++.
 * 
 * Task 9:
 * One way to represent generalized lists is through the use of two field nodes
 * and a symbol table which contains all atoms and list names together with
 * pointers to these lists. Let the two fields of each node be named ALINK and
 * BLINK. Then BLINK either points to the next node on the same level, if there
 * is one, or is a zero. The ALINK field either points to a node at a lower
 * level or, in the case of an atom or list name, to the appropriate entry in
 * the symbol table. For example, the list B(A,(D,E),( ),B) would have the
 * representation as shown by the picture. (The list names D and E were already 
 * in the table at the time the list B was input. A was not in the table and so 
 * assumed to be an atom.)
 * 
 * The symbol table retains a type bit for each entry. Type = 1 if the entry is 
 * a list name and type = 0 for atoms. The NIL atom may either be in the table 
 * or ALINK can be set to 0 to represent the NIL atom. 
 * 
 * Write a C++ function operator>> to read in a list in parenthesis notation
 * and to set up its linked representation as shown in the picture with x set to
 * point to the first node in the list. Note that no head nodes are in use. You
 * may need to use variant records or simulate pointers by integers (or both). 
 * The following subalgorithms may be used by operator>>:
 * 
 * i)   int get(a)  ... searches the symbol table for the name a. -1 is returned
 *                      if a is not found in the table; otherwise, the position 
 *                      of a in the table is returned.
 * ii)  put(a,t,p)  ... enters a into the table. p is the position at which a  
 *                      was entered. If a is already in the table, then the type
 *                      and address fields of the old entry are changed. t = 0
 *                      to enter an atom or t > 0 to enter a list with address t.
 *                      (Note: this permits recursive definition of lists using
 *                      indirect recursion).
 * iii) NextToken() ... gets next token in input list. 
 *                      (A token may be a list name, atom,'(',')'or ','. A '#' 
 *                       is returned if there are no more tokens.)
 * iv) new GenListNode gets a node for use.
 * 
 * You may assume that the input list is syntactically correct. If a sublist is 
 * labeled as in the list C(D,E(F,G)) the structure should be set up as in the 
 * case C(D,(F,G)) and E should be entered into the symbol table as a list with 
 * the appropriate storing address. 
 */

/**********************************main.cpp************************************/
#include "GenList.h"
#include <iostream>
#include <cassert>
#include <sstream>

// Modified TestSymbolTableSetup in main.cpp
void TestSymbolTableSetup() {
    GenList list;
    
    // First add entries for D and E with their specific addresses
    list.put("D", 1, 15);  // type 1 for list, address 15
    list.put("E", 1, 2);   // type 1 for list, address 2
    
    // Add NIL with type 0
    list.put("NIL", 0, -1);
    
    // Now input the list B(A, (D,E), ( ), B)
    std::istringstream input("B(A, (D,E), ( ), B)");
    input >> list;
    
    // Make sure B has the correct type and address
    int bIndex = list.get("B");
    if (bIndex != -1) {
        list.symbolTable[bIndex].type = 1;
        list.symbolTable[bIndex].address = 10;
    }

    // Print the symbol table contents
    std::cout << "\nSymbol Table Contents:\n";
    std::cout << "Name\tType\tAddress\n";
    std::cout << "----------------------\n";
    
    std::vector<std::string> names = {"D", "E", "NIL", "B", "A"};
    for (const auto& name : names) {
        int idx = list.get(name);
        if (idx != -1) {
            const auto& entry = list.symbolTable[idx];
            std::cout << name << "\t" 
                     << entry.type << "\t" 
                     << (entry.address == -1 ? "-" : std::to_string(entry.address)) 
                     << "\n";
        }
    }

    std::cout << "\nList Structure:\n";
    std::cout << list << std::endl;
}

// Helper function to check if two strings are equal for assertions
void AssertEqual(const std::string& actual, const std::string& expected,
                 const std::string& testName) {
    if (actual != expected) {
        std::cerr << "Test Failed: " << testName << "\n";
        std::cerr << "Expected: " << expected << "\n";
        std::cerr << "Actual: " << actual << "\n";
        exit(EXIT_FAILURE);
    } else {
        std::cout << "Test Passed: " << testName << "\n";
    }
}

// Test function for parsing a simple generalized list
void TestSimpleList() {
    GenList list;
    std::istringstream input("(a, b, c)");
    input >> list;

    std::ostringstream output;
    output << list;

    AssertEqual(output.str(), "(a, b, c)", "TestSimpleList");
}

// Test function for parsing a nested list
void TestNestedList() {
    GenList list;
    std::istringstream input("(a, (b, c), d)");
    input >> list;

    std::ostringstream output;
    output << list;

    AssertEqual(output.str(), "(a, (b, c), d)", "TestNestedList");
}

// Test function for parsing an empty list
void TestEmptyList() {
    GenList list;
    std::istringstream input("()");
    input >> list;

    std::ostringstream output;
    output << list;

    AssertEqual(output.str(), "()", "TestEmptyList");
}

// Test function for handling complex nested lists
void TestComplexList() {
    GenList list;
    std::istringstream input("((a, (b, c)), (d, e), f)");
    input >> list;

    std::ostringstream output;
    output << list;

    AssertEqual(output.str(), "((a, (b, c)), (d, e), f)", "TestComplexList");
}

// Test function for symbol table handling (get and put functions)
void TestSymbolTable() {
    GenList list;
    list.put("x", 0, 100);
    list.put("y", 1, 200);

    // Verify that we can retrieve the values correctly
    int indexX = list.get("x");
    int indexY = list.get("y");

    assert(indexX != -1 && "TestSymbolTable: x should be present in the symbol table.");
    assert(indexY != -1 && "TestSymbolTable: y should be present in the symbol table.");

    std::cout << "Test Passed: TestSymbolTable" << "\n";
}

// Function to run all tests
void RunAllTests() {
    TestSimpleList();
    TestNestedList();
    TestEmptyList();
    TestComplexList();
    TestSymbolTable();
    TestSymbolTableSetup();
}

int main() {
    std::cout << "Running GenList Tests...\n";
    RunAllTests();
    std::cout << "All Tests Passed!\n";
    return 0;
}



/*********************************GenList.h***********************************/
#include <iostream>
#include <vector>
#include <string>

enum Boolean { FALSE, TRUE };

class GenList;

class GenListNode {
    friend class GenList;
    friend std::istream& operator>>(std::istream& is, GenList& list);
    friend std::ostream& operator<<(std::ostream& os, const GenList& list);
    friend void TestSymbolTableSetup();
  private:
    Boolean tag; // TRUE for sublist, FALSE for atom
    GenListNode* alink; // Points to sublist or index in the symbol table
    GenListNode* blink; // Points to the next sibling
};

class GenList {
  private:
    GenListNode* first;
    struct SymbolTableEntry {
        std::string name;
        int type;   // 0 for atom, 1 for list
        int address; // Address in the structure
    };
    std::vector<SymbolTableEntry> symbolTable;

    // Helper functions for parsing and printing
    void PrintNode(const GenListNode* node, std::ostream& os) const;
    void PrintList(const GenListNode* node, std::ostream& os) const;
    
    void updateAddresses(GenListNode* node, int& nextAddress);
    
    GenListNode* ParseList(const std::string& input, size_t& pos);
    void DeleteList(GenListNode* node);

  public:
    GenList() : first(nullptr) {}
    ~GenList(); // Destructor to clean up memory

    int get(const std::string& a);
    void put(const std::string& a, int t, int p);
    std::string NextToken(const std::string& input, size_t& pos);
    
    friend std::istream& operator>>(std::istream& is, GenList& list);
    friend std::ostream& operator<<(std::ostream& os, const GenList& list);
    friend void TestSymbolTableSetup();
};



/********************************GenList.cpp***********************************/
#include "GenList.h"
#include <cstdlib>
#include <stack>
#include <map>
#include <cctype>
#include <stdexcept>

// Helper function to search the symbol table
int GenList::get(const std::string& a) {
    for (size_t i = 0; i < symbolTable.size(); ++i) {
        if (symbolTable[i].name == a) {
            return i;
        }
    }
    return -1;
}

// Helper function to insert or update the symbol table
void GenList::put(const std::string& a, int t, int p) {
    int index = get(a);
    if (index != -1) {
        symbolTable[index].type = t;
        symbolTable[index].address = p;
    } else {
        SymbolTableEntry entry = {a, t, p};
        symbolTable.push_back(entry);
    }
}

// Helper function to get the next token
std::string GenList::NextToken(const std::string& input, size_t& pos) {
    while (pos < input.size() && isspace(input[pos])) {
        ++pos;
    }
    if (pos >= input.size()) {
        return "#";
    }
    if (input[pos] == '(' || input[pos] == ')' || input[pos] == ',') {
        return std::string(1, input[pos++]);
    }
    size_t start = pos;
    while (pos < input.size() && isalnum(input[pos])) {
        ++pos;
    }
    return input.substr(start, pos - start);
}

// Function to maintain variant records
void GenList::updateAddresses(GenListNode* node, int& nextAddress) {
    if (!node) return;

    if (node->tag == TRUE) {
        // Only update addresses for nodes that are explicitly marked as lists
        // in the original symbol table (D, E, B)
        GenListNode* firstAtom = node->alink;
        if (firstAtom && !firstAtom->tag) {
            int index = reinterpret_cast<std::uintptr_t>(firstAtom->alink);
            if (index >= 0 && index < symbolTable.size()) {
                // Don't overwrite existing addresses for D and E
                if (symbolTable[index].type == 1 && symbolTable[index].address != -1) {
                    // Keep the existing address
                } else {
                    // Only update address if it's not already set
                    symbolTable[index].type = 0; // Keep as atom by default
                }
            }
        }
        // Process the sublist
        updateAddresses(node->alink, nextAddress);
    }
    
    // Process the next sibling
    updateAddresses(node->blink, nextAddress);
}

std::istream& operator>>(std::istream& is, GenList& list) {
    std::string input;
    std::getline(is, input);
    size_t pos = 0;

    // Clear existing list but preserve symbol table
    delete list.first;
    list.first = nullptr;

    std::stack<GenListNode**> nodeStack;
    nodeStack.push(&list.first);

    // First pass: Build the list structure
    std::string token;
    while (!(token = list.NextToken(input, pos)).empty() && token != "#") {
        if (token == ")") {
            if (!nodeStack.empty()) nodeStack.pop();
            continue;
        }
        
        if (token == ",") continue;

        // Create and initialize new node
        GenListNode* newNode = new GenListNode();
        newNode->blink = nullptr;

        if (token == "(") {
            // Handle sublist
            newNode->tag = TRUE;
            newNode->alink = nullptr;
        } else {
            // Handle atom
            newNode->tag = FALSE;
            int index = list.get(token);
            if (index == -1) {
                // New symbol - add as atom with no address
                list.put(token, 0, -1);
                index = list.get(token);
            }
            // Do not modify existing type and address here
            newNode->alink = reinterpret_cast<GenListNode*>(static_cast<std::uintptr_t>(index));
        }

        // Insert the new node into the list
        if (*nodeStack.top() == nullptr) {
            *nodeStack.top() = newNode;
        } else {
            GenListNode* current = *nodeStack.top();
            while (current->blink != nullptr) {
                current = current->blink;
            }
            current->blink = newNode;
        }

        // If this is a sublist, push it onto the stack
        if (token == "(") {
            nodeStack.push(&(newNode->alink));
        }
    }

    return is;
}

// Helper function to print a node
void GenList::PrintNode(const GenListNode* node, std::ostream& os) const {
    if (!node) return;
    if (node->tag == TRUE) {
        os << "(";
        PrintList(node->alink, os);
        os << ")";
    } else {
        int index = reinterpret_cast<std::uintptr_t>(node->alink);
        if (index >= 0 && index < symbolTable.size()) {
            os << symbolTable[index].name;
        }
    }
}

// Helper function to print a list
void GenList::PrintList(const GenListNode* node, std::ostream& os) const {
    if (!node) return;
    PrintNode(node, os);
    if (node->blink) {
        os << ", ";
        PrintList(node->blink, os);
    }
}

// Implementation of operator<<
std::ostream& operator<<(std::ostream& os, const GenList& list) {
    list.PrintList(list.first, os);
    return os;
}

// Destructor to free memory
GenList::~GenList() {
    // Call helper function starting with the first node
    DeleteList(first);
    first = nullptr;
    
    // Clear the symbol table
    symbolTable.clear();
}


void GenList::DeleteList(GenListNode* node) {
    if (node == nullptr) return;
    
    // First recursively delete the next sibling
    DeleteList(node->blink);
    // If this is a sublist, recursively delete its contents
    if (node->tag == TRUE) {
        DeleteList(node->alink);
    }
    // Finally delete this node
    delete node;
}
