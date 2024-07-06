/**
 * Solutions to Chapter 2, Section 2, Exercise 1 of Horowitz's
 * Data Structures in C++.
 */

/****************************main.cpp********************************/
#include <iostream>
#include "CppArray.h"

int main() {
    CppArray a(5, 1.0f); // Create an array of size 5, initialized with 1.0
    std::cout << "Array a: " << a << std::endl;

    CppArray b = a; // Use copy constructor
    std::cout << "Array b after copying from a: " << b << std::endl;

    CppArray c;
    c = a; // Use assignment operator
    std::cout << "Array c after assignment from a: " << c << std::endl;

    // Modify array through subscript operator
    a[2] = 3.14f;
    std::cout << "Array a after modification: " << a << std::endl;

    // Get size of the array
    std::cout << "Size of array a: " << a.getSize() << std::endl;

    // Input new values into array b
    std::cout << "Enter 5 new values for array b: ";
    std::cin >> b;
    std::cout << "Array b after input: " << b << std::endl;

    return 0;
}



/***************************CppArray.h******************************/
#ifndef CPPARRAY_H
#define CPPARRAY_H

#include <iostream>

class CppArray {
  private:
    int size;
    float* data;
    static const int defaultSize;
    static const float defaultValue;

  public:
    // Constructor
    CppArray(int size = defaultSize, float initValue = defaultValue);

    // Copy constructor
    CppArray(const CppArray& cp2);
    
    // Assignment operator
    CppArray& operator=(const CppArray& cp2);

    // Destructor
    ~CppArray();

    // Subscript operator
    float& operator[](int i);

    // Get size of the array
    int getSize() const;

    // Friend functions for input and output
    friend std::ostream& operator<<(std::ostream& os, const CppArray& arr);
    friend std::istream& operator>>(std::istream& is, CppArray& arr);
};

#endif



/**************************CppArray.cpp*****************************/
#include <stdexcept>
#include "CppArray.h"

// Static member initialization
const int CppArray::defaultSize = 10;
const float CppArray::defaultValue = 0.0f;

// Constructor
CppArray::CppArray(int size, float initValue) : size(size) {
    if (size <= 0) {
        throw std::invalid_argument("Size must be greater than 0");
    }
    data = new float[size];
    for (int i = 0; i < size; ++i) {
        data[i] = initValue;
    }
}

// Copy constructor
CppArray::CppArray(const CppArray& cp2) : size(cp2.size) {
    data = new float[size];
    for (int i = 0; i < size; ++i) {
        data[i] = cp2.data[i];
    }
}

// Assignment operator
CppArray& CppArray::operator=(const CppArray& cp2) {
    if (this == &cp2) {
        return *this;
    }
    delete[] data;
    size = cp2.size;
    data = new float[size];
    for (int i = 0; i < size; ++i) {
        data[i] = cp2.data[i];
    }
    return *this;
}

// Destructor
CppArray::~CppArray() {
    delete[] data;
}

// Subscript operator
float& CppArray::operator[](int i) {
    if (i < 0 || i >= size) {
        throw std::out_of_range("Index out of range");
    }
    return data[i];
}

// Get size of the array
int CppArray::getSize() const {
    return size;
}

// Overloaded output operator
std::ostream& operator<<(std::ostream& os, const CppArray& arr) {
    for (int i = 0; i < arr.size; ++i) {
        os << arr.data[i] << ' ';
    }
    return os;
}

// Overloaded input operator
std::istream& operator>>(std::istream& is, CppArray& arr) {
    for (int i = 0; i < arr.size; ++i) {
        is >> arr.data[i];
    }
    return is;
}
