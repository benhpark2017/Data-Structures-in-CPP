/**
 * Solutions to Chapter 4, Section 8, Exercises 1 to 5 and 10 of Horowitz's
 * Fundamentals of Data Structures in C++.
 * Note that this self-contained solution to Exercise 10 includes solutions
 * to modified versions of numbered exercises 1 through 5.
 */

/************************************main.cpp**********************************/
#include "SparseMatrix.h"
#include <iostream>

int main() {
    SparseMatrix m1, m2, m3;
    SparseMatrix sum, difference, product;
    SparseMatrix transpose;
    
    //Testing input and output (checked)
    std::cin >> m1;
    std::cout << "The matrix you have entered is: \n";
    std::cout << m1 << std::endl;
    
    std::cin >> m2;
    std::cout << "The matrix you have entered is: \n";
    std::cout << m2 << std::endl;
    
    //Testing assignment operator (checked)
    m3 = m1;
    std::cout << "The matrix 1 has been successfully assigned to matrix 3. \n";
    std::cout << m3 << std::endl;
    
    //Testing copy constructor (checked)
    SparseMatrix m4(m1);
    std::cout << "Copy constructor called on Matrix 1: " << std::endl;
    std::cout << m4 << std::endl;
    
    //Testing addition. (checked)
    sum = m1 + m2;
    std::cout << "The sum of two matrices 1 and 2 is: " << std::endl;
    std::cout << sum << std::endl;
    
    //Testing subtraction. (checked)
    difference = m1 - m2;
    std::cout << "The difference of two matrices 1 and 2 is: " << std::endl;
    std::cout << difference << std::endl;
    
    //Testing matrix multiplication (checked).
    product = m1 * m2;
    std::cout << "The product of two matrices 1 and 2 is: " << std::endl;
    std::cout << product << std::endl;
    
    //Testing the transpose function (checked).
    transpose = SparseMatrix::Transpose(m1);
    std::cout << "The transpose of Matrix 1 is" << std::endl;
    std::cout << transpose << std::endl;
    
    return 0;
}



/******************************SparseMatrix.h**********************************/
#ifndef SPARSE_MATRIX_H
#define SPARSE_MATRIX_H

#include <iostream>

class SparseMatrix;

class MatrixNode {
    friend class SparseMatrix;
    friend std::istream& operator>>(std::istream& is, SparseMatrix& matrix);
    friend std::ostream& operator<<(std::ostream& os, const SparseMatrix& matrix);
    
    friend SparseMatrix operator+(const SparseMatrix& a, const SparseMatrix& b);
    friend SparseMatrix operator-(const SparseMatrix& a, const SparseMatrix& b);
    friend SparseMatrix operator*(const SparseMatrix& a, const SparseMatrix& b);
    
private:
    MatrixNode* down;
    MatrixNode* right;
    int row;
    int col;
    int value;

    MatrixNode(int r = 0, int c = 0, int v = 0) : down(nullptr), right(nullptr), row(r), col(c), value(v) {}
};

class SparseMatrix {
    friend std::istream& operator>>(std::istream& is, SparseMatrix& matrix);
    friend std::ostream& operator<<(std::ostream& os, const SparseMatrix& matrix);

    friend SparseMatrix operator+(const SparseMatrix& a, const SparseMatrix& b);
    friend SparseMatrix operator-(const SparseMatrix& a, const SparseMatrix& b);
    friend SparseMatrix operator*(const SparseMatrix& a, const SparseMatrix& b);
    
public:
    SparseMatrix(); //default constructor
    SparseMatrix(int rows, int cols); //constructor with dimension parameters
    SparseMatrix(const SparseMatrix& other); //copy constructor
    ~SparseMatrix(); //destructor
    
    SparseMatrix& operator=(const SparseMatrix& other);
    static SparseMatrix Transpose(const SparseMatrix &a);

private:
    MatrixNode* headNode;
    MatrixNode* dimensionNode;
};

#endif // SPARSE_MATRIX_H



/****************************SparseMatrix.cpp**********************************/
#include "SparseMatrix.h"
#include <iomanip> // for setw
#include <iostream>

// Default constructor
SparseMatrix::SparseMatrix() : headNode(new MatrixNode()), dimensionNode(new MatrixNode(0, 0, 0)) {
    headNode->right = headNode;
    headNode->down = headNode;
}

// Constructor with dimensions
SparseMatrix::SparseMatrix(int rows, int cols) : headNode(new MatrixNode()), dimensionNode(new MatrixNode(rows, cols, 0)) {
    headNode->right = headNode;
    headNode->down = headNode;
}

// Copy constructor.
SparseMatrix::SparseMatrix(const SparseMatrix& other) : headNode(nullptr), dimensionNode(nullptr) {
    if (other.dimensionNode) {
        dimensionNode = new MatrixNode(other.dimensionNode->row, other.dimensionNode->col, other.dimensionNode->value);
    }
    
    if (other.headNode) {
        headNode = new MatrixNode();
        headNode->right = headNode;
        
        MatrixNode* otherCurrent = other.headNode->right;
        MatrixNode* current = headNode;
        
        while (otherCurrent != other.headNode) {
            MatrixNode* newNode = new MatrixNode(otherCurrent->row, otherCurrent->col, otherCurrent->value);
            current->right = newNode;
            current = newNode;
            otherCurrent = otherCurrent->right;
        }
        
        current->right = headNode;
    }
}

SparseMatrix::~SparseMatrix() {
    if (headNode) {
        MatrixNode* current = headNode->right;
        while (current != headNode) {
            MatrixNode* temp = current;
            current = current->right;
            delete temp;
        }
        delete headNode;
    }
    delete dimensionNode;
}

std::istream& operator>>(std::istream& is, SparseMatrix& matrix) {
    int rows, cols, nonZeroElements;
    std::cout << "Please enter number of rows, number of columns,"
              << " and number of nonzero entries: " << std::endl;
    is >> rows >> cols >> nonZeroElements;

    // Clear existing matrix
    if (matrix.headNode) {
        MatrixNode* current = matrix.headNode->right;
        while (current != matrix.headNode) {
            MatrixNode* temp = current;
            current = current->right;
            delete temp;
        }
        delete matrix.headNode;
    }
    delete matrix.dimensionNode;

    // Initialize new matrix
    matrix.headNode = new MatrixNode();
    matrix.dimensionNode = new MatrixNode(rows, cols, nonZeroElements);
    matrix.headNode->right = matrix.headNode;

    MatrixNode* lastNode = matrix.headNode;

    // Read and insert elements
    for (int i = 0; i < nonZeroElements; ++i) {
        int row, col, value;
        std::cout << "Enter row index, column index, value:" << std::endl;
        is >> row >> col >> value;

        MatrixNode* newNode = new MatrixNode(row, col, value);

        // Insert maintaining row and column order
        MatrixNode* curr = matrix.headNode;
        while (curr->right != matrix.headNode && 
               (curr->right->row < row || (curr->right->row == row && curr->right->col < col))) {
            curr = curr->right;
        }

        newNode->right = curr->right;
        curr->right = newNode;

        // Update lastNode to the newly inserted node
        lastNode = newNode;
    }

    // Connect the last node back to the head
    lastNode->right = matrix.headNode;
    
    return is;
}

std::ostream& operator<<(std::ostream& os, const SparseMatrix& matrix) {
    os << "Number of Rows: " << matrix.dimensionNode->row << "\n" 
       << "Number of Columns: " << matrix.dimensionNode->col << "\n"
       << "Number of Non-zero Elements: " << matrix.dimensionNode->value << std::endl;

    os << "Matrix elements:" << std::endl;

    MatrixNode* current = matrix.headNode->right;
    while (current != matrix.headNode) {
        os << "Row: " << current->row << std::setw(10)
           << "Col: " << current->col << std::setw(10) 
           << "Value: " << current->value << std::endl;
        current = current->right;
    }

    return os;
}

//User-defined assignment operator function
SparseMatrix& SparseMatrix::operator=(const SparseMatrix& other) {
    if (this != &other) {
        // Clear existing matrix
        this->~SparseMatrix();
        
        // Copy from other
        if (other.dimensionNode) {
            dimensionNode = new MatrixNode(other.dimensionNode->row, other.dimensionNode->col, other.dimensionNode->value);
        }
        
        if (other.headNode) {
            headNode = new MatrixNode();
            headNode->right = headNode;
            
            MatrixNode* otherCurrent = other.headNode->right;
            MatrixNode* current = headNode;
            
            while (otherCurrent != other.headNode) {
                MatrixNode* newNode = new MatrixNode(otherCurrent->row, otherCurrent->col, otherCurrent->value);
                current->right = newNode;
                current = newNode;
                otherCurrent = otherCurrent->right;
            }
            
            current->right = headNode;
        }
    }
    return *this;
}

//User-defined addition operator function.
SparseMatrix operator+(const SparseMatrix& a, const SparseMatrix& b) {
    if (a.dimensionNode->row != b.dimensionNode->row || a.dimensionNode->col != b.dimensionNode->col) {
        throw std::invalid_argument("Matrices dimensions must match for addition");
    }

    SparseMatrix result(a.dimensionNode->row, a.dimensionNode->col);
    result.headNode = new MatrixNode();
    result.headNode->right = result.headNode;
    
    MatrixNode* aCurrent = a.headNode->right;
    MatrixNode* bCurrent = b.headNode->right;
    MatrixNode* lastNode = result.headNode;
    int nonZeroCount = 0;

    while (aCurrent != a.headNode || bCurrent != b.headNode) {
        if (aCurrent != a.headNode && (bCurrent == b.headNode || (aCurrent->row < bCurrent->row || (aCurrent->row == bCurrent->row && aCurrent->col < bCurrent->col)))) {
            // Copy element from a
            MatrixNode* newNode = new MatrixNode(aCurrent->row, aCurrent->col, aCurrent->value);
            lastNode->right = newNode;
            lastNode = newNode;
            aCurrent = aCurrent->right;
        } else if (bCurrent != b.headNode && (aCurrent == a.headNode || (bCurrent->row < aCurrent->row || (bCurrent->row == aCurrent->row && bCurrent->col < aCurrent->col)))) {
            // Copy element from b
            MatrixNode* newNode = new MatrixNode(bCurrent->row, bCurrent->col, bCurrent->value);
            lastNode->right = newNode;
            lastNode = newNode;
            bCurrent = bCurrent->right;
        } else {
            // Add elements from a and b
            int sum = aCurrent->value + bCurrent->value;
            if (sum != 0) {
                MatrixNode* newNode = new MatrixNode(aCurrent->row, aCurrent->col, sum);
                lastNode->right = newNode;
                lastNode = newNode;
            }
            aCurrent = aCurrent->right;
            bCurrent = bCurrent->right;
        }
        nonZeroCount++;
    }

    lastNode->right = result.headNode;

    result.dimensionNode->value = nonZeroCount;
    
    return result;
}

//User-defined subtraction operator function.
SparseMatrix operator-(const SparseMatrix& a, const SparseMatrix& b) {
    if (a.dimensionNode->row != b.dimensionNode->row || a.dimensionNode->col != b.dimensionNode->col) {
        throw std::invalid_argument("Matrices dimensions must match for subtraction");
    }

    SparseMatrix result(a.dimensionNode->row, a.dimensionNode->col);
    result.headNode = new MatrixNode();
    result.headNode->right = result.headNode;
    
    MatrixNode* aCurrent = a.headNode->right;
    MatrixNode* bCurrent = b.headNode->right;
    MatrixNode* lastNode = result.headNode;
    int nonZeroCount = 0;

    while (aCurrent != a.headNode || bCurrent != b.headNode) {
        if (aCurrent != a.headNode && (bCurrent == b.headNode || (aCurrent->row < bCurrent->row || (aCurrent->row == bCurrent->row && aCurrent->col < bCurrent->col)))) {
            // Copy element from a
            MatrixNode* newNode = new MatrixNode(aCurrent->row, aCurrent->col, aCurrent->value);
            lastNode->right = newNode;
            lastNode = newNode;
            aCurrent = aCurrent->right;
        } else if (bCurrent != b.headNode && (aCurrent == a.headNode || (bCurrent->row < aCurrent->row || (bCurrent->row == aCurrent->row && bCurrent->col < aCurrent->col)))) {
            // Copy element from b with negated value
            MatrixNode* newNode = new MatrixNode(bCurrent->row, bCurrent->col, -bCurrent->value);
            lastNode->right = newNode;
            lastNode = newNode;
            bCurrent = bCurrent->right;
        } else {
            // Subtract elements from a and b
            int diff = aCurrent->value - bCurrent->value;
            if (diff != 0) {
                MatrixNode* newNode = new MatrixNode(aCurrent->row, aCurrent->col, diff);
                lastNode->right = newNode;
                lastNode = newNode;
            }
            aCurrent = aCurrent->right;
            bCurrent = bCurrent->right;
        }
        nonZeroCount++;
    }

    lastNode->right = result.headNode;

    result.dimensionNode->value = nonZeroCount;
    
    return result;
}

SparseMatrix operator*(const SparseMatrix& a, const SparseMatrix& b) {
    if (a.dimensionNode->col != b.dimensionNode->row) {
        throw std::invalid_argument("Number of columns in first matrix must equal number of rows in second matrix for multiplication");
    }

    SparseMatrix result(a.dimensionNode->row, b.dimensionNode->col);
    result.headNode = new MatrixNode();
    result.headNode->right = result.headNode;
    
    MatrixNode* lastNode = result.headNode;
    int nonZeroCount = 0;

    for (int i = 0; i < a.dimensionNode->row; ++i) {
        MatrixNode* aRow = a.headNode->right;
        while (aRow != a.headNode && aRow->row < i) {
            aRow = aRow->right;
        }

        if (aRow != a.headNode && aRow->row == i) {
            int* tempRow = new int[b.dimensionNode->col]();  // Initialize to 0

            while (aRow != a.headNode && aRow->row == i) {
                MatrixNode* bCol = b.headNode->right;
                while (bCol != b.headNode && bCol->row < aRow->col) {
                    bCol = bCol->right;
                }

                while (bCol != b.headNode && bCol->row == aRow->col) {
                    tempRow[bCol->col] += aRow->value * bCol->value;
                    bCol = bCol->right;
                }

                aRow = aRow->right;
            }

            for (int j = 0; j < b.dimensionNode->col; ++j) {
                if (tempRow[j] != 0) {
                    MatrixNode* newNode = new MatrixNode(i, j, tempRow[j]);
                    lastNode->right = newNode;
                    lastNode = newNode;
                    nonZeroCount++;
                }
            }

            delete[] tempRow;
        }
    }

    lastNode->right = result.headNode;
    result.dimensionNode->value = nonZeroCount;
    
    return result;
}

SparseMatrix SparseMatrix::Transpose(const SparseMatrix &a) {
    SparseMatrix result(a.dimensionNode->col, a.dimensionNode->row);
    result.headNode = new MatrixNode();
    result.headNode->right = result.headNode;
    
    MatrixNode* lastNode = result.headNode;
    int nonZeroCount = 0;

    // First, count the number of elements in each column of the original matrix
    int* colCounts = new int[a.dimensionNode->col]();
    MatrixNode* current = a.headNode->right;
    while (current != a.headNode) {
        colCounts[current->col]++;
        current = current->right;
        nonZeroCount++;
    }

    // Create an array to store the starting positions of each row in the transposed matrix
    int* rowStarts = new int[a.dimensionNode->col]();
    for (int i = 1; i < a.dimensionNode->col; i++) {
        rowStarts[i] = rowStarts[i-1] + colCounts[i-1];
    }

    // Create an array to temporarily store the transposed matrix
    MatrixNode** transposedNodes = new MatrixNode*[nonZeroCount];
    for (int i = 0; i < nonZeroCount; i++) {
        transposedNodes[i] = nullptr;
    }

    // Populate the transposed nodes array
    current = a.headNode->right;
    while (current != a.headNode) {
        int index = rowStarts[current->col]++;
        transposedNodes[index] = new MatrixNode(current->col, current->row, current->value);
        current = current->right;
    }

    // Link the nodes in the result matrix
    for (int i = 0; i < nonZeroCount; i++) {
        lastNode->right = transposedNodes[i];
        lastNode = transposedNodes[i];
    }
    lastNode->right = result.headNode;

    result.dimensionNode->value = nonZeroCount;

    delete[] colCounts;
    delete[] rowStarts;
    delete[] transposedNodes;
    
    return result;
}
