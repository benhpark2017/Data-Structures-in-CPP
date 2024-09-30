/**
 * Solutions to Chapter 5, Section 5, Exercises 1 to 3 of Horowitz's
 * Fundamentals of Data Structures in C++.
 * 
 * Task 1:
 * Write an algorithm that inserts a new node l as the left child of node s in
 * a threaded binary tree. The left subtree of s becomes the left subtree of l.
 * 
 * Task 2:
 * Write a function to traverse a threaded binary tree in postorder. What are 
 * the time and space requirements of your function?
 * The Postorder() function also uses recursion, and its space complexity
 * depends on the depth of recursion. Similar to Preorder(), the recursion depth
 * is the height of the tree, leading to a space complexity of O(h), where h is
 * the height of the tree. In the worst case (skewed tree), the space complexity
 * is O(n), and for a balanced binary tree, it is O(log n).
 * 
 * Task 3:
 * Write a function to traverse a threaded binary tree in preorder. What are 
 * the time and space requirements of your function?
 * The space complexity is determined by the recursion depth, which
 * corresponds to the maximum depth of the tree (i.e., the height of the tree).
 * In the worst case, the tree could be skewed (like a linked list), leading
 * to a maximum depth of n.
 * However, for a balanced binary tree, the recursion depth would be the
 * height of the tree, which is O(log n). Therefore, the space complexity of
 * Preorder() is O(h), where h is the height of the tree. In the worst case
 * (skewed tree), h = n, so the space complexity is O(n).
 * 
 */

#include "ThreadedTree.h"
#include <iostream>

int main() {
    ThreadedTree tree;
    tree.Initialize();
    
    // Inorder traversal
    ThreadedInorderIterator inorderIterator(tree);
    std::cout << "In-order traversal of the threaded binary tree." << std::endl;
    inorderIterator.Inorder();
    std::cout << '\n' << std::endl;

    // Postorder traversal using a set of driver and workhorse functions
    std::cout << "Post-order traversal of the threaded binary tree." << std::endl;
    tree.Postorder();
    std::cout << '\n' << std::endl;
    
    // Preorder traversal using a set of driver and workhorse functions
    std::cout << "Pre-order traversal of the threaded binary tree." << std::endl;
    tree.Preorder();
    std::cout << '\n' << std::endl;
    
    std::cout << "End of program." << std::endl;
    return 0;
}



#ifndef THREADED_TREE_H
#define THREADED_TREE_H

enum Boolean {FALSE, TRUE};

class ThreadedNode {
    friend class ThreadedTree;
    friend class ThreadedInorderIterator;
    friend class ThreadedPostorderIterator;
    
  private:
    Boolean LeftThread;
    ThreadedNode* LeftChild;
    char data;
    ThreadedNode* RightChild;
    Boolean RightThread;
  public:
    ThreadedNode() : LeftChild(nullptr), RightChild(nullptr) {} 
        //Changed RightThread to RightChild.
    ThreadedNode(char temp, ThreadedNode *left, ThreadedNode *right, Boolean leftT, Boolean rightT)
        : data(temp), LeftChild(nullptr), RightChild(nullptr), LeftThread(leftT), RightThread(rightT) {}

};

class ThreadedTree {
    friend class ThreadedInorderIterator;
    friend class ThreadedPostorderIterator;
    
  public:
    ThreadedTree();
    void Initialize();
    
    void Postorder();
    void Preorder();
    void InsertRight(ThreadedNode* s, ThreadedNode* r);
    void InsertLeft(ThreadedNode* s, ThreadedNode* l);
    ThreadedNode* InorderSucc(ThreadedNode* r);
    
  private:
    ThreadedNode* root;
    void Postorder(ThreadedNode* node);
    void Preorder(ThreadedNode* node);
};

class ThreadedInorderIterator {
  public:
    char* Next();
    void Inorder();
    ThreadedInorderIterator(ThreadedTree tree) : t(tree), CurrentNode(t.root) {}
  private:
    ThreadedTree t;
    ThreadedNode* CurrentNode;
};

#endif //THREADED_TREE_H



#include "ThreadedTree.h"
#include <iostream>

//Find the in-order successor of CurrentNode in a threaded binary tree
char* ThreadedInorderIterator::Next() {
    ThreadedNode* temp = CurrentNode->RightChild;
    if (!CurrentNode->RightThread)
        while (!temp->LeftThread) temp = temp->LeftChild;
    CurrentNode = temp;
    if (CurrentNode == t.root) return 0;
    else return &CurrentNode->data;
}

void ThreadedInorderIterator::Inorder() {
    for (char* ch = Next(); ch; ch = Next())
        std::cout << *ch << " ";
}

ThreadedTree::ThreadedTree() {
    root = new ThreadedNode;
    root->RightChild = root->LeftChild = root;
    root->LeftThread = TRUE;
    root->RightThread = FALSE;
    
}

void ThreadedTree::Initialize() { 
    // Declare node pointers
    ThreadedNode *A, *B, *C, *D, *E, *F, *G, *H, *I;

    // Initialize root and first node (A)
    root->LeftChild = A = new ThreadedNode('A', nullptr, nullptr, FALSE, FALSE);
    root->LeftThread = FALSE;

    // Initialize the left and right children of A
    A->LeftChild = B = new ThreadedNode('B', nullptr, nullptr, FALSE, FALSE);
    A->RightChild = C = new ThreadedNode('C', nullptr, nullptr, FALSE, FALSE);

    // Initialize the left and right children of B
    B->LeftChild = D = new ThreadedNode('D', nullptr, nullptr, FALSE, FALSE);
    B->RightChild = E = new ThreadedNode('E', nullptr, nullptr, TRUE, TRUE);  // E is a leaf node

    // Initialize the left and right children of D
    D->LeftChild = H = new ThreadedNode('H', nullptr, nullptr, TRUE, TRUE);  // H is a leaf node
    D->RightChild = I = new ThreadedNode('I', nullptr, nullptr, TRUE, TRUE);  // I is a leaf node

    // Initialize the left and right children of C
    C->LeftChild = F = new ThreadedNode('F', nullptr, nullptr, TRUE, TRUE);  // F is a leaf node
    C->RightChild = G = new ThreadedNode('G', nullptr, nullptr, TRUE, TRUE);  // G is a leaf node

    // Threading connections (to maintain the threaded binary tree structure)
    H->LeftChild = root;  // Left thread of H points to root (dummy node)
    H->RightChild = D;    // Right thread of H points to D (inorder predecessor)

    I->LeftChild = D;     // Left thread of I points to D (inorder predecessor)
    I->RightChild = B;    // Right thread of I points to B (inorder predecessor)

    E->LeftChild = B;     // Left thread of E points to B (inorder predecessor)
    E->RightChild = A;    // Right thread of E points to A (inorder successor)

    F->LeftChild = A;     // Left thread of F points to A (inorder predecessor)
    F->RightChild = C;    // Right thread of F points to C (inorder successor)

    G->LeftChild = C;     // Left thread of G points to C (inorder predecessor)
    G->RightChild = root; // Right thread of G points to root (dummy node)
}

void ThreadedTree::InsertLeft(ThreadedNode *s, ThreadedNode *l) {
    l->LeftChild = s->LeftChild;
    l->LeftThread = s->LeftThread;
    l->RightChild = s;
    l->RightThread = TRUE;
    s->LeftChild = l;
    s->LeftThread = FALSE;
    if (!l->LeftThread) {
        ThreadedNode *temp = InorderSucc(l->LeftChild);
        temp->RightChild = l;
    }
}

void ThreadedTree::InsertRight(ThreadedNode* s, ThreadedNode* r) {
    r->RightChild = s->RightChild;
    r->RightThread = s->RightThread;
    r->LeftChild = s;
    r->LeftThread = TRUE;
    s->RightChild = r; //attach r to s
    s->RightThread = FALSE;
    if (!r->RightThread) {
        ThreadedNode* temp = InorderSucc(r);
        temp->LeftChild = r;
    }
}

ThreadedNode* ThreadedTree::InorderSucc(ThreadedNode* r) {
    if (r->RightThread == TRUE) {
        // Case 1: r has a right thread, so the successor is the node it points to
        return r->RightChild;
    } else {
        // Case 2: r has a real right child, find the leftmost node in its right subtree
        ThreadedNode* temp = r->RightChild;
        while (temp->LeftThread == FALSE) {
            temp = temp->LeftChild;  // Go to the leftmost node
        }
        return temp;  // This is the inorder successor
    }
}

// Driver code for post-order traversal
void ThreadedTree::Postorder() {
    Postorder(root->LeftChild);
}

void ThreadedTree::Postorder(ThreadedNode* node) {
    if (node == nullptr || node == root) {
        return;  // Base case: If node is null or we reach the dummy root, return
    }

    if (!node->LeftThread) {
        Postorder(node->LeftChild);  // Traverse left subtree if it's not threaded
    }

    if (!node->RightThread) {
        Postorder(node->RightChild);  // Traverse right subtree if it's not threaded
    }

    std::cout << node->data << " ";  // Process the current node
}

// Driver code for pre-order traversal
void ThreadedTree::Preorder() {
    Preorder(root->LeftChild);  // Start from the root's left child (the actual root of the tree)
}

// Workhorse function for pre-order traversal
void ThreadedTree::Preorder(ThreadedNode* node) {
    if (node == nullptr || node == root) {
        return;  // Base case: If node is null or we reach the dummy root, return
    }

    std::cout << node->data << " ";  // Process the current node (pre-order: root first)

    if (!node->LeftThread) {
        Preorder(node->LeftChild);  // Traverse the left subtree if it's not threaded
    }

    if (!node->RightThread) {
        Preorder(node->RightChild);  // Traverse the right subtree if it's not threaded
    }
}
