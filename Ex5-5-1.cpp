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

/**************************************main.cpp******************************************/
#include "ThreadedTree.h"
#include <iostream>

int main() {
    ThreadedTree tree;
    tree.Initialize();
    
    // In-order traversal of threaded binary tree using an iterator
    ThreadedInorderIterator inorderIterator(tree);
    std::cout << "In-order traversal of threaded binary tree using an iterator."
              << std::endl;
    inorderIterator.Inorder();
    std::cout << std::endl;
    
    // In-order traversal using a set of driver and workhorse functions
    std::cout << "In-order traversal of the threaded binary tree." << std::endl;
    tree.Inorder();
    std::cout << '\n' << std::endl;
    
    // Post-order traversal of threaded binary tree using an iterator
    ThreadedPostorderIterator postorderIterator(tree);
    std::cout << "Post-order traversal of threaded binary tree using an iterator:"
              << std::endl;
    postorderIterator.Postorder();
    std::cout << std::endl;

    // Postorder traversal using a set of driver and workhorse functions
    std::cout << "Post-order traversal of the threaded binary tree." << std::endl;
    tree.Postorder();
    std::cout << '\n' << std::endl;
    
    // Pre-order traversal of threaded binary tree using an iterator
    ThreadedPreorderIterator preorderIterator(tree);
    std::cout << "Pre-order traversal of threaded binary tree using an iterator:"
              << std::endl;
    preorderIterator.Preorder();
    std::cout << std::endl;
    
    // Preorder traversal using a set of driver and workhorse functions
    std::cout << "Pre-order traversal of the threaded binary tree." << std::endl;
    tree.Preorder();
    std::cout << '\n' << std::endl;
    
    std::cout << "End of program." << std::endl;
    return 0;
}



/***********************************ThreadedTree.h***************************************/
#ifndef THREADED_TREE_H
#define THREADED_TREE_H

enum Boolean {FALSE, TRUE};

class ThreadedNode {
    friend class ThreadedTree;
    friend class ThreadedInorderIterator;
    friend class ThreadedPostorderIterator;
    friend class ThreadedPreorderIterator;
    
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
    friend class ThreadedPreorderIterator;
    
  public:
    ThreadedTree();
    void Initialize();
    
    void Inorder();
    void Postorder();
    void Preorder();
    
    void InsertRight(ThreadedNode* s, ThreadedNode* r);
    void InsertLeft(ThreadedNode* s, ThreadedNode* l);
    ThreadedNode* InorderSucc(ThreadedNode* r);
    
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

class ThreadedPostorderIterator {
  public:
    char* Next();
    void Postorder();
    ThreadedPostorderIterator(ThreadedTree& tree) : t(tree), CurrentNode(nullptr) {}
  private:
    ThreadedTree& t;
    ThreadedNode* CurrentNode;
};

class ThreadedPreorderIterator {
  public:
    char* Next();
    void Preorder();
    ThreadedPreorderIterator(ThreadedTree& tree) : t(tree), CurrentNode(nullptr) {}
  private:
    ThreadedTree& t;
    ThreadedNode* CurrentNode;
};

#endif //THREADED_TREE_H



/**********************************ThreadedTree.cpp**************************************/
#include "ThreadedTree.h"
#include <iostream>
#include <stack>

//Find the in-order successor of CurrentNode in a threaded binary tree
char* ThreadedInorderIterator::Next() {
    ThreadedNode* temp = CurrentNode->RightChild;
    if (!CurrentNode->RightThread)
        while (!temp->LeftThread) temp = temp->LeftChild;
    CurrentNode = temp;
    if (CurrentNode == t.root) return 0;
    else return &CurrentNode->data;
}

// Driver function to print the nodes
void ThreadedInorderIterator::Inorder() {
    for (char* ch = Next(); ch; ch = Next())
        std::cout << *ch << " ";
}

//Find the post-order successor of CurrentNode in a threaded binary tree
char* ThreadedPostorderIterator::Next() {
    if (CurrentNode == t.root) {
        return nullptr; // Traversal completed
    }

    ThreadedNode* p = CurrentNode;

    // First call: move to the leftmost node
    if (p == nullptr) {
        p = t.root->LeftChild;
        while (!p->LeftThread || !p->RightThread) {
            if (!p->LeftThread) {
                p = p->LeftChild;  // Move to the leftmost node
            } else {
                p = p->RightChild;
            }
        }
        CurrentNode = p;
        return &p->data;
    }

    // Find the parent node
    ThreadedNode* parent = nullptr;
    std::stack<ThreadedNode*> s;
    s.push(t.root);
    while (!s.empty()) {
        ThreadedNode* q = s.top();
        s.pop();

        if (q->LeftChild == p || q->RightChild == p) {
            parent = q;
            break;
        }

        if (!q->RightThread) {
            s.push(q->RightChild);
        }
        if (!q->LeftThread) {
            s.push(q->LeftChild);
        }
    }

    if (parent) {
        // If p is the right child or the right subtree is already visited
        if (parent->RightChild == p || parent->RightThread || parent->RightChild == CurrentNode) {
            p = parent;
        } else {
            // Move to the right subtree
            p = parent->RightChild;
            while (!p->LeftThread || !p->RightThread) {
                if (!p->LeftThread) {
                    p = p->LeftChild;
                } else {
                    p = p->RightChild;
                }
            }
        }
        CurrentNode = p;

        // Check if we've reached the root node after visiting 'A'
        if (CurrentNode == t.root->LeftChild) { // 'A' node is visited
            CurrentNode = t.root; // Set to root to mark end of traversal
        }

        return &p->data;
    } else {
        CurrentNode = t.root;
        return nullptr; // Traversal completed
    }
}

// Driver function to print the nodes
void ThreadedPostorderIterator::Postorder() {
    for (char* ch = Next(); ch; ch = Next()) {
        std::cout << *ch << " ";  // Print data (skip the root/dummy node)
    }
}

char* ThreadedPreorderIterator::Next() {
    // If traversal has finished (i.e., CurrentNode is set to the dummy root), return nullptr
    if (CurrentNode == t.root) {
        return nullptr;
    }

    // First call: Start from the root of the actual tree
    if (CurrentNode == nullptr) {
        CurrentNode = t.root->LeftChild;  // Start at the actual root (first node of the tree)
        return &CurrentNode->data;
    }

    // Get current node reference
    ThreadedNode* p = CurrentNode;

    // Move to the left child if it exists (and is not a thread)
    if (!p->LeftThread) {
        CurrentNode = p->LeftChild;
    }
    // Otherwise, if there is a right child, move to it
    else if (!p->RightThread) {
        CurrentNode = p->RightChild;
    }
    // If both are threads, follow the threads to the next node in preorder
    else {
        // Continue moving to the right until we find a node without a thread
        while (p->RightThread && p->RightChild != t.root) {
            p = p->RightChild;
        }
        CurrentNode = p->RightChild;

        // If we reach the dummy root, traversal is done
        if (CurrentNode == t.root) {
            return nullptr;
        }
    }

    // Return the data of the current node
    return &CurrentNode->data;
}


// Driver function to print the nodes
void ThreadedPreorderIterator::Preorder() {
    for (char* ch = Next(); ch; ch = Next()) {
        std::cout << *ch << " ";  // Print data (skip the root/dummy node)
    }
}

ThreadedTree::ThreadedTree() {
    root = new ThreadedNode;
    root->RightChild = root->LeftChild = root;
    root->LeftThread = TRUE;
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

void ThreadedTree::Inorder() {
    if (root->LeftChild != nullptr) {
        Inorder(root->LeftChild);  // Call the workhorse function with the root's left child (actual root)
    }
}


void ThreadedTree::Inorder(ThreadedNode* node) {
    // Find the leftmost node (the first node in in-order sequence)
    while (!node->LeftThread) {
        node = node->LeftChild;
    }

    // Traverse the tree in in-order
    while (node != root) {
        // Print the data of the current node
        std::cout << node->data << " ";

        // If the current node has a right thread, follow it to the next node in in-order sequence
        if (node->RightThread) {
            node = node->RightChild;
        } else {
            // Otherwise, move to the leftmost node in the right subtree
            node = node->RightChild;
            while (!node->LeftThread) {
                node = node->LeftChild;
            }
        }
    }
}
