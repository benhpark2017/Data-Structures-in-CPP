/**
 * Solutions to Chapter 5, Section 3, Exercise 9 of Horowitz's
 * Fundamentals of Data Structures in C++.
 * 
 * Task 9 [Programming Project]:
 * Write the C++ template class definition for binary trees. Next, define
 * iterators for inorder, preorder, postorder, and level order traversals
 * on binary trees (At this point, your program will contain six class 
 * definitions). Finally, write a function that prints all tree elements 
 * in each of the four orders.
 * 
 */

/*********************************main.cpp*************************************/
#include "Tree.h"
#include <iostream>

int main() {
    Tree<char> binaryTree;
    Tree<char> notationTree;

    binaryTree.insert('A');
    binaryTree.insert('B');
    binaryTree.insert('C');
    binaryTree.insert('D');
    binaryTree.insert('E');
    binaryTree.insert('F');
    binaryTree.insert('G');
    binaryTree.insert('H');
    binaryTree.insert('I');

    TreeNode<char>* plus = new TreeNode<char>('+');
    TreeNode<char>* multiply1 = new TreeNode<char>('*');
    TreeNode<char>* multiply2 = new TreeNode<char>('*');
    TreeNode<char>* divide = new TreeNode<char>('/');
    TreeNode<char>* A = new TreeNode<char>('A');
    TreeNode<char>* B = new TreeNode<char>('B');
    TreeNode<char>* C = new TreeNode<char>('C');
    TreeNode<char>* D = new TreeNode<char>('D');
    TreeNode<char>* E = new TreeNode<char>('E');

    plus->leftChild = multiply1;
    plus->rightChild = E;

    multiply1->leftChild = multiply2;
    multiply1->rightChild = D;

    multiply2->leftChild = divide;
    multiply2->rightChild = C;

    divide->leftChild = A;
    divide->rightChild = B;

    notationTree.setRoot(plus);


    // Display nonrecursive in-order traversal to confirm tree structure
    std::cout << "Nonrecursive in-order traversal of the complete binary tree: ";
    binaryTree.inorderNonrec();
    std::cout << std::endl;
    
    // Display nonrecursive pre-order traversal to confirm tree structure
    std::cout << "Nonrecursive pre-order traversal of the complete binary tree: ";
    binaryTree.preorderNonrec();
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

    // Display nonrecursive in-order traversal to confirm tree structure
    std::cout << "Nonrecursive in-order traversal of the notation tree: ";
    notationTree.inorderNonrec();
    std::cout << std::endl;
    
    // Display nonrecursive pre-order traversal to confirm tree structure
    std::cout << "Nonrecursive pre-order traversal of the notation tree: ";
    notationTree.preorderNonrec();
    std::cout << std::endl;

    // Display nonrecursive post-order traversal to confirm tree structure
    std::cout << "Nonrecursive post-order traversal of the notation tree: ";
    notationTree.postorderNonrec();
    std::cout << std::endl;

    // Display level order traversal to confirm tree structure
    std::cout << "Level order traversal of the notation tree: ";
    notationTree.levelorder();
    std::cout << std::endl;
    
    std::cout << std::endl;

    // Print all orders of the binaryTree using iterators
    std::cout << "Iterator-based traversals of the complete binary tree:" << std::endl;
    binaryTree.printAllOrdersWithIterators();
    std::cout << std::endl;

    // Print all orders of the notationTree using iterators
    std::cout << "Iterator-based traversals of the notation tree:" << std::endl;
    notationTree.printAllOrdersWithIterators();
    std::cout << std::endl;

    std::cout << "End of program.\n";
    return 0;
}



/*********************************Tree.h*************************************/
#ifndef TREE_H
#define TREE_H

#include "Stack.h"
#include "Queue.h"

// TreeNode Class Definition (template version)
template <typename T>
class TreeNode {
public:
    T data;               // Generic data type
    TreeNode<T>* leftChild;   // Pointer to the left child
    TreeNode<T>* rightChild;  // Pointer to the right child

    // Constructor to initialize the node with generic data
    TreeNode(T c) : data(c), leftChild(nullptr), rightChild(nullptr) {}
};

// Tree Class Definition (template version)
template <typename T>
class Tree {
public:
    Tree();                       // Constructor
    Tree(const Tree<T>& other);   // Copy constructor
    ~Tree();                      // Destructor

    void insert(T data);          // Insert function to insert generic data
    void inorder() const;         // Inorder traversal
    void preorder() const;        // Preorder traversal
    void postorder() const;       // Postorder traversal
    void levelorder() const;      // Level order traversal

    void inorderNonrec() const;   // Non-recursive inorder traversal
    void preorderNonrec() const;  // Non-recursive preorder traversal
    void postorderNonrec() const; // Non-recursive postorder traversal

    void setRoot(TreeNode<T>* node);     // Set the root manually
    TreeNode<T>* getRoot() const { return root; };
    
    void printAllOrdersWithIterators();  //print function for all four orders


private:
    TreeNode<T>* root;  // Pointer to the root of the tree

    // Helper functions
    void copyTree(TreeNode<T>*& thisRoot, TreeNode<T>* otherRoot);
    void deleteTree(TreeNode<T>* node);
    void inorder(TreeNode<T>* node) const;
    void preorder(TreeNode<T>* node) const;
    void postorder(TreeNode<T>* node) const;
};


// InorderIterator Class Definition (template version)
template <typename T>
class InorderIterator {
  private:
    Tree<T>& t;
    TreeNode<T>* node;
    Stack<TreeNode<T>*> s; // Stack with TreeNode<T>* type

  public:
    InorderIterator(Tree<T>& tree);
    T Next();
};

// PreorderIterator Class Definition (template version)
template <typename T>
class PreorderIterator {
private:
    Stack<TreeNode<T>*> s;
    Tree<T>& t;
    TreeNode<T>* currentNode;

public:
    PreorderIterator(Tree<T>& tree);
    T Next();
};

// Post-order iterator Class Definition (needs work)
template <typename T>
class PostorderIterator {
  private:
    Stack<TreeNode<T>*> s;
    Tree<T>& t;
    TreeNode<T>* currentNode;

  public:
    PostorderIterator(Tree<T>& tree);
    T Next();
};


// LevelOrderIterator Class Definition (template version)
template <typename T>
class LevelOrderIterator {
  private:
    Queue<TreeNode<T>*> q;  // Use a queue to store nodes for level-order traversal
    Tree<T>& t;             // Reference to the tree being traversed
    TreeNode<T>* currentNode;

  public:
    // Constructor to initialize the iterator
    LevelOrderIterator(Tree<T>& tree);
    // Function to get the next node in level-order traversal
    T Next();
};

#include "Tree.tpp"
#endif //TREE_H



/*********************************Tree.tpp************************************/
template <typename T>
Tree<T>::Tree() : root(nullptr) {}

template <typename T>
Tree<T>::Tree(const Tree<T>& other) : root(nullptr) {
    copyTree(this->root, other.root);
}

template <typename T>
Tree<T>::~Tree() {
    deleteTree(root);
}

// Insert function to insert generic data into the tree (template version)
template <typename T>
void Tree<T>::insert(T data) {
    TreeNode<T>* newNode = new TreeNode<T>(data);  // Create a new node

    if (root == nullptr) {  // If the tree is empty
        root = newNode;     // Set the new node as the root
        return;
    }

    Queue<TreeNode<T>*> q;    // Using custom Queue template for TreeNode<T>*
    q.enqueue(root);          // Enqueue the root node

    while (!q.isEmpty()) {
        TreeNode<T>* temp;
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

template <typename T>
void Tree<T>::inorder() const {
    inorder(root);
}

template <typename T>
void Tree<T>::preorder() const {
    preorder(root);
}

template <typename T>
void Tree<T>::postorder() const {
    postorder(root);
}

template <typename T>
void Tree<T>::levelorder() const {
    Queue<TreeNode<T>*> q;
    TreeNode<T> *node = root;
    while (node) {
        std::cout << node->data << " ";
        if (node->leftChild) q.enqueue(node->leftChild);
        if (node->rightChild) q.enqueue(node->rightChild);
        node = q.dequeue(node);
    }
}

template <typename T>
void Tree<T>::setRoot(TreeNode<T>* node) {
    root = node;
}

template <typename T>
void Tree<T>::copyTree(TreeNode<T>*& thisRoot, TreeNode<T>* otherRoot) {
    if (otherRoot == nullptr) {
        thisRoot = nullptr;
    } else {
        thisRoot = new TreeNode<T>(otherRoot->data);
        copyTree(thisRoot->leftChild, otherRoot->leftChild);
        copyTree(thisRoot->rightChild, otherRoot->rightChild);
    }
}

template <typename T>
void Tree<T>::deleteTree(TreeNode<T>* node) {
    if (node != nullptr) {
        deleteTree(node->leftChild);
        deleteTree(node->rightChild);
        delete node;
    }
}

template <typename T>
void Tree<T>::inorder(TreeNode<T>* node) const {
    if (node != nullptr) {
        inorder(node->leftChild);
        std::cout << node->data << " ";
        inorder(node->rightChild);
    }
}

template <typename T>
void Tree<T>::preorder(TreeNode<T>* node) const {
    if (node != nullptr) {
        std::cout << node->data << " ";
        preorder(node->leftChild);
        preorder(node->rightChild);
    }
}

template <typename T>
void Tree<T>::postorder(TreeNode<T>* node) const {
    if (node != nullptr) {
        postorder(node->leftChild);
        postorder(node->rightChild);
        std::cout << node->data << " ";
    }
}

template <typename T>
void Tree<T>::inorderNonrec() const {
    Stack<TreeNode<T>*> s;
    TreeNode<T>* node = root;
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

template <typename T>
void Tree<T>::preorderNonrec() const {
    if (root == nullptr) {
        return; // If the tree is empty, just return
    }

    Stack<TreeNode<T>*> s;  // Create a stack for TreeNode pointers
    s.push(root);        // Push the root node onto the stack

    while (!s.isEmpty()) {
        TreeNode<T>* node = nullptr; // Declare a variable to hold the popped node
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

template <typename T>
void Tree<T>::postorderNonrec() const {
    if (root == nullptr) {
        return; // If the tree is empty, just return
    }

    Stack<TreeNode<T>*> s;           // Stack to hold TreeNode pointers
    TreeNode<T>* current = root;     // Start from the root
    TreeNode<T>* lastVisited = nullptr; // Keep track of the last visited node
    TreeNode<T>* tempNode = nullptr; // Temporary variable for pop()

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

template <typename T>
void Tree<T>::printAllOrdersWithIterators() {
    // In-order traversal using iterator
    InorderIterator<T> inOrder(*this);
    std::cout << "In-order traversal using iterator: ";
    T data;
    while ((data = inOrder.Next()) != '\0') {
        std::cout << data << " ";
    }
    std::cout << std::endl;

    // Pre-order traversal using iterator
    PreorderIterator<T> preOrder(*this);
    std::cout << "Pre-order traversal using iterator: ";
    while ((data = preOrder.Next()) != '\0') {
        std::cout << data << " ";
    }
    std::cout << std::endl;

    // Post-order traversal using iterator
    PostorderIterator<T> postOrder(*this);
    std::cout << "Post-order traversal using iterator: ";
    while ((data = postOrder.Next()) != '\0') {
        std::cout << data << " ";
    }
    std::cout << std::endl;

    // Level-order traversal using iterator
    LevelOrderIterator<T> levelOrder(*this);
    std::cout << "Level-order traversal using iterator: ";
    while ((data = levelOrder.Next()) != '\0') {
        std::cout << data << " ";
    }
    std::cout << std::endl;
}



// Implementation of InorderIterator (template version)
template <typename T>
InorderIterator<T>::InorderIterator(Tree<T>& tree) : t(tree), node(t.getRoot()) {}

template <typename T>
T InorderIterator<T>::Next() {
    TreeNode<T>* tempNode = nullptr;

    while (!s.isEmpty() || node != nullptr) {
        if (node != nullptr) {
            s.push(node);
            node = node->leftChild; // Traverse to the left subtree
        } else {
            node = s.pop(tempNode); // Pop from the stack
            T result = node->data; // Store the current node's data
            node = node->rightChild; // Move to the right subtree
            return result; // Return the in-order node's data
        }
    }

    return '\0'; // Return null character if traversal is complete
}

// PreorderIterator Class Implementation
template <typename T>
PreorderIterator<T>::PreorderIterator(Tree<T>& tree) : t(tree) {
    if (t.getRoot()) {
        s.push(t.getRoot());
    }
}

// Next() function of Pre-order iterator
template <typename T>
T PreorderIterator<T>::Next() {
    if (s.isEmpty()) {
        return T(); // Return default-constructed T if no more nodes
    }

    s.pop(currentNode);

    // Store the current node's data to return
    T result = currentNode->data;

    // Push right child first so that the left child is processed next
    if (currentNode->rightChild) {
        s.push(currentNode->rightChild);
    }
    if (currentNode->leftChild) {
        s.push(currentNode->leftChild);
    }
        
    return result;
}

// PostorderIterator Constructor
template <typename T>
PostorderIterator<T>::PostorderIterator(Tree<T>& tree) : t(tree), currentNode(nullptr) {
    if (t.getRoot()) {
        s.push(t.getRoot());  // Start by pushing only the root
    }
}

// Next function Implementation
template <typename T>
T PostorderIterator<T>::Next() {
    if (s.isEmpty()) {
        return T();  // Return default T if no more nodes
    }    

    while (!s.isEmpty()) {
        TreeNode<T>* topNode = s.top();

        // If we're at a leaf node or if both children have been visited
        if ((!topNode->leftChild && !topNode->rightChild) || 
            (currentNode && (currentNode == topNode->leftChild || currentNode == topNode->rightChild))) {
            s.pop(topNode);  // Pop this node and return its data
            currentNode = topNode;
            return currentNode->data;
        }

        // Push the right and left children (if they exist) in postorder
        if (topNode->rightChild) {
            s.push(topNode->rightChild);
        }
        if (topNode->leftChild) {
            s.push(topNode->leftChild);
        }
    }
    return T();  // This should only be reached when the stack is empty
}

// Level order iterator class implementation
template <typename T>
LevelOrderIterator<T>::LevelOrderIterator(Tree<T>& tree) : t(tree), currentNode(nullptr) {
    if (t.getRoot()) {
        q.enqueue(t.getRoot());  // Start by enqueuing the root node
    }
}

template <typename T>
T LevelOrderIterator<T>::Next() {
    if (q.isEmpty()) {
        return T();  // Return default T if no more nodes
    }

    // Dequeue the front node and store it in currentNode
    q.dequeue(currentNode);

    // Enqueue the left child if it exists
    if (currentNode->leftChild) {
        q.enqueue(currentNode->leftChild);
    }

    // Enqueue the right child if it exists
    if (currentNode->rightChild) {
        q.enqueue(currentNode->rightChild);
    }

    // Return the data of the current node
    return currentNode->data;
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
