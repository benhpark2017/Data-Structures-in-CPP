/**
 * Solutions to Chapter 2, Section 8, Exercise 5 of Horowitz's
 * Fundamentals of Data Structure in C++.
 */

/*********************************main.cpp************************************/
#include "MatrixStorage.h"
#include <iostream>
#include <ctime>

int main() {
    int N;
    srand(static_cast<unsigned>(time(nullptr)));

    std::cout << "Enter the size of the square matrix: ";
    std::cin >> N;

    MatrixStorage storage(N);

    storage.initializeMatrices();

    std::cout << "Printing matrix A:" << std::endl;
    storage.printMatrix(storage.a, N, N);

    std::cout << std::endl << "Printing matrix B:" << std::endl;
    storage.printMatrix(storage.b, N, N);

    storage.combineMatrices();

    std::cout << std::endl << "Combined matrix C:" << std::endl;
    storage.printCombinedMatrix();
    
    std::cout << std::endl << "Tracking addresses:" << std::endl;
    int i = 1, j = 1;  // Example indices
    int* addrA = storage.getAddressA(i, j);
    int* addrB = storage.getAddressB(i, j);

    if (addrA) {
        std::cout << "A[" << i << "][" << j << "] is stored at C[" 
                  << N - i - 1 << "]" << "[" << N - j <<"] " << std::endl;
        std::cout << "Value: " << *addrA << std::endl;
    }

    if (addrB) {
        std::cout << "B[" << i << "][" << j << "] is stored at C[" 
                  << i << "]" << "[" << j <<"] " << std::endl;
        std::cout << "Value: " << *addrB << std::endl;
    }

    return 0;
}


/******************************MatrixStorage.h***********************************/
#ifndef MATRIX_STORAGE_H
#define MATRIX_STORAGE_H

class MatrixStorage {
private:
    int N;

public:
    int **a, **b, **c;
    MatrixStorage(int size);
    ~MatrixStorage();

    int* getAddressA(int i, int j);
    int* getAddressB(int i, int j);
    void initializeMatrices();
    void printMatrix(int** matrix, int rows, int cols);
    void combineMatrices();
    void printCombinedMatrix();
};

#endif



/*****************************MatrixStorage.cpp**********************************/
#include "MatrixStorage.h"
#include <iostream>
#include <cstdlib>

MatrixStorage::MatrixStorage(int size) : N(size) {
    a = new int*[N];
    b = new int*[N];
    c = new int*[N];

    for (int i = 0; i < N; i++) {
        a[i] = new int[i + 1];
        b[i] = new int[i + 1];
        c[i] = new int[N + 1];
    }
}

MatrixStorage::~MatrixStorage() {
    for (int i = 0; i < N; i++) {
        delete[] a[i];
        delete[] b[i];
        delete[] c[i];
    }
    delete[] a;
    delete[] b;
    delete[] c;
}

void MatrixStorage::initializeMatrices() {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j <= i; j++) {
            a[i][j] = rand() % 100 + 1;
            b[i][j] = rand() % 100 + 1;
        }
        for (int j = 0; j <= N; j++) {
            c[i][j] = 0;
        }
    }
}

void MatrixStorage::printMatrix(int** matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            std::cout << matrix[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

void MatrixStorage::combineMatrices() {
    int cnt = N - 1, num = N;
    for (int i = 0; i < N; i++) {
        for (int j = i + 1; j < (N + 1); j++) {
            if (cnt >= 0) {
                c[i][j] = a[num - 1][cnt];
                cnt -= 1;
            } else {
                num -= 1;
                cnt = (N - 1) - i;
                c[i][j] = a[num - 1][cnt];
                cnt -= 1;
            }
        }
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < i + 1; j++) {
            c[i][j] = b[i][j];
        }
    }
}

void MatrixStorage::printCombinedMatrix() {
    printMatrix(c, N, N + 1);
}

int* MatrixStorage::getAddressA(int i, int j) {
    if (i < 0 || i >= N || j < 0 || j > i) {
        return nullptr;  // Invalid indices
    }
    // A is stored in the upper-right triangle of C, flipped upside down
    return &c[N - 1 - i][N - j];
}

int* MatrixStorage::getAddressB(int i, int j) {
    if (i < 0 || i >= N || j < 0 || j > i) {
        return nullptr;  // Invalid indices
    }
    // B is stored in the lower-left triangle of C
    return &c[i][j];
}