/**
 * Solutions to Chapter 2, Section 3, Exercise 1 of Horowitz's
 * Data Structures in C++.
 */

/***************************OrderedList.h******************************/
#ifndef ORDEREDLIST_H
#define ORDEREDLIST_H
#include <iostream>

class OrderedList {

  private:
    int *arr;
	int length;

  public:
    // Destructor
    virtual ~OrderedList() {}

    // Find the length of the list
    virtual int length() const = 0;

    // Read the list from left to right
    virtual void readLeftToRight() const = 0;

    // Read the list from right to left
    virtual void readRightToLeft() const = 0;

    // Retrieve the ith element, 0 <= i < n
    virtual int retrieve(int i) const = 0;

    // Store a new value into the ith position, 0 <= i < n
    virtual void store(int i, int value) = 0;

    // Insert a new element at the position i, 0 <= i < n
    virtual void insert(int i, int value) = 0;

    // Delete the element at position i, 0 <= i < n
    virtual void remove(int i) = 0;
};

#endif
/***********************End of OrderedList.h**************************/