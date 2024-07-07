/**
 * Solutions to Chapter 2, Section 3, Exercise 10 of Horowitz's
 * Data Structures in C++.
 */
 
/********************************main.cpp*********************************/
#include <iostream>
#include "Polynomial.h"

int main() {
    Polynomial p1, p2;

    std::cout << "Enter polynomial p1:" << std::endl;
    std::cin >> p1;
    std::cout << "Entered polynomial p1: " << p1 << std::endl;

    std::cout << "Enter polynomial p2:" << std::endl;
    std::cin >> p2;
    std::cout << "Entered polynomial p2: " << p2 << std::endl;

    std::cout << "The value of polynomial p1 at x = 2.0 is: " 
              << p1.Evaluate(2.0) << std::endl;
    std::cout << "The value of polynomial p2 at x = 2.0 is: " 
              << p2.Evaluate(2.0) << std::endl;

    Polynomial p3 = p1.Add(p2);
    std::cout << "p1 + p2 = " << p3 << std::endl;
    
    Polynomial p4 = p1.Multiply(p2);
    std::cout << "p1 * p2 = " << p4 << std::endl;

    std::cout << "The value of the sum polynomial p3 at x = 2.0 is: " 
              << p3.Evaluate(2.0) << std::endl;
    std::cout << "The value of the product polynomial p4 at x = 2.0 is: " 
              << p4.Evaluate(2.0) << std::endl;

    return 0;
}



/************************Polynomial.h****************************/
#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include <iostream>

class Polynomial;  // Forward declaration

class Term {
    friend class Polynomial;  // Allow Polynomial to access private members

private:
    int expon;
    float coef;

public:
    Term(float c = 0, int e = 0) : coef(c), expon(e) {}
    int getExpon() const { return expon; }
    float getCoef() const { return coef; }
    void setExpon(int e) { expon = e; }
    void setCoef(float c) { coef = c; }
};

class Polynomial {
private:
    Term *termArray;
    int capacity;
    int terms;

    void NewTerm(float theCoeff, int theExp);

public:
    Polynomial();
    Polynomial(const Polynomial& p);
    Polynomial& operator=(const Polynomial& p);
    ~Polynomial();

    Polynomial Add(const Polynomial& b) const;
    Polynomial Multiply(const Polynomial& b) const;
    float Evaluate(float x) const;

    // Accessor functions
    int getTerms() const { return terms; }
    int getCapacity() const { return capacity; }
    const Term& getTerm(int index) const;
    void setTerm(int index, float coef, int expon);

    friend std::ostream& operator<<(std::ostream& os, const Polynomial& c);
    friend std::istream& operator>>(std::istream& is, Polynomial& c);
};

#endif //POLYNOMIAL_H



/***************************Polynomial.cpp*******************************/
#include "Polynomial.h"
#include <cmath>
#include <stdexcept>

Polynomial::Polynomial() : termArray(nullptr), capacity(0), terms(0) {}

Polynomial::Polynomial(const Polynomial& p) : capacity(p.terms), terms(p.terms) {
    termArray = new Term[capacity];
    for (int i = 0; i < terms; i++) {
        termArray[i] = p.termArray[i];
    }
}

Polynomial& Polynomial::operator=(const Polynomial& p) {
    if (this != &p) {
        delete[] termArray;
        capacity = terms = p.terms;
        termArray = new Term[capacity];
        for (int i = 0; i < terms; i++) {
            termArray[i] = p.termArray[i];
        }
    }
    return *this;
}

Polynomial::~Polynomial() {
    delete[] termArray;
}

void Polynomial::NewTerm(float theCoeff, int theExp) {
    if (terms == capacity) {
        capacity = (capacity == 0) ? 1 : capacity * 2;
        Term* newArray = new Term[capacity];
        for (int i = 0; i < terms; i++) {
            newArray[i] = termArray[i];
        }
        delete[] termArray;
        termArray = newArray;
    }
    termArray[terms].setCoef(theCoeff);
    termArray[terms++].setExpon(theExp);
}

Polynomial Polynomial::Add(const Polynomial& b) const {
    Polynomial c;
    int aPos = 0, bPos = 0;

    while (aPos < terms && bPos < b.terms) {
        if (termArray[aPos].getExpon() == b.termArray[bPos].getExpon()) {
            float t = termArray[aPos].getCoef() + b.termArray[bPos].getCoef();
            if (t != 0) c.NewTerm(t, termArray[aPos].getExpon());
            aPos++; bPos++;
        }
        else if (termArray[aPos].getExpon() < b.termArray[bPos].getExpon()) {
            c.NewTerm(b.termArray[bPos].getCoef(), b.termArray[bPos].getExpon());
            bPos++;
        }
        else {
            c.NewTerm(termArray[aPos].getCoef(), termArray[aPos].getExpon());
            aPos++;
        }
    }

    for (; aPos < terms; aPos++)
        c.NewTerm(termArray[aPos].getCoef(), termArray[aPos].getExpon());
    for (; bPos < b.terms; bPos++)
        c.NewTerm(b.termArray[bPos].getCoef(), b.termArray[bPos].getExpon());

    return c;
}

Polynomial Polynomial::Multiply(const Polynomial& b) const {
    Polynomial c;

    for (int i = 0; i < terms; i++) {
        for (int j = 0; j < b.terms; j++) {
            float newCoef = termArray[i].getCoef() * b.termArray[j].getCoef();
            int newExp = termArray[i].getExpon() + b.termArray[j].getExpon();
            
            // Find if this exponent already exists in c
            int k;
            for (k = 0; k < c.terms; k++) {
                if (c.termArray[k].getExpon() == newExp) {
                    c.termArray[k].setCoef(c.termArray[k].getCoef() + newCoef);
                    break;
                }
            }
            
            // If not found, add a new term
            if (k == c.terms && newCoef != 0) {
                c.NewTerm(newCoef, newExp);
            }
        }
    }

    return c;
}

float Polynomial::Evaluate(float x) const {
    float result = 0;
    for (int i = 0; i < terms; i++) {
        result += termArray[i].getCoef() * std::pow(x, termArray[i].getExpon());
    }
    return result;
}

const Term& Polynomial::getTerm(int index) const {
    if (index < 0 || index >= terms) {
        throw std::out_of_range("Term index out of range");
    }
    return termArray[index];
}

void Polynomial::setTerm(int index, float coef, int expon) {
    if (index < 0 || index >= terms) {
        throw std::out_of_range("Term index out of range");
    }
    termArray[index].setCoef(coef);
    termArray[index].setExpon(expon);
}

std::ostream& operator<<(std::ostream& os, const Polynomial& c) {
    if (c.terms == 0) {
        os << "0";
    } else {
        for (int i = 0; i < c.terms; i++) {
            if (i > 0) os << " + ";
            os << c.termArray[i].getCoef() << "x^(" << c.termArray[i].getExpon() << ")";
        }
    }
    return os;
}

std::istream& operator>>(std::istream& is, Polynomial& c) {
    int numTerms;
    std::cout << "Enter the number of terms: ";
    is >> numTerms;

    c = Polynomial();  // Reset the polynomial

    std::cout << "Enter each term as coefficient exponent:" << std::endl;
    for (int i = 0; i < numTerms; i++) {
        float coef;
        int exp;
        is >> coef >> exp;
        c.NewTerm(coef, exp);
    }

    return is;
}
