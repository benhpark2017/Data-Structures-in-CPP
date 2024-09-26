/**
 * Solutions to Chapter 5, Section 3, Exercises 11 to 13 of Horowitz's
 * Fundamentals of Data Structures in C++.
 * 
 * Task 11:
 * The NoStackInorder() function performs an inorder traversal without using
 * threads, a stack, or parent field. Verify that the algorithm is correct
 * by running it on a variety of binary trees that cause every statement to
 * execute at least once.
 * 
 * Task 12:
 * Write a nonrecursive version of function postorder() function using only
 * a fixed amount of additional space, using the ideas of the previous
 * exercise.
 * 
 * Task 13:
 * Do the preceding exercise for the case of preorder().
 * 
 * For complete binary tree:
 * In-order should print H D I B E A F C G 
 * Pre-order should print A B D H I E C F G
 * Post-order should print H I D E B F G C A 
 * Level-order should print A B C D E F G H I
 * 
 * For notation tree:
 * In-order should print A / B * C * D + E
 * Pre-order should print + * * / A B C D E
 * Post-order should print A B / C * D * E +
 * Level-order should print + * E * D / C A B
 * 
 */

/*********************************main.cpp*************************************/
#include "Tree.h"
#include <iostream>

int main() {
    
    //Setting up two different trees
    Tree binaryTree;
    Tree notationTree;

    //Setting up the complete binary tree
    binaryTree.insert('A');                // Root (Level 1)
    binaryTree.insert('B');                // Level 2
    binaryTree.insert('C');                // Level 2
    binaryTree.insert('D');                // Level 3
    binaryTree.insert('E');                // Level 3
    binaryTree.insert('F');                // Level 3
    binaryTree.insert('G');                // Level 3
    binaryTree.insert('H');                // Level 4
    binaryTree.insert('I');                // Level 4

    //Setting up the notation binary tree
    // Manually build the tree structure for the expression A / B * C * D + E
    TreeNode* plus = new TreeNode('+');    // Root
    TreeNode* multiply1 = new TreeNode('*');
    TreeNode* multiply2 = new TreeNode('*');
    TreeNode* divide = new TreeNode('/');
    TreeNode* A = new TreeNode('A');
    TreeNode* B = new TreeNode('B');
    TreeNode* C = new TreeNode('C');
    TreeNode* D = new TreeNode('D');
    TreeNode* E = new TreeNode('E');

    // Build the tree
    plus->leftChild = multiply1;
    plus->rightChild = E;

    multiply1->leftChild = multiply2;
    multiply1->rightChild = D;

    multiply2->leftChild = divide;
    multiply2->rightChild = C;

    divide->leftChild = A;
    divide->rightChild = B;

    // Set the root of the tree
    notationTree.setRoot(plus);

    
    // Display in-order traversal without using stacks, threads, or parent field
    std::cout << "In-order traversal of the complete binary tree without stacks: ";
    binaryTree.NoStackInorder();
    std::cout << std::endl;

    //Testing implementation of postorder traversal without stacks
    std::cout << "Post-order traversal of the complete binary tree without stacks: ";
    binaryTree.NoStackPostorder();
    std::cout << std::endl;

    std::cout << std::endl;

    // Display in-order traversal to confirm tree structure
    std::cout << "In-order traversal of the notation tree without stacks: ";
    notationTree.NoStackInorder();
    std::cout << std::endl;

    //Testing implementation of postorder traversal without stacks
    std::cout << "Post-order traversal of the notation tree without stacks: ";
    notationTree.NoStackPostorder();
    std::cout << std::endl;

    std::cout << "\nEnd of program.\n";

    return 0;
}



/*********************************Tree.h*************************************/
#ifndef TREE_H
#define TREE_H

#include "Stack.h"
#include "Queue.h"

// TreeNode Class Definition
class TreeNode {
public:
    char data;             // Character data type
    TreeNode* leftChild;   // Pointer to the left child
    TreeNode* rightChild;  // Pointer to the right child

    // Constructor to initialize the node with character data
    TreeNode(char c) : data(c), leftChild(nullptr), rightChild(nullptr) {}
};

// Tree Class Definition
class Tree {
public:
    Tree();                   // Constructor
    Tree(const Tree& other);  // Copy constructor
    ~Tree();                  // Destructor

    void insert(char data);   // Insert function to insert character data

    // Solution to Exercise 11
    void NoStackInorder();
    
    // Solution to Exercise 12
    void NoStackPostorder();

    void setRoot(TreeNode* node);   // Set the root manually
    TreeNode* getRoot() { return root; }

private:
    TreeNode* root;  // Pointer to the root of the tree

    // Helper functions
    void copyTree(TreeNode*& thisRoot, TreeNode* otherRoot);
    void deleteTree(TreeNode* node);
};

#endif //TREE_H



/*********************************Tree.cpp************************************/
#include "Tree.h"
#include "Stack.h"
#include "Queue.h"

Tree::Tree() : root(nullptr) {}

Tree::Tree(const Tree& other) : root(nullptr) {
    copyTree(this->root, other.root);
}

Tree::~Tree() {
    deleteTree(root);
}

void Tree::insert(char data) {
    TreeNode* newNode = new TreeNode(data);  // Create a new node

    if (root == nullptr) {  // If the tree is empty
        root = newNode;     // Set the new node as the root
        return;
    }

    Queue<TreeNode*> q;    // Using custom Queue template for TreeNode*
    q.enqueue(root);       // Enqueue the root node

    while (!q.isEmpty()) {
        TreeNode* temp;
        q.dequeue(temp);    // Dequeue the front node

        // Check for the left child
        if (temp->leftChild == nullptr) {
            temp->leftChild = newNode;  // Insert new node as left child
            return;  // Exit after insertion
        } else {
            q.enqueue(temp->leftChild);  // Enqueue the left child for further traversal
        }

        // Check for the right child
        if (temp->rightChild == nullptr) {
            temp->rightChild = newNode;  // Insert new node as right child
            return;  // Exit after insertion
        } else {
            q.enqueue(temp->rightChild);  // Enqueue the right child for further traversal
        }
    }
}

void Tree::setRoot(TreeNode* node) {
    root = node;
}

void Tree::copyTree(TreeNode*& thisRoot, TreeNode* otherRoot) {
    if (otherRoot == nullptr) {
        thisRoot = nullptr;
    } else {
        thisRoot = new TreeNode(otherRoot->data);
        copyTree(thisRoot->leftChild, otherRoot->leftChild);
        copyTree(thisRoot->rightChild, otherRoot->rightChild);
    }
}

void Tree::deleteTree(TreeNode* node) {
    if (node != nullptr) {
        deleteTree(node->leftChild);
        deleteTree(node->rightChild);
        delete node;
    }
}

//Inorder traversal of binary tree using a fixed amount of additional storage
void Tree::NoStackInorder() {
	if (!root) return; //empty binary tree
	TreeNode *top = nullptr;
	TreeNode *LastRight = nullptr;
	TreeNode *p, *q, *r, *r1;
	p = q = root;
	
	while (true) {
		while (true) {
			//leaf node
			if ((!p->leftChild) && (!p->rightChild)) {
				std::cout << p->data << " ";
				break;
			}
			//visit p and move to p->rightChild
			if (!p->leftChild) {
				std::cout << p->data << " ";
				r = p->rightChild;
				p->rightChild = q;
				q = p;
				p = r;
			} else { //move to p->leftChild
				r = p->leftChild;
				p->leftChild = q;
				q = p;
				p = r;
			}
		} // end of inner while
		//p is a leaf node, move upward to a node whose right subtree
		//has not yet been examined
		TreeNode* av = p;
		while (true) {
			if (p == root) return;
			if (!q->leftChild) { // q is linked via rightChild
				r = q->rightChild;
				q->rightChild = p;
				p = q;
				q = r;
			} else if (!q->rightChild) {
				r = q->rightChild;
				q->rightChild = p;
				p = q;
				q = r;
				std::cout << p->data << " ";
			} else {
				if (q == LastRight) {
					r = top;
					LastRight = r->leftChild;
					top = r->rightChild; //unstack
					r->leftChild = r->rightChild = nullptr;
					r = q->rightChild;
					q->rightChild = p;
					p = q;
					q = r;
				} else { // if p is leftChild of q
					std::cout << q->data << " "; // visit q
					av->leftChild = LastRight;
					av->rightChild = top;
					top = av;
					LastRight = q;
					r = q->leftChild;
					q->leftChild = p; //restore link to p
					r1 = q->rightChild;
					q->rightChild = r;
					p = r1;
					break;
				} // end inner if-else
			} // end outer if-else
		} // end inner while (true)
	} //end outer while (true)
} // end of NoStackInorder()

//Postorder traversal of binary tree using a fixed amount of additional storage
void Tree::NoStackPostorder() {
    if (root == nullptr) return;

    TreeNode dummy(0);  // Create a dummy node
    dummy.leftChild = root;
    TreeNode* current = &dummy;
    TreeNode* prev = nullptr;

    while (current != nullptr) {
        if (current->leftChild == nullptr) {
            current = current->rightChild;
        } else {
            prev = current->leftChild;
            while (prev->rightChild != nullptr && prev->rightChild != current) {
                prev = prev->rightChild;
            }

            if (prev->rightChild == nullptr) {
                prev->rightChild = current;
                current = current->leftChild;
            } else {
                // Start of printReverse functionality
                TreeNode* from = current->leftChild;
                TreeNode* to = prev;

                // Start of reverseTree functionality
                if (from != to) {
                    TreeNode* x = from;
                    TreeNode* y = from->rightChild;
                    TreeNode* z;
                    while (x != to) {
                        z = y->rightChild;
                        y->rightChild = x;
                        x = y;
                        y = z;
                    }
                }
                // End of reverseTree functionality

                TreeNode* node = to;
                while (true) {
                    std::cout << node->data << " ";
                    if (node == from) break;
                    node = node->rightChild;
                }

                // Start of reverseTree functionality (again)
                if (from != to) {
                    TreeNode* x = to;
                    TreeNode* y = to->rightChild;
                    TreeNode* z;
                    while (x != from) {
                        z = y->rightChild;
                        y->rightChild = x;
                        x = y;
                        y = z;
                    }
                }

                prev->rightChild = nullptr;
                current = current->rightChild;
            }
        }
    }
}

/*********************************Queue.h************************************/
#ifndef QUEUE_H
#define QUEUE_H

// Template class for the queue node
template <typename T>
class QueueNode {
public:
    T data;              // The data stored in the queue node
    QueueNode* next;     // Pointer to the next node in the queue

    // Constructor
    QueueNode(T nodeData) : data(nodeData), next(nullptr) {}
};

// Template class for the queue
template <typename T>
class Queue {
public:
    Queue();              // Constructor
    ~Queue();             // Destructor

    void enqueue(T node); // Add an element to the queue
    T dequeue(T& node); // Remove an element from the queue and return pointer to deleted element
    T front() const;      // Return the front element
    bool isEmpty() const; // Check if the queue is empty

private:
    QueueNode<T>* frontNode;  // Points to the front of the queue
    QueueNode<T>* rearNode;   // Points to the rear of the queue
};

#include "Queue.tpp"  // Include the template implementation

#endif



/*********************************Queue.tpp***********************************/
#include <iostream>

// Constructor
template <typename T>
Queue<T>::Queue() : frontNode(nullptr), rearNode(nullptr) {}

// Destructor
template <typename T>
Queue<T>::~Queue() {
    while (!isEmpty()) {
        T temp;  // Declare a variable to hold the dequeued data
        dequeue(temp);  // Dequeue the node and clean up
    }
}

// Add an element to the queue (enqueue operation)
template <typename T>
void Queue<T>::enqueue(T node) {
    QueueNode<T>* newNode = new QueueNode<T>(node);
    if (isEmpty()) {
        frontNode = rearNode = newNode;
    } else {
        rearNode->next = newNode;
        rearNode = newNode;
    }
}

// Remove an element from the queue (dequeue operation)
template <typename T>
T Queue<T>::dequeue(T& node) {  // Change to take T& instead of T
    if (isEmpty()) {
        node = nullptr;  // Set the node to nullptr if the queue is empty
        return nullptr;  // Return nullptr for pointer types if queue is empty
    }
    QueueNode<T>* temp = frontNode;
    frontNode = frontNode->next;
    
    if (frontNode == nullptr) {  // If queue becomes empty
        rearNode = nullptr;
    }
    
    node = temp->data;  // Get the data from the dequeued node
    delete temp;        // Delete the node
    return node;        // Return the data
}

// Return the front element
template <typename T>
T Queue<T>::front() const {
    if (!isEmpty()) {
        return frontNode->data;
    }
    return nullptr;  // Return nullptr for pointer types if queue is empty
}

// Check if the queue is empty
template <typename T>
bool Queue<T>::isEmpty() const {
    return frontNode == nullptr;
}



/*********************************Stack.h************************************/
#ifndef STACK_H
#define STACK_H

#include <iostream>

// Template class for the stack node
template <typename T>
class StackNode {
public:
    T data;              // The data stored in the stack node
    StackNode* next;     // Pointer to the next node in the stack

    // Constructor
    StackNode(T nodeData) : data(nodeData), next(nullptr) {}
};

// Template class for the stack
template <typename T>
class Stack {
public:
    Stack();            // Constructor
    ~Stack();           // Destructor

    void push(T node);  // Add an element to the stack
    T pop(T& node);     // Remove an element from the stack and return pointer to deleted element
    T top() const;      // Return the top element
    bool isEmpty() const; // Check if the stack is empty

private:
    StackNode<T>* topNode;  // Points to the top of the stack
};

#include "Stack.tpp"  // Include the template implementation

#endif



/********************************Stack.tpp***********************************/
// Stack constructor
template <typename T>
Stack<T>::Stack() : topNode(nullptr) {}

// Stack destructor
template <typename T>
Stack<T>::~Stack() {
    T tempNode;
    while (!isEmpty()) {
        pop(tempNode);  // Clean up each node in the stack
    }
}

// Push function to add an element to the stack
template <typename T>
void Stack<T>::push(T node) {
    StackNode<T>* newNode = new StackNode<T>(node);
    newNode->next = topNode;
    topNode = newNode;
}

// Pop function to remove an element from the stack
template <typename T>
T Stack<T>::pop(T& node) {
    if (isEmpty()) {
        return nullptr;  // Handle empty stack case
    }

    node = topNode->data;  // Set node to the top element's data (T type)
    StackNode<T>* temp = topNode;  // Temporarily hold the top element
    topNode = topNode->next;   // Move the top pointer to the next element
    delete temp;  // Delete the old top element

    return node;  // Return the popped element
}

// Return the top element of the stack
template <typename T>
T Stack<T>::top() const {
    if (isEmpty()) {
        return nullptr;  // Handle empty stack case
    }
    return topNode->data;  // Return the top element's data
}

// Check if the stack is empty
template <typename T>
bool Stack<T>::isEmpty() const {
    return topNode == nullptr;
}
