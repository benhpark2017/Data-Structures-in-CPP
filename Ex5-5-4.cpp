/**
 * Solutions to Chapter 5, Section 5, Exercise 4 of Horowitz's
 * Fundamentals of Data Structures in C++.
 * 
 * Task 5:
 * Consider threading a binary tree using preorder threads rather than in-order
 * threads as in the text. Which of the traversals can be done without the use
 * of a stack? For those that can be performed without a stack, write a C++
 * function and analyze its space complexity.
 *  
 * The pre-order traversal can be done without using a stack.
 * The in-order and post-order traversals, however, require the use of a stack.
 * 
 * The time complexity of the function ThreadedPreorderIterator::Next() is
 * O(1), as it performs a constant number of operations regardless of the
 * size of the threaded binary tree.
 *
 * The space complexity is also O(1), as it only uses a fixed amount of
 * space to store the current node and the root node of the threaded binary
 * tree.
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
    // In-order traversal using a set of driver and workhorse functions
    std::cout << "In-order traversal of the threaded binary tree." << std::endl;
    tree.Inorder();
    std::cout << '\n' << std::endl;
    //Output should be H D I B E A F C G.
    
    // Pre-order traversal
    ThreadedPreorderIterator preorderIterator(tree);
    std::cout << "Pre-order traversal of threaded binary tree using an iterator:"
              << std::endl;
    preorderIterator.Preorder();
    // Pre-order traversal using a set of driver and workhorse functions
    std::cout << "Pre-order traversal of the threaded binary tree." << std::endl;
    tree.Preorder();
    std::cout << '\n' << std::endl;
    //Output should be A B D H I E C F G
    
    // Post-order traversal
    ThreadedPostorderIterator postorderIterator(tree);
    std::cout << "Post-order traversal of threaded binary tree using an iterator:"
              << std::endl;
    postorderIterator.Postorder();
    // Postorder traversal using a set of driver and workhorse functions
    std::cout << "Post-order traversal of the threaded binary tree." << std::endl;
    tree.Postorder();
    std::cout << '\n' << std::endl;
    //Output should be H I D E B F G C A
    
    std::cout << "End of program." << std::endl;
    
    return 0;
}


/****************************ThreadedTree.h********************************/
#ifndef THREADED_TREE_H
#define THREADED_TREE_H

#include <iostream>
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
    ThreadedTree();     
    void Initialize();  
    
    void Inorder();
    void Preorder();
    void Postorder();
    //add more driver traversal functions here
    
  private:
    ThreadedNode* root;
    
    void Inorder(ThreadedNode* node);
    void Preorder(ThreadedNode* node);
    void Postorder(ThreadedNode* node);
    //add more workhorse traversal functions here
};

class ThreadedInorderIterator {
  public:
    ThreadedInorderIterator(ThreadedTree tree) : t(tree), CurrentNode(nullptr) {
        InitializeStack();
    }
    char* Next();
    void Inorder();
    
  private:
    ThreadedTree t;
    ThreadedNode* CurrentNode;
    std::stack<ThreadedNode*> s;
    void InitializeStack();
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
    ThreadedPostorderIterator(ThreadedTree tree) : t(tree), CurrentNode(nullptr) {
        InitializeStack();
    }
    char* Next();
    void Postorder();
  private:
    ThreadedTree t;
    ThreadedNode* CurrentNode;
    std::stack<ThreadedNode*> s;
    void InitializeStack();
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
//This function is good as is. Do not modify this function!
void ThreadedTree::Initialize() {
    // Declare node pointers
    ThreadedNode *A, *B, *C, *D, *E, *F, *G, *H, *I;
    
    // Initialize root and first node (A)
    root->LeftChild = A = new ThreadedNode('A', nullptr, nullptr, FALSE, FALSE);
    root->LeftThread = FALSE;
    
    // Initialize nodes
    A->LeftChild = B = new ThreadedNode('B', nullptr, nullptr, FALSE, FALSE);
    A->RightChild = C = new ThreadedNode('C', nullptr, nullptr, FALSE, FALSE);
    
    B->LeftChild = D = new ThreadedNode('D', nullptr, nullptr, FALSE, FALSE);
    B->RightChild = E = new ThreadedNode('E', nullptr, nullptr, TRUE, TRUE);
    
    C->LeftChild = F = new ThreadedNode('F', nullptr, nullptr, TRUE, TRUE);
    C->RightChild = G = new ThreadedNode('G', nullptr, nullptr, TRUE, TRUE);
    
    D->LeftChild = H = new ThreadedNode('H', nullptr, nullptr, TRUE, TRUE);
    D->RightChild = I = new ThreadedNode('I', nullptr, nullptr, TRUE, TRUE);
    
    // Set up pre-order threads
    // Preorder: A B D H I E C F G
    
    H->LeftChild = D;        // H's left points to root
    H->RightChild = I;       // H's right points to I (preorder successor)
    
    I->LeftChild = H;        // I's left points to H
    I->RightChild = E;       // I's right points to E (preorder successor)
    
    E->LeftChild = I;        // E's left points to D
    E->RightChild = C;       // E's right points to C (preorder successor)
    
    F->LeftChild = C;        // F's left points to C
    F->RightChild = G;       // F's right points to G (preorder successor)
    
    G->LeftChild = F;        // G's left points to F
    G->RightChild = root;    // G's right points to root (end of traversal)
    
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
void ThreadedTree::Inorder(ThreadedNode* node) {
    if (node == nullptr) return;
    
    // Process left subtree
    if (!node->LeftThread) {
        Inorder(node->LeftChild);
    }
    
    // Process current node
    std::cout << node->data << " ";
    
    // Process right subtree
    if (!node->RightThread) {
        Inorder(node->RightChild);
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
void ThreadedTree::Preorder(ThreadedNode* node) {
    if (node == nullptr) return;
    
    // Process current node
    std::cout << node->data << " ";
    
    // Process left subtree
    if (!node->LeftThread) {
        Preorder(node->LeftChild);
    }
    
    // Process right subtree
    if (!node->RightThread) {
        Preorder(node->RightChild);
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

void ThreadedTree::Postorder(ThreadedNode* node) {
    if (node == nullptr) return;
    
    // Process left subtree
    if (!node->LeftThread) {
        Postorder(node->LeftChild);
    }
    
    // Process right subtree
    if (!node->RightThread) {
        Postorder(node->RightChild);
    }
    
    // Process current node
    std::cout << node->data << " ";
}

/*********************Traversal functions using iterators***********************/

void ThreadedInorderIterator::Inorder() {
    for (char* ch = Next(); ch; ch = Next())
        std::cout << *ch << " ";
    std::cout << std::endl;
}

void ThreadedInorderIterator::InitializeStack() {
    // Start from root's left child (A node)
    ThreadedNode* current = t.root->LeftChild;
    
    // Push all left nodes onto stack
    while (current && !current->LeftThread) {
        s.push(current);
        current = current->LeftChild;
    }
    
    // Push the leftmost node
    if (current) {
        s.push(current);
    }
}

char* ThreadedInorderIterator::Next() {
    // If stack is empty, we're done
    if (s.empty()) {
        return nullptr;
    }
    
    // Get the next node from the stack
    CurrentNode = s.top();
    s.pop();
    char* result = &CurrentNode->data;
    
    // If this node has a right child (not a thread)
    if (!CurrentNode->RightThread) {
        ThreadedNode* temp = CurrentNode->RightChild;
        
        // Push all left nodes of the right subtree
        while (temp && !temp->LeftThread) {
            s.push(temp);
            temp = temp->LeftChild;
        }
        
        // Push the leftmost node of right subtree
        if (temp) {
            s.push(temp);
        }
    }
    
    return result;
}


void ThreadedPreorderIterator::Preorder() {
    for (char* ch = Next(); ch; ch = Next())
        std::cout << *ch << " ";
    std::cout << std::endl;
}

char* ThreadedPreorderIterator::Next() {
    // If we're at root, move to the first node (which is the left child of root)
    if (CurrentNode == t.root) {
        CurrentNode = CurrentNode->LeftChild;
        return &CurrentNode->data;
    }
        
    // If we can go left, do so
    if (!CurrentNode->LeftThread) {
        CurrentNode = CurrentNode->LeftChild;
        return &CurrentNode->data;
    }
        
    // If we can go right, do so
    if (!CurrentNode->RightThread) {
        CurrentNode = CurrentNode->RightChild;
        return &CurrentNode->data;
    }
        
    // Both children are threaded, follow the right thread
    ThreadedNode* next = CurrentNode->RightChild;
        
    // If we've reached the root, we're done
    if (next == t.root) {
        return nullptr;
    }
        
    CurrentNode = next;
    return &CurrentNode->data;
}

void ThreadedPostorderIterator::Postorder() {
    for (char* ch = Next(); ch; ch = Next())
        std::cout << *ch << " ";
    std::cout << std::endl;
}

void ThreadedPostorderIterator::InitializeStack() {
    // Initialize by pushing the leftmost node in postorder traversal
    CurrentNode = t.root->LeftChild;
    
    while (CurrentNode != nullptr && CurrentNode != t.root) {
        s.push(CurrentNode);
        
        // Always go to the leftmost child first, unless it's a thread
        if (!CurrentNode->LeftThread) {
            CurrentNode = CurrentNode->LeftChild;
        } else if (!CurrentNode->RightThread) {
            // If left is threaded, and right is not, go right
            CurrentNode = CurrentNode->RightChild;
        } else {
            // If both are threads, stop descending
            break;
        }
    }
}

char* ThreadedPostorderIterator::Next() {
    static char output;
    
    if (s.empty()) {
        return nullptr;
    }
    
    // Pop the top of the stack
    CurrentNode = s.top();
    s.pop();
    output = CurrentNode->data;

    // If there's a parent node whose right subtree is unvisited
    if (!s.empty() && s.top()->LeftChild == CurrentNode && 
        !s.top()->RightThread && s.top()->RightChild != CurrentNode) {
        
        ThreadedNode* parent = s.top();
        ThreadedNode* rightChild = parent->RightChild;
        
        // Push right subtree's leftmost child onto stack
        while (rightChild != nullptr && rightChild != t.root) {
            s.push(rightChild);
            
            // Go left first unless it's threaded
            if (!rightChild->LeftThread) {
                rightChild = rightChild->LeftChild;
            } else if (!rightChild->RightThread) {
                rightChild = rightChild->RightChild;
            } else {
                break;
            }
        }
    }
    return &output;
}
