/**
 * Solutions to Chapter 2, Section 4, Exercise 8 of Horowitz's
 * Data Structures in C++.
 */

/**************************************main.cpp*******************************************/
#include <iostream>
#include <stdexcept>
#include "SparseMatrix.h"

int main() {
    SparseMatrix matrix1(0, 0, 100), matrix2(0, 0, 100);

    std::cout << "Enter matrix 1 (format: #Rows #Cols #Entries):" << std::endl;
    std::cin >> matrix1;

    std::cout << "Enter matrix 2 (format: #Rows #Cols #Entries):" << std::endl;
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

    SparseMatrix fastTranspose2 = matrix2.FastTranspose();
    std::cout << "\nFast Transpose of Matrix 2:" << std::endl;
    std::cout << fastTranspose2;

    SparseMatrix product;
    try {
        product = matrix1.Multiply(matrix2);
        std::cout << "\nProduct of Matrix 1 and Matrix 2:" << std::endl;
        std::cout << product;
    } catch (const std::invalid_argument& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}



/***********************************SparseMatrix.h****************************************/
#ifndef SPARSEMATRIX_H
#define SPARSEMATRIX_H

#include <iostream>

class MatrixTerm {
private:
    int row, col, value;

public:
    int getRow() const { return row; }
    int getCol() const { return col; }
    int getValue() const { return value; }
    void setRow(int r) { row = r; }
    void setCol(int c) { col = c; }
    void setValue(int v) { value = v; }
};

class SparseMatrix {
private:
    int Rows, Cols, NonZeroCount;
    MatrixTerm *SparseMatrixArray;
    int Capacity;

public:
    SparseMatrix(int MaxRow = 0, int MaxCol = 0, int MaxTerms = 0);
    ~SparseMatrix();
    SparseMatrix(const SparseMatrix& other);
    SparseMatrix& operator=(const SparseMatrix& other);

    // Accessors
    int getRows() const { return Rows; }
    int getCols() const { return Cols; }
    int getNonZeroCount() const { return NonZeroCount; }
    MatrixTerm getElement(int index) const { return SparseMatrixArray[index]; }

    // Mutators
    void setRows(int r) { Rows = r; }
    void setCols(int c) { Cols = c; }
    void setNonZeroCount(int count) { NonZeroCount = count; }
    void setElement(int index, const MatrixTerm& term);

    SparseMatrix EmptyMatrix() const;
    SparseMatrix Transpose() const;
    SparseMatrix FastTranspose() const;
    int StoreSum(int sum, int& LastInResult, int r, int c);
    void Resize(int newCapacity);
    SparseMatrix Add(SparseMatrix b) const;
    SparseMatrix Multiply(const SparseMatrix& b) const;
    
    friend std::istream& operator>>(std::istream& is, SparseMatrix& matrix);
    friend std::ostream& operator<<(std::ostream& os, const SparseMatrix& matrix);
};

#endif



/***********************************SparseMatrix.cpp**************************************/
#include <stdexcept>
#include <iostream>
#include "SparseMatrix.h"

SparseMatrix::SparseMatrix(int MaxRow, int MaxCol, int MaxTerms) {
    Rows = MaxRow;
    Cols = MaxCol;
    NonZeroCount = 0;
    Capacity = MaxTerms;
    SparseMatrixArray = new MatrixTerm[Capacity];
}

SparseMatrix::~SparseMatrix() {
    delete[] SparseMatrixArray;
}

SparseMatrix::SparseMatrix(const SparseMatrix& other) {
    Rows = other.Rows;
    Cols = other.Cols;
    NonZeroCount = other.NonZeroCount;
    Capacity = other.Capacity;
    SparseMatrixArray = new MatrixTerm[Capacity];
    for (int i = 0; i < NonZeroCount; i++) {
        SparseMatrixArray[i] = other.SparseMatrixArray[i];
    }
}

SparseMatrix& SparseMatrix::operator=(const SparseMatrix& other) {
    if (this != &other) {
        delete[] SparseMatrixArray;
        Rows = other.Rows;
        Cols = other.Cols;
        NonZeroCount = other.NonZeroCount;
        Capacity = other.Capacity;
        SparseMatrixArray = new MatrixTerm[Capacity];
        for (int i = 0; i < NonZeroCount; i++) {
            SparseMatrixArray[i] = other.SparseMatrixArray[i];
        }
    }
    return *this;
}

void SparseMatrix::setElement(int index, const MatrixTerm& term) {
    if (index >= Capacity) {
        Resize(index + 1);
    }
    SparseMatrixArray[index] = term;
    if (index >= NonZeroCount) NonZeroCount = index + 1;
}

void SparseMatrix::Resize(int newCapacity) {
    MatrixTerm* newArray = new MatrixTerm[newCapacity];
    int minTerms = (NonZeroCount < newCapacity) ? NonZeroCount : newCapacity;
    for (int i = 0; i < minTerms; i++) {
        newArray[i] = SparseMatrixArray[i];
    }
    delete[] SparseMatrixArray;
    SparseMatrixArray = newArray;
    Capacity = newCapacity;
    if (NonZeroCount > Capacity) NonZeroCount = Capacity;
}

SparseMatrix SparseMatrix::EmptyMatrix() const {
    return SparseMatrix(0, 0, 0);
}

SparseMatrix SparseMatrix::Transpose() const {
    SparseMatrix b;
    b.setRows(getCols());
    b.setCols(getRows());
    b.setNonZeroCount(getNonZeroCount());
    if (getNonZeroCount() > 0) {
        int currB = 0;
        for (int c = 0; c < getCols(); c++) {
            for (int i = 0; i < getNonZeroCount(); i++) {
                if (getElement(i).getCol() == c) {
                    MatrixTerm term;
                    term.setRow(c);
                    term.setCol(getElement(i).getRow());
                    term.setValue(getElement(i).getValue());
                    b.setElement(currB, term);
                    currB++;
                }
            }
        }
    }
    return b;
}

SparseMatrix SparseMatrix::FastTranspose() const {
    SparseMatrix b(getCols(), getRows(), getNonZeroCount());
    b.setNonZeroCount(getNonZeroCount());

    if (getNonZeroCount() > 0) {
        int *RowInfo = new int[getCols()]();

        for (int i = 0; i < getNonZeroCount(); i++) {
            RowInfo[getElement(i).getCol()]++;
        }

        int *StartPos = new int[getCols()];
        StartPos[0] = 0;
        for (int i = 1; i < getCols(); i++) {
            StartPos[i] = StartPos[i - 1] + RowInfo[i - 1];
        }

        for (int i = 0; i < getNonZeroCount(); i++) {
            int col = getElement(i).getCol();
            int pos = StartPos[col];

            MatrixTerm term;
            term.setRow(getElement(i).getCol());
            term.setCol(getElement(i).getRow());
            term.setValue(getElement(i).getValue());

            b.setElement(pos, term);

            StartPos[col]++;
        }

        delete[] RowInfo;
        delete[] StartPos;
    }

    return b;
}

SparseMatrix SparseMatrix::Add(SparseMatrix b) const {
    if (getRows() != b.getRows() || getCols() != b.getCols()) {
        throw std::invalid_argument("Matrices dimensions do not match.");
    }

    SparseMatrix result(getRows(), getCols());
    result.setNonZeroCount(0);

    int aPos = 0, bPos = 0;

    while (aPos < getNonZeroCount() && bPos < b.getNonZeroCount()) {
        int aRow = getElement(aPos).getRow();
        int aCol = getElement(aPos).getCol();
        int bRow = b.getElement(bPos).getRow();
        int bCol = b.getElement(bPos).getCol();

        if (aRow == bRow && aCol == bCol) {
            int sumValue = getElement(aPos).getValue() + b.getElement(bPos).getValue();
            if (sumValue != 0) {
                MatrixTerm term;
                term.setRow(aRow);
                term.setCol(aCol);
                term.setValue(sumValue);
                result.setElement(result.getNonZeroCount(), term);
                result.setNonZeroCount(result.getNonZeroCount() + 1);
            }
            aPos++;
            bPos++;
        } else if (aRow < bRow || (aRow == bRow && aCol < bCol)) {
            result.setElement(result.getNonZeroCount(), getElement(aPos));
            result.setNonZeroCount(result.getNonZeroCount() + 1);
            aPos++;
        } else {
            result.setElement(result.getNonZeroCount(), b.getElement(bPos));
            result.setNonZeroCount(result.getNonZeroCount() + 1);
            bPos++;
        }
    }

    while (aPos < getNonZeroCount()) {
        result.setElement(result.getNonZeroCount(), getElement(aPos));
        result.setNonZeroCount(result.getNonZeroCount() + 1);
        aPos++;
    }

    while (bPos < b.getNonZeroCount()) {
        result.setElement(result.getNonZeroCount(), b.getElement(bPos));
        result.setNonZeroCount(result.getNonZeroCount() + 1);
        bPos++;
    }

    return result;
}

int SparseMatrix::StoreSum(int sum, int& LastInResult, int r, int c) {
    if (sum != 0) {
        if (LastInResult < Capacity - 1) {
            LastInResult++;
            MatrixTerm term;
            term.setRow(r);
            term.setCol(c);
            term.setValue(sum);
            setElement(LastInResult, term);
            return 0;
        } else {
            Resize(Capacity * 2);
            LastInResult++;
            MatrixTerm term;
            term.setRow(r);
            term.setCol(c);
            term.setValue(sum);
            setElement(LastInResult, term);
            return 0;
        }
    } else {
        return 0;
    }
}

SparseMatrix SparseMatrix::Multiply(const SparseMatrix& b) const {
    if (getCols() != b.getRows()) {
        std::cout << "Incompatible matrices." << std::endl;
        return EmptyMatrix();
    }
    SparseMatrix bTranspose = b.FastTranspose();
    SparseMatrix result(getRows(), b.getCols(), getNonZeroCount() * b.getNonZeroCount());
    int currRowIndex = 0, LastInResult = -1;
    int currRowBegin = 0, currRowA = getElement(0).getRow();
    
    // Create a temporary non-const copy for modification
    SparseMatrix tempThis(*this);
    SparseMatrix tempBTranspose(bTranspose);

    MatrixTerm term;
    term.setRow(getRows());
    tempThis.setElement(getNonZeroCount(), term);
    term.setRow(b.getCols());
    term.setCol(-1);
    tempBTranspose.setElement(b.getNonZeroCount(), term);
    int sum = 0;
    
    while (currRowIndex < getNonZeroCount()) { 
        int currColB = tempBTranspose.getElement(0).getRow();
        int currColIndex = 0;
        while (currColIndex <= b.getNonZeroCount()) {
            if (tempThis.getElement(currRowIndex).getRow() != currRowA) {
                result.StoreSum(sum, LastInResult, currRowA, currColB);
                sum = 0;
                currRowIndex = currRowBegin;
                while (tempBTranspose.getElement(currColIndex).getRow() == currColB)
                    currColIndex++;
                currColB = tempBTranspose.getElement(currColIndex).getRow();
            } else if (tempBTranspose.getElement(currColIndex).getRow() != currColB) {
                result.StoreSum(sum, LastInResult, currRowA, currColB);
                sum = 0;
                currRowIndex = currRowBegin;
                currColB = tempBTranspose.getElement(currColIndex).getRow();
            } else {
                if (tempThis.getElement(currRowIndex).getCol() < tempBTranspose.getElement(currColIndex).getCol()) {
                    currRowIndex++;
                } else if (tempThis.getElement(currRowIndex).getCol() == tempBTranspose.getElement(currColIndex).getCol()) {
                    sum += tempThis.getElement(currRowIndex).getValue() * tempBTranspose.getElement(currColIndex).getValue();
                    currRowIndex++;
                    currColIndex++;
                } else {
                    currColIndex++;
                }
            }
        }
        while (tempThis.getElement(currRowIndex).getRow() == currRowA) currRowIndex++;
        currRowBegin = currRowIndex;
        currRowA = tempThis.getElement(currRowIndex).getRow();
    }
    result.setRows(getRows());
    result.setCols(b.getCols());
    result.setNonZeroCount(LastInResult + 1);
    return result;
}

std::istream& operator>>(std::istream& is, SparseMatrix& matrix) {
    int rows, cols, terms;
    is >> rows >> cols >> terms;
    matrix = SparseMatrix(rows, cols);
    matrix.setNonZeroCount(terms);
    
    std::cout << "Enter " << terms << " non-zero elements (row col value):" << std::endl;
    for (int i = 0; i < terms; ++i) {
        MatrixTerm term;
        int row, col, value;
        is >> row >> col >> value;
        term.setRow(row);
        term.setCol(col);
        term.setValue(value);
        matrix.setElement(i, term);
    }
    return is;
}

std::ostream& operator<<(std::ostream& os, const SparseMatrix& matrix) {
    os << "Sparse Matrix (" << matrix.getRows() << "x" << matrix.getCols() 
       << ") with " << matrix.getNonZeroCount() << " non-zero elements:" << std::endl;
    
    for (int i = 0; i < matrix.getNonZeroCount(); ++i) {
        MatrixTerm term = matrix.getElement(i);
        os << "(" << term.getRow() << ", " << term.getCol() << "): " << term.getValue() << std::endl;
    }
    return os;
}
