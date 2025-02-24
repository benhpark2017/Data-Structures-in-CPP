/**
 * Solutions to Chapter 9, Section 3, Exercise 5 of Horowitz, Sahni, and Mehta's 
 * Fundamentals of Data Structures in C++, 1st Edition.
 * 
 * Task 5: Lazy Deletion.
 * Another way to handle the deletion of arbitrary elements from a min leftist
 * tree is lazy deletion, wherein a bool data member, deleted, is used. When an 
 * element is deleted by DeleteElement function, its deleted data member is set 
 * to true. However, the node is not physically deleted. When a DeleteMin 
 * operation is performed, we first search for the minimum element not deleted 
 * by performing a limited pre-order search. This pre-order search traverses 
 * only the upper part of the tree as needed to identify the min element. All 
 * deleted elements encountered are physically deleted, and their subtrees are 
 * combined to obtain the new min leftist tree. 
 * (a) Write a function to delete the element in node x of a min leftist tree. 
 * (b) Write another function that deletes the min element from a min leftist 
 *     tree from which several elements have been deleted using the former 
 *     function.
 * (c) What is the complexity of the latter as a function of the number of
 *     deleted elements encountered and the number of elements in the entire
 *     tree?
 * 
 * Response to conceptual question:
 * (c) The complexity of DeleteMin function is O(n + k*log(n)), where k is the
 *     number of deleted elements encountered and n is the number of elements
 *     in the entire tree.
 *     In the worst case, the search visits O(h) nodes, where h = O(log(n)) is
 *     the height of the leftist tree. Hence, the search takes O(log(n)) time.
 *     For the physical deletion of deleted nodes, the function traverses the
 *     entire tree once, visiting O(n) nodes. For each deleted node k, the
 *     MinUnion operation takes O(log(n)) time, so if there are k deleted
 *     nodes, the total time for merging is O(k*log(n)).
 *     Hence, the overall time complexity is O(log(n) + n + k*log(n)), which
 *     gives O(n + k*log(n)).
 */

/*********************************main.cpp*************************************/
#include <iostream>
#include <algorithm>
#include <queue>
#include "MinLeftistTree.h"

int main() {
    MinLeftistTree<int> tree;
    Element<int> elements[] = {1, 10, 5, 20, 3, 8, 35, 40, 18, 21};

    tree.Initialize(elements, 10);
    std::cout << "Initial tree:" << std::endl;
    tree.PrintTree();

    // Test DeleteElement
    std::cout << "\n=== Testing DeleteElement ===\n";
    
    // Delete some elements using DeleteElement
    int keysToMarkDeleted[] = {3, 18, 35};
    for (int key : keysToMarkDeleted) {
        Element<int>* elementToDelete = new Element<int>(key);
        std::cout << "\nMarking " << key << " as deleted (lazy deletion):" << std::endl;
        tree.DeleteElement(elementToDelete);
        tree.PrintTree();
        delete elementToDelete;
    }
    
    // Try to delete a non-existent element
    Element<int>* nonExistentElement = new Element<int>(100);
    std::cout << "\nTrying to delete non-existent element 100:" << std::endl;
    tree.DeleteElement(nonExistentElement);
    delete nonExistentElement;
    
    // Test DeleteMin
    std::cout << "\n=== Testing DeleteMin ===\n";
    
    // Delete minimum element several times
    for (int i = 0; i < 5; i++) {
        Element<int> minElement;
        Element<int>* result = tree.DeleteMin(minElement);
        
        if (result) {
            std::cout << "\nDeleted minimum element: " << minElement.getKey() << std::endl;
            std::cout << "Tree after DeleteMin:" << std::endl;
            tree.PrintTree();
        } else {
            std::cout << "\nNo minimum element found (tree may be empty)." << std::endl;
            break;
        }
    }
    
    // Test original deletion method
    std::cout << "\n=== Testing Lazy Deletion Method ===\n";
    
    // Reinitialize tree
    tree.Initialize(elements, 10);
    std::cout << "\nReinitialized tree:" << std::endl;
    tree.PrintTree();
    
    // Delete elements using the original method
    int keysToDelete[] = {3, 5, 8, 10, 18, 21, 35, 40};
    for (int key : keysToDelete) {
        Element<int>* elementToDelete = new Element<int>(key);
        std::cout << "\nDeleting " << key << ":" << std::endl;
        tree.DeleteElement(elementToDelete);
        tree.PrintTree();
        delete elementToDelete;
    }
    
    // Test case for DeleteMin on remaining elements
    std::cout << "\n=== Final DeleteMin Tests ===\n";
    
    // Delete the two remaining elements, 1 and 20.

    
    while (true) {
        Element<int> minElement;
        Element<int>* result = tree.DeleteMin(minElement);
        
        if (result) {
            std::cout << "\nDeleted minimum element: " << minElement.getKey() << std::endl;
            std::cout << "Tree after DeleteMin:" << std::endl;
            tree.PrintTree();
        } else {
            std::cout << "\nNo more elements to delete. Tree is empty." << std::endl;
            break;
        }
    }

    return 0;
}



/*********************************Element.h************************************/
#ifndef ELEMENT_H
#define ELEMENT_H

// Element class definition
template <typename KeyType>
class Element {
  private:
    KeyType key;

  public:
    Element() {}
    Element(KeyType k) : key(k) {}

    // Getter for key
    KeyType getKey() const { return key; }

    // Setter for key
    void setKey(KeyType k) { key = k; }

    bool operator>(const Element<KeyType>& other) const { return key > other.getKey(); }
};

#endif // ELEMENT_H



/*******************************LeftistNode.h**********************************/
#ifndef LEFTIST_NODE_H
#define LEFTIST_NODE_H

#include "Element.h"

// Forward declaration of MinLeftistTree
template <typename KeyType> class MinLeftistTree;

// LeftistNode class definition
template <typename KeyType> 
class LeftistNode {
    friend class MinLeftistTree<KeyType>;

private:
    Element<KeyType> data;
    LeftistNode *LeftChild, *RightChild;
    int shortest;
    bool deleted; // Flag to mark deleted nodes

public:
    LeftistNode() : LeftChild(nullptr), RightChild(nullptr), shortest(1), deleted(false) {}
    LeftistNode(const Element<KeyType>& el) : data(el), LeftChild(nullptr), RightChild(nullptr), shortest(1), deleted(false) {}

    // Getter for data
    Element<KeyType> getData() const {
        return data;
    }

    // Setter for data
    void setData(const Element<KeyType>& el) {
        data = el;
    }

    // Getter for LeftChild
    LeftistNode* getLeftChild() const {
        return LeftChild;
    }

    // Setter for LeftChild
    void setLeftChild(LeftistNode* child) {
        LeftChild = child;
    }

    // Getter for RightChild
    LeftistNode* getRightChild() const {
        return RightChild;
    }

    // Setter for RightChild
    void setRightChild(LeftistNode* child) {
        RightChild = child;
    }

    // Getter for shortest
    int getShortest() const {
        return shortest;
    }

    // Setter for shortest
    void setShortest(int s) {
        shortest = s;
    }

    // Getter for deleted
    bool isDeleted() const {
        return deleted;
    }

    // Setter for deleted
    void setDeleted(bool d) {
        deleted = d;
    }
};

#endif // LEFTIST_NODE_H



/**********************************MinPQ.h*************************************/
#ifndef MIN_PQ_H
#define MIN_PQ_H

#include "Element.h"

// MinPQ abstract base class
template <typename KeyType>
class MinPQ {
  public:
    virtual void Insert(const Element<KeyType>&) = 0;
    virtual Element<KeyType>* DeleteMin(Element<KeyType>&) = 0;
    virtual ~MinPQ() {}
};

#endif // MIN_PQ_H



/*****************************MinLeftistTree.h*********************************/
#ifndef MIN_LEFTIST_TREE_H
#define MIN_LEFTIST_TREE_H

#include "MinPQ.h"
#include "LeftistNode.h"

// MinLeftistTree class implementation
template <typename KeyType>
class MinLeftistTree : public MinPQ<KeyType> {
  public:
    MinLeftistTree(LeftistNode<KeyType> *init = nullptr) : root(init) {}
    ~MinLeftistTree();
    void InitializeShortestPath(); // Driver function
    void ConvertToLeftistTree(); // Driver function
    void Insert(const Element<KeyType>&) override;
    Element<KeyType>* DeleteMin(Element<KeyType>&) override;
    void MinCombine(MinLeftistTree<KeyType>*);
    void PrintTree() const; // Helper for visualization
    bool IsEmpty() const { return root == nullptr; }
    void Initialize(const Element<KeyType>* elements, int numElements);
    void DeleteElement(Element<KeyType>* element);

  private:
    int InitializeShortestPath(LeftistNode<KeyType>*); // Workhorse function
    LeftistNode<KeyType>* ConvertToLeftistTree(LeftistNode<KeyType>*); // Workhorse function
    LeftistNode<KeyType>* MinUnion(LeftistNode<KeyType>*, LeftistNode<KeyType>*);
    
    //Helper functions for lazy deletion
    bool findAndMarkDeleted(LeftistNode<KeyType>* node, Element<KeyType>* element);
    void findMinNotDeleted(LeftistNode<KeyType>* node, 
                           LeftistNode<KeyType>*& minNode, 
                           KeyType& minKey, 
                           bool& foundMin);
    LeftistNode<KeyType>* removeDeletedNodes(LeftistNode<KeyType>* node, 
                                             LeftistNode<KeyType>* minNode);
    
    void DeleteHelper(LeftistNode<KeyType>*); // For destructor
    
    LeftistNode<KeyType> *root;
};

#include "MinLeftistTree.tpp"

#endif // MIN_LEFTIST_TREE_H



/****************************MinLeftistTree.tpp********************************/
// Destructor implementation
template <typename KeyType>
MinLeftistTree<KeyType>::~MinLeftistTree() {
    DeleteHelper(root);
}

template <typename KeyType>
void MinLeftistTree<KeyType>::DeleteHelper(LeftistNode<KeyType>* node) {
    if (node) {
        DeleteHelper(node->getLeftChild());
        DeleteHelper(node->getRightChild());
        delete node;
    }
}

/**
 * Solution to Chapter 9, Section 3, Exercise 3, Part 1.
 * A function with Θ(n) time complexity that initializes a minimum leftist tree.
 */
template <typename KeyType>
void MinLeftistTree<KeyType>::Initialize(const Element<KeyType> elements[], int numElements) {
    // Clear the existing tree
    DeleteHelper(root);
    root = nullptr;

    // Build the tree by combining nodes
    for (int i = 0; i < numElements; ++i) {
        LeftistNode<KeyType>* newNode = new LeftistNode<KeyType>(elements[i]);
        MinLeftistTree<KeyType> tempTree(newNode);
        if (root) {
            root = MinUnion(root, tempTree.root);
        } else {
            root = tempTree.root;
        }
        tempTree.root = nullptr; // Prevent double deletion
    }
}

// MinCombine implementation
template <typename KeyType>
void MinLeftistTree<KeyType>::MinCombine(MinLeftistTree<KeyType>* b) {
    // Combines the min leftist tree b with the given min leftist tree.
    // b is set to the empty minimum leftist tree.
    if (!root) root = b->root;
    else if (b->root) root = MinUnion(root, b->root);
    b->root = nullptr;
}

// MinUnion implementation
template <typename KeyType>
LeftistNode<KeyType>* MinLeftistTree<KeyType>::MinUnion(LeftistNode<KeyType>* a,
                                                        LeftistNode<KeyType>* b) {
    // Recursive function that combines two nonempty min leftist trees rooted
    // at a and b. The root of the resulting min leftist tree is returned.
    
    if (!a) return b;
    if (!b) return a;
    
    // Set a to be min leftist tree with smaller root.
    if (a->getData().getKey() > b->getData().getKey()) { LeftistNode<KeyType> *t = a; a = b; b = t; }
    
    // Create binary tree such that the smallest key in each subtree is in the root
    if (!a->getRightChild()) a->setRightChild(b);
    else a->setRightChild(MinUnion(a->getRightChild(), b));
    
    // Leftist Tree property
    // Interchange subtrees if needed
    if (!a->getLeftChild()) { a->setLeftChild(a->getRightChild()); a->setRightChild(nullptr); }
    else if (!a->getRightChild() || a->getLeftChild()->getShortest() < a->getRightChild()->getShortest()) {
        LeftistNode<KeyType> *t = a->getLeftChild();
        a->setLeftChild(a->getRightChild());
        a->setRightChild(t);
    }
    
    // Set 'shortest' data member
    if (!a->getRightChild()) a->setShortest(1);
    else a->setShortest(a->getRightChild()->getShortest() + 1);
    
    return a;
}

// Initialize shortest path implementation
template <typename KeyType>
void MinLeftistTree<KeyType>::InitializeShortestPath() {
    // Call recursive helper function starting from the root
    if (root) {
        InitializeShortestPath(root);
    }
}

// Helper function that recursively initializes the 'shortest' data member
template <typename KeyType>
int MinLeftistTree<KeyType>::InitializeShortestPath(LeftistNode<KeyType>* node) {
    if (!node) {
        return 0; // Null node has a path length of 0
    }
    
    // Recursively calculate shortest paths for left and right subtrees
    int leftShortest = InitializeShortestPath(node->getLeftChild());
    int rightShortest = InitializeShortestPath(node->getRightChild());
    
    // For leftist trees, the shortest path is always on the right side
    // Set the shortest path value for this node
    if (!node->getRightChild()) {
        node->setShortest(1); // If there's no right child, shortest path is 1
    } else {
        // Otherwise, it's the right child's shortest path plus 1
        node->setShortest(rightShortest + 1);
    }
    
    return node->getShortest();
}

// Convert to leftist tree implementation
template <typename KeyType>
void MinLeftistTree<KeyType>::ConvertToLeftistTree() {
    // Start conversion from the root
    if (root) {
        root = ConvertToLeftistTree(root);
    }
}

template <typename KeyType>
LeftistNode<KeyType>* MinLeftistTree<KeyType>::ConvertToLeftistTree(LeftistNode<KeyType>* node) {
    if (!node) {
        return nullptr;
    }
    
    // First, recursively convert left and right subtrees
    node->setLeftChild(ConvertToLeftistTree(node->getLeftChild()));
    node->setRightChild(ConvertToLeftistTree(node->getRightChild()));
    
    // Calculate shortest paths
    int leftShortest = (node->getLeftChild()) ? node->getLeftChild()->getShortest() : 0;
    int rightShortest = (node->getRightChild()) ? node->getRightChild()->getShortest() : 0;
    
    // If the leftist property is violated, swap the children
    if ((node->getLeftChild() == nullptr && node->getRightChild() != nullptr) || 
        (node->getLeftChild() != nullptr && node->getRightChild() != nullptr && 
         leftShortest < rightShortest)) {
        LeftistNode<KeyType>* temp = node->getLeftChild();
        node->setLeftChild(node->getRightChild());
        node->setRightChild(temp);
    }
    
    // Update the shortest path value for this node
    if (!node->getRightChild()) {
        node->setShortest(1);
    } else {
        node->setShortest(node->getRightChild()->getShortest() + 1);
    }
    
    return node;
}

// Insert implementation
template <typename KeyType>
void MinLeftistTree<KeyType>::Insert(const Element<KeyType>& element) {
    // Create a new leftist node with the given element
    LeftistNode<KeyType>* newNode = new LeftistNode<KeyType>(element);
    
    // Create a temporary leftist tree with the new node as root
    MinLeftistTree<KeyType> tempTree(newNode);
    
    // Combine the current tree with the temporary tree
    MinCombine(&tempTree);
}

template <typename KeyType>
void MinLeftistTree<KeyType>::DeleteElement(Element<KeyType>* element) {
    // Start the search from the root
    if (!findAndMarkDeleted(root, element)) {
        // Element not found
        std::cout << "Element " << element->getKey() << " not found in the tree." << std::endl;
    }
}

template <typename KeyType>
Element<KeyType>* MinLeftistTree<KeyType>::DeleteMin(Element<KeyType>& minElement) {
    // Handle empty tree case
    if (!root) {
        return nullptr;
    }
    
    // Find the real minimum (not deleted) element using limited pre-order search
    LeftistNode<KeyType>* minNode = nullptr;
    KeyType minKey = KeyType(); // Initialize with default value
    bool foundMin = false;
    
    // Start the search from the root
    findMinNotDeleted(root, minNode, minKey, foundMin);
    
    // If no valid min node found (all nodes are deleted)
    if (!minNode) {
        root = nullptr;
        return nullptr;
    }
    
    // Set return value
    minElement = minNode->getData();
    
    // Remove the min node and any other deleted nodes, rebuilding the tree
    root = removeDeletedNodes(root, minNode);
    
    // Return a pointer to the minimum element
    return &minElement;
}

// Helper function for physically removing deleted nodes during traversal
template <typename KeyType>
LeftistNode<KeyType>* MinLeftistTree<KeyType>::removeDeletedNodes(LeftistNode<KeyType>* node, 
                                                                 LeftistNode<KeyType>* minNode) {
    if (!node) return nullptr;
    
    // If this node is deleted or is the min node we're removing
    if (node->isDeleted() || node == minNode) {
        // Save children for combining
        LeftistNode<KeyType>* leftChild = node->getLeftChild();
        LeftistNode<KeyType>* rightChild = node->getRightChild();
        
        // Delete this node
        delete node;
        
        // Process children recursively to remove any deleted nodes
        LeftistNode<KeyType>* processedLeft = removeDeletedNodes(leftChild, minNode);
        LeftistNode<KeyType>* processedRight = removeDeletedNodes(rightChild, minNode);
        
        // Combine the processed subtrees
        return MinUnion(processedLeft, processedRight);
    }
    
    // If node is not deleted, process its children
    node->setLeftChild(removeDeletedNodes(node->getLeftChild(), minNode));
    node->setRightChild(removeDeletedNodes(node->getRightChild(), minNode));
    
    // Update shortest path values
    if (!node->getRightChild()) {
        node->setShortest(1);
    } else {
        node->setShortest(node->getRightChild()->getShortest() + 1);
    }
    
    // Ensure leftist property
    if (node->getLeftChild() && node->getRightChild() && 
        node->getLeftChild()->getShortest() < node->getRightChild()->getShortest()) {
        // Swap children to maintain leftist property
        LeftistNode<KeyType>* temp = node->getLeftChild();
        node->setLeftChild(node->getRightChild());
        node->setRightChild(temp);
    }
    
    return node;
}

// Helper function to find and mark an element as deleted using pre-order traversal
template <typename KeyType>
bool MinLeftistTree<KeyType>::findAndMarkDeleted(LeftistNode<KeyType>* node,
                                                 Element<KeyType>* element) {
    if (!node) return false;
    
    // Check if this is the element we want to delete
    if (node->getData().getKey() == element->getKey()) {
        // Mark as deleted using the lazy deletion approach
        node->setDeleted(true);
        return true; // Element found and marked
    }
    
    // Pre-order traversal: first check left child, then right child
    return findAndMarkDeleted(node->getLeftChild(), element) || 
           findAndMarkDeleted(node->getRightChild(), element);
}

// Helper function to find minimum non-deleted element using pre-order traversal
template <typename KeyType>
void MinLeftistTree<KeyType>::findMinNotDeleted(LeftistNode<KeyType>* node, 
                                               LeftistNode<KeyType>*& minNode, 
                                               KeyType& minKey, 
                                               bool& foundMin) {
    if (!node) return;
    
    // Process this node
    if (!node->isDeleted()) {
        if (!foundMin || node->getData().getKey() < minKey) {
            minNode = node;
            minKey = node->getData().getKey();
            foundMin = true;
        }
    }
    
    // Only traverse to children if they could contain a smaller element
    // This is the "limited" part of the pre-order search
    if (node->getLeftChild() && 
        (!foundMin || node->getLeftChild()->getData().getKey() < minKey)) {
        findMinNotDeleted(node->getLeftChild(), minNode, minKey, foundMin);
    }
    
    if (node->getRightChild() && 
        (!foundMin || node->getRightChild()->getData().getKey() < minKey)) {
        findMinNotDeleted(node->getRightChild(), minNode, minKey, foundMin);
    }
}

template <typename KeyType>
void MinLeftistTree<KeyType>::PrintTree() const {
    if (root) {
        std::cout << "root: " << root->getData().getKey() << "(" << root->getShortest() << ")" << std::endl;

        // Initialize arrays for current level and parents
        int currentLevelSize = 1;
        LeftistNode<KeyType>** currentLevel = new LeftistNode<KeyType>*[currentLevelSize];
        currentLevel[0] = root;

        LeftistNode<KeyType>** parents = new LeftistNode<KeyType>*[currentLevelSize];
        parents[0] = nullptr;

        while (currentLevelSize > 0) {
            // Calculate the size of the next level
            int nextLevelSize = 0;
            for (int i = 0; i < currentLevelSize; i++) {
                if (currentLevel[i]->getLeftChild()) nextLevelSize++;
                if (currentLevel[i]->getRightChild()) nextLevelSize++;
            }

            // Allocate arrays for the next level and new parents
            LeftistNode<KeyType>** nextLevel = new LeftistNode<KeyType>*[nextLevelSize];
            LeftistNode<KeyType>** newParents = new LeftistNode<KeyType>*[nextLevelSize];

            // Populate the next level and new parents arrays
            int index = 0;
            for (int i = 0; i < currentLevelSize; i++) {
                LeftistNode<KeyType>* node = currentLevel[i];
                LeftistNode<KeyType>* parent = parents[i];

                if (node->getLeftChild()) {
                    nextLevel[index] = node->getLeftChild();
                    newParents[index] = node;
                    index++;
                }
                if (node->getRightChild()) {
                    nextLevel[index] = node->getRightChild();
                    newParents[index] = node;
                    index++;
                }
            }

            // Print the next level
            if (nextLevelSize > 0) {
                for (int i = 0; i < nextLevelSize; i++) {
                    LeftistNode<KeyType>* node = nextLevel[i];
                    LeftistNode<KeyType>* parent = newParents[i];

                    std::cout << node->getData().getKey() << "(" << node->getShortest() << ")";
                    if (parent) {
                        std::cout << "<under " << parent->getData().getKey() << ">";
                    }

                    if (i < nextLevelSize - 1) {
                        std::cout << " ";
                    }
                }
                std::cout << std::endl;
            }

            // Clean up the current level and parents arrays
            delete[] currentLevel;
            delete[] parents;

            // Update for the next iteration
            currentLevel = nextLevel;
            parents = newParents;
            currentLevelSize = nextLevelSize;
        }

        // Clean up the final arrays
        delete[] currentLevel;
        delete[] parents;
    } else {
        std::cout << "Empty tree" << std::endl;
    }
}
