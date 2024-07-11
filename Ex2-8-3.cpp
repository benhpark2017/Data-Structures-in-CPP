/**
 * Solutions to Chapter 2, Section 8, Exercise 3 of Horowitz's 
 * Fundamentals of Data Structure in C++. 
 */
 
#include <iostream>
#define MAX_ROW 100
#define MAX_COL 100

bool isSaddlePoint(int matrix[MAX_ROW][MAX_COL], int n) {
    for (int i = 0; i < n; i++) {
        int minRow = matrix[i][0];
        int col = 0;
        for (int j = 1; j < n; j++) {
            if (minRow > matrix[i][j]){
                minRow = matrix[i][j];
            }
        }
        int k = 0;
        for (k = 0; k < n; k++)
            if (minRow < matrix[k][col]) break;
        if (k == n) {
            std::cout << "Saddle Point: " << minRow << std::endl;
            return true;
        }
    }
    return false;
}

int main() {
    int matrix[MAX_ROW][MAX_COL] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    
    std::cout << "Printing the 2-dimensional matrix:" << std::endl;
    
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++)
            std::cout << matrix[i][j] << " ";
        std::cout << std::endl;
    }
    
    if (isSaddlePoint(matrix, 3) == false) {
        std::cout << "There exists no saddle point." << std::endl;
    }

    std::cout << std::endl;
    return 0;
}