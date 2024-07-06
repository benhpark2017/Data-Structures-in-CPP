/**
 * Solutions to Chapter 2, Section 1, Exercise 1 of Horowitz's
 * Data Structures in C++.
 */

/****************************main.cpp********************************/
#include <iostream>
#include "Rectangle.h"

int main(){
    Rectangle r;  // Use default constructor
    Rectangle s(0, 0, 5, 10); // Initialize with some values
    Rectangle *t = &s;
    
    if (r.getHeight() * r.getWidth() > t->getHeight() * t->getWidth())
        std::cout << "r";
    else std::cout << "s";
    std::cout << " has the greater area." << std::endl;

    return 0;
}

/**************************Rectangle.h*******************************/
#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <iostream> // Include this for ostream

class Rectangle {
  public:
    Rectangle(); // Default constructor
    Rectangle(int x, int y, int height, int width); // Parameterized constructor
    ~Rectangle(); // Destructor

    int getHeight();
    int getWidth();
    int operator==(const Rectangle& s);

    // Declare the friend function for operator<<
    friend std::ostream& operator<<(std::ostream& os, const Rectangle& r);
    friend bool operator<(const Rectangle& r, const Rectangle& s);
    
  private:
    int x1, y1, h, w;
};

#endif


/**************************Rectangle.cpp****************************/
#include "Rectangle.h"
#include <iostream>

// Define the constructors
Rectangle::Rectangle() {
    x1 = 0;
    y1 = 0;
    h = 0;
    w = 0;
}

Rectangle::Rectangle(int x, int y, int height, int width) {
    x1 = x;
    y1 = y;
    h = height;
    w = width;
}

// Define the destructor
Rectangle::~Rectangle() {
    // Destructor definition (empty in this case)
}

// Define the member functions
int Rectangle::getHeight() { 
    return h; 
}

int Rectangle::getWidth() { 
    return w; 
}

int Rectangle::operator==(const Rectangle& s) {
    if (this == &s) return 1;
    if ((x1 == s.x1) && (y1 == s.y1) && (h == s.h) && (w == s.w)) return 1;
    else return 0;
}

// Define the friend function for operator<
bool operator<(const Rectangle& r, const Rectangle& s) {
    if (r.h * r.w < s.h * s.w) return 1;
    else return 0;
}

// Define the friend function for operator<<
std::ostream& operator<<(std::ostream& os, const Rectangle& r) {
    os << "Position is " << r.x1 << " " << r.y1 << std::endl;
    os << "Height is " << r.h << std::endl;
    os << "Width is " << r.w << std::endl;
    return os;
}
