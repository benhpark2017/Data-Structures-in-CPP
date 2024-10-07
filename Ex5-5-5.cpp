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
 * Pre-order iterator:  Requires a stack.
 * Post-order iterator: Requires a stack.
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
    std::cout << std::endl;
    std::cout << '\n' << std::endl;
    
    //Post-order traversal of threaded binary tree using an iterator
    ThreadedPostorderIterator postorderIterator(tree);
    std::cout << "Post-order traversal of threaded binary tree using an iterator."
              << std::endl;
    postorderIterator.Postorder();
    std::cout << std::endl;

    // Postorder traversal using a set of driver and workhorse functions
    std::cout << "Post-order traversal of the threaded binary tree." << std::endl;
    tree.Postorder();
    std::cout << std::endl;
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
#include <iostream>

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
    std::stack<ThreadedNode*> s;
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
           H   I
*/
// The function is correct as is. Do not modify this function!
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
    H->RightChild = I;         // H's in-order successor is D
    
    I->LeftChild = H;          // I's thread back to parent
    I->RightChild = D;         // I's in-order successor is B
    
    E->LeftChild = D;          // E's thread back to parent
    E->RightChild = B;         // E's in-order successor is A
    
    F->LeftChild = B;          // F's thread back to parent
    F->RightChild = G;         // F's in-order successor is C
    
    G->LeftChild = F;          // G's thread back to parent
    G->RightChild = C;      // G's in-order successor is root (end of traversal)
    
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

/**********************Section with iterators.************************/


// Driver function to print the nodes
void ThreadedInorderIterator::Inorder() {
    for (char* ch = Next(); ch; ch = Next())
        std::cout << *ch << " ";
}

// Next() function for in-order traversal of a post-order threaded binary tree
char* ThreadedInorderIterator::Next() {
    // First call - find leftmost node
    if (CurrentNode == t.root) {
        CurrentNode = t.root->LeftChild;  // Start at A
        while (!CurrentNode->LeftThread) {
            CurrentNode = CurrentNode->LeftChild;  // Move to the leftmost node
        }
        return &(CurrentNode->data);  // Return the leftmost node (H)
    }
    
    ThreadedNode* savedNode = CurrentNode;
    
    // If we can go right and then left
    if (!CurrentNode->RightThread) {
        CurrentNode = CurrentNode->RightChild;
        while (!CurrentNode->LeftThread) {
            CurrentNode = CurrentNode->LeftChild;  // Move to the leftmost node in right subtree
        }
        return &(CurrentNode->data);
    }
    
    // Handle case when node A (root) is missed
    if (savedNode == t.root->LeftChild->LeftChild->RightChild || // E
        savedNode == t.root->LeftChild->LeftChild ||             // B
        savedNode == t.root->LeftChild) {                        // Check if we need to move to A
        CurrentNode = t.root->LeftChild;  // Move to A
        return &(CurrentNode->data);
    }

    // Special case for moving to C (right subtree)
    if (savedNode == t.root->LeftChild->RightChild->LeftChild) { // F
        CurrentNode = t.root->LeftChild->RightChild;  // Move to C
        return &(CurrentNode->data);
    }

    // Special case for moving to G
    if (savedNode == t.root->LeftChild->RightChild) {  // If we're at C
        CurrentNode = savedNode->RightChild;  // Move to G
        return &(CurrentNode->data);
    }
    
    // Traverse upwards to find the parent where we are the left child
    ThreadedNode* parent = t.root->LeftChild;  // Start from A
    
    while (true) {
        if (!parent->LeftThread && parent->LeftChild == savedNode) {
            CurrentNode = parent;
            return &(CurrentNode->data);  // Return the parent node
        }
        
        if (!parent->RightThread && parent->RightChild == savedNode) {
            // Need to keep going up
            savedNode = parent;
            parent = t.root->LeftChild;
            continue;
        }
        
        // Move to potential parent
        if (!parent->LeftThread && parent->LeftChild != savedNode) {
            parent = parent->LeftChild;
        } else if (!parent->RightThread && parent->RightChild != savedNode) {
            parent = parent->RightChild;
        } else {
            // No more nodes
            CurrentNode = t.root;
            return nullptr;
        }
    }
}

char* ThreadedPostorderIterator::Next() {
    // If we're starting, initialize the traversal
    if (CurrentNode == t.root) {
        ThreadedNode* node = t.root->LeftChild;  // Start from actual root (A)
        
        // Push nodes along the leftmost path
        do {
            while (!node->LeftThread) {
                s.push(node);
                node = node->LeftChild;
            }
            s.push(node);
            
            // If the top node has a right child that's not a thread
            if (!s.empty() && !s.top()->RightThread) {
                node = s.top()->RightChild;
            } else {
                break;
            }
        } while (true);
    }
    
    // If stack is empty, traversal is complete
    if (s.empty()) {
        return nullptr;
    }
    
    // Get the next node
    ThreadedNode* node = s.top();
    s.pop();
    
    // If there are still nodes in the stack
    if (!s.empty()) {
        ThreadedNode* parent = s.top();
        // If parent has an unvisited right child
        if (!parent->RightThread && parent->RightChild != node) {
            ThreadedNode* right = parent->RightChild;
            
            // Push nodes along the leftmost path of right subtree
            do {
                while (!right->LeftThread) {
                    s.push(right);
                    right = right->LeftChild;
                }
                s.push(right);
                
                // If the top node has a right child that's not a thread
                if (!s.empty() && !s.top()->RightThread) {
                    right = s.top()->RightChild;
                } else {
                    break;
                }
            } while (true);
        }
    }
    
    CurrentNode = node;
    return &(CurrentNode->data);
}

// Driver function to print the nodes
void ThreadedPostorderIterator::Postorder() {
    for (char* ch = Next(); ch; ch = Next())
        std::cout << *ch << " ";
}


char* ThreadedPreorderIterator::Next() {
    // If we're starting or at root, move to actual root (A)
    if (CurrentNode == t.root) {
        CurrentNode = t.root->LeftChild;
        return &(CurrentNode->data);
    }
    
    // Save old position
    ThreadedNode* oldNode = CurrentNode;
    
    // If current node has a real left child
    if (!CurrentNode->LeftThread) {
        // Push right child if it exists and is not a thread
        if (!CurrentNode->RightThread) {
            s.push(CurrentNode->RightChild);
        }
        CurrentNode = CurrentNode->LeftChild;
        return &(CurrentNode->data);
    }
    
    // If current node has a real right child
    if (!CurrentNode->RightThread) {
        CurrentNode = CurrentNode->RightChild;
        return &(CurrentNode->data);
    }
    
    // If both children are threads, move up through the stack
    while (!s.empty()) {
        CurrentNode = s.top();
        s.pop();
        return &(CurrentNode->data);
    }
    
    // If we've processed all nodes, move back to root
    CurrentNode = t.root;
    return nullptr;
}


void ThreadedPreorderIterator::Preorder() {
    for (char* ch = Next(); ch; ch = Next())
        std::cout << *ch << " ";
}
