/**
 * Solutions to Chapter 4, Section 10, Exercise 1 of Horowitz's
 * Fundamentals of Data Structures in C++.
 * 
 * The program maintains a generalized list to express a multivariate
 * polynomial using the following node structure.
 * 
 * |----------------|------------|------|
 * |tag = FALSE/TRUE| data/dlink | link |
 * |----------------|------------|------|
 */

/*********************************main.cpp*************************************/

#include "GenList.h"
#include "GenListNode.h"
#include <iostream>

void printNodeInfo(const GenListNode* node, const std::string& name) {
    std::cout << "Node " << name << ": ";
    if (node) {
        std::cout << "Tag=" << (node->getTag() == TRUE ? "TRUE" : "FALSE")
                  << ", Data=" << (node->getTag() == FALSE ? node->getData() : '_')
                  << ", Link=" << (node->getLink() ? "exists" : "null")
                  << ", DLink=" << (node->getTag() == TRUE && node->getDLink() ? "exists" : "null")
                  << std::endl;
    } else {
        std::cout << "null" << std::endl;
    }
}

int main() {
    // Create (3x^4 + 5x^3 + 7x)y^6 + y(6x)
    // First term: (3x^4 + 5x^3 + 7x)y^6
    GenListNode* x4 = new GenListNode(FALSE, '4'); // Exponent 4
    GenListNode* xVar = new GenListNode(FALSE, 'x'); // Variable 'x'
    xVar->setLink(x4);
    GenListNode* coef3 = new GenListNode(FALSE, '3'); // Coefficient '3'
    coef3->setLink(xVar);
    GenListNode* x3 = new GenListNode(FALSE, '3'); // Exponent 3
    GenListNode* xVar3 = new GenListNode(FALSE, 'x'); // Variable 'x'
    xVar3->setLink(x3);
    GenListNode* coef5 = new GenListNode(FALSE, '5'); // Coefficient '5'
    coef5->setLink(xVar3);
    GenListNode* x1 = new GenListNode(FALSE, '1'); // Exponent 1 (optional)
    GenListNode* xVar1 = new GenListNode(FALSE, 'x'); // Variable 'x'
    xVar1->setLink(x1);
    GenListNode* coef7 = new GenListNode(FALSE, '7'); // Coefficient '7'
    coef7->setLink(xVar1);
    x4->setLink(coef5);
    x3->setLink(coef7);
    GenListNode* sublist1 = new GenListNode(TRUE, coef3);
    GenListNode* y1 = new GenListNode(FALSE, 'y');
    GenListNode* y1Exp = new GenListNode(FALSE, '6'); // y^6
    y1->setLink(y1Exp);
    y1Exp->setLink(sublist1);

    // Second term: y(6x)
    GenListNode* x = new GenListNode(FALSE, '1'); // Exponent 1
    GenListNode* xVar2 = new GenListNode(FALSE, 'x'); // Variable 'x'
    xVar2->setLink(x);
    GenListNode* coef6 = new GenListNode(FALSE, '6'); // Coefficient '6'
    coef6->setLink(xVar2);
    GenListNode* sublist2 = new GenListNode(TRUE, coef6);
    GenListNode* y2 = new GenListNode(FALSE, 'y');
    y2->setLink(sublist2);

    // Link the two terms
    sublist1->setLink(y2);

    // Create the polynomial
    GenList polynomial;
    polynomial.setFirst(y1);

    // Debug output: Print node information
    std::cout << "Node structure:" << std::endl;
    printNodeInfo(y1, "y1");
    printNodeInfo(y1Exp, "y1Exp");
    printNodeInfo(sublist1, "sublist1");
    printNodeInfo(coef3, "coef3");
    printNodeInfo(xVar, "xVar");
    printNodeInfo(x4, "x4");
    printNodeInfo(coef5, "coef5");
    printNodeInfo(xVar3, "xVar3");
    printNodeInfo(x3, "x3");
    printNodeInfo(coef7, "coef7");
    printNodeInfo(xVar1, "xVar1");
    printNodeInfo(x1, "x1");
    printNodeInfo(y2, "y2");
    printNodeInfo(sublist2, "sublist2");
    printNodeInfo(coef6, "coef6");
    printNodeInfo(xVar2, "xVar2");
    printNodeInfo(x, "x");

    // Print the polynomial
    std::cout << "\nPolynomial: ";
    polynomial.print();

    // Calculate and print the depth
    std::cout << "\nDepth calculation:" << std::endl;
    int depth = polynomial.depth();
    std::cout << "Final depth of the polynomial: " << depth << std::endl;

    // Copy the polynomial
    GenList copiedPolynomial;
    copiedPolynomial.Copy(polynomial);
    std::cout << "\nCopied polynomial: ";
    copiedPolynomial.print();

    // Check if the original and copied polynomials are equal
    std::cout << "Are the original and copied polynomials equal? "
              << (polynomial == copiedPolynomial ? "Yes" : "No") << std::endl;

    std::cout << "End of program." << std::endl;

    return 0;
}



/********************************GenList.h************************************/

#ifndef GENLIST_H
#define GENLIST_H
#include "GenListNode.h"
class GenList {
public:
    GenList() : first(nullptr) {}
    void Copy(const GenList& l);
    bool operator==(const GenList& m) const;
    int depth() const;
    // Setter for the first node
    void setFirst(GenListNode* node) { first = node; }
    // Getter for the first node (if needed)
    GenListNode* getFirst() const { return first; }
    // Print the generalized list
    void print() const {
        if (first) {
            first->print();
            std::cout << std::endl;
        } else {
            std::cout << "Empty Polynomial." << std::endl;
        }
    }
private:
    GenListNode* first;
    int equal(const GenListNode* s, const GenListNode* t) const;
    GenListNode* Copy(GenListNode* p);
    int depth(GenListNode* s) const;
};
#endif // GENLIST_H


/*******************************GenList.cpp**********************************/

#include "GenList.h"

// Deep copy of a generalized list
void GenList::Copy(const GenList& l) {
    first = Copy(l.first);
}

GenListNode* GenList::Copy(GenListNode* p) {
    if (!p) return nullptr;
    GenListNode* q = new GenListNode;
    q->tag = p->tag;
    if (!p->tag) {
        q->data = p->data;
    } else {
        q->dlink = Copy(p->dlink);
    }
    q->link = Copy(p->link);
    return q;
}

bool GenList::operator==(const GenList& m) const {
    return equal(first, m.first);
}

int GenList::equal(const GenListNode* s, const GenListNode* t) const {
    if (!s && !t) return true;
    if (!s || !t) return false;
    if (s->tag != t->tag) return false;
    if (s->tag == TRUE) {
        return equal(s->dlink, t->dlink) && equal(s->link, t->link);
    }
    return s->data == t->data && equal(s->link, t->link);
}

int GenList::depth() const {
    return depth(first);
}

int GenList::depth(GenListNode* s) const {
    if (!s) return 0;

    int maxDepth = 0;
    GenListNode *p = s;
    int m = 0;
    while (p) {
        if (p->tag) {
            int n = depth(p->dlink);
            if (m < n) m = n;
        }
        p = p->link;
    }
    return m + 1;
}


/******************************GenListNode.h*********************************/

#ifndef GENLISTNODE_H
#define GENLISTNODE_H
#include <iostream>

enum Boolean { FALSE, TRUE };
class GenListNode {
    friend class GenList;
    friend int equal(const GenListNode* s, const GenListNode* t);
private:
    GenListNode* link;
    Boolean tag;
    union {
        char data; // Store single character (variable or coefficient)
        GenListNode* dlink; // For nested sublist
    };
public:
    // Default constructor
    GenListNode() : link(nullptr), tag(FALSE), data('\0') {}
    // Constructor for initializing with a data value
    GenListNode(Boolean t, char d) : link(nullptr), tag(t), data(d) {}
    // Constructor for initializing with a dlink
    GenListNode(Boolean t, GenListNode* d) : link(nullptr), tag(t), dlink(d) {}
    // Setter and getter for link
    void setLink(GenListNode* node) { link = node; }
    GenListNode* getLink() const { return link; }
    // Setter and getter for tag
    void setTag(Boolean t) { tag = t; }
    Boolean getTag() const { return tag; }
    // Setter and getter for data
    void setData(char d) { if (tag == FALSE) data = d; }
    char getData() const { return tag == FALSE ? data : '\0'; }
    // Setter and getter for dlink
    void setDLink(GenListNode* node) { if (tag == TRUE) dlink = node; }
    GenListNode* getDLink() const { return tag == TRUE ? dlink : nullptr; }
    // Print function with recursive printing of sublists
    void print() const {
        printPolynomial(this);
    }

private:
    // Helper function to print sublist contents
    void printSublist(const GenListNode* node) const {
        if (!node) return;
        std::cout << "(";
        bool isFirstTerm = true;
        while (node) {
            if (!isFirstTerm) {
                std::cout << " + ";
            }
            if (node->tag == FALSE) {
                // Print coefficient if it's not 1 or if it's the only term
                if (node->data != '1' || (!node->link || (node->link->tag == FALSE && node->link->data != 'x'))) {
                    std::cout << node->data;
                }
                // Print 'x' and its exponent
                if (node->link && node->link->tag == FALSE && node->link->data == 'x') {
                    std::cout << "x";
                    if (node->link->link && node->link->link->tag == FALSE) {
                        if (node->link->link->data != '1') {
                            std::cout << "^" << node->link->link->data;
                        }
                    }
                    node = node->link->link ? node->link->link : node->link;
                }
            }
            isFirstTerm = false;
            node = node->link;
        }
        std::cout << ")";
    }

    // Helper function to print the entire polynomial
    void printPolynomial(const GenListNode* node) const {
        bool isFirstTerm = true;
        while (node) {
            if (node->tag == FALSE && node->data == 'y') {
                if (!isFirstTerm) {
                    std::cout << " + ";
                }
                if (node->link) {
                    if (node->link->tag == FALSE) {
                        // Handle y^6 case
                        printSublist(node->link->link->dlink);
                        std::cout << "y^" << node->link->data;
                    } else if (node->link->tag == TRUE) {
                        // Handle y case (no exponent)
                        printSublist(node->link->dlink);
                        std::cout << "y";
                    }
                }
                isFirstTerm = false;
            }
            node = node->link;
        }
    }
};
#endif // GENLISTNODE_H

