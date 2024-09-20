/**
 * Solutions to Chapter 4, Section 12, Exercise 4 and 5 of Horowitz's
 * Fundamentals of Data Structures in C++.
 * 
 * Task 4: 
 * Reimplement the class definition for sparse matrices using heterogeneous
 * lists. 
 * 
 * Task 5: 
 * Reimplement the algorithm for reading in a matrix and setting up the linked
 * representation using the class definition of the previous exercise.
 */

/*********************************main.cpp*************************************/
#include <iostream>
#include "SparseMatrix.h"
#include "HeterogeneousList.h"

int main() {
    Matrix matrix;
    
    // Test operator>>: Input the sparse matrix
    std::cin >> matrix;  // This will call the overloaded operator>> to input the matrix

    // Test operator<<: Output the sparse matrix
    std::cout << "The sparse matrix is:" << std::endl;
    std::cout << matrix;  // This will call the overloaded operator<< to output the matrix

    std::cout << "End of program.\n";
    
    return 0;
}



/******************************SparseMatrix.h**********************************/
#ifndef SPARSEMATRIX_H
#define SPARSEMATRIX_H

#include <iostream>
#include "HeterogeneousList.h"  // Include the heterogeneous list

// Updated Triple structure to use heterogeneous data
struct Triple {
    int row, col;
    Data value; // Use Data to store heterogeneous types
};

class Matrix;
class MatrixNode {
    friend class Matrix;
    
    friend std::istream& operator>>(std::istream& is, Matrix& m);
    friend std::ostream& operator<<(std::ostream& os, Matrix& m);
    
  private:
    MatrixNode *down, *right;
    Boolean head;
    union {
        MatrixNode* next;
        Triple triple;
    };
    MatrixNode(Boolean b, Triple* t); // Constructor
};

typedef MatrixNode* MatrixNodePtr;

class Matrix {
    
    friend std::istream& operator>>(std::istream& is, Matrix& m);
    friend std::ostream& operator<<(std::ostream& os, Matrix& m);
    
  public:
    Matrix(); // Constructor
    Matrix(const Matrix& other); // Copy constructor
    ~Matrix(); // Destructor
    
    static Matrix Transpose(const Matrix &a);
    
  private:
    MatrixNode *headNode;
};

#endif



/*****************************SparseMatrix.cpp*********************************/
#include "SparseMatrix.h"

// Constructor for MatrixNode with heterogeneous data
MatrixNode::MatrixNode(Boolean b, Triple* t) {
    head = b;
    if (head) {
        down = right = this;
    } else {
        triple = *t;  // Assign the triple value (including heterogeneous data)
    }
}

// Matrix constructor
Matrix::Matrix() {
    // Initialize the matrix with a head node
    Triple t = { 0, 0, {0} };  // Default triple for the head node
    headNode = new MatrixNode(TRUE, &t);
}

Matrix::~Matrix() {
    if (!headNode) return; // If the matrix is empty, there's nothing to free.

    MatrixNode *rowHead = headNode->right; // Start from the first row

    while (rowHead != headNode) {
        MatrixNode *current = rowHead->right; // Start from the first element in the row
        MatrixNode *nextRow = rowHead->next;  // Store the next row before deleting current row

        // Delete all elements in the current row
        while (current != rowHead) {
            MatrixNode *temp = current;
            current = current->right;
            delete temp;
        }

        delete rowHead; // Delete the row head
        rowHead = nextRow; // Move to the next row
    }

    delete headNode; // Finally, delete the head node of the entire matrix
}


std::istream& operator>>(std::istream& is, Matrix& matrix) {
    Triple s;
    int p;
    
    // Input for row, column, and type of value
    std::cout << "Enter number of rows, columns, and the number of non-zero values: ";
    is >> s.row >> s.col >> s.value.i;  // Assuming value.i stores the number of non-zero entries
    if (s.row > s.col) 
        p = s.row;
    else 
        p = s.col;

    // Create head node for the matrix
    matrix.headNode = new MatrixNode(TRUE, nullptr);

    // If there are no non-zero elements, exit early
    if (p == 0) {
        matrix.headNode->right = matrix.headNode;
        return is;
    }

    // Create and initialize the array of head nodes
    MatrixNodePtr *head = new MatrixNodePtr[p];
    for (int i = 0; i < p; i++) {
        head[i] = new MatrixNode(TRUE, nullptr);
        head[i]->next = head[i];  // Initialize the next pointer for column head
    }

    int currentRow = 0;
    MatrixNode *last = head[0]; // last node in the current row

    // Loop to input non-zero values
    for (int i = 0; i < s.value.i; i++) {
        Triple t;
        int type; // 0 = char, 1 = int, 2 = float
        
        std::cout << "Enter row, column, value type (0 for char, 1 for int, 2 for float): ";
        is >> t.row >> t.col >> type;

        // Input the correct type based on user input
        switch (type) {
            case 0: // char
                t.value.id = 0;
                std::cout << "Enter char value: ";
                is >> t.value.c;
                break;
            case 1: // int
                t.value.id = 1;
                std::cout << "Enter int value: ";
                is >> t.value.i;
                break;
            case 2: // float
                t.value.id = 2;
                std::cout << "Enter float value: ";
                is >> t.value.f;
                break;
            default:
                std::cerr << "Invalid value type!" << std::endl;
                i--; // Retry input for this element
                continue;
        }

        // Row change handling: move to a new row if needed
        if (t.row > currentRow) {
            last->right = head[currentRow]; // Close the current row
            currentRow = t.row;             // Move to the next row
            last = head[currentRow];        // Update the last node pointer
        }

        // Insert the new node into the row list
        last = last->right = new MatrixNode(FALSE, &t);

        // Insert into the column list (initialize next pointer if it's the first node in column)
        if (head[t.col]->next == head[t.col]) {
            head[t.col]->next = last;
        } else {
            MatrixNode *colLast = head[t.col]->next;
            while (colLast->down != head[t.col]) {
                colLast = colLast->down;
            }
            colLast->down = last;
        }

        // Update the down pointer for the column head
        last->down = head[t.col];
    }

    // Close the current row list
    last->right = head[currentRow];

    // Close the column lists
    for (int i = 0; i < p; i++) {
        MatrixNode *colLast = head[i]->next;
        while (colLast->down != head[i]) {
            colLast = colLast->down;
        }
        colLast->down = head[i];
    }

    // Link all the head nodes together in a circular list
    for (int i = 0; i < p - 1; i++) {
        head[i]->next = head[i + 1];
    }
    head[p - 1]->next = matrix.headNode;
    matrix.headNode->right = head[0];

    // Cleanup dynamically allocated head array
    delete[] head;
    
    return is;
}


std::ostream& operator<<(std::ostream &os, Matrix &m) {
    if (!m.headNode) {
        os << "Empty matrix" << std::endl;
        return os;
    }
    
    MatrixNode *rowHead = m.headNode->right;
    MatrixNode *startNode = m.headNode;

    while (rowHead != startNode) {
        MatrixNode *cur = rowHead->right;
        while (cur != rowHead) {
            os << "(" << cur->triple.row << ", " << cur->triple.col << ", ";
            switch (cur->triple.value.id) {
                case 0: os << "'" << cur->triple.value.c << "'"; break;
                case 1: os << cur->triple.value.i; break;
                case 2: os << cur->triple.value.f; break;
                default: os << "Unknown type"; break;
            }
            os << ") ";
            cur = cur->right;
            os << std::endl;
        }
        //os << std::endl;
        rowHead = rowHead->next; // Use 'next' to move to the next row head
    }
    
    return os;
}


// Transpose function (example usage of heterogeneous data)
Matrix Matrix::Transpose(const Matrix &a) {
    // Implement the transpose logic, taking into account heterogeneous data types
    Matrix result;
    
    // Traverse 'a' and create transposed nodes with the same heterogeneous data
    MatrixNode* row = a.headNode->right;
    while (row != a.headNode) {
        MatrixNode* element = row->right;
        while (element != row) {
            // Create transposed element by swapping row and column
            Triple t = { element->triple.col, element->triple.row, element->triple.value };
            // Insert into the result matrix (to be implemented)
            element = element->right;
        }
        row = row->down;
    }
    
    return result;
}



/****************************HeterogeneousList.h*******************************/
#ifndef HETEROGENEOUS_LIST_H
#define HETEROGENEOUS_LIST_H

#include <iostream>

enum Boolean { FALSE, TRUE };

struct Data {
    int id; // 0 for char, 1 for int, 2 for float
    union {
        int i;
        char c;
        float f;
    };
};

// Abstract base class for a node in the list
class Node {
    friend class List;
    friend class ListIterator;
  protected:
    Node* link; // Pointer to the next node
    virtual Data GetData() = 0;
    virtual void Display() = 0;
};

// Template class for a node containing specific data type
template<typename Type>
class DerivedNode : public Node {
    friend class List;
    friend class ListIterator;
  public:
    DerivedNode(Type item); // Constructor
  private:
    Type data;
    Data GetData() override; // Function to get node's data
    void Display() override; // Function to display node's data
};

// List class for managing the nodes
class List {
    friend class ListIterator;
  public:
    List(); // Constructor
    void InsertFirst(int value); // Function to insert a node at the beginning
    void Display() const; // Function to display the list
  private:
    Node* first; // Pointer to the first node in the list
};

// Iterator class for traversing the list
class ListIterator {
  public: 
    ListIterator(const List &l); // Constructor
    Data* First(); // Get the data from the first node
    Data* Next(); // Get the data from the next node
    Boolean NotNull(); // Check if current node is not null
    Boolean NextNotNull(); // Check if next node is not null
  private:
    const List &list;
    Node* current;
    Data temp;
};

#endif // HETEROGENEOUS_LIST_H



/***************************HeterogeneousList.cpp******************************/
#include "HeterogeneousList.h"

// List constructor
List::List() : first(nullptr) {}

// Insert a new node at the beginning of the list
void List::InsertFirst(int value) {
    Node* newNode = new DerivedNode<int>(value); // Create a new node
    newNode->link = first; // Set the link to the current first node
    first = newNode; // Update the first pointer to the new node
}

// Display the contents of the list
void List::Display() const {
    ListIterator it(*this); // Create an iterator for this list
    for (Data* data = it.First(); it.NotNull(); data = it.Next()) {
        switch (data->id) {
            case 0: // char
                std::cout << data->c << " ";
                break;
            case 1: // int
                std::cout << data->i << " ";
                break;
            case 2: // float
                std::cout << data->f << " ";
                break;
        }
    }
    std::cout << std::endl;
}

// ListIterator constructor
ListIterator::ListIterator(const List &l) : list(l), current(l.first) {}

// Check if current node is not null
Boolean ListIterator::NotNull() {
    return current ? TRUE : FALSE;
}

// Check if the next node is not null
Boolean ListIterator::NextNotNull() {
    return (current && current->link) ? TRUE : FALSE;
}

// Get the data from the first node
Data* ListIterator::First() {
    if (list.first) {
        temp = list.first->GetData(); // Retrieve the data from the first node
        return &temp;
    }
    return nullptr;
}

// Get the data from the next node
Data* ListIterator::Next() {
    if (current) {
        current = current->link; // Move to the next node
        if (current) {
            temp = current->GetData(); // Retrieve the data
            return &temp;
        }
    }
    return nullptr;
}

// Constructor for DerivedNode
template<typename Type>
DerivedNode<Type>::DerivedNode(Type item) : data(item) {
    link = nullptr; // Initialize link to nullptr
}

// Display function for DerivedNode
template<typename Type>
void DerivedNode<Type>::Display() {
    std::cout << data << " "; // Print the data
}

// Specialization for char type
template<>
Data DerivedNode<char>::GetData() {
    Data t;
    t.id = 0;
    t.c = data; // Assign the char data
    return t;
}

// Specialization for int type
template<>
Data DerivedNode<int>::GetData() {
    Data t;
    t.id = 1;
    t.i = data; // Assign the int data
    return t;
}

// Specialization for float type
template<>
Data DerivedNode<float>::GetData() {
    Data t;
    t.id = 2;
    t.f = data; // Assign the float data
    return t;
}
