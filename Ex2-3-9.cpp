/**
 * Solutions to Chapter 2, Section 3, Exercise 9 of Horowitz's
 * Data Structures in C++.
 */
 
/********************************main.cpp*********************************/
#include <iostream>
#include "Polynomial.h"

int main() {
    Polynomial p1(0);  // Create polynomial p1 with index 0
    Polynomial p2(1);  // Create polynomial p2 with index 1

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
    std::cout << "The value of polynomial p1 at x = 2.0 is: " 
              << p1.Evaluate(2.0) << std::endl;
    std::cout << "The value of polynomial p2 at x = 2.0 is: " 
              << p2.Evaluate(2.0) << std::endl;

    // Add p1 and p2
    Polynomial p3 = p1.Add(p2);
    std::cout << "p1 + p2 = " << p3 << std::endl;
    
    // Multiply p1 and p2
    Polynomial p4 = p1.Multiply(p2);
    std::cout << "p1 * p2 = " << p4 << std::endl;

    // Evaluate polynomials p3 and p4 at 2.0
    std::cout << "The value of the sum polynomial p3 at x = 2.0 is: " 
              << p3.Evaluate(2.0) << std::endl;
    std::cout << "The value of the product polynomial p4 at x = 2.0 is: " 
              << p4.Evaluate(2.0) << std::endl;

    return 0;
}


/******************************Polynomial.h********************************/
#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include <iostream>

const int MAX_POLYS = 100;  // Maximum number of polynomials
const int MAX_TERMS = 100;  // Maximum number of terms per polynomial

class Polynomial;  // Forward declaration

class Term {
    friend class Polynomial;  // Allow Polynomial to access private members
    friend std::ostream& operator<<(std::ostream& os, const Polynomial& c);  // For output
    friend std::istream& operator>>(std::istream& is, Polynomial& c);  // For input

private:
    float coef;  // Coefficient of the term
    int expon;   // Exponent of the term

public:
    int getExp() const { return expon; }  // Getter for exponent
    float getCoef() const { return coef; }  // Getter for coefficient
};

class Polynomial {
private:
    static Term termArray[MAX_POLYS][MAX_TERMS + 1];  // Array of polynomials
    int polyIndex;  // Index of this polynomial in termArray
    int terms;      // Number of nonzero terms

    void NewTerm(float theCoeff, int theExp);  // Add a new term to the polynomial

public:
    Polynomial(int index);  // Constructor

    Polynomial Add(const Polynomial& b) const;  // Add two polynomials
    
    Polynomial Multiply(const Polynomial& b) const; // Multiply two polynomials

    float Evaluate(float x) const;

    // Input and output operators
    friend std::ostream& operator<<(std::ostream& os, const Polynomial& c);
    friend std::istream& operator>>(std::istream& is, Polynomial& c);
};

#endif //POLYNOMIAL_H



/*****************************Polynomial.cpp*******************************/
#include "Polynomial.h"
#include <cmath>

Term Polynomial::termArray[MAX_POLYS][MAX_TERMS + 1];

Polynomial::Polynomial(int index) : polyIndex(index), terms(0) {
    if (index < 0 || index >= MAX_POLYS) {
        throw std::out_of_range("Polynomial index out of range");
    }
}

void Polynomial::NewTerm(float theCoeff, int theExp) {
    if (terms >= MAX_TERMS) {
        throw std::overflow_error("Too many terms in polynomial");
    }
    termArray[polyIndex][terms].coef = theCoeff;
    termArray[polyIndex][terms++].expon = theExp;
}

Polynomial Polynomial::Add(const Polynomial& b) const {
    Polynomial c(polyIndex + b.polyIndex + 1);  // New polynomial index
    int aPos = 0, bPos = 0;

    while (aPos < terms && bPos < b.terms) {
        if (termArray[polyIndex][aPos].expon == termArray[b.polyIndex][bPos].expon) {
            float t = termArray[polyIndex][aPos].coef + termArray[b.polyIndex][bPos].coef;
            if (t) c.NewTerm(t, termArray[polyIndex][aPos].expon);
            aPos++;
            bPos++;
        } else if (termArray[polyIndex][aPos].expon < termArray[b.polyIndex][bPos].expon) {
            c.NewTerm(termArray[b.polyIndex][bPos].coef, termArray[b.polyIndex][bPos].expon);
            bPos++;
        } else {
            c.NewTerm(termArray[polyIndex][aPos].coef, termArray[polyIndex][aPos].expon);
            aPos++;
        }
    }

    for (; aPos < terms; aPos++) {
        c.NewTerm(termArray[polyIndex][aPos].coef, termArray[polyIndex][aPos].expon);
    }

    for (; bPos < b.terms; bPos++) {
        c.NewTerm(termArray[b.polyIndex][bPos].coef, termArray[b.polyIndex][bPos].expon);
    }

    return c;
}

Polynomial Polynomial::Multiply(const Polynomial& b) const {
    Polynomial c(polyIndex + b.polyIndex + 2);  // New polynomial index

    // Multiply each term of the first polynomial with each term of the second polynomial
    for (int i = 0; i < terms; i++) {
        for (int j = 0; j < b.terms; j++) {
            float newCoef = termArray[polyIndex][i].coef * termArray[b.polyIndex][j].coef;
            int newExp = termArray[polyIndex][i].expon + termArray[b.polyIndex][j].expon;
            c.NewTerm(newCoef, newExp);
        }
    }

    // Combine terms with the same exponent
    Polynomial result(c.polyIndex + 1);
    for (int i = 0; i < c.terms; i++) {
        if (termArray[c.polyIndex][i].coef != 0) {
            float coefSum = termArray[c.polyIndex][i].coef;
            for (int j = i + 1; j < c.terms; j++) {
                if (termArray[c.polyIndex][i].expon == termArray[c.polyIndex][j].expon && termArray[c.polyIndex][j].coef != 0) {
                    coefSum += termArray[c.polyIndex][j].coef;
                    termArray[c.polyIndex][j].coef = 0;  // Mark this term as used
                }
            }
            if (coefSum != 0) {
                result.NewTerm(coefSum, termArray[c.polyIndex][i].expon);
            }
        }
    }
    return result;
}

float Polynomial::Evaluate(float x) const {
    float result = 0;

    for (int i = 0; i < terms; i++) {
        result += termArray[polyIndex][i].coef * std::pow(x, termArray[polyIndex][i].expon);
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
            os << c.termArray[c.polyIndex][i].coef << "x^(" << c.termArray[c.polyIndex][i].expon << ")";
        }
    }

    return os;
}

std::istream& operator>>(std::istream& is, Polynomial& c) {
    c.terms = 0;

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
