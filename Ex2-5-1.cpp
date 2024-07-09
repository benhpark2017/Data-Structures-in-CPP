/**
 * Solutions to Chapter 2, Section 5, Exercise 1 of Horowitz's
 * Data Structures in C++.
 */

/**************************************main.cpp*******************************************/
#include <iostream>
#include <fstream>
#include "MultiDimArray.h"

int main() {
    try {
        // Define the bounds for the multi-dimensional array
        int lowerBounds[2] = {1, 1};
        int upperBounds[2] = {3, 3};

        // Create the multi-dimensional array
        MultiDimArray array(2, lowerBounds, upperBounds);

        // Define indices and assign values
        int indices1[2] = {1, 1};
        int indices2[2] = {2, 2};

        array.at(indices1) = 5.0;
        array.at(indices2) = 10.0;

        // Display the assigned values
        std::cout << "Element at (1,1): " << array.at(indices1) << std::endl;
        std::cout << "Element at (2,2): " << array.at(indices2) << std::endl;
        std::cout << "\n";
        
        // Write the array to a file
        array.writeToFile("array.dat");

        // Read and display the contents of the file
        std::cout << "Contents of array.dat:" << std::endl;
        std::ifstream inFile("array.dat");
        if (!inFile) {
            throw std::ios_base::failure("Failed to open file for reading");
        }
        std::string line;
        while (std::getline(inFile, line)) {
            std::cout << line << std::endl;
        }

        // Create a new array and read data from the file
        MultiDimArray arrayFromFile(2, lowerBounds, upperBounds);
        arrayFromFile.readFromFile("array.dat");

        // Display the values read from the file
        std::cout << "Element at (1,1) from file: " << arrayFromFile.at(indices1) << std::endl;
        std::cout << "Element at (2,2) from file: " << arrayFromFile.at(indices2) << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}



/***********************************MultiDimArray.h****************************************/
#ifndef MULTIDIMARRAY_H
#define MULTIDIMARRAY_H

#include <stdexcept>
#include <fstream>

class MultiDimArray {
private:
    double* elements;         // Array of elements
    int* lowerBounds;         // Array that stores the lower bound of each dimension
    int* upperBounds;         // Array that stores the upper bound of each dimension
    int numDimensions;        // Number of dimensions
    int totalSize;            // Total size of the array

    int calculateIndex(const int* indices) const;
    void initialize(int dimensions, const int* lower, const int* upper);

public:
    MultiDimArray(int dimensions, const int* lower, const int* upper);
    MultiDimArray(const MultiDimArray& other);
    MultiDimArray& operator=(const MultiDimArray& other);
    ~MultiDimArray();

    double& at(const int* indices);
    const double& at(const int* indices) const;

    void resize(const int* lower, const int* upper);
    int size() const;

    void writeToFile(const char* filename) const;
    void readFromFile(const char* filename);
};

#endif // MULTIDIMARRAY_H



/***********************************MultiDimArray.cpp**************************************/
#include "MultiDimArray.h"
#include <stdexcept>
#include <fstream>

int MultiDimArray::calculateIndex(const int* indices) const {
    int index = 0;
    int product = 1;
    for (int i = 0; i < numDimensions; ++i) {
        if (indices[i] < lowerBounds[i] || indices[i] > upperBounds[i]) {
            throw std::out_of_range("Index out of bounds");
        }
        index += (indices[i] - lowerBounds[i]) * product;
        product *= (upperBounds[i] - lowerBounds[i] + 1);
    }
    return index;
}

void MultiDimArray::initialize(int dimensions, const int* lower, const int* upper) {
    if (dimensions <= 0) {
        throw std::invalid_argument("Number of dimensions must be greater than 0");
    }

    numDimensions = dimensions;
    lowerBounds = new int[numDimensions];
    upperBounds = new int[numDimensions];
    totalSize = 1;

    for (int i = 0; i < numDimensions; ++i) {
        if (lower[i] > upper[i]) {
            throw std::invalid_argument("Lower bound must be less than or equal to upper bound");
        }
        lowerBounds[i] = lower[i];
        upperBounds[i] = upper[i];
        totalSize *= (upperBounds[i] - lowerBounds[i] + 1);
    }

    elements = new double[totalSize]();
}

MultiDimArray::MultiDimArray(int dimensions, const int* lower, const int* upper) {
    initialize(dimensions, lower, upper);
}

MultiDimArray::MultiDimArray(const MultiDimArray& other) {
    initialize(other.numDimensions, other.lowerBounds, other.upperBounds);
    for (int i = 0; i < totalSize; ++i) {
        elements[i] = other.elements[i];
    }
}

MultiDimArray& MultiDimArray::operator=(const MultiDimArray& other) {
    if (this == &other) {
        return *this;
    }

    delete[] elements;
    delete[] lowerBounds;
    delete[] upperBounds;

    initialize(other.numDimensions, other.lowerBounds, other.upperBounds);
    for (int i = 0; i < totalSize; ++i) {
        elements[i] = other.elements[i];
    }

    return *this;
}

MultiDimArray::~MultiDimArray() {
    delete[] elements;
    delete[] lowerBounds;
    delete[] upperBounds;
}

double& MultiDimArray::at(const int* indices) {
    return elements[calculateIndex(indices)];
}

const double& MultiDimArray::at(const int* indices) const {
    return elements[calculateIndex(indices)];
}

void MultiDimArray::resize(const int* lower, const int* upper) {
    delete[] elements;
    delete[] lowerBounds;
    delete[] upperBounds;

    initialize(numDimensions, lower, upper);
}

int MultiDimArray::size() const {
    return totalSize;
}

void MultiDimArray::writeToFile(const char* filename) const {
    std::ofstream ofs(filename);
    if (!ofs) {
        throw std::ios_base::failure("Failed to open file for writing");
    }
    
    // Write number of dimensions
    ofs << numDimensions << std::endl;
    
    // Write lower and upper bounds
    for (int i = 0; i < numDimensions; ++i) {
        ofs << lowerBounds[i] << " " << upperBounds[i] << std::endl;
    }
    
    // Write array elements with row separation
    int rowSize = upperBounds[1] - lowerBounds[1] + 1;
    for (int i = 0; i < totalSize; ++i) {
        ofs << elements[i] << " ";
        if ((i + 1) % rowSize == 0) {
            ofs << std::endl; // New line after each row
        }
    }
    
    ofs << std::endl;
}

void MultiDimArray::readFromFile(const char* filename) {
    std::ifstream ifs(filename);
    if (!ifs) {
        throw std::ios_base::failure("Failed to open file for reading");
    }

    int dimensions;
    ifs >> dimensions;
    
    int* lower = new int[dimensions];
    int* upper = new int[dimensions];
    
    for (int i = 0; i < dimensions; ++i) {
        ifs >> lower[i] >> upper[i];
    }
    
    resize(lower, upper);

    delete[] lower;
    delete[] upper;

    for (int i = 0; i < totalSize; ++i) {
        ifs >> elements[i];
    }
}