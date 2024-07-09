/**
 * Solutions to Chapter 2, Section 4, Exercise 7 of Horowitz's
 * Data Structures in C++.
 */
 
/**************************************************main.cpp*************************************************/
#include <iostream>
#include <stdexcept>
#include "SparseMatrix.h"

void testBasicOperations() {
    std::cout << "Testing basic operations:\n";
    SparseMatrix m(3, 3);
    m.set(0, 0, 1);
    m.set(1, 1, 2);
    m.set(2, 2, 3);

    std::cout << "Matrix m:\n" << m << std::endl;

    std::cout << "Element at (1, 1): " << m.get(1, 1) << std::endl;
    std::cout << "Element at (0, 1): " << m.get(0, 1) << std::endl;

    try {
        m.get(3, 3);
    } catch (const std::out_of_range& e) {
        std::cout << "Caught exception: " << e.what() << std::endl;
    }
}

void testAddition() {
    std::cout << "\nTesting addition:\n";
    SparseMatrix a(2, 2), b(2, 2);
    a.set(0, 0, 1);
    a.set(1, 1, 2);
    b.set(0, 1, 3);
    b.set(1, 0, 4);

    std::cout << "Matrix a:\n" << a << std::endl;
    std::cout << "Matrix b:\n" << b << std::endl;

    SparseMatrix c = a.add(b);
    std::cout << "Matrix c (a + b):\n" << c << std::endl;
}

void testInputOutput() {
    std::cout << "\nTesting input/output:\n";
    SparseMatrix m;
    std::cout << "Enter a sparse matrix (format: rows cols non_zero_count):\n";
    std::cin >> m;
    std::cout << "You entered:\n" << m << std::endl;
}

void testMultiplication() {
    std::cout << "\nTesting multiplication:\n";
    SparseMatrix a(2, 3), b(3, 2);
    a.set(0, 0, 1); a.set(0, 2, 2);
    a.set(1, 1, 3);
    b.set(0, 0, 4); b.set(1, 1, 5);
    b.set(2, 0, 6);

    std::cout << "Matrix a:\n" << a << std::endl;
    std::cout << "Matrix b:\n" << b << std::endl;

    SparseMatrix c = a.multiply(b);
    std::cout << "Matrix c (a * b):\n" << c << std::endl;
}

void testTranspose() {
    std::cout << "\nTesting transpose:\n";
    SparseMatrix m(2, 3);
    m.set(0, 0, 1); m.set(0, 2, 2);
    m.set(1, 1, 3);

    std::cout << "Matrix m:\n" << m << std::endl;

    SparseMatrix mt = m.transpose();
    std::cout << "Matrix m^T (transpose of m):\n" << mt << std::endl;
}

void testRandomAccess() {
    std::cout << "\nTesting random access:\n";
    SparseMatrix m(3, 4);
    m.set(0, 1, 5);
    m.set(1, 2, 7);
    m.set(2, 0, 3);
    m.set(2, 3, 1);

    std::cout << "Matrix m:\n" << m << std::endl;

    std::cout << "Random access tests:\n";
    std::cout << "m(0,1) = " << m.randomAccess(0, 1) << std::endl;
    std::cout << "m(1,2) = " << m.randomAccess(1, 2) << std::endl;
    std::cout << "m(2,0) = " << m.randomAccess(2, 0) << std::endl;
    std::cout << "m(2,3) = " << m.randomAccess(2, 3) << std::endl;
    std::cout << "m(1,1) = " << m.randomAccess(1, 1) << std::endl;  // Should be 0

    try {
        m.randomAccess(3, 4);  // This should throw an exception
    } catch (const std::out_of_range& e) {
        std::cout << "Caught exception: " << e.what() << std::endl;
    }
}

int main() {
    testBasicOperations();
    testAddition();
    testInputOutput();
    testMultiplication();
    testTranspose();
    testRandomAccess();
    return 0;
}



/**********************************************SparseMatrix.h**************************************************/
#ifndef SPARSEMATRIX_H
#define SPARSEMATRIX_H

#include <iostream>

class SparseMatrix {
private:
    bool** bits;
    int* values;
    int rows, cols;
    int capacity;
    int nonZeroCount;

    void resize();

public:
    SparseMatrix(int n = 0, int m = 0);
    ~SparseMatrix();
    SparseMatrix(const SparseMatrix& other);
    SparseMatrix& operator=(const SparseMatrix& other);

    void set(int i, int j, int value);
    int get(int i, int j) const;
    SparseMatrix add(const SparseMatrix& other) const;
    SparseMatrix multiply(const SparseMatrix& other) const;
    SparseMatrix transpose() const;
    int randomAccess(int i, int j) const;
    void print() const;

    int getRows() const { return rows; }
    int getCols() const { return cols; }
    int getNonZeroCount() const { return nonZeroCount; }

    friend std::istream& operator>>(std::istream& is, SparseMatrix& matrix);
    friend std::ostream& operator<<(std::ostream& os, const SparseMatrix& matrix);
};

#endif // SPARSEMATRIX_H



/*******************************************SparseMatrix.cpp************************************************/
#include "SparseMatrix.h"
#include <stdexcept>

SparseMatrix::SparseMatrix(int n, int m) : rows(n), cols(m), capacity(10), nonZeroCount(0) {
    bits = new bool*[rows];
    for (int i = 0; i < rows; ++i) {
        bits[i] = new bool[cols];
        for (int j = 0; j < cols; ++j) {
            bits[i][j] = false;
        }
    }
    values = new int[capacity];
}

SparseMatrix::~SparseMatrix() {
    for (int i = 0; i < rows; ++i) {
        delete[] bits[i];
    }
    delete[] bits;
    delete[] values;
}

SparseMatrix::SparseMatrix(const SparseMatrix& other) 
    : rows(other.rows), cols(other.cols), capacity(other.capacity), nonZeroCount(other.nonZeroCount) {
    bits = new bool*[rows];
    for (int i = 0; i < rows; ++i) {
        bits[i] = new bool[cols];
        for (int j = 0; j < cols; ++j) {
            bits[i][j] = other.bits[i][j];
        }
    }
    values = new int[capacity];
    for (int i = 0; i < nonZeroCount; ++i) {
        values[i] = other.values[i];
    }
}

SparseMatrix& SparseMatrix::operator=(const SparseMatrix& other) {
    if (this != &other) {
        for (int i = 0; i < rows; ++i) {
            delete[] bits[i];
        }
        delete[] bits;
        delete[] values;

        rows = other.rows;
        cols = other.cols;
        capacity = other.capacity;
        nonZeroCount = other.nonZeroCount;

        bits = new bool*[rows];
        for (int i = 0; i < rows; ++i) {
            bits[i] = new bool[cols];
            for (int j = 0; j < cols; ++j) {
                bits[i][j] = other.bits[i][j];
            }
        }
        values = new int[capacity];
        for (int i = 0; i < nonZeroCount; ++i) {
            values[i] = other.values[i];
        }
    }
    return *this;
}

void SparseMatrix::resize() {
    capacity *= 2;
    int* newValues = new int[capacity];
    for (int i = 0; i < nonZeroCount; ++i) {
        newValues[i] = values[i];
    }
    delete[] values;
    values = newValues;
}

void SparseMatrix::set(int i, int j, int value) {
    if (i < 0 || i >= rows || j < 0 || j >= cols) {
        throw std::out_of_range("Matrix indices out of range");
    }

    if (value != 0) {
        if (!bits[i][j]) {
            if (nonZeroCount == capacity) {
                resize();
            }
            // Find the correct position to insert the new value
            int index = 0;
            for (int r = 0; r < i; ++r) {
                for (int c = 0; c < cols; ++c) {
                    if (bits[r][c]) ++index;
                }
            }
            for (int c = 0; c < j; ++c) {
                if (bits[i][c]) ++index;
            }
            // Shift elements to make room for the new value
            for (int k = nonZeroCount; k > index; --k) {
                values[k] = values[k-1];
            }
            values[index] = value;
            bits[i][j] = true;
            ++nonZeroCount;
        } else {
            // Update existing non-zero value
            int index = 0;
            for (int r = 0; r < i; ++r) {
                for (int c = 0; c < cols; ++c) {
                    if (bits[r][c]) ++index;
                }
            }
            for (int c = 0; c <= j; ++c) {
                if (bits[i][c]) ++index;
            }
            values[index-1] = value;
        }
    } else if (bits[i][j]) {
        // Remove existing non-zero value
        int index = 0;
        for (int r = 0; r < i; ++r) {
            for (int c = 0; c < cols; ++c) {
                if (bits[r][c]) ++index;
            }
        }
        for (int c = 0; c <= j; ++c) {
            if (bits[i][c]) ++index;
        }
        for (int k = index-1; k < nonZeroCount-1; ++k) {
            values[k] = values[k+1];
        }
        bits[i][j] = false;
        --nonZeroCount;
    }
}

int SparseMatrix::get(int i, int j) const {
    if (i < 0 || i >= rows || j < 0 || j >= cols) {
        throw std::out_of_range("Matrix indices out of range");
    }

    if (!bits[i][j]) {
        return 0;
    }

    int index = 0;
    for (int r = 0; r < i; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (bits[r][c]) {
                ++index;
            }
        }
    }
    for (int c = 0; c <= j; ++c) {
        if (bits[i][c]) {
            ++index;
        }
    }
    return values[index - 1];
}

SparseMatrix SparseMatrix::add(const SparseMatrix& other) const {
    if (rows != other.rows || cols != other.cols) {
        throw std::invalid_argument("Matrix dimensions do not match");
    }

    SparseMatrix result(rows, cols);

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            int sum = get(i, j) + other.get(i, j);
            if (sum != 0) {
                result.set(i, j, sum);
            }
        }
    }

    return result;
}

SparseMatrix SparseMatrix::multiply(const SparseMatrix& other) const {
    if (cols != other.rows) {
        throw std::invalid_argument("Matrix dimensions do not match for multiplication");
    }

    SparseMatrix result(rows, other.cols);

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < other.cols; ++j) {
            int sum = 0;
            for (int k = 0; k < cols; ++k) {
                sum += get(i, k) * other.get(k, j);
            }
            if (sum != 0) {
                result.set(i, j, sum);
            }
        }
    }

    return result;
}

SparseMatrix SparseMatrix::transpose() const {
    SparseMatrix result(cols, rows);

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            int value = get(i, j);
            if (value != 0) {
                result.set(j, i, value);
            }
        }
    }

    return result;
}

int SparseMatrix::randomAccess(int i, int j) const {
    if (i < 0 || i >= rows || j < 0 || j >= cols) {
        throw std::out_of_range("Matrix indices out of range");
    }

    // If the bit is not set, the value is 0
    if (!bits[i][j]) {
        return 0;
    }

    // Count the number of set bits before this position
    int count = 0;
    for (int r = 0; r < i; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (bits[r][c]) {
                ++count;
            }
        }
    }
    for (int c = 0; c <= j; ++c) {
        if (bits[i][c]) {
            ++count;
        }
    }

    // The value is at index (count - 1) in the values array
    return values[count - 1];
}

void SparseMatrix::print() const {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            std::cout << get(i, j) << " ";
        }
        std::cout << std::endl;
    }
}

std::istream& operator>>(std::istream& is, SparseMatrix& matrix) {
    int rows, cols, nonZeroCount;
    is >> rows >> cols >> nonZeroCount;
    matrix = SparseMatrix(rows, cols);
    
    std::cout << "Enter " << nonZeroCount << " non-zero elements (row col value):" << std::endl;
    for (int i = 0; i < nonZeroCount; ++i) {
        int row, col, value;
        is >> row >> col >> value;
        matrix.set(row, col, value);
    }
    return is;
}

std::ostream& operator<<(std::ostream& os, const SparseMatrix& matrix) {
    os << "Sparse Matrix (" << matrix.getRows() << "x" << matrix.getCols() 
       << ") with " << matrix.getNonZeroCount() << " non-zero elements:" << std::endl;
    matrix.print();
    return os;
}
