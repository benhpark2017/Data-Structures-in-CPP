/**
 * Solutions to Chapter 4, Section 8, Exercises 1 to 5 and 9 of Horowitz's
 * Fundamentals of Data Structures in C++.
 * Note that this self-contained solution to Exercise 9 includes solutions
 * to numbered exercises 1 through 5.
 */

/***********************************main.cpp***********************************/
#include <iostream>
#include "SparseMatrix.h"

int main() {
    Matrix m1, m2;
    Matrix sum;
    Matrix difference;
    Matrix transpose;
    Matrix product;
    
    std::cout << "Enter number of rows, number of columns, "
              << "and number of entries in the first matrix:" << std::endl;
    std::cin >> m1;
    
    std::cout << "Enter number of rows, number of columns, "
              << "and number of entries in the second matrix:" << std::endl;
    std::cin >> m2;
    
    std::cout << "First matrix:" << std::endl;
    std::cout << m1;
    
    std::cout << "Second matrix:" << std::endl;
    std::cout << m2;
    
    // Testing copy constructor (checked.)
    Matrix m3(m1);
    std::cout << "Copy of first matrix:" << std::endl;
    std::cout << m3;
    
    // Testing sum of two sparse matrices. (checked.)
    sum = m1 + m2;
    std::cout << "Result of addition:" << std::endl;
    std::cout << sum;
    
    // Testing transpose of a sparse matrix (checked.)
    transpose = Matrix::Transpose(m1);
    std::cout << "Result of Transpose:" << std::endl;
    std::cout << transpose;
    
    // Testing difference of two matrices. (checked.)
    difference = m1 - m2;
    std::cout << "Result of subtraction:" << std::endl;
    std::cout << difference;
    
    // Testing matrix product of two matrices. (checked.)
    product = m1 * m2;
    std::cout << "Result of matrix product:" << std::endl;
    std::cout << product;
    
    return 0;
}



/******************************SparseMatrix.h**********************************/
#ifndef SPARSEMATRIX_H
#define SPARSEMATRIX_H

#include <iostream> // Include necessary header

enum Boolean { FALSE, TRUE };
struct Triple { int row, col, value; };

class Matrix;
class MatrixNode {
    friend class Matrix;
    
    friend std::istream& operator>>(std::istream& is, Matrix& matrix);
    friend std::ostream& operator<<(std::ostream& os, Matrix& matrix);
    
    friend Matrix operator+(const Matrix& a, const Matrix& b);
    friend Matrix operator-(const Matrix& a, const Matrix& b); 
    friend Matrix operator*(const Matrix& a, const Matrix& b);
    
  private:
    MatrixNode *down, *right;
    Boolean head;
    union {
        MatrixNode* next;
        Triple triple;
    };
    MatrixNode(Boolean b, Triple* t); //constructor
};

typedef MatrixNode* MatrixNodePtr;
extern MatrixNodePtr av; // Extern to declare the global variable

class Matrix {
    
    friend std::istream& operator>>(std::istream& is, Matrix& matrix);
    friend std::ostream& operator<<(std::ostream& os, Matrix& matrix);
    
    friend Matrix operator+(const Matrix& a, const Matrix& b);
    friend Matrix operator-(const Matrix& a, const Matrix& b);
    friend Matrix operator*(const Matrix& a, const Matrix& b);
  
  public:
    Matrix(); //Constructor
    Matrix(const Matrix& other); //Copy constructor
    ~Matrix(); //Destructor
    Matrix& operator=(const Matrix& a);
    static Matrix Transpose(const Matrix &a);
    
  private:
    MatrixNode *headNode;
};

#endif



/*****************************SparseMatrix.cpp*********************************/
#include <iostream>
#include "SparseMatrix.h"

MatrixNodePtr av = nullptr; // Define the global variable

MatrixNode::MatrixNode(Boolean b, Triple* t) {
    head = b;
    if (b) {
        right = next = this;
    } else {
        triple = *t;
    }
}

//Reads in a matrix and sets up its linked representation.
//An auxiliary array head[] is used.
std::istream& operator>>(std::istream& is, Matrix& matrix) {
    Triple s; int p;
    is >> s.row >> s.col >> s.value;
    if (s.row > s.col) p = s.row;
    else p = s.col;
    //Sets up headNode for list of head nodes.
    matrix.headNode = new MatrixNode(FALSE, &s);
    if (p == 0) {
        matrix.headNode->right = matrix.headNode;
        return is;
    }
    std::cout << "Please specify row, column, and value." << std::endl;
    //At least one row or column
    MatrixNodePtr *head = new MatrixNodePtr[p]; //initialize head nodes
    for (int i = 0; i < p; i++) 
        head[i] = new MatrixNode(TRUE, nullptr);
    int CurrentRow = 0; MatrixNode *last = head[0]; //last node in the current row
    for (int i = 0; i < s.value; i++) {
        std::cout << "Row, Column, Value: ";
        Triple t;
        is >> t.row >> t.col >> t.value;
        if (t.row > CurrentRow) {
            last->right = head[CurrentRow];
            CurrentRow = t.row;
            last = head[CurrentRow];
        } //end if
        last = last->right = new MatrixNode(FALSE, &t); //link new node into row list
        head[t.col]->next = head[t.col]->next->down = last; //link into column list
    } //end of for
    
    last->right = head[CurrentRow]; //close last row
    for (int i = 0; i < s.col; i++) head[i]->next->down = head[i]; //close all column lists
    //link the head nodes together
    for (int i = 0; i < p - 1; i++) head[i]->next = head[i + 1];
    head[p - 1]->next = matrix.headNode;
    matrix.headNode->right = head[0];
    delete[] head;
    return is;
} //end of overloaded operator function

std::ostream& operator<<(std::ostream &os, Matrix &m) {
    MatrixNode *headNode = m.headNode->right;
    while (headNode != m.headNode) {
        //This line causes a segmentation fault.
        for (MatrixNode *cur = headNode->right; cur != headNode; cur = cur->right)
            os << "(" << cur->triple.row << ", " << cur->triple.col << ", " << cur->triple.value << ")" << std::endl;
        headNode = headNode->next;
    }
    return os;
}

Matrix operator+(const Matrix& a, const Matrix& b) {
    if (a.headNode->triple.row != b.headNode->triple.row || 
        a.headNode->triple.col != b.headNode->triple.col) {
        throw std::invalid_argument("Matrix dimensions do not match");
    }

    Matrix result;
    result.headNode = new MatrixNode(FALSE, &a.headNode->triple);
    int rows = result.headNode->triple.row;
    int cols = result.headNode->triple.col;

    // Create head nodes for rows
    MatrixNodePtr* rowHeads = new MatrixNodePtr[rows];
    for (int i = 0; i < rows; i++) {
        rowHeads[i] = new MatrixNode(TRUE, nullptr);
        rowHeads[i]->right = rowHeads[i];  // Initialize right pointer
    }

    MatrixNodePtr aRowHead = a.headNode->right;
    MatrixNodePtr bRowHead = b.headNode->right;

    while (aRowHead != a.headNode && bRowHead != b.headNode) {
        MatrixNodePtr aPtr = aRowHead->right;
        MatrixNodePtr bPtr = bRowHead->right;

        while (aPtr != aRowHead || bPtr != bRowHead) {
            Triple t;
            if (aPtr == aRowHead || (bPtr != bRowHead && 
                (bPtr->triple.row < aPtr->triple.row || 
                (bPtr->triple.row == aPtr->triple.row && bPtr->triple.col < aPtr->triple.col)))) {
                t = bPtr->triple;
                bPtr = bPtr->right;
            } else if (bPtr == bRowHead || aPtr->triple.row < bPtr->triple.row || 
                       (aPtr->triple.row == bPtr->triple.row && aPtr->triple.col < bPtr->triple.col)) {
                t = aPtr->triple;
                aPtr = aPtr->right;
            } else {
                t = aPtr->triple;
                t.value += bPtr->triple.value;
                aPtr = aPtr->right;
                bPtr = bPtr->right;
            }

            if (t.value != 0) {
                MatrixNodePtr newNode = new MatrixNode(FALSE, &t);
                MatrixNodePtr rowPrev = rowHeads[t.row];
                while (rowPrev->right != rowHeads[t.row] && rowPrev->right->triple.col < t.col) {
                    rowPrev = rowPrev->right;
                }
                newNode->right = rowPrev->right;
                rowPrev->right = newNode;
            }
        }
        
        aRowHead = aRowHead->next;
        bRowHead = bRowHead->next;
    }

    // Process any remaining rows in a
    while (aRowHead != a.headNode) {
        MatrixNodePtr aPtr = aRowHead->right;
        while (aPtr != aRowHead) {
            Triple t = aPtr->triple;
            if (t.value != 0) {
                MatrixNodePtr newNode = new MatrixNode(FALSE, &t);
                MatrixNodePtr rowPrev = rowHeads[t.row];
                while (rowPrev->right != rowHeads[t.row] && rowPrev->right->triple.col < t.col) {
                    rowPrev = rowPrev->right;
                }
                newNode->right = rowPrev->right;
                rowPrev->right = newNode;
            }
            aPtr = aPtr->right;
        }
        aRowHead = aRowHead->next;
    }

    // Process any remaining rows in b
    while (bRowHead != b.headNode) {
        MatrixNodePtr bPtr = bRowHead->right;
        while (bPtr != bRowHead) {
            Triple t = bPtr->triple;
            if (t.value != 0) {
                MatrixNodePtr newNode = new MatrixNode(FALSE, &t);
                MatrixNodePtr rowPrev = rowHeads[t.row];
                while (rowPrev->right != rowHeads[t.row] && rowPrev->right->triple.col < t.col) {
                    rowPrev = rowPrev->right;
                }
                newNode->right = rowPrev->right;
                rowPrev->right = newNode;
            }
            bPtr = bPtr->right;
        }
        bRowHead = bRowHead->next;
    }

    // Link row heads
    for (int i = 0; i < rows - 1; i++) {
        rowHeads[i]->next = rowHeads[i + 1];
    }
    rowHeads[rows - 1]->next = result.headNode;
    result.headNode->right = rowHeads[0];

    // Count non-zero elements
    int valueCount = 0;
    for (int i = 0; i < rows; i++) {
        for (MatrixNodePtr p = rowHeads[i]->right; p != rowHeads[i]; p = p->right) {
            valueCount++;
        }
    }
    result.headNode->triple.value = valueCount;

    delete[] rowHeads;
    return result;
} 

Matrix operator-(const Matrix& a, const Matrix& b) {
    if (a.headNode->triple.row != b.headNode->triple.row || 
        a.headNode->triple.col != b.headNode->triple.col) {
        throw std::invalid_argument("Matrix dimensions do not match for subtraction");
    }

    Matrix result;
    Triple headTriple = {a.headNode->triple.row, a.headNode->triple.col, 0};
    result.headNode = new MatrixNode(FALSE, &headTriple);
    int rows = result.headNode->triple.row;
    int cols = result.headNode->triple.col;

    // Create head nodes for rows
    MatrixNodePtr* rowHeads = new MatrixNodePtr[rows];
    for (int i = 0; i < rows; i++) {
        rowHeads[i] = new MatrixNode(TRUE, nullptr);
        rowHeads[i]->right = rowHeads[i];  // Initialize right pointer
    }

    MatrixNodePtr aRowHead = a.headNode->right;
    MatrixNodePtr bRowHead = b.headNode->right;

    while (aRowHead != a.headNode || bRowHead != b.headNode) {
        MatrixNodePtr aPtr = (aRowHead != a.headNode) ? aRowHead->right : nullptr;
        MatrixNodePtr bPtr = (bRowHead != b.headNode) ? bRowHead->right : nullptr;

        while (aPtr != aRowHead || bPtr != bRowHead) {
            Triple t;
            if (!bPtr || (aPtr && (aPtr->triple.row < bPtr->triple.row || 
                (aPtr->triple.row == bPtr->triple.row && aPtr->triple.col < bPtr->triple.col)))) {
                t = aPtr->triple;
                aPtr = aPtr->right;
            } else if (!aPtr || bPtr->triple.row < aPtr->triple.row || 
                       (bPtr->triple.row == aPtr->triple.row && bPtr->triple.col < aPtr->triple.col)) {
                t = bPtr->triple;
                t.value = -t.value;  // Negate the value from b
                bPtr = bPtr->right;
            } else {
                t = aPtr->triple;
                t.value -= bPtr->triple.value;
                aPtr = aPtr->right;
                bPtr = bPtr->right;
            }

            if (t.value != 0) {
                MatrixNodePtr newNode = new MatrixNode(FALSE, &t);
                MatrixNodePtr rowPrev = rowHeads[t.row];
                while (rowPrev->right != rowHeads[t.row] && rowPrev->right->triple.col < t.col) {
                    rowPrev = rowPrev->right;
                }
                newNode->right = rowPrev->right;
                rowPrev->right = newNode;
                result.headNode->triple.value++;  // Increment non-zero count
            }
        }
        
        if (aRowHead != a.headNode) aRowHead = aRowHead->next;
        if (bRowHead != b.headNode) bRowHead = bRowHead->next;
    }

    // Link row heads
    for (int i = 0; i < rows - 1; i++) {
        rowHeads[i]->next = rowHeads[i + 1];
    }
    rowHeads[rows - 1]->next = result.headNode;
    result.headNode->right = rowHeads[0];

    delete[] rowHeads;
    return result;
}

Matrix::Matrix() : headNode(nullptr) {};

Matrix::Matrix(const Matrix& other) : headNode(nullptr) {
    if (other.headNode) {
        Triple headTriple = other.headNode->triple;
        headNode = new MatrixNode(FALSE, &headTriple);
        int rows = headTriple.row;
        int cols = headTriple.col;

        // Create head nodes for rows
        MatrixNodePtr* rowHeads = new MatrixNodePtr[rows];
        for (int i = 0; i < rows; i++) {
            rowHeads[i] = new MatrixNode(TRUE, nullptr);
            rowHeads[i]->right = rowHeads[i];  // Initialize right pointer
        }

        MatrixNodePtr otherRowHead = other.headNode->right;

        while (otherRowHead != other.headNode) {
            MatrixNodePtr otherCurrent = otherRowHead->right;
            while (otherCurrent != otherRowHead) {
                Triple t = otherCurrent->triple;
                MatrixNodePtr newNode = new MatrixNode(FALSE, &t);
                MatrixNodePtr rowPrev = rowHeads[t.row];
                while (rowPrev->right != rowHeads[t.row] && rowPrev->right->triple.col < t.col) {
                    rowPrev = rowPrev->right;
                }
                newNode->right = rowPrev->right;
                rowPrev->right = newNode;

                // Link into column list
                rowHeads[t.col]->next = rowHeads[t.col]->next->down = newNode;

                otherCurrent = otherCurrent->right;
            }
            otherRowHead = otherRowHead->next;
        }

        // Link row heads
        for (int i = 0; i < rows - 1; i++) {
            rowHeads[i]->next = rowHeads[i + 1];
        }
        rowHeads[rows - 1]->next = headNode;
        headNode->right = rowHeads[0];

        delete[] rowHeads;
    }
}

/**Returns all nodes to the av (available space) list. This list is a chain
 * linked via the right field. av is a global variable of type MatrixNode*
 * and points to its first node.
 */ 
Matrix::~Matrix() {
    if (!headNode) return; //no node to dispose
    MatrixNode *x = headNode->right;
    MatrixNode *y;
    headNode->right = av;
    av = headNode; //return headNode
    while (x != headNode) {
        y = x->right;
        x->right = av;
        av = y;
        x = x->next;
    }
    headNode = nullptr;
}

// Assignment operator
Matrix& Matrix::operator=(const Matrix& a) {
    if (this == &a) {
        return *this; // Handle self-assignment
    }

    // Release any existing resources
    this->~Matrix();

    if (a.headNode) {
        Triple headTriple = a.headNode->triple;
        headNode = new MatrixNode(FALSE, &headTriple);
        int rows = headTriple.row;
        int cols = headTriple.col;

        // Create head nodes for rows
        MatrixNodePtr* rowHeads = new MatrixNodePtr[rows];
        for (int i = 0; i < rows; i++) {
            rowHeads[i] = new MatrixNode(TRUE, nullptr);
            rowHeads[i]->right = rowHeads[i];  // Initialize right pointer
        }

        MatrixNodePtr otherRowHead = a.headNode->right;

        while (otherRowHead != a.headNode) {
            MatrixNodePtr otherCurrent = otherRowHead->right;
            while (otherCurrent != otherRowHead) {
                Triple t = otherCurrent->triple;
                MatrixNodePtr newNode = new MatrixNode(FALSE, &t);
                MatrixNodePtr rowPrev = rowHeads[t.row];
                while (rowPrev->right != rowHeads[t.row] && rowPrev->right->triple.col < t.col) {
                    rowPrev = rowPrev->right;
                }
                newNode->right = rowPrev->right;
                rowPrev->right = newNode;

                // Link into column list
                rowHeads[t.col]->next = rowHeads[t.col]->next->down = newNode;

                otherCurrent = otherCurrent->right;
            }
            otherRowHead = otherRowHead->next;
        }

        // Link row heads
        for (int i = 0; i < rows - 1; i++) {
            rowHeads[i]->next = rowHeads[i + 1];
        }
        rowHeads[rows - 1]->next = headNode;
        headNode->right = rowHeads[0];

        delete[] rowHeads;
    }

    return *this;
}

//New Transpose function added here.
Matrix Matrix::Transpose(const Matrix &a) {
    Matrix result;
    if (!a.headNode) return result;  // Return empty matrix if a is empty

    // Create the head node for the result matrix
    Triple headTriple = {a.headNode->triple.col, a.headNode->triple.row, a.headNode->triple.value};
    result.headNode = new MatrixNode(FALSE, &headTriple);

    int cols = a.headNode->triple.col;
    int nonZeroCount = a.headNode->triple.value;

    if (cols == 0) return result;  // Return if there are no columns

    // Create an array to store the number of elements in each column of a
    int *colCount = new int[cols]();
    for (int i = 0; i < cols; i++) colCount[i] = 0;

    // Count the number of elements in each column of a
    MatrixNodePtr current = a.headNode->right;
    while (current != a.headNode) {
        for (MatrixNodePtr node = current->right; node != current; node = node->right) {
            colCount[node->triple.col]++;
        }
        current = current->next;
    }

    // Calculate the starting position of each column in the transposed matrix
    int *startingPos = new int[cols];
    startingPos[0] = 0;
    for (int i = 1; i < cols; i++) {
        startingPos[i] = startingPos[i-1] + colCount[i-1];
    }

    // Create head nodes for rows in the result matrix
    MatrixNodePtr *rowHeads = new MatrixNodePtr[cols];
    for (int i = 0; i < cols; i++) {
        rowHeads[i] = new MatrixNode(TRUE, nullptr);
        rowHeads[i]->right = rowHeads[i];  // Initialize right pointer
    }

    // Transpose the matrix
    current = a.headNode->right;
    while (current != a.headNode) {
        for (MatrixNodePtr node = current->right; node != current; node = node->right) {
            int j = node->triple.col;
            Triple newTriple = {j, node->triple.row, node->triple.value};
            MatrixNodePtr newNode = new MatrixNode(FALSE, &newTriple);

            // Insert into the appropriate row of the transposed matrix
            MatrixNodePtr prev = rowHeads[j];
            while (prev->right != rowHeads[j] && prev->right->triple.col < newTriple.col) {
                prev = prev->right;
            }
            newNode->right = prev->right;
            prev->right = newNode;

            startingPos[j]++;
        }
        current = current->next;
    }

    // Link the row heads
    for (int i = 0; i < cols - 1; i++) {
        rowHeads[i]->next = rowHeads[i + 1];
    }
    rowHeads[cols - 1]->next = result.headNode;
    result.headNode->right = rowHeads[0];

    // Clean up
    delete[] colCount;
    delete[] startingPos;
    delete[] rowHeads;

    return result;
}

Matrix operator*(const Matrix& a, const Matrix& b) {
    if (a.headNode->triple.col != b.headNode->triple.row) {
        throw std::invalid_argument("Matrix dimensions do not match for multiplication");
    }

    Matrix result;
    Triple headTriple = {a.headNode->triple.row, b.headNode->triple.col, 0};
    result.headNode = new MatrixNode(FALSE, &headTriple);
    int rows = a.headNode->triple.row;
    int cols = b.headNode->triple.col;

    // Create head nodes for rows in the result matrix
    MatrixNodePtr* rowHeads = new MatrixNodePtr[rows];
    for (int i = 0; i < rows; i++) {
        rowHeads[i] = new MatrixNode(TRUE, nullptr);
        rowHeads[i]->right = rowHeads[i];  // Initialize right pointer
    }

    MatrixNodePtr aRowHead = a.headNode->right;

    while (aRowHead != a.headNode) {
        MatrixNodePtr aPtr = aRowHead->right;
        
        while (aPtr != aRowHead) {
            int aRow = aPtr->triple.row;
            int aCol = aPtr->triple.col;
            int aValue = aPtr->triple.value;

            MatrixNodePtr bColHead = b.headNode->right;
            while (bColHead != b.headNode) {
                MatrixNodePtr bPtr = bColHead->right;
                while (bPtr != bColHead) {
                    if (bPtr->triple.row == aCol) {
                        int bCol = bPtr->triple.col;
                        int bValue = bPtr->triple.value;
                        int productValue = aValue * bValue;

                        // Find or create the corresponding element in the result matrix
                        MatrixNodePtr resultPtr = rowHeads[aRow]->right;
                        MatrixNodePtr prevPtr = rowHeads[aRow];
                        while (resultPtr != rowHeads[aRow] && resultPtr->triple.col < bCol) {
                            prevPtr = resultPtr;
                            resultPtr = resultPtr->right;
                        }

                        if (resultPtr != rowHeads[aRow] && resultPtr->triple.col == bCol) {
                            resultPtr->triple.value += productValue;
                            if (resultPtr->triple.value == 0) {
                                // Remove the node if the value becomes zero
                                prevPtr->right = resultPtr->right;
                                delete resultPtr;
                                result.headNode->triple.value--;
                            }
                        } else if (productValue != 0) {
                            // Create a new node for the product
                            Triple newTriple = {aRow, bCol, productValue};
                            MatrixNodePtr newNode = new MatrixNode(FALSE, &newTriple);
                            newNode->right = resultPtr;
                            prevPtr->right = newNode;
                            result.headNode->triple.value++;
                        }
                    }
                    bPtr = bPtr->right;
                }
                bColHead = bColHead->next;
            }
            aPtr = aPtr->right;
        }
        aRowHead = aRowHead->next;
    }

    // Link the row heads
    for (int i = 0; i < rows - 1; i++) {
        rowHeads[i]->next = rowHeads[i + 1];
    }
    rowHeads[rows - 1]->next = result.headNode;
    result.headNode->right = rowHeads[0];

    delete[] rowHeads;
    return result;
}
