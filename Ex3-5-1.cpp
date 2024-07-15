/**
 * Solutions to Chapter 3, Section 5, Exercise 1 of Horowitz's
 * Fundamentals of Data Structures in C++.
 */
 
/************************************main.cpp*************************************/
#include "Maze.h"
#include <iostream>

int main() {
    std::cout << "Maze Layout" << std::endl;
    for (int i = 0; i < 13; i++) {
        for (int j = 0; j < 17; j++)
            std::cout << maze[i][j] << " ";
        std::cout << std::endl;
    }
    std::cout << std::endl;

    std::cout << "Coordinate reading format: (row, column, direction)" << std::endl;
    Path();

    return 0;
}



/************************************Stack.h**************************************/
#ifndef STACK_H
#define STACK_H

#include <iostream>

template <class KeyType>
class Stack {
private:
    KeyType* stack;    // Stack array
    int top;           // Index of the top element
    int capacity;      // Capacity of the stack

public:
    // Constructor with default capacity of 10
    Stack(int stackCapacity = 10);

    // Destructor
    ~Stack();

    // Check if the stack is empty
    bool IsEmpty() const;

    // Get the top element of the stack
    KeyType& Top() const;

    // Push an element onto the stack
    void Push(const KeyType& x);

    // Pop the top element from the stack
    void Pop();

    // Friend function to print the stack
    template <class KT>
    friend std::ostream& operator<<(std::ostream& os, Stack<KT>& s);
};

template <class KeyType>
void ChangeSize1D(KeyType*& a, const int oldSize, const int newSize);

#include "Stack.cpp"

#endif // STACK_H




/***********************************Stack.cpp*************************************/
#ifndef STACK_CPP
#define STACK_CPP

#include "Stack.h"
#include <algorithm>

template <class KeyType>
Stack<KeyType>::Stack(int stackCapacity) : capacity(stackCapacity) {
    if (capacity < 1)
        throw "Stack capacity must be greater than 0";
    stack = new KeyType[capacity];
    top = -1;
}

template <class KeyType>
Stack<KeyType>::~Stack() {
    delete[] stack;
}

template <class KeyType>
bool Stack<KeyType>::IsEmpty() const {
    return top == -1;
}

template <class KeyType>
KeyType& Stack<KeyType>::Top() const {
    if (IsEmpty())
        throw "Stack is empty";
    return stack[top];
}

template <class KeyType>
void Stack<KeyType>::Push(const KeyType& x) {
    if (top == capacity - 1) {
        ChangeSize1D(stack, capacity, 2 * capacity);
        capacity *= 2;
    }
    stack[++top] = x;
}

template <class KeyType>
void Stack<KeyType>::Pop() {
    if (IsEmpty())
        throw "Stack is empty. Can't Delete";
    stack[top--].~KeyType();
}

template <class KeyType>
std::ostream& operator<<(std::ostream& os, Stack<KeyType>& s) {
    os << "Current Top: " << s.Top() << std::endl;
    for (int i = 0; i <= s.top; i++)
        os << i + 1 << ": " << s.stack[i] << std::endl;
    return os;
}

template <class KeyType>
void ChangeSize1D(KeyType*& a, const int oldSize, const int newSize) {
    if (newSize < 0)
        throw "New length must be >= 0";
    KeyType* temp = new KeyType[newSize];
    int number = std::min(oldSize, newSize);
    std::copy(a, a + number, temp);
    delete[] a;
    a = temp;
}

//Explicit instantiation of Stack class template
template class Stack<int>;

#endif //STACK_CPP




/************************************Maze.h***************************************/
#ifndef MAZE_H
#define MAZE_H

#include "Stack.h"

struct Items {
    int x, y, dir;
    Items() {};  // Default constructor
    Items(int a, int b, int d) : x(a), y(b), dir(d) {}
};

extern int maze[13][17];

// Overloaded << operator to print Items
std::ostream& operator<<(std::ostream& os, Items& item);

// Function to find a path through the maze
void Path();

#endif // MAZE_H



/***********************************Maze.cpp**************************************/
#ifndef MAZE_CPP
#define MAZE_CPP

#include "Maze.h"
#include <iostream>

enum directions { N, NE, E, SE, S, SW, W, NW };

// Struct to represent movement offsets
typedef struct {
    int a, b;
} offsets;

// Array of movement offsets corresponding to directions
offsets moveto[8] = {
    {-1, 0}, {-1, 1}, {0, 1}, {1, 1},
    {1, 0}, {1, -1}, {0, -1}, {-1, -1}
};

// Maze and mark arrays
int mark[13][17] = { 0 };

// Maze array as seen in Figure 3.11: 
int maze[13][17] = {
    { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
    { 1, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1 },
    { 1, 1, 0, 0, 0, 1, 1, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1 },
    { 1, 0, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1 },
    { 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 0, 1 },
    { 1, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1 },
    { 1, 0, 0, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1 },
    { 1, 0, 0, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1 },
    { 1, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1 },
    { 1, 1, 1, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 1 },
    { 1, 0, 1, 0, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1 },
    { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
};

// Overloaded << operator to print Items
std::ostream& operator<<(std::ostream& os, Items& item) {
    os << item.x << ", " << item.y << ", ";
    switch (item.dir) {
        case 1: os << "N"; break;
        case 2: os << "NE"; break;
        case 3: os << "E"; break;
        case 4: os << "SE"; break;
        case 5: os << "S"; break;
        case 6: os << "SW"; break;
        case 7: os << "W"; break;
        case 8: os << "NW"; break;
    }
    return os;
}

// Function to find a path through the maze
void Path() {
    const int m = 11; // Rows in the maze (excluding walls)
    const int p = 15; // Columns in the maze (excluding walls)

    // Starting position at (1, 1)
    mark[1][1] = 1;
    Stack<Items> stack(m * p);
    Items temp(1, 1, E);
    stack.Push(temp);

    while (!stack.IsEmpty()) {
        temp = stack.Top();
        stack.Pop();
        int i = temp.x;
        int j = temp.y;
        int d = temp.dir;

        while (d < 8) {
            int g = i + moveto[d].a; // Move to the next position
            int h = j + moveto[d].b;

            if ((g == m) && (h == p)) { // Exit found
                std::cout << stack;
                std::cout << "Last coordinate before exit: " << i << " " << j << std::endl;
                std::cout << "Exit coordinate: " << m << " " << p << std::endl;
                return;
            }

            if ((!maze[g][h]) && (!mark[g][h])) { // Valid move
                mark[g][h] = 1; // Mark the position
                temp.x = i; // Save current position
                temp.y = j;
                temp.dir = d + 1; // Move to the next direction in the stack
                stack.Push(temp);
                i = g; // Move to the next position
                j = h;
                d = N;
            } else {
                d++; // Try the next direction
            }
        }
    }

    std::cout << "No path found in the maze" << std::endl;
}

#endif //MAZE_CPP