/**
 * Solutions to Chapter 9, Section 3, Exercise 1 of Horowitz, Sahni, and Mehta's
 * Fundamentals of Data Structures in C++, 1st Edition.
 * 
 * Task 1:
 * Let t be an arbitrary binary tree represented using the node structure for a
 * leftist tree. 
 * (a) Write a function to initialize the shortest data member of each node in
 *     t.
 * (b) Write a function to convert t into a leftist tree.
 * (c) What is the complexity of each of these two functions?
 * 
 * Solution to conceptual questions:
 * (c) InitializeShortestPath(): O(n) time
 *     ConvertToLeftistTree(): O(n) time
 */

#include <iostream>
#include <vector>

// Element class definition
template <typename KeyType>
class Element {
private:
    KeyType key;

public:
    Element() {}
    Element(KeyType k) : key(k) {}

    // Getter for key
    KeyType getKey() const {
        return key;
    }

    // Setter for key
    void setKey(KeyType k) {
        key = k;
    }

    bool operator>(const Element<KeyType>& other) const {
        return key > other.getKey();
    }
};

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

public:
    LeftistNode() : LeftChild(nullptr), RightChild(nullptr), shortest(1) {}
    LeftistNode(const Element<KeyType>& el) : data(el), LeftChild(nullptr), RightChild(nullptr), shortest(1) {}

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
};

// MinPQ abstract base class
template <typename KeyType>
class MinPQ {
public:
    virtual void Insert(const Element<KeyType>&) = 0;
    virtual Element<KeyType>* DeleteMin(Element<KeyType>&) = 0;
    virtual ~MinPQ() {}
};

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

private:
    int InitializeShortestPath(LeftistNode<KeyType>*); // Workhorse function
    LeftistNode<KeyType>* ConvertToLeftistTree(LeftistNode<KeyType>*); // Workhorse function
    LeftistNode<KeyType>* MinUnion(LeftistNode<KeyType>*, LeftistNode<KeyType>*);
    void DeleteHelper(LeftistNode<KeyType>*); // For destructor
    LeftistNode<KeyType> *root;
};

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

// DeleteMin implementation
template <typename KeyType>
Element<KeyType>* MinLeftistTree<KeyType>::DeleteMin(Element<KeyType>& minElement) {
    // Check if the tree is empty
    if (!root) {
        return nullptr;  // Cannot delete from an empty tree
    }
    
    // Store the minimum element (at the root)
    minElement = root->getData();
    
    // Save pointers to the left and right subtrees
    LeftistNode<KeyType>* leftTree = root->getLeftChild();
    LeftistNode<KeyType>* rightTree = root->getRightChild();
    
    // Delete the root node
    delete root;
    
    // If one of the subtrees is empty, the result is the other subtree
    if (!leftTree) {
        root = rightTree;
    } else if (!rightTree) {
        root = leftTree;
    } else {
        // Combine the left and right subtrees using MinUnion
        root = MinUnion(leftTree, rightTree);
    }
    
    return &minElement;
}

// Helper method for horizontal tree visualization
template <typename KeyType>
void MinLeftistTree<KeyType>::PrintTree() const {
    if (root) {
        std::cout << "root: " << root->getData().getKey() << "(" << root->getShortest() << ")" << std::endl;
        std::vector<LeftistNode<KeyType>*> currentLevel = {root};
        std::vector<LeftistNode<KeyType>*> nextLevel;
        std::vector<LeftistNode<KeyType>*> parents = {nullptr};  // Track parent for each node
        
        while (!currentLevel.empty()) {
            nextLevel.clear();
            std::vector<LeftistNode<KeyType>*> newParents;
            
            for (size_t i = 0; i < currentLevel.size(); i++) {
                LeftistNode<KeyType>* node = currentLevel[i];
                LeftistNode<KeyType>* parent = parents[i];
                
                if (node->getLeftChild()) {
                    nextLevel.push_back(node->getLeftChild());
                    newParents.push_back(node);
                }
                
                if (node->getRightChild()) {
                    nextLevel.push_back(node->getRightChild());
                    newParents.push_back(node);
                }
            }
            
            if (!nextLevel.empty()) {
                for (size_t i = 0; i < nextLevel.size(); i++) {
                    LeftistNode<KeyType>* node = nextLevel[i];
                    LeftistNode<KeyType>* parent = newParents[i];
                    
                    std::cout << node->getData().getKey() << "(" << node->getShortest() << ")";
                    if (parent) {
                        std::cout << "<under " << parent->getData().getKey() << ">";
                    }
                    
                    if (i < nextLevel.size() - 1) {
                        std::cout << " ";
                    }
                }
                std::cout << std::endl;
            }
            
            currentLevel = nextLevel;
            parents = newParents;
        }
    } else {
        std::cout << "Empty tree" << std::endl;
    }
}

// Helper function to create a test tree manually (for specific structures)
template <typename KeyType>
LeftistNode<KeyType>* createNodeWithChildren(KeyType key, 
                                            LeftistNode<KeyType>* left = nullptr, 
                                            LeftistNode<KeyType>* right = nullptr,
                                            int shortest = 1) {
    LeftistNode<KeyType>* node = new LeftistNode<KeyType>(Element<KeyType>(key));
    node->setLeftChild(left);
    node->setRightChild(right);
    node->setShortest(shortest);
    return node;
}

// Helper function to build the first test tree as shown in Figure 9.13 (a)
MinLeftistTree<int> buildFirstTestTree() {
    // Create nodes bottom-up
    LeftistNode<int>* node13 = createNodeWithChildren(13);
    LeftistNode<int>* node11 = createNodeWithChildren(11, node13);
    LeftistNode<int>* node80 = createNodeWithChildren(80);
    LeftistNode<int>* node50 = createNodeWithChildren(50, node80);
    LeftistNode<int>* node7 = createNodeWithChildren(7, node11);
    LeftistNode<int>* node2 = createNodeWithChildren(2, node7, node50, 2);
    
    // Create tree with root
    return MinLeftistTree<int>(node2);
}

// Helper function to build the second test tree as shown in Figure 9.13 (b)
MinLeftistTree<int> buildSecondTestTree() {
    // Create nodes bottom-up
    LeftistNode<int>* node20 = createNodeWithChildren(20);
    LeftistNode<int>* node18 = createNodeWithChildren(18);
    LeftistNode<int>* node15 = createNodeWithChildren(15);
    LeftistNode<int>* node10 = createNodeWithChildren(10, node15);
    LeftistNode<int>* node12 = createNodeWithChildren(12, node20, node18, 2);
    LeftistNode<int>* node8 = createNodeWithChildren(8, node10);
    LeftistNode<int>* node9 = createNodeWithChildren(9, node12);
    LeftistNode<int>* node5 = createNodeWithChildren(5, node9, node8, 2);
    
    // Create tree with root
    return MinLeftistTree<int>(node5);
}

int main() {
    // Create the two min leftist trees from Figure 9.13
    MinLeftistTree<int> tree1 = buildFirstTestTree();
    MinLeftistTree<int> tree2 = buildSecondTestTree();
    
    std::cout << "Tree 1 (from Figure 9.13a):" << std::endl;
    tree1.PrintTree();
    std::cout << "\nTree 2 (from Figure 9.13b):" << std::endl;
    tree2.PrintTree();
    
    // Test 1: Combine the two trees as shown in Figure 9.14c
    std::cout << "\n-- Test 1: Combine Trees 1 and 2 --" << std::endl;
    tree1.MinCombine(&tree2);
    std::cout << "Combined tree:" << std::endl;
    tree1.PrintTree();
    std::cout << "Tree 2 after combination (should be empty):" << std::endl;
    tree2.PrintTree();
    
    // Test 2: Insert multiple elements
    std::cout << "\n-- Test 2: Insert multiple elements into a new tree --" << std::endl;
    MinLeftistTree<int> tree3;
    std::vector<int> elementsToInsert = {30, 25, 40, 10, 5, 35, 15, 20};
    for (int val : elementsToInsert) {
        tree3.Insert(Element<int>(val));
    }
    std::cout << "Tree 3 after inserting multiple elements:" << std::endl;
    tree3.PrintTree();
    
    // Test 3: Delete multiple elements (extracting the priority queue in order)
    std::cout << "\n-- Test 3: Delete elements in order from Tree 1 --" << std::endl;
    std::cout << "Elements in order: ";
    while (!tree3.IsEmpty()) {
        Element<int> min;
        tree3.DeleteMin(min);
        std::cout << min.getKey() << " ";
    }
    std::cout << std::endl;
    
    // All dynamically allocated memory should be freed by the destructors
    std::cout << "\nTests completed successfully!" << std::endl;
    
    return 0;
}
