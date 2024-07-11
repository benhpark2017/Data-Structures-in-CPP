/**
 * Solutions to Chapter 2, Section 8, Exercise 10 of Horowitz's
 * Fundamentals of Data Structures in C++.
 */
 
/**************************************main.cpp****************************************/
#include <iostream>
#include "ChessMoves.h"

int main() {
    ChessMoves chessMoves;

    // Initialize the board with all zeros
    int board[N][N] = {0};

    // Possible moves a knight can make
    int moveKnightTraversal[8][2] = {
        {2, 1}, {1, 2}, {-1, 2}, {-2, 1},
        {-2, -1}, {-1, -2}, {1, -2}, {2, -1}
    };

    // Set the knight's possible moves
    chessMoves.setKnightTraversal(moveKnightTraversal);

    // Set the initial position of the knight
    int initialX = 0;
    int initialY = 0;
    chessMoves.setX(initialX);
    chessMoves.setY(initialY);
    board[initialX][initialY] = 1; // Mark the initial position on the board
    chessMoves.setBoard(board);

    // Run the knight's traversal
    chessMoves.knightTraversal();

    return 0;
}



/***********************************ChessMoves.h*************************************/
#ifndef CHESSMOVES_H
#define CHESSMOVES_H

#include <iostream>

#define N 8

class ChessMoves {
  private:
    int board[N][N];
    int moveKnightTraversal[8][2]; // Assuming it stores possible knight moves
    int x;
    int y;
    
  public:
    // Getters
    int (&getBoard())[N][N]; // Return a reference to a 2D array
    int (*getKnightTraversal())[2]; // Return a pointer to an array of knight moves
    int getX() const;
    int getY() const;
    
    // Setters
    void setBoard(int newBoard[N][N]);
    void setKnightTraversal(int newTraversal[8][2]);
    void setX(int newX);
    void setY(int newY);
    
    // Other methods
    void PrintBoard() const;
    bool isPossibleMove(int nextX, int nextY) const; // Renamed from MovePossible
    bool findTraversal(int moveCount);
    void knightTraversal();
};

#endif // CHESSMOVES_H



/**********************************ChessMoves.cpp************************************/
#include <iomanip>
#include "ChessMoves.h"

// Getters
int (&ChessMoves::getBoard())[N][N] {
    return board;
}

int (*ChessMoves::getKnightTraversal())[2] {
    return moveKnightTraversal;
}

int ChessMoves::getX() const {
    return x;
}

int ChessMoves::getY() const {
    return y;
}

// Setters
void ChessMoves::setBoard(int newBoard[N][N]) {
    for(int i = 0; i < N; ++i)
        for(int j = 0; j < N; ++j)
            board[i][j] = newBoard[i][j];
}

void ChessMoves::setKnightTraversal(int newTraversal[8][2]) {
    for(int i = 0; i < 8; ++i) {
        moveKnightTraversal[i][0] = newTraversal[i][0];
        moveKnightTraversal[i][1] = newTraversal[i][1];
    }
}

void ChessMoves::setX(int newX) {
    x = newX;
}

void ChessMoves::setY(int newY) {
    y = newY;
}

// Other methods
void ChessMoves::PrintBoard() const {
    for(int i = 0; i < N; ++i) {
        for(int j = 0; j < N; ++j) {
            std::cout << std::setw(3) << board[i][j]  << " ";
        }
        std::cout << std::endl;
    }
}

bool ChessMoves::isPossibleMove(int nextX, int nextY) const {
    if(nextX >= 0 && nextX < N && nextY >= 0 && nextY < N && board[nextX][nextY] == 0)
        return true;
    return false;
}

bool ChessMoves::findTraversal(int moveCount) {
    if (moveCount == N * N - 1)
        return true;
    
    for (int i = 0; i < 8; i++) {
        int nextX = x + moveKnightTraversal[i][0];
        int nextY = y + moveKnightTraversal[i][1];

        if (isPossibleMove(nextX, nextY)) {
            board[nextX][nextY] = moveCount + 1;
            int oldX = x, oldY = y; // Save current position
            x = nextX;
            y = nextY;

            if (findTraversal(moveCount + 1))
                return true;
            else {
                board[nextX][nextY] = 0; // Backtrack
                x = oldX; // Restore previous position
                y = oldY;
            }
        }
    }
    return false;
}

void ChessMoves::knightTraversal() {
    x = 0;
    y = 0;
    board[x][y] = 1;
    if (!findTraversal(1)) {
        std::cout << "No solution found!" << std::endl;
    }
    PrintBoard();
}

