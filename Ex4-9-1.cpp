/**
 * Solutions to Chapter 4, Section 9, Exercise 1 of Horowitz's 
 * Fundamentals of Data Structure in C++.
 */

/********************************main.cpp**************************************/
#include "Deque.tpp"
#include <iostream>

// Example usage
int main() {
    Deque<int> deque;
    deque.insertFront(1);
    deque.insertRear(2);
    deque.insertFront(0);
    deque.display(); // Output: 0 1 2

    deque.removeFront();
    deque.display(); // Output: 1 2

    deque.removeRear();
    deque.display(); // Output: 1

    return 0;
}



/********************************Deque.tpp*************************************/
#include <iostream>
#include <stdexcept>

// Node class for doubly linked list
template <class KeyType>
class Node {
public:
    KeyType data;
    Node* next;
    Node* prev;

    Node(KeyType value) : data(value), next(nullptr), prev(nullptr) {}
};

// Deque class definition
template <class KeyType>
class Deque {
private:
    Node<KeyType>* front;
    Node<KeyType>* rear;
    int size;

public:
    Deque() : front(nullptr), rear(nullptr), size(0) {}

    ~Deque() {
        while (!isEmpty()) {
            removeFront();
        }
    }

    bool isEmpty() const {
        return size == 0;
    }

    int getSize() const {
        return size;
    }

    void insertFront(KeyType value) {
        Node<KeyType>* newNode = new Node<KeyType>(value);
        if (isEmpty()) {
            front = rear = newNode;
        } else {
            newNode->next = front;
            front->prev = newNode;
            front = newNode;
        }
        ++size;
    }

    void insertRear(KeyType value) {
        Node<KeyType>* newNode = new Node<KeyType>(value);
        if (isEmpty()) {
            front = rear = newNode;
        } else {
            newNode->prev = rear;
            rear->next = newNode;
            rear = newNode;
        }
        ++size;
    }

    void removeFront() {
        if (isEmpty()) {
            throw std::out_of_range("Deque is empty");
        }
        Node<KeyType>* temp = front;
        front = front->next;
        if (front) {
            front->prev = nullptr;
        } else {
            rear = nullptr;
        }
        delete temp;
        --size;
    }

    void removeRear() {
        if (isEmpty()) {
            throw std::out_of_range("Deque is empty");
        }
        Node<KeyType>* temp = rear;
        rear = rear->prev;
        if (rear) {
            rear->next = nullptr;
        } else {
            front = nullptr;
        }
        delete temp;
        --size;
    }

    KeyType getFront() const {
        if (isEmpty()) {
            throw std::out_of_range("Deque is empty");
        }
        return front->data;
    }

    KeyType getRear() const {
        if (isEmpty()) {
            throw std::out_of_range("Deque is empty");
        }
        return rear->data;
    }

    void display() const {
        Node<KeyType>* current = front;
        while (current) {
            std::cout << current->data << " ";
            current = current->next;
        }
        std::cout << std::endl;
    }
};
