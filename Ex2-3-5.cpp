/**
 * Solutions to Chapter 2, Section 3, Exercise 5, 6, 7 of Horowitz's
 * Data Structures in C++.
 */

/**************************main.cpp******************************/
#include <iostream>
#include "Polynomial.h"

int main() {
    Polynomial p1;
    Polynomial p2;

    // Prompt user to enter polynomial p1
    std::cout << "Enter polynomial p1:" << std::endl;
    std::cin >> p1;

    // Output the polynomial p1
    std::cout << "Entered polynomial p1: " << p1 << std::endl;

    // Prompt user to enter polynomial p2
    std::cout << "Enter polynomial p2:" << std::endl;
    std::cin >> p2;

    // Output the polynomial p2
    std::cout << "Entered polynomial p2: " << p2 << std::endl;

    // Evaluate polynomials p1 and p2 at 2.0
    std::cout << "The value of polynomial p1 is: " 
              << p1.Evaluate(2.0) << std::endl;
    std::cout << "The value of polynomial p2 is: " 
              << p2.Evaluate(2.0) << std::endl;

    // Add p1 and p2
    Polynomial p3 = p1.Add(p2);
    std::cout << "p1 + p2 = " << p3 << std::endl;
    
    Polynomial p4 = p1.Multiply(p2);
    std::cout << "p1 * p2 = " << p4 << std::endl;

    return 0;
}




/************************Polynomial.h****************************/
#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include <iostream>
#include <algorithm>  // For std::copy

class Polynomial;  // Forward declaration

class Term {
    friend class Polynomial;  // Allow Polynomial to access private members
    friend std::ostream& operator<<(std::ostream& os, const Polynomial& c);  // For output
    friend std::istream& operator>>(std::istream& is, Polynomial& c);  // For input

private:
    float coef;  // Coefficient of the term
    int expon;     // Exponent of the term

public:
    int getExp() const { return expon; }  // Getter for exponent
    float getCoef() const { return coef; }  // Getter for coefficient
};

class Polynomial {
private:
    Term* termArray;  // Array of nonzero terms
    int capacity;     // Capacity of the term array
    int terms;        // Number of nonzero terms

    void NewTerm(float theCoeff, int theExp);  // Add a new term to the polynomial

public:
    Polynomial();  // Constructor

    Polynomial Add(const Polynomial& b) const;  // Add two polynomials
    
    Polynomial Multiply(const Polynomial& b) const; // Multiply two polynomials

    float Evaluate(float x) const;

    // Input and output operators
    friend std::ostream& operator<<(std::ostream& os, const Polynomial& c);
    friend std::istream& operator>>(std::istream& is, Polynomial& c);

    ~Polynomial();  // Destructor to release allocated memory
};

#endif  // POLYNOMIAL_H




/***************************Polynomial.cpp*******************************/
#include "Polynomial.h"

Polynomial::Polynomial() : capacity(1), terms(0) {
    termArray = new Term[capacity];
}

Polynomial::~Polynomial() {
    delete[] termArray;
}

void Polynomial::NewTerm(float theCoeff, int theExp) {
    if (terms == capacity) {
        capacity *= 2;
        Term* temp = new Term[capacity];
        std::copy(termArray, termArray + terms, temp);
        delete[] termArray;
        termArray = temp;
    }
    termArray[terms].coef = theCoeff;
    termArray[terms++].expon = theExp;
}

Polynomial Polynomial::Add(const Polynomial& b) const {
    Polynomial c;
    int aPos = 0, bPos = 0;

    while (aPos < terms && bPos < b.terms) {
        if (termArray[aPos].expon == b.termArray[bPos].expon) {
            float t = termArray[aPos].coef + b.termArray[bPos].coef;
            if (t) c.NewTerm(t, termArray[aPos].expon);
            aPos++;
            bPos++;
        } else if (termArray[aPos].expon < b.termArray[bPos].expon) {
            c.NewTerm(b.termArray[bPos].coef, b.termArray[bPos].expon);
            bPos++;
        } else {
            c.NewTerm(termArray[aPos].coef, termArray[aPos].expon);
            aPos++;
        }
    }

    for (; aPos < terms; aPos++) {
        c.NewTerm(termArray[aPos].coef, termArray[aPos].expon);
    }

    for (; bPos < b.terms; bPos++) {
        c.NewTerm(b.termArray[bPos].coef, b.termArray[bPos].expon);
    }

    return c;
}

Polynomial Polynomial::Multiply(const Polynomial& b) const {
    Polynomial c;

    for (int i = 0; i < terms; i++) {
        for (int j = 0; j < b.terms; j++) {
            float newCoef = termArray[i].coef * b.termArray[j].coef;
            int newExp = termArray[i].expon + b.termArray[j].expon;

            // Add the new term to the resulting polynomial
            c.NewTerm(newCoef, newExp);
        }
    }

    return c;
}

float Polynomial::Evaluate(float x) const {
    float result = 0;

    for (int i = 0; i < terms; i++) {
        float temp = 1;  // Initialize temp to 1 for multiplication

        for (int j = 0; j < termArray[i].expon; j++) {
            temp *= x;  // Compute x^exp
        }

        result += (temp * termArray[i].coef);  // Add term's contribution to result
    }

    return result;
}


std::ostream& operator<<(std::ostream& os, const Polynomial& c) {
    if (c.terms == 0) {
        os << "0";
    } else {
        for (int i = 0; i < c.terms; i++) {
            if (i > 0) {
                os << " + ";
            }
            os << c.termArray[i].coef << "x^(" << c.termArray[i].expon << ")";
        }
    }

    return os;
}

std::istream& operator>>(std::istream& is, Polynomial& c) {
    c.terms = 0;
    c.capacity = 1;
    delete[] c.termArray;
    c.termArray = new Term[c.capacity];

    int newTerms;
    std::cout << "Enter the number of terms: ";
    is >> newTerms;

    float coef;
    int expon;

    std::cout << "Enter coefficient and exponent pairs (e.g., 3 4 for 3x^4):" << std::endl;
    for (int i = 0; i < newTerms; i++) {
        std::cout << "Coefficient, Exponent: ";
        is >> coef >> expon;
        c.NewTerm(coef, expon);
    }

    return is;
}


