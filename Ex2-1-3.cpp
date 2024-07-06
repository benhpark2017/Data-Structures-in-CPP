/**
 * Solutions to Chapter 2, Section 1, Exercise 3 of Horowitz's
 * Data Structures in C++.
 */

/****************************main.cpp********************************/
#include "Quadratic.h"
#include <iostream>
#include <cmath>  // For sqrt function

using namespace std;

int main() {
    // Create two quadratic polynomials
    Quadratic q1, q2(1, -3, 2);  // q1 is 0x^2 + 0x + 0, q2 is x^2 - 3x + 2

    // Read a polynomial from the user
    cout << "Enter coefficients for a quadratic polynomial (a b c): ";
    cin >> q1;

    // Output the polynomials
    cout << "First polynomial: " << q1 << endl;
    cout << "Second polynomial: " << q2 << endl;

    // Add the polynomials
    Quadratic q3 = q1 + q2;
    cout << "Sum of polynomials: " << q3 << endl;

    // Evaluate the polynomial q1 at x = 2
    double x = 2;
    double result = q1.evalPolynomial(x);
    cout << "Value of the first polynomial at x = " << x << " is: " << result << endl;

    // Solve the quadratic equation for q2
    Complex x1, x2;
    q2.solve(x1, x2);
    cout << "Roots of the equation " << q2 << " are: " << x1 << " and " << x2 << endl;

    return 0;
}


/***************************Quadratic.h******************************/
#ifndef QUADRATIC_H
#define QUADRATIC_H

#include <iostream>
#include "Complex.h"

class Quadratic {
public:
    Quadratic();  // Default constructor
    Quadratic(double a, double b, double c);  // Parameterized constructor

    Quadratic operator+(const Quadratic& other) const;
    friend std::ostream& operator<<(std::ostream& os, const Quadratic& q);
    friend std::istream& operator>>(std::istream& is, Quadratic& q);

    double evalPolynomial(double x) const;
    void solve(Complex& x1, Complex& x2) const;  // Note: Updated to use Complex for solutions

private:
    double a, b, c;
};

#endif


/**************************Quadratic.cpp*****************************/
#include "Quadratic.h"
#include <cmath>

// Default constructor
Quadratic::Quadratic() : a(0), b(0), c(0) {}

// Parameterized constructor
Quadratic::Quadratic(double a, double b, double c) : a(a), b(b), c(c) {}

// Overloaded operator+ to add two polynomials
Quadratic Quadratic::operator+(const Quadratic& other) const {
    return Quadratic(a + other.a, b + other.b, c + other.c);
}

// Overloaded operator<< to print a polynomial
std::ostream& operator<<(std::ostream& os, const Quadratic& q) {
    if (q.a != 0) os << q.a << "x^2";
    if (q.b > 0) os << " + " << q.b << "x";
    else if (q.b < 0) os << " - " << -q.b << "x";
    if (q.c > 0) os << " + " << q.c;
    else if (q.c < 0) os << " - " << -q.c;
    return os;
}

// Overloaded operator>> to read a polynomial
std::istream& operator>>(std::istream& is, Quadratic& q) {
    is >> q.a >> q.b >> q.c;
    return is;
}

// Evaluate polynomial for a given x
double Quadratic::evalPolynomial(double x) const {
    return a * x * x + b * x + c;
}

// Solve the quadratic equation a*x^2 + b*x + c = 0
void Quadratic::solve(Complex& x1, Complex& x2) const {
    double discriminant = b * b - 4 * a * c;

    if (discriminant >= 0) {
        x1 = Complex((-b + sqrt(discriminant)) / (2 * a), 0);
        x2 = Complex((-b - sqrt(discriminant)) / (2 * a), 0);
    } else {
        x1 = Complex(-b / (2 * a), sqrt(-discriminant) / (2 * a));
        x2 = Complex(-b / (2 * a), -sqrt(-discriminant) / (2 * a));
    }
}


/***************************Complex.h********************************/
#ifndef COMPLEX_H
#define COMPLEX_H

#include <iostream>

class Complex {
public:
    Complex(double real = 0.0, double imag = 0.0);

    Complex operator+(const Complex& other) const;
    Complex operator*(const Complex& other) const;
    friend std::ostream& operator<<(std::ostream& os, const Complex& c);
    friend std::istream& operator>>(std::istream& is, Complex& c);

    double real() const { return realPart; }
    double imag() const { return imagPart; }

private:
    double realPart;
    double imagPart;
};

#endif


/**************************Complex.cpp*******************************/
#include "Complex.h"

// Constructor
Complex::Complex(double real, double imag) : realPart(real), imagPart(imag) {}

// Overloaded operator+ for addition
Complex Complex::operator+(const Complex& other) const {
    return Complex(realPart + other.realPart, imagPart + other.imagPart);
}

// Overloaded operator* for multiplication
Complex Complex::operator*(const Complex& other) const {
    return Complex(realPart * other.realPart - imagPart * other.imagPart,
                   realPart * other.imagPart + imagPart * other.realPart);
}

// Overloaded operator<< for output
std::ostream& operator<<(std::ostream& os, const Complex& c) {
    os << c.realPart;
    if (c.imagPart >= 0) os << " + " << c.imagPart << "i";
    else os << " - " << -c.imagPart << "i";
    return os;
}

// Overloaded operator>> for input
std::istream& operator>>(std::istream& is, Complex& c) {
    is >> c.realPart >> c.imagPart;
    return is;
}
