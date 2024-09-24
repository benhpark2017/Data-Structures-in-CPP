/**
 * Solutions to Chapter 5, Section 3, Exercises 4 to 8 of Horowitz's
 * Fundamentals of Data Structures in C++.
 * 
 * Task 4:
 * Implement an iterator class LevelOrderIterator to traverse a binary tree
 * in level order similar to the inorder iterator described in the text.
 * 
 * Task 5:
 * Write a nonrecursive version of the function preorder().
 * 
 * Task 6:
 * Use the results of the previous exercise to implement class PreorderIterator
 * similar to the inorder iterator described in the text.
 * 
 * Task 7:
 * Write a nonrecursive version of the function postorder().
 * 
 * Task 8:
 * Use the results of the previous exercise to implement class PostorderIterator
 * similar to the inorder iterator described in the text.
 */

/*********************************main.cpp*************************************/
#include "Tree.h"
#include <iostream>

int main() {
    Tree binaryTree;
    Tree notationTree;

    binaryTree.insert('A');                // Root (Level 1)
    binaryTree.insert('B');                // Level 2
    binaryTree.insert('C');                // Level 2
    binaryTree.insert('D');                // Level 3
    binaryTree.insert('E');                // Level 3
    binaryTree.insert('F');                // Level 3
    binaryTree.insert('G');                // Level 3
    binaryTree.insert('H');                // Level 4
    binaryTree.insert('I');                // Level 4

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


    // Display in-order traversal to confirm tree structure
    std::cout << "In-order traversal of the complete binary tree: ";
    binaryTree.inorder();
    std::cout << std::endl;

    // Display nonrecursive in-order traversal to confirm tree structure
    std::cout << "Nonrecursive in-order traversal of the complete binary tree: ";
    binaryTree.inorderNonrec();
    std::cout << std::endl;

    // Display pre-order traversal to confirm tree structure
    std::cout << "Pre-order traversal of the complete binary tree: ";
    binaryTree.preorder();
    std::cout << std::endl;
    
    // Display nonrecursive pre-order traversal to confirm tree structure
    std::cout << "Nonrecursive pre-order traversal of the complete binary tree: ";
    binaryTree.preorderNonrec();
    std::cout << std::endl;

    // Display post-order traversal to confirm tree structure
    std::cout << "Post-order traversal of the complete binary tree: ";
    binaryTree.postorder();
    std::cout << std::endl;

    // Display nonrecursive post-order traversal to confirm tree structure
    std::cout << "Nonrecursive post-order traversal of the complete binary tree: ";
    binaryTree.postorderNonrec();
    std::cout << std::endl;

    // Display level order traversal to confirm tree structure
    std::cout << "Level order traversal of the complete binary tree: ";
    binaryTree.levelorder();
    std::cout << std::endl;
    
    std::cout << std::endl;

    // Display in-order traversal to confirm tree structure
    std::cout << "In-order traversal of the notation tree: ";
    notationTree.inorder();
    std::cout << std::endl;

    // Display pre-order traversal to confirm tree structure
    std::cout << "Pre-order traversal of the notation tree: ";
    notationTree.preorder();
    std::cout << std::endl;

    // Display post-order traversal to confirm tree structure
    std::cout << "Post-order traversal of the notation tree: ";
    notationTree.postorder();
    std::cout << std::endl;

    // Display level order traversal to confirm tree structure
    std::cout << "Level order traversal of the notation tree: ";
    notationTree.levelorder();
    std::cout << std::endl;

    std::cout << std::endl;

    InorderIterator inOrder(notationTree);
    char data1;
    std::cout << "In-order traversal of the tree using iterator: ";
    while (data1 = inOrder.Next()) {
        std::cout << data1 << " ";
    }
    std::cout << std::endl;

    PreorderIterator preOrder(notationTree);
    char data2;
    std::cout << "Pre-order traversal of the tree using iterator: ";
    while (data2 = preOrder.Next()) {
        std::cout << data2 << " ";
    }
    std::cout << std::endl;


    PostorderIterator postOrder(notationTree);
    char data3;
    std::cout << "Post-order traversal of the tree using iterator: ";
    while (data3 = postOrder.Next()) {
        std::cout << data3 << " ";
    }
    std::cout << std::endl;
    
    
    LevelOrderIterator levelOrder(notationTree);
    char data4;
    std::cout << "Level order traversal of the tree using iterator: ";
    while (data4 = levelOrder.Next()) {
        std::cout << data4 << " ";
    }
    std::cout << std::endl;

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
    Tree();               // Constructor
    Tree(const Tree& other);  // Copy constructor
    ~Tree();              // Destructor

    void insert(char data);   // Insert function to insert character data
    
    void inorder() const;     // Inorder traversal
    void preorder() const;    // Preorder traversal
    void postorder() const;   // Postorder traversal
    void levelorder() const;  // Level order traversal

    void inorderNonrec() const;     //Non-recursive pre-order traversal
    void preorderNonrec() const;    //Non-recursive pre-order traversal
    void postorderNonrec() const;   //Non-recursive post-order traversal

    void setRoot(TreeNode* node);   // Set the root manually
    TreeNode* getRoot() { return root; }

private:
    TreeNode* root;  // Pointer to the root of the tree

    // Helper functions
    void copyTree(TreeNode*& thisRoot, TreeNode* otherRoot);
    void deleteTree(TreeNode* node);
    void inorder(TreeNode* node) const;
    void preorder(TreeNode* node) const;
    void postorder(TreeNode* node) const;
};

// In-order iterator Class Definition
class InorderIterator {
  private:
    Tree& t;
    TreeNode* node;
    Stack<TreeNode*> s; // Stack with TreeNode* type

  public:
    InorderIterator(Tree& tree);
    char Next();
};

// Preorder iterator Class Definition
class PreorderIterator {
  private:
    Tree& t;
    TreeNode* node;
    Stack<TreeNode*> s;
    
  public:
    PreorderIterator(Tree& tree);
    char Next();
};

// Post-order iterator Class Definition
class PostorderIterator {
private:
    Tree& t;                    // Reference to the tree being traversed
    TreeNode* current;          // Current node being processed
    Stack<TreeNode*> s;         // Stack to hold nodes during traversal
    TreeNode* lastVisited;      // Last visited node to determine traversal order

public:
    PostorderIterator(Tree& tree);  // Constructor
    char Next();                    // Get the next node's data in postorder
};

// Level order iterator Class Definition
class LevelOrderIterator {
  private:
    Tree& t;
    TreeNode* node;
    Queue<TreeNode*> q;
    
  public:
    LevelOrderIterator(Tree& tree);
    char Next();
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


void Tree::inorder() const {
    inorder(root);
}

void Tree::preorder() const {
    preorder(root);
}

void Tree::postorder() const {
    postorder(root);
}

void Tree::levelorder() const {
    Queue<TreeNode*> q;
    TreeNode *node = root;
    while (node) {
        std::cout << node->data << " ";
        if (node->leftChild) q.enqueue(node->leftChild);
        if (node->rightChild) q.enqueue(node->rightChild);
        node = q.dequeue(node);
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

void Tree::inorder(TreeNode* node) const {
    if (node != nullptr) {
        inorder(node->leftChild);
        std::cout << node->data << " ";
        inorder(node->rightChild);
    }
}

void Tree::preorder(TreeNode* node) const {
    if (node != nullptr) {
        std::cout << node->data << " ";
        preorder(node->leftChild);
        preorder(node->rightChild);
    }
}

void Tree::postorder(TreeNode* node) const {
    if (node != nullptr) {
        postorder(node->leftChild);
        postorder(node->rightChild);
        std::cout << node->data << " ";
    }
}

void Tree::inorderNonrec() const {
    Stack<TreeNode*> s;
    TreeNode* node = root;
    while (true) {
        while (node) {
            s.push(node);
            node = node->leftChild;
        }
        if (!s.isEmpty()) {
            node = s.pop(node);
            std::cout << node->data << " ";
            node = node->rightChild;
        }
        else break;
    }
}

void Tree::preorderNonrec() const {
    if (root == nullptr) {
        return; // If the tree is empty, just return
    }

    Stack<TreeNode*> s;  // Create a stack for TreeNode pointers
    s.push(root);        // Push the root node onto the stack

    while (!s.isEmpty()) {
        TreeNode* node = nullptr; // Declare a variable to hold the popped node
        s.pop(node);              // Pop the top node from the stack into 'node'
        std::cout << node->data << " "; // Process the current node

        // Push the right child first so that the left child is processed next
        if (node->rightChild) {
            s.push(node->rightChild);
        }
        if (node->leftChild) {
            s.push(node->leftChild);
        }
    }
}

void Tree::postorderNonrec() const {
    if (root == nullptr) {
        return; // If the tree is empty, just return
    }

    Stack<TreeNode*> s;           // Stack to hold TreeNode pointers
    TreeNode* current = root;     // Start from the root
    TreeNode* lastVisited = nullptr; // Keep track of the last visited node
    TreeNode* tempNode = nullptr; // Temporary variable for pop()

    while (!s.isEmpty() || current != nullptr) {
        // Reach the leftmost node
        while (current != nullptr) {
            s.push(current);       // Push current node onto the stack
            current = current->leftChild; // Move to the left child
        }

        // Peek the node at the top of the stack without popping it
        current = s.pop(tempNode); // Pop the top node into tempNode

        // If the right child is null or has been visited, visit the current node
        if (current->rightChild == nullptr || current->rightChild == lastVisited) {
            std::cout << current->data << " "; // Process the current node
            lastVisited = current; // Mark this node as last visited
            current = nullptr; // Set current to nullptr to avoid revisiting
        } else {
            // If the right child exists, push the current node back onto the stack
            // and process the right child next
            s.push(current);
            current = current->rightChild;
        }
    }
}



// InorderIterator Class Implementation
InorderIterator::InorderIterator(Tree& tree) : t(tree), node(t.getRoot()) {}

char InorderIterator::Next() {
    TreeNode* tempNode = nullptr;

    while (!s.isEmpty() || node != nullptr) {
        if (node != nullptr) {
            s.push(node);
            node = node->leftChild; // Traverse to the left subtree
        } else {
            node = s.pop(tempNode); // Pop from the stack
            char result = node->data; // Store the current node's data
            node = node->rightChild; // Move to the right subtree
            return result; // Return the in-order node's data
        }
    }

    return '\0'; // Return null character if traversal is complete
}


// PreorderIterator Class Implementation
PreorderIterator::PreorderIterator(Tree& tree) : t(tree), node(t.getRoot()) {
    if (node) {
        s.push(node); // Push the root node onto the stack
    }
}

char PreorderIterator::Next() {
    if (s.isEmpty()) {
        return '\0';  // Return null character if no more nodes
    }

    TreeNode* tempNode = nullptr; // Declare a temporary pointer
    s.pop(tempNode);              // Pop the top node from the stack

    char result = tempNode->data; // Store the current node's data

    // Push right child first so that the left child is processed next
    if (tempNode->rightChild) {
        s.push(tempNode->rightChild);
    }
    if (tempNode->leftChild) {
        s.push(tempNode->leftChild);
    }

    return result; // Return the data of the current node
}

// PostorderIterator Constructor Implementation
PostorderIterator::PostorderIterator(Tree& tree) 
    : t(tree), current(t.getRoot()), lastVisited(nullptr) {}

// Next function Implementation
char PostorderIterator::Next() {
    if (s.isEmpty() && current == nullptr) {
        return '\0'; // Return null character if traversal is complete
    }

    while (current != nullptr || !s.isEmpty()) {
        if (current != nullptr) {
            s.push(current);
            current = current->leftChild;
        } else {
            TreeNode* peekNode = nullptr;
            s.pop(peekNode);  // Peek at the top node
            s.push(peekNode); // Push it back

            // If right child exists and hasn't been visited yet
            if (peekNode->rightChild != nullptr && peekNode->rightChild != lastVisited) {
                current = peekNode->rightChild;
            } else {
                char result = peekNode->data;
                lastVisited = peekNode;
                s.pop(peekNode); // Actually remove the node from stack
                current = nullptr; // Move to the next iteration
                return result;
            }
        }
    }

    return '\0'; // This line should never be reached, but is here for completeness
}

// Level order iterator class implementation
LevelOrderIterator::LevelOrderIterator(Tree& tree) : t(tree), node(t.getRoot()) {
    if (node) q.enqueue(node);
}

char LevelOrderIterator::Next() {
    if (q.isEmpty()) {
        return '\0';  // Return null character if no more nodes
    }

    // Dequeue the next node to process
    q.dequeue(node); // Remove the front node from the queue

    // Enqueue the left and right children, if they exist
    if (node->leftChild) {
        q.enqueue(node->leftChild);
    }
    if (node->rightChild) {
        q.enqueue(node->rightChild);
    }

    // Return the data of the current node
    return node->data;
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
