/**
 * Solutions to Chapter 5, Section 5, Exercise 5 of Horowitz's
 * Fundamentals of Data Structures in C++.
 * 
 * Task 5:
 * Consider threading a binary tree using postorder threads rather than in-order
 * threads as in the text. Which of the traversals can be done without the use
 * of a stack? For those that can be performed without a stack, write a C++
 * function and analyze its space complexity.
 * 
 * Response:
 * In-order iterator:   Time complexity is O(h), where h is height of the tree;
 *                      Space complexity is O(1).
 * Pre-order iterator:  Time complexity is O(n) in the worst case, where n is
 *                      the number of nodes.
 *                      Space complexity is O(1)
 * Post-order iterator: Time complexity is O(n) in the worst case, where n is
 *                      the number of nodes.
 *                      Space complexity is also O(n).
 */

/*******************************main.cpp***********************************/
#include "ThreadedTree.h"
#include <iostream>

int main() {
    ThreadedTree tree;
    tree.Initialize();
    
    //In-order traversal of threaded binary tree using an iterator
    ThreadedInorderIterator inorderIterator(tree);
    std::cout << "In-order traversal of threaded binary tree using an iterator."
              << std::endl;
    inorderIterator.Inorder();
    std::cout << std::endl;
    
    // In-order traversal using a set of driver and workhorse functions
    std::cout << "In-order traversal of the threaded binary tree." << std::endl;
    tree.Inorder();
    std::cout << '\n' << std::endl;
    
    //Post-order traversal of threaded binary tree using an iterator
    ThreadedPostorderIterator postorderIterator(tree);
    std::cout << "Post-order traversal of threaded binary tree using an iterator."
              << std::endl;
    postorderIterator.Postorder();
    
    // Postorder traversal using a set of driver and workhorse functions
    std::cout << "Post-order traversal of the threaded binary tree." << std::endl;
    tree.Postorder();
    std::cout << '\n' << std::endl;
    
    // Pre-order traversal of threaded binary tree using an iterator
    ThreadedPreorderIterator preorderIterator(tree);
    std::cout << "Pre-order traversal of threaded binary tree using an iterator."
              << std::endl;
    preorderIterator.Preorder();
    std::cout << std::endl;
    
    // Pre-order traversal using a set of driver and workhorse functions
    std::cout << "Pre-order traversal of the threaded binary tree." << std::endl;
    tree.Preorder();
    std::cout << '\n' << std::endl;
    
    std::cout << "End of program." << std::endl;
    return 0;
}


/****************************ThreadedTree.h********************************/
#ifndef THREADED_TREE_H
#define THREADED_TREE_H

#include <stack>

enum Boolean {FALSE, TRUE};

class ThreadedNode {
    friend class ThreadedTree;
    friend class ThreadedInorderIterator;
    friend class ThreadedPreorderIterator;
    friend class ThreadedPostorderIterator;
    
  private:
    Boolean LeftThread;
    ThreadedNode* LeftChild;
    char data;
    ThreadedNode* RightChild;
    Boolean RightThread;
    
  public:
    ThreadedNode() : LeftChild(nullptr), RightChild(nullptr) {} 
    ThreadedNode(char temp, ThreadedNode *left, ThreadedNode *right, Boolean leftT, Boolean rightT)
        : data(temp), LeftChild(nullptr), RightChild(nullptr), LeftThread(leftT), RightThread(rightT) {}

};

class ThreadedTree {
    friend class ThreadedInorderIterator;
    friend class ThreadedPreorderIterator;
    friend class ThreadedPostorderIterator;
    
  public:
    ThreadedTree();         //written
    void Initialize();      //written
    
    void Inorder();
    void Postorder();  
    void Preorder();     

  private:
    ThreadedNode* root;
    
    void Inorder(ThreadedNode* node);
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

class ThreadedPreorderIterator {
  public:
    char* Next();
    void Preorder();
    ThreadedPreorderIterator(ThreadedTree tree) : t(tree), CurrentNode(t.root) {}
  private:
    ThreadedTree t;
    ThreadedNode* CurrentNode;
};

class ThreadedPostorderIterator {
  public:
    char* Next();
    void Postorder();
    ThreadedPostorderIterator(ThreadedTree tree) : t(tree), CurrentNode(t.root) {}
  private:
    ThreadedTree t;
    ThreadedNode* CurrentNode;
    std::stack<ThreadedNode*> s;
};

#endif //THREADED_TREE_H


/***************************ThreadedTree.cpp*******************************/
#include "ThreadedTree.h"
#include <iostream>

ThreadedTree::ThreadedTree() {
    root = new ThreadedNode;
    root->RightChild = root->LeftChild = root;
    root->LeftThread = FALSE;
    root->RightThread = FALSE;
}

//Initialize a complete threaded binary tree consisting of nine nodes
/**The tree should look like this:
                  A
                /  \
               B    C
              / \  / \
             D  E F  G
            / \
           H  I
*/
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
    B->RightChild = E = new ThreadedNode('E', nullptr, nullptr, TRUE, TRUE);
    
    // Initialize the left and right children of D
    D->LeftChild = H = new ThreadedNode('H', nullptr, nullptr, TRUE, TRUE);
    D->RightChild = I = new ThreadedNode('I', nullptr, nullptr, TRUE, TRUE);
    
    // Initialize the left and right children of C
    C->LeftChild = F = new ThreadedNode('F', nullptr, nullptr, TRUE, TRUE);
    C->RightChild = G = new ThreadedNode('G', nullptr, nullptr, TRUE, TRUE);
    
    // Set up threads for in-order traversal
    H->LeftChild = root;       // H has no left child
    H->RightChild = D;         // H's in-order successor is D
    
    I->LeftChild = D;          // I's thread back to parent
    I->RightChild = B;         // I's in-order successor is B
    
    E->LeftChild = B;          // E's thread back to parent
    E->RightChild = A;         // E's in-order successor is A
    
    F->LeftChild = A;          // F's thread back to parent
    F->RightChild = C;         // F's in-order successor is C
    
    G->LeftChild = C;          // G's thread back to parent
    G->RightChild = root;      // G's in-order successor is root (end of traversal)
    
    // Set up root's right child to point back to root
    root->RightChild = root;
}


// Driver function
void ThreadedTree::Inorder() {
    if (root->LeftThread) {  // Empty tree
        std::cout << "The tree is empty." << std::endl;
        return;
    }
    Inorder(root->LeftChild);
}

// Workhorse function
void ThreadedTree::Inorder(ThreadedNode* current) {
    if (current == nullptr) return;
    
    // Process left subtree
    if (!current->LeftThread) {
        Inorder(current->LeftChild);
    }
    
    // Process current node
    std::cout << current->data << " ";
    
    // Process right subtree
    if (!current->RightThread) {
        Inorder(current->RightChild);
    }
}

// Driver function
void ThreadedTree::Preorder() {
    if (root->LeftThread) {  // Empty tree
        std::cout << "The tree is empty." << std::endl;
        return;
    }
    Preorder(root->LeftChild);
}

// Workhorse function for preorder traversal of a post-order threaded binary tree
void ThreadedTree::Preorder(ThreadedNode* current) {
    if (current == nullptr) return;
    
    // Process current node
    std::cout << current->data << " ";
    
    // Process left subtree
    if (!current->LeftThread) {
        Preorder(current->LeftChild);
    }
    
    // Process right subtree
    if (!current->RightThread) {
        Preorder(current->RightChild);
    }
}

// Recursive implementation
void ThreadedTree::Postorder() {
    if (root->LeftThread) {  // Empty tree
        std::cout << "The tree is empty." << std::endl;
        return;
    }
    Postorder(root->LeftChild);
}

void ThreadedTree::Postorder(ThreadedNode* current) {
    if (current == nullptr) return;
    
    // Process left subtree
    if (!current->LeftThread) {
        Postorder(current->LeftChild);
    }
    
    // Process right subtree
    if (!current->RightThread) {
        Postorder(current->RightChild);
    }
    
    // Process current node
    std::cout << current->data << " ";
}


/****************From here and onward, We use iterators.******************/

// Driver function to print the nodes
void ThreadedInorderIterator::Inorder() {
    for (char* ch = Next(); ch; ch = Next())
        std::cout << *ch << " ";
}

// In-order iterator function.
// Solution that does not depend on helper functions or internal stacks
char* ThreadedInorderIterator::Next() {
    // First call: start at root and find the first post-order node (H)
    if (CurrentNode == t.root) {
        ThreadedNode* p = t.root->LeftChild;  // Start at A
        // Find the leftmost leaf
        while (!p->LeftThread) {
            p = p->LeftChild;
        }
        CurrentNode = p;
        return &(CurrentNode->data);
    }

    // If we're returning to a parent (via right thread), 
    // then that parent is next in the post-order sequence
    if (CurrentNode->RightThread) {
        ThreadedNode* next = CurrentNode->RightChild;
        if (next == t.root) {  // If we've reached the end
            CurrentNode = t.root;
            return nullptr;
        }
        CurrentNode = next;
        return &(CurrentNode->data);
    }
    
    // Otherwise, we need to go to the right subtree
    // and find its leftmost leaf
    ThreadedNode* p = CurrentNode->RightChild;
    while (!p->LeftThread) {
        p = p->LeftChild;
    }
    CurrentNode = p;
    return &(CurrentNode->data);
}

// Driver function to print the nodes
void ThreadedPreorderIterator::Preorder() {
    for (char* ch = Next(); ch; ch = Next())
        std::cout << *ch << " ";
}

char* ThreadedPreorderIterator::Next() {
    // Special case for the first call (root)
    if (CurrentNode == t.root) {
        if (t.root->LeftThread) return 0; // Empty tree
        CurrentNode = t.root->LeftChild; // Move to A
        return &(CurrentNode->data);
    }

    // If the node has a left child, that's our next node
    if (!CurrentNode->LeftThread) {
        CurrentNode = CurrentNode->LeftChild;
        return &(CurrentNode->data);
    }
    
    // If the node has a right child, that's our next node
    if (!CurrentNode->RightThread) {
        CurrentNode = CurrentNode->RightChild;
        return &(CurrentNode->data);
    }
    
    // If we get here, we're at a leaf node and need to find the next node to visit
    ThreadedNode* next = CurrentNode;
    
    while (true) {
        // Follow the post-order thread
        next = next->RightChild;
        
        // If we've reached the root, we're done
        if (next == t.root) {
            CurrentNode = t.root;
            return 0;
        }
        
        // Check if this node has an unvisited right child
        if (!next->RightThread && 
            next->RightChild != CurrentNode && 
            (CurrentNode->RightChild != next->RightChild)) {
            CurrentNode = next->RightChild;
            return &(CurrentNode->data);
        }
        
        CurrentNode = next;
    }
}

// Driver function to print the nodes
void ThreadedPostorderIterator::Postorder() {
    for (char* ch = Next(); ch; ch = Next()) {
        std::cout << *ch << " ";
    }
    std::cout << std::endl;
}

// Function for post-order traversal using a stack.
char* ThreadedPostorderIterator::Next() {
    // First time initialization
    static bool firstCall = true;
    if (firstCall) {
        firstCall = false;
        if (t.root->LeftThread) return nullptr; // Empty tree

        // Find the leftmost leaf node
        CurrentNode = t.root->LeftChild;
        while (!CurrentNode->LeftThread || !CurrentNode->RightThread) {
            s.push(CurrentNode);
            if (!CurrentNode->LeftThread) {
                CurrentNode = CurrentNode->LeftChild;
            } else if (!CurrentNode->RightThread) {
                CurrentNode = CurrentNode->RightChild;
            }
        }
        return &(CurrentNode->data);
    }

    // If we've returned to root, we're done
    if (CurrentNode == t.root) {
        return nullptr;
    }

    // If stack is empty, we must be done (including processing the root)
    if (s.empty()) {
        return nullptr;  // Do not return 'A' again
    }

    ThreadedNode* parent = s.top();

    // If current node is a left child and parent has an unvisited right subtree
    if (parent->LeftChild == CurrentNode && !parent->RightThread) {
        // Move to the parent's right subtree
        CurrentNode = parent->RightChild;

        // Find the leftmost leaf in this right subtree
        while (!CurrentNode->LeftThread || !CurrentNode->RightThread) {
            s.push(CurrentNode);
            if (!CurrentNode->LeftThread) {
                CurrentNode = CurrentNode->LeftChild;
            } else if (!CurrentNode->RightThread) {
                CurrentNode = CurrentNode->RightChild;
            }
        }
    } else {
        // We've finished a right subtree or there is no right subtree
        // Time to process the parent
        CurrentNode = parent;
        s.pop();
    }

    return &(CurrentNode->data);
}
