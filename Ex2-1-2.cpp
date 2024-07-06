/**
 * Solutions to Chapter 2, Section 1, Exercise 2 of Horowitz's
 * Data Structures in C++.
 */

/****************************main.cpp********************************/
#include <iostream>
#include "Complex.h"

using namespace std;

int main() {
    
    cout << "Complex number #1 set automatically to 0 + 0i." << endl;
    cout << "Complex number #2 set automatically to 3 + 2i." << endl;

    Complex c1;
    Complex c2(3, 2);
    Complex c3, c4;

    // Read two complex numbers
    cout << "Enter the first complex number (real and imaginary parts)"
         << "\nseparated by a space: ";
    cin >> c3;
    cout << "Enter the second complex number (real and imaginary parts)"
         << "\nseparated by a space: ";
    cin >> c4;

    // Add the complex numbers
    Complex sum1 = c1 + c2;
    cout << "Sum of complex numbers #1 and #2: " << sum1 << endl;
    
    Complex sum2 = c1 + c3;
    cout << "Sum of complex numbers #1 and #3: " << sum2 << endl;

    Complex sum3 = c1 + c4;
    cout << "Sum of complex numbers #1 and #4: " << sum3 << endl;
    
    Complex sum4 = c2 + c3;
    cout << "Sum of complex numbers #2 and #3: " << sum4 << endl;

    Complex sum5 = c2 + c4;
    cout << "Sum of complex numbers #2 and #4: " << sum5 << endl;
    
    Complex sum6 = c3 + c4;
    cout << "Sum of complex numbers #3 and #4: " << sum6 << endl;


    // Multiply the complex numbers
    Complex product1 = c1 * c2;
    cout << "Product of complex numbers #1 and #2: " << product1 << endl;
    
    Complex product2 = c1 * c3;
    cout << "Product of complex numbers #1 and #3: " << product2 << endl;
    
    Complex product3 = c1 * c4;
    cout << "Product of complex numbers #1 and #4: " << product3 << endl;
    
    Complex product4 = c2 * c3;
    cout << "Product of complex numbers #2 and #3: " << product4 << endl;
    
    Complex product5 = c2 * c4;
    cout << "Product of complex numbers #2 and #4: " << product5 << endl;
    
    Complex product6 = c3 * c4;
    cout << "Product of complex numbers #3 and #4: " << product6 << endl;
    
    return 0;
}


/***************************Complex.h*******************************/
#ifndef COMPLEX_H
#define COMPLEX_H

#include <iostream>

class Complex {
public:
    // Default constructor and parameterized constructor
    // Complex();
    Complex(double real = 0.0, double imag = 0.0);

    // Overloaded operators
    Complex operator+(const Complex& other) const;
    Complex operator*(const Complex& other) const;

    // Friend functions for overloaded stream operators
    friend std::ostream& operator<<(std::ostream& os, const Complex& c);
    friend std::istream& operator>>(std::istream& is, Complex& c);

private:
    double real;
    double imag;
};

#endif


/**************************Complex.cpp****************************/
#include "Complex.h"

/**Default Constructor
Complex::Complex() {
    real = 0.0;
    imag = 0.0;
}
*/

// Parameterized Constructor
Complex::Complex(double real, double imag) {
    this->real = real;
    this->imag = imag;
}

// Overloaded operator+ to add two complex numbers
Complex Complex::operator+(const Complex& other) const {
    return Complex(real + other.real, imag + other.imag);
}

// Overloaded operator* to multiply two complex numbers
Complex Complex::operator*(const Complex& other) const {
    double newReal = real * other.real - imag * other.imag;
    double newImag = real * other.imag + imag * other.real;
    return Complex(newReal, newImag);
}

// Overloaded operator<< to print a complex number
std::ostream& operator<<(std::ostream& os, const Complex& c) {
    os << c.real << " + " << c.imag << "i";
    return os;
}

// Overloaded operator>> to read a complex number
std::istream& operator>>(std::istream& is, Complex& c) {
    is >> c.real >> c.imag;
    return is;
}