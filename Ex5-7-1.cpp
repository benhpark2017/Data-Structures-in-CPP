/**
 * Solutions to Chapter 5, Section 7, Exercises 1, 3, 4, 5, and 6 of Horowitz's
 * Fundamentals of Data Structures in C++.
 * 
 * Task 1: 
 * Write a C++ function to delete an element X from a binary search tree. What
 * is the time complexity of your function?
 * 
 * Task 3:
 * Suppose that each node in a binary search tree also has the field LeftSize
 * as described in the text. Write a C++ function to insert an element x into
 * such a binary search tree. The complexity of your algorithm should be O(h),
 * where h is the height of the search tree. Show that this is the case.
 * 
 * Task 4:
 * Repeat Exercise 3, but this time write a C++ function to delete the element
 * with the kth smallest key in the binary search tree.
 * 
 * Task 5:
 * Write a C++ function that implements the three-way join operation in O(1)
 * time.
 * 
 * Task 6:
 * Write a C++ function that implements the two-way join operation in
 * O(height(A)) time, where A is one of the two trees being joined.
 */

/**********************************main.cpp************************************/
#include "BST.h"
#include <iostream>

// Driver code for testing
int main() {
    BST<int> bst;
    
    // Create elements to insert into the BST
    Element<int> e1 = {10}; // Create Element with key 10
    Element<int> e2 = {5};  // Create Element with key 5
    Element<int> e3 = {15}; // Create Element with key 15
    Element<int> e4 = {12}; // Create Element with key 12
    Element<int> e5 = {20}; // Create Element with key 20
    Element<int> e6 = {30}; // Create Element with key 30

    // Insert elements into the BST
    std::cout << "Inserting 10: " << bst.Insert(e1) << std::endl; // Should succeed
    std::cout << "Inserting 5: " << bst.Insert(e2) << std::endl;  // Should succeed
    std::cout << "Inserting 15: " << bst.Insert(e3) << std::endl; // Should succeed
    std::cout << "Inserting 12: " << bst.Insert(e4) << std::endl; // Should succeed
    std::cout << "Inserting 20: " << bst.Insert(e5) << std::endl; // Should succeed
    std::cout << "Inserting duplicate 10: " << bst.Insert(e1) << std::endl; // Should fail (FALSE)
    std::cout << "Inserting 30: " << bst.Insert(e6) << std::endl; // Should succeed
    
    // Search operation: Search for key 15
    Element<int> searchElement;
    searchElement.key = 15;  // Set key for search
    BstNode<int>* found = bst.Search(searchElement);
    if (found) {
        std::cout << "Element " << searchElement.key << " found in the BST." << std::endl;
    } else {
        std::cout << "Element " << searchElement.key << " not found in the BST." << std::endl;
    }
    
    // Deletion:
    Element<int> deleteElement = {20}; //Create element for deletion
    Boolean deleteSuccessful = bst.DeleteBST(deleteElement);
    if (deleteSuccessful) std::cout << "Deletion of " << deleteElement.key
                                    << " was successful." << std::endl;
    else std::cout << "Element to delete " << deleteElement.key << " not found." << std::endl;
    
    // K-th smallest search: k = 3 (3rd smallest element)
    // should remove the third largest element, which is 12, from the binary search tree
    int k = 3;
    BstNode<int>* kthNode = bst.Search(k);
    if (kthNode && (k % 10 == 1)) {
        std::cout << k << "st smallest element is: " << kthNode->data.key << std::endl;
    } else if (kthNode && (k % 10 == 2)) {
        std::cout << k << "nd smallest element is: " << kthNode->data.key << std::endl;
    } else if (kthNode && (k % 10 == 3)) {
        std::cout << k << "rd smallest element is: " << kthNode->data.key << std::endl;
    } else if (kthNode) {
        std::cout << k << "th smallest element is: " << kthNode->data.key << std::endl;
    } else {
        std::cout << k << "th smallest element not found." << std::endl;
    }
    
    // Duplicate insertion: Try inserting key 10 again
    std::cout << "Attempting to insert duplicate key 10." << std::endl;
    Boolean success = bst.Insert(e1);
    if (!success) {
        std::cout << "Insertion failed: Key 10 is a duplicate." << std::endl;
    } else {
        std::cout << "Insertion succeeded for key 10." << std::endl;
    }
    
    bst.PrintTree();
    
    Boolean kthDeletion{bst.DeleteKth(k)};
    if (kthDeletion && (k % 10 == 1)) {
        std::cout << k << "st smallest element successfully deleted.\n" << std::endl;
    } else if (kthDeletion && (k % 10 == 2)) {
        std::cout << k << "nd smallest element successfully deleted.\n" << std::endl;
    } else if (kthDeletion && (k % 10 == 3)) {
        std::cout << k << "rd smallest element successfully deleted.\n" << std::endl;
    } else if (kthDeletion) {
        std::cout << k << "th smallest element successfully deleted.\n" << std::endl;
    } else {
        std::cout << k << "th smallest element not found." << std::endl;
    }
    
    bst.PrintTree();
    
    // Tree splitting: Split at key 15
    BST<int> leftTree, rightTree;
    Element<int> splitElement;
    std::cout << "Splitting the tree at key 15." << std::endl;
    bst.Split(15, leftTree, splitElement, rightTree);
    
    if (splitElement.key == 15) {
        std::cout << "Tree successfully split at key: " << splitElement.key << std::endl;
    } else {
        std::cout << "Split failed." << std::endl;
    }

    leftTree.PrintTree();
    rightTree.PrintTree();
    
    BST<int> A, B, C;
    
    // Create tree A with elements smaller than x
    Element<int> elementsA[] = {{10}, {5}, {15}};
    for (const auto& elem : elementsA) {
        A.Insert(elem);
    }
    
    // Create tree B with elements larger than x
    Element<int> elementsB[] = {{30}, {25}, {35}};
    for (const auto& elem : elementsB) {
        B.Insert(elem);
    }
    
    // Create element x
    Element<int> x = {20};
    
    // Print original trees
    std::cout << "Tree A (elements < 20): ";
    A.PrintTree();
    std::cout << "Tree B (elements > 20): ";
    B.PrintTree();
    std::cout << "Element x: " << x.key << std::endl;
    
    // Perform ThreeWayJoin
    C.ThreeWayJoin(A, x, B);
    
    // Verify the result
    std::cout << "Three-way joined Tree C\n";
    C.PrintTree();
    
    
    BST<int> D, E, F;
    
    // Test Case 1: Normal case
    std::cout << "Test Case 1: Normal case\n";
    // Create tree D with smaller elements
    Element<int> elementsD[] = {{10}, {5}, {15}};
    for (const auto& elem : elementsD) {
        D.Insert(elem);
    }
    
    // Create tree E with larger elements
    Element<int> elementsE[] = {{30}, {25}, {35}};
    for (const auto& elem : elementsE) {
        E.Insert(elem);
    }
    
    std::cout << "Tree D: ";
    D.PrintTree();
    std::cout << "Tree E: ";
    E.PrintTree();
    
    F.TwoWayJoin(D, E);
    
    std::cout << "Two-way joined Tree F\n";
    F.PrintTree();
    
    std::cout << "End of program." << std::endl;
    
    return 0;
}



/***********************************BST.h*************************************/
#ifndef BST_H
#define BST_H

enum Boolean { FALSE, TRUE };

// Forward declarations
template <typename Type> class BstNode;
template <typename Type> class BST;

template <typename Type>
class Element {
  public:
    Type key;

    friend class BstNode<Type>;
    friend class BST<Type>;
};

template <typename Type>
class BstNode {
  private:
    BstNode* LeftChild;
    BstNode* RightChild;
    int LeftSize;
  public:
    Element<Type> data;

    BstNode(const Element<Type>& keyValue) 
        : LeftChild(nullptr), RightChild(nullptr), LeftSize(1), data(keyValue) {}
    
    // Add a constructor that takes just the key value
    BstNode(const Type& key) 
        : LeftChild(nullptr), RightChild(nullptr), LeftSize(1) {
        data.key = key;
    }

    friend class BST<Type>;
};

template <typename Type>
class BST {
    BstNode<Type>* root;

public:
    BST() : root(nullptr) {}

    void InorderTraversal(BstNode<Type>* node);
    
    void PrintTree();
    
    void ThreeWayJoin(BST<Type> &A, Element<Type> &x, BST<Type> &B);
    
    void TwoWayJoin(BST<Type> &A, BST<Type> &B);
    
    BstNode<Type>* Search(const Element<Type>& x);
    BstNode<Type>* Search(BstNode<Type>* b, const Element<Type>& x);
    BstNode<Type>* IterSearch(const Element<Type>& x);
    BstNode<Type>* Search(int k);
    Boolean Insert(const Element<Type>& x);
    Boolean DeleteKth(int k);
    Boolean DeleteBST(const Element<Type>& x);
    Element<Type>* Split(Type i, BST<Type>& B, Element<Type>& x, BST<Type>& C);
};

#include "BST.tpp"

#endif //BST_H



/***********************************BST.tpp************************************/
#include <iostream>

template <typename Type>
void BST<Type>::InorderTraversal(BstNode<Type>* node) {
    if (node) {
        InorderTraversal(node->LeftChild);
        std::cout << node->data.key << " ";
        InorderTraversal(node->RightChild);
    }
}

template <typename Type>
void BST<Type>::PrintTree() {
    std::cout << "Tree contents: ";
    InorderTraversal(root);
    std::cout << std::endl;
}

template <typename Type>
void BST<Type>::ThreeWayJoin(BST<Type> &A, Element<Type> &x, BST<Type> &B) {
    // Create a new node for x
    BstNode<Type>* newNode = new BstNode<Type>(x);
    
    // Set the left and right children
    newNode->LeftChild = A.root;
    newNode->RightChild = B.root;
    
    // Calculate LeftSize for the new node
    newNode->LeftSize = 1;  // Count itself
    if (A.root) {
        newNode->LeftSize += A.root->LeftSize;  // Add size of left subtree
        
        // Count right children in A's root if they exist
        BstNode<Type>* temp = A.root;
        while (temp->RightChild) {
            newNode->LeftSize += temp->RightChild->LeftSize;
            temp = temp->RightChild;
        }
    }
    
    // Set the new root
    this->root = newNode;
    
    // Clear A and B (they no longer own their nodes)
    A.root = nullptr;
    B.root = nullptr;
}


template <typename Type>
void BST<Type>::TwoWayJoin(BST<Type> &A, BST<Type> &B) {
    // Handle empty tree cases
    if (!A.root) {
        this->root = B.root;
        B.root = nullptr;
        return;
    }
    if (!B.root) {
        this->root = A.root;
        A.root = nullptr;
        return;
    }
    
    // Find the largest node in A
    BstNode<Type>* maxA = A.root;
    BstNode<Type>* parentMaxA = nullptr;
    while (maxA->RightChild) {
        parentMaxA = maxA;
        maxA = maxA->RightChild;
    }
    
    // Remove maxA from tree A
    if (parentMaxA) {
        parentMaxA->RightChild = maxA->LeftChild;
    } else {
        A.root = maxA->LeftChild;
    }
    
    // Use maxA as the new root
    maxA->LeftChild = A.root;
    maxA->RightChild = B.root;
    
    // Calculate new LeftSize for maxA
    maxA->LeftSize = 1;  // Count itself
    if (A.root) {
        maxA->LeftSize += A.root->LeftSize;
        
        // Count right children in A's root if they exist
        BstNode<Type>* temp = A.root;
        while (temp->RightChild) {
            maxA->LeftSize += temp->RightChild->LeftSize;
            temp = temp->RightChild;
        }
    }
    
    // Set the new root
    this->root = maxA;
    
    // Clear A and B as they no longer own their nodes
    A.root = nullptr;
    B.root = nullptr;
}


// Driver function that searches the binary tree *this for an element
template <typename Type>
BstNode<Type>* BST<Type>::Search(const Element<Type>& x) {
    return Search(root, x);
}

// Workhorse function for search
template <typename Type>
BstNode<Type>* BST<Type>::Search(BstNode<Type>* b, const Element<Type>& x) {
    if (!b) return 0;
    if (x.key == b->data.key) return b;
    if (x.key < b->data.key) return Search(b->LeftChild, x);
    return Search(b->RightChild, x);
}

template <typename Type>
BstNode<Type>* BST<Type>::IterSearch(const Element<Type>& x) {
    for (BstNode<Type>* t = root; t; ) {
        if (x.key == t->data.key) return t;
        if (x.key < t->data.key) t = t->LeftChild;
        else t = t->RightChild;
    }
    return 0;
}

// Searches the binary search tree for the k-th smallest element.
template <typename Type>
BstNode<Type>* BST<Type>::Search(int k) {
    BstNode<Type>* t = root;
    while (t) {
        if (k == t->LeftSize) return t;
        if (k < t->LeftSize) t = t->LeftChild;
        else {
            k -= t->LeftSize;
            t = t->RightChild;
        }
    }
    return 0;
}

template <typename Type>
Boolean BST<Type>::Insert(const Element<Type>& x) {
    BstNode<Type>* p = root;
    BstNode<Type>* q = nullptr;

    while (p) {
        q = p;
        if (x.key == p->data.key) return FALSE;
        if (x.key < p->data.key) { 
            p->LeftSize++; // Delete this line for an insert function without LeftSize
            p = p->LeftChild; 
        }
        else p = p->RightChild;
    }

    // Create new node with the Element
    p = new BstNode<Type>(x);

    if (!root) root = p;
    else if (x.key < q->data.key) q->LeftChild = p;
    else q->RightChild = p;

    return TRUE;
}

template <typename Type>
Boolean BST<Type>::DeleteKth(int k) {
    if (k < 1 || !root) return FALSE;
    
    BstNode<Type>* p = root;      // Current node
    BstNode<Type>* parent = nullptr;  // Parent of current node
    Boolean isLeftChild = TRUE;    // Is p the left child of parent?
    
    // Search for the kth smallest element
    while (p) {
        if (k == p->LeftSize) {
            // Found the kth element, now delete it
            break;
        }
        else if (k < p->LeftSize) {
            // kth element is in left subtree
            parent = p;
            p = p->LeftChild;
            isLeftChild = TRUE;
            // No need to adjust k as we're going left
        }
        else {
            // kth element is in right subtree
            k -= p->LeftSize;  // Adjust k for right subtree
            parent = p;
            p = p->RightChild;
            isLeftChild = FALSE;
        }
    }
    
    // If k is larger than the tree size
    if (!p) return FALSE;
    
    // Now we need to handle the deletion
    // Update LeftSize for all ancestors
    BstNode<Type>* temp = root;
    int originalK = p->LeftSize;
    while (temp != p) {
        if (p->data.key < temp->data.key) {
            temp->LeftSize--;
            temp = temp->LeftChild;
        }
        else {
            temp = temp->RightChild;
        }
    }
    
    // Case 1: Node to delete has no children
    if (!p->LeftChild && !p->RightChild) {
        if (p == root)
            root = nullptr;
        else if (isLeftChild)
            parent->LeftChild = nullptr;
        else
            parent->RightChild = nullptr;
        delete p;
    }
    // Case 2: Node to delete has only right child
    else if (!p->LeftChild) {
        if (p == root)
            root = p->RightChild;
        else if (isLeftChild)
            parent->LeftChild = p->RightChild;
        else
            parent->RightChild = p->RightChild;
        delete p;
    }
    // Case 3: Node to delete has only left child
    else if (!p->RightChild) {
        if (p == root)
            root = p->LeftChild;
        else if (isLeftChild)
            parent->LeftChild = p->LeftChild;
        else
            parent->RightChild = p->LeftChild;
        delete p;
    }
    // Case 4: Node to delete has both children
    else {
        // Find in-order successor (smallest item in right subtree)
        BstNode<Type>* successor = p->RightChild;
        BstNode<Type>* successorParent = p;
        
        while (successor->LeftChild) {
            successorParent->LeftSize--;  // Update LeftSize while finding successor
            successorParent = successor;
            successor = successor->LeftChild;
        }
        
        // Copy successor data to p
        p->data = successor->data;
        
        // Update LeftSize of the node we're deleting
        p->LeftSize = originalK;
        
        // Delete successor node (which is guaranteed to have at most one child)
        if (successorParent == p)
            p->RightChild = successor->RightChild;
        else
            successorParent->LeftChild = successor->RightChild;
        
        delete successor;
    }
    
    return TRUE;
}

template <typename Type>
Boolean BST<Type>::DeleteBST(const Element<Type>& x) {
    // First, find the node to delete and its parent
    BstNode<Type>* p = root;    // Current node
    BstNode<Type>* parent = nullptr;  // Parent of current node
    Boolean isLeftChild = TRUE;  // Is p the left child of parent?

    // Search for the node to delete
    while (p && p->data.key != x.key) {
        parent = p;
        if (x.key < p->data.key) {
            p = p->LeftChild;
            isLeftChild = TRUE;
        } else {
            p = p->RightChild;
            isLeftChild = FALSE;
        }
    }

    // If element not found, return FALSE
    if (!p) return FALSE;

    // Case 1: Node to delete has no children (leaf node)
    if (!p->LeftChild && !p->RightChild) {
        if (p == root)
            root = nullptr;
        else if (isLeftChild)
            parent->LeftChild = nullptr;
        else
            parent->RightChild = nullptr;
        delete p;
    }
    // Case 2: Node to delete has only right child
    else if (!p->LeftChild) {
        if (p == root)
            root = p->RightChild;
        else if (isLeftChild)
            parent->LeftChild = p->RightChild;
        else
            parent->RightChild = p->RightChild;
        delete p;
    }
    // Case 3: Node to delete has only left child
    else if (!p->RightChild) {
        if (p == root)
            root = p->LeftChild;
        else if (isLeftChild)
            parent->LeftChild = p->LeftChild;
        else
            parent->RightChild = p->LeftChild;
        delete p;
    }
    // Case 4: Node to delete has both children
    else {
        // Find in-order successor (smallest item in right subtree)
        BstNode<Type>* successor = p->RightChild;
        BstNode<Type>* successorParent = p;
        
        while (successor->LeftChild) {
            successorParent = successor;
            successor = successor->LeftChild;
        }
        
        // Copy successor data to p
        p->data = successor->data;
        
        // Delete successor node (which is guaranteed to have at most one child)
        if (successorParent == p)
            p->RightChild = successor->RightChild;
        else
            successorParent->LeftChild = successor->RightChild;
        
        delete successor;
    }

    return TRUE;
}

template <typename Type>
Element<Type>* BST<Type>::Split(Type i, BST<Type>& B, Element<Type>& x, BST<Type>& C) {
    if (!root) {
        B.root = C.root = nullptr;
        return nullptr;
    }

    // Create nodes using the new constructor that takes just the key
    BstNode<Type> *Y = new BstNode<Type>(i), *L = Y;
    BstNode<Type> *Z = new BstNode<Type>(i), *R = Z;
    BstNode<Type>* t = root;

    // Rest of the Split function remains the same
    while (t) {
        if (i == t->data.key) {
            L->RightChild = t->LeftChild;
            R->LeftChild = t->RightChild;
            x = t->data;
            B.root = Y->RightChild;
            delete Y;
            C.root = Z->LeftChild;
            delete Z;
            return &x;
        } else if (i < t->data.key) {
            R->LeftChild = t;
            R = t;
            t = t->LeftChild;
        } else {
            L->RightChild = t;
            L = t;
            t = t->RightChild;
        }
    }
    L->RightChild = R->LeftChild = nullptr;
    B.root = Y->RightChild;
    delete Y;
    C.root = Z->LeftChild;
    delete Z;
    return nullptr;
}
