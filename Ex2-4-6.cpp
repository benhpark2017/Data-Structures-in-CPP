/**
 * Solutions to Chapter 2, Section 4, Exercise 4 of Horowitz's
 * Data Structures in C++.
 */
 
/**************************************************main.cpp*************************************************/
#include <iostream>
#include <stdexcept>
#include "SparseMatrix.h"

int main() {
    SparseMatrix matrix1, matrix2;

    std::cout << "Enter matrix 1 (format: Rows Cols Terms):" << std::endl;
    std::cin >> matrix1;

    std::cout << "Enter matrix 2 (format: Rows Cols Terms):" << std::endl;
    std::cin >> matrix2;

    std::cout << "\nMatrix 1:" << std::endl;
    std::cout << matrix1;

    std::cout << "\nMatrix 2:" << std::endl;
    std::cout << matrix2;

    SparseMatrix transpose1 = matrix1.Transpose();
    std::cout << "\nTranspose of Matrix 1:" << std::endl;
    std::cout << transpose1;

    SparseMatrix fastTranspose1 = matrix1.FastTranspose();
    std::cout << "\nFast Transpose of Matrix 1:" << std::endl;
    std::cout << fastTranspose1;

    SparseMatrix sum = matrix1.Add(matrix2);
    std::cout << "\nSum of Matrix 1 and Matrix 2:" << std::endl;
    std::cout << sum;

    SparseMatrix product = matrix1.Multiply(matrix2);
    std::cout << "\nProduct of Matrix 1 and Matrix 2:" << std::endl;
    std::cout << product;

    return 0;
}




/**********************************************SparseMatrix.h**************************************************/
#ifndef SPARSEMATRIX_H
#define SPARSEMATRIX_H

#include <iostream>

#define MAX_TERMS 100

class SparseMatrix;

class MatrixTerm {
    friend class SparseMatrix;
    friend std::istream& operator>>(std::istream& is, SparseMatrix& matrix);
    friend std::ostream& operator<<(std::ostream& os, const SparseMatrix& matrix);

private:
    int row, col, value;

public:
    // Accessors
    int getRow() const { return row; }
    int getCol() const { return col; }
    int getValue() const { return value; }

    // Mutators
    void setRow(int r) { row = r; }
    void setCol(int c) { col = c; }
    void setValue(int v) { value = v; }
};

class SparseMatrix {
private:
    int Rows, Cols, Terms;
    MatrixTerm SparseMatrixArray[MAX_TERMS];

public:
    SparseMatrix(int MaxRow = 0, int MaxCol = 0); // Modified constructor

    // Accessors
    int getRows() const { return Rows; }
    int getCols() const { return Cols; }
    int getTerms() const { return Terms; }
    MatrixTerm getTerm(int index) const { return SparseMatrixArray[index]; }

    // Mutators
    void setRows(int r) { Rows = r; }
    void setCols(int c) { Cols = c; }
    void setTerms(int t) { Terms = t; }
    void setTerm(int index, const MatrixTerm& term) { SparseMatrixArray[index] = term; }

    SparseMatrix EmptyMatrix();
    SparseMatrix Transpose();
    SparseMatrix FastTranspose();
    int StoreSum(int sum, int& LastInResult, int r, int c);
    SparseMatrix Add(SparseMatrix b);
    SparseMatrix Multiply(const SparseMatrix& b);
    
    friend std::istream& operator>>(std::istream& is, SparseMatrix& matrix);
    friend std::ostream& operator<<(std::ostream& os, const SparseMatrix& matrix);
};

#endif // SPARSEMATRIX_H




/*******************************************SparseMatrix.cpp************************************************/
#include <stdexcept>
#include <iostream>
#include "SparseMatrix.h"

SparseMatrix::SparseMatrix(int MaxRow, int MaxCol) {
    setRows(MaxRow);
    setCols(MaxCol);
    setTerms(0); // Initially, there are no non-zero terms
}

SparseMatrix SparseMatrix::EmptyMatrix() {
    return SparseMatrix(0, 0); // Returns an empty matrix
}

SparseMatrix SparseMatrix::Transpose() {
    SparseMatrix b;
    b.setRows(getCols());
    b.setCols(getRows());
    b.setTerms(getTerms());
    if (getTerms() > 0) {
        int currB = 0;
        for (int c = 0; c < getCols(); c++) {
            for (int i = 0; i < getTerms(); i++) {
                if (getTerm(i).getCol() == c) {
                    MatrixTerm term;
                    term.setRow(c);
                    term.setCol(getTerm(i).getRow());
                    term.setValue(getTerm(i).getValue());
                    b.setTerm(currB, term);
                    currB++;
                }
            }
        }
    }
    return b;
}

SparseMatrix SparseMatrix::FastTranspose() {
    int *RowSize = new int[getCols()];
    int *RowStart = new int[getCols()];
    SparseMatrix b;
    b.setRows(getCols());
    b.setCols(getRows());
    b.setTerms(getTerms());
    
    if (getTerms() > 0) {
        for (int i = 0; i < getCols(); i++) RowSize[i] = 0;
        for (int i = 0; i < getTerms(); i++) RowSize[getTerm(i).getCol()]++;
        
        RowStart[0] = 0;
        for (int i = 1; i < getCols(); i++) RowStart[i] = RowStart[i - 1] + RowSize[i - 1];
        
        for (int i = 0; i < getTerms(); i++) {
            int j = RowStart[getTerm(i).getCol()];
            MatrixTerm term;
            term.setRow(getTerm(i).getCol());
            term.setCol(getTerm(i).getRow());
            term.setValue(getTerm(i).getValue());
            b.setTerm(j, term);
            RowStart[getTerm(i).getCol()]++;
        }
    }
    delete [] RowSize;
    delete [] RowStart;
    return b;
}

SparseMatrix SparseMatrix::Add(SparseMatrix b) {
    if (getRows() != b.getRows() || getCols() != b.getCols()) {
        throw std::invalid_argument("Matrices dimensions do not match.");
    }

    SparseMatrix result(getRows(), getCols());
    result.setTerms(0);

    int aPos = 0, bPos = 0;

    while (aPos < getTerms() && bPos < b.getTerms()) {
        int aRow = getTerm(aPos).getRow();
        int aCol = getTerm(aPos).getCol();
        int bRow = b.getTerm(bPos).getRow();
        int bCol = b.getTerm(bPos).getCol();

        if (aRow == bRow && aCol == bCol) {
            int sumValue = getTerm(aPos).getValue() + b.getTerm(bPos).getValue();
            if (sumValue != 0) {
                MatrixTerm term;
                term.setRow(aRow);
                term.setCol(aCol);
                term.setValue(sumValue);
                result.setTerm(result.getTerms(), term);
                result.setTerms(result.getTerms() + 1);
            }
            aPos++;
            bPos++;
        } else if (aRow < bRow || (aRow == bRow && aCol < bCol)) {
            result.setTerm(result.getTerms(), getTerm(aPos));
            result.setTerms(result.getTerms() + 1);
            aPos++;
        } else {
            result.setTerm(result.getTerms(), b.getTerm(bPos));
            result.setTerms(result.getTerms() + 1);
            bPos++;
        }
    }

    while (aPos < getTerms()) {
        result.setTerm(result.getTerms(), getTerm(aPos));
        result.setTerms(result.getTerms() + 1);
        aPos++;
    }

    while (bPos < b.getTerms()) {
        result.setTerm(result.getTerms(), b.getTerm(bPos));
        result.setTerms(result.getTerms() + 1);
        bPos++;
    }

    return result;
}

int SparseMatrix::StoreSum(int sum, int& LastInResult, int r, int c) {
    if (sum != 0) {
        if (LastInResult < MAX_TERMS - 1) {
            LastInResult++;
            MatrixTerm term;
            term.setRow(r);
            term.setCol(c);
            term.setValue(sum);
            setTerm(LastInResult, term);
            return 0;
        } else {
            std::cerr << "Number of terms in product exceeds maximum of terms." << std::endl;
            return 1;
        }
    } else return 0;
}

//Solutions to Chapter 2, Section 4, Exercise 6
SparseMatrix SparseMatrix::Multiply(const SparseMatrix& b) {
    if (getCols() != b.getRows()) {
        std::cout << "Incompatible matrices." << std::endl;
        return EmptyMatrix();
    }
    if ((getTerms() == MAX_TERMS) || (b.getTerms() == MAX_TERMS)) {
        std::cout << "One additional space in a or b needed." << std::endl;
    }

    SparseMatrix result(getRows(), b.getCols());
    
    // Create row pointers for matrix A
    int* rowPtr = new int[getRows() + 1]();
    for (int i = 0; i < getTerms(); i++) {
        rowPtr[getTerm(i).getRow() + 1]++;
    }
    for (int i = 1; i <= getRows(); i++) {
        rowPtr[i] += rowPtr[i-1];
    }

    // Create column pointers for matrix B
    int* colPtr = new int[b.getCols() + 1]();
    for (int i = 0; i < b.getTerms(); i++) {
        colPtr[b.getTerm(i).getCol() + 1]++;
    }
    for (int i = 1; i <= b.getCols(); i++) {
        colPtr[i] += colPtr[i-1];
    }

    int lastInResult = -1;

    for (int i = 0; i < getRows(); i++) {
        for (int j = 0; j < b.getCols(); j++) {
            int sum = 0;
            int a_index = rowPtr[i];
            int b_index = colPtr[j];

            while (a_index < rowPtr[i+1] && b_index < colPtr[j+1]) {
                if (getTerm(a_index).getCol() < b.getTerm(b_index).getRow()) {
                    a_index++;
                } else if (getTerm(a_index).getCol() > b.getTerm(b_index).getRow()) {
                    b_index++;
                } else {
                    sum += getTerm(a_index).getValue() * b.getTerm(b_index).getValue();
                    a_index++;
                    b_index++;
                }
            }

            if (sum != 0) {
                if (lastInResult < MAX_TERMS - 1) {
                    lastInResult++;
                    MatrixTerm term;
                    term.setRow(i);
                    term.setCol(j);
                    term.setValue(sum);
                    result.setTerm(lastInResult, term);
                } else {
                    std::cout << "Number of terms in product exceeds " << MAX_TERMS << std::endl;
                    delete[] rowPtr;
                    delete[] colPtr;
                    return EmptyMatrix();
                }
            }
        }
    }

    result.setTerms(lastInResult + 1);
    delete[] rowPtr;
    delete[] colPtr;
    return result;
}

std::istream& operator>>(std::istream& is, SparseMatrix& matrix) {
    int rows, cols, terms;
    is >> rows >> cols >> terms;
    matrix = SparseMatrix(rows, cols);
    matrix.setTerms(terms);
    
    std::cout << "Enter " << terms << " non-zero elements (row col value):" << std::endl;
    for (int i = 0; i < terms; ++i) {
        MatrixTerm term;
        int row, col, value;
        is >> row >> col >> value;
        term.setRow(row);
        term.setCol(col);
        term.setValue(value);
        matrix.setTerm(i, term);
    }
    return is;
}

std::ostream& operator<<(std::ostream& os, const SparseMatrix& matrix) {
    os << "Sparse Matrix (" << matrix.getRows() << "x" << matrix.getCols() 
       << ") with " << matrix.getTerms() << " non-zero elements:" << std::endl;
    
    for (int i = 0; i < matrix.getTerms(); ++i) {
        MatrixTerm term = matrix.getTerm(i);
        os << "(" << term.getRow() << ", " << term.getCol() << "): " << term.getValue() << std::endl;
    }
    return os;
}