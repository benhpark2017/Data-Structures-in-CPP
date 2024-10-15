/**
 * Solutions to Chapter 5, Section 9, Exercises 1 through 6 of Horowitz's
 * Fundamentals of Data Structures in C++.
 * 
 * Task 1:
 * Write C++ class definitions for a forest of binary trees using templates.
 * 
 * Task 2:
 * Define the inverse transformation of the one that creates the associated
 * binary tree from a forest. Are these transformations unique?
 * The transformation is not unique. As shown by the function binaryTreeToForest
 * the first trees are two different trees with different connectivities. The
 * result of transforming a binary tree into a forest depends on where the
 * binary tree is fragmented, as well as the branching of the original trees.
 * 
 * Task 3:
 * Prove that the preorder traversal of a forest and the preorder traversal of
 * its associated binary tree give the same result.
 * 
 * Task 4:
 * Prove that the inorder traversal of a forest and the inorder traversal of
 * its associated binary tree give the same result.
 *
 * Task 5:
 * Prove that the postorder traversal of a forest and the postorder traversal of
 * its associated binary tree do not necessarily yield the same result.
 * 
 * Task 6:
 * Prove that the level order traversal of a forest and the level order
 * traversal of its associated binary tree do not necessarily yield the same
 * result.
 * 
 * Task 7:
 * Write a nonrecursive function to traverse the associated binary tree of a
 * forest in forest postorder. What are the time and space complexities of your
 * function?
 * Time Complexity: O(n)
 * Space Complexity: O(w), where w is the maximum width of the tree, which can
 * be O(n) in the worst case for complete binary trees.
 * 
 * Task 8:
 * Write a nonrecursive function to traverse the associated binary tree of a
 * forest in forest level order. What are the time and space complexities of 
 * your function?
 * Time Complexity: O(n)
 * Space Complexity: O(h), where h is the height of the tree, which can be O(n)
 * in the worst case for skewed trees, or O(log n) for balanced trees.
 * 
 */
 
/*********************************main.cpp*************************************/
#include "Forest.h"
#include <iostream>

int main() {
    std::cout << "Instantiating a forest of trees..." << std::endl;
    Forest<char> myForest;
    
    // Creating first binary tree
    BinaryTree<char> tree1;
    TreeNode<char>* nodeA = new TreeNode<char>('A');
    TreeNode<char>* nodeB = new TreeNode<char>('B');
    TreeNode<char>* nodeC = new TreeNode<char>('C');
    nodeA->left = nodeB;
    nodeA->right = nodeC;
    tree1.setRoot(nodeA);
    std::cout << "Tree 1:\n";
    std::cout << tree1 << std::endl;
    
    // Creating second binary tree
    BinaryTree<char> tree2;
    TreeNode<char>* nodeD = new TreeNode<char>('D');
    TreeNode<char>* nodeE = new TreeNode<char>('E');
    nodeD->left = nodeE;
    tree2.setRoot(nodeD);
    std::cout << "Tree 2:\n";
    std::cout << tree2 << std::endl;
    
    // Creating third binary tree
    BinaryTree<char> tree3;
    TreeNode<char>* nodeF = new TreeNode<char>('F');
    tree3.setRoot(nodeF);
    std::cout << "Tree 3:\n";
    std::cout << tree3 << std::endl;

    // Adding binary trees to the forest
    myForest.addTree(std::move(tree1));
    myForest.addTree(std::move(tree2));
    myForest.addTree(std::move(tree3));
    
    std::cout << "Converting the forest to a single binary tree..." << std::endl;
    TreeNode<char>* resultRoot = myForest.forestToBinaryTree();
    BinaryTree<char> resultTree;
    resultTree.setRoot(resultRoot);

    // Output the resulting tree
    std::cout << "The resulting binary tree is:" << std::endl;
    std::cout << resultTree << std::endl;

    // Output the pre-order traversal of the resulting tree
    std::cout << "The pre-order traversal of the resulting binary tree is:" << std::endl;
    resultTree.preOrder();
    std::cout << std::endl << std::endl;
    
    // Output the forest pre-order traversal
    std::cout << "The pre-order traversal of the forest is: " << std::endl;
    myForest.preOrderForest(); // Call the pre-order function
    std::cout << std::endl << std::endl;

    // Output the in-order traversal of the resulting tree
    std::cout << "The in-order traversal of the resulting binary tree is:" << std::endl;
    resultTree.inOrder();
    std::cout << std::endl << std::endl;
    
    // Output the forest in-order traversal
    std::cout << "The in-order traversal of the forest is: " << std::endl;
    myForest.inOrderForest(); // Call the in-order function
    std::cout << std::endl << std::endl;
    
    // Output the post-order traversal of the resulting tree
    std::cout << "The post-order traversal of the resulting binary tree is:" << std::endl;
    resultTree.postOrder();
    std::cout << std::endl << std::endl;
    
    // Output the forest post-order traversal
    std::cout << "The post-order traversal of the forest is: " << std::endl;
    myForest.postOrderForest(); // Call the post-order function
    std::cout << std::endl << std::endl;
    
    // Output the level order traversal of the resulting tree
    std::cout << "The level-order traversal of the resulting binary tree is:" << std::endl;
    resultTree.levelOrder();
    std::cout << std::endl << std::endl;
    
    // Output the forest level order traversal
    std::cout << "The level order traversal of the forest is: " << std::endl;
    myForest.levelOrderForest();
    std::cout << std::endl << std::endl;
    
    // Output the forest post-order traversal using a nonrecursive function
    std::cout << "The nonrecursive post-order traversal of the forest is: " << std::endl;
    myForest.postOrderForestIter(); // Call the post-order function
    std::cout << std::endl << std::endl;
    
    std::cout << "The nonrecursive level-order traversal of the forest is: " << std::endl;
    resultTree.levelOrderIterative();
    std::cout << std::endl << std::endl;
    
    // Now we have an associated binary tree (resultRoot)
    std::cout << "Converting the binary tree back to a forest of trees..." << std::endl;
    std::vector<BinaryTree<char>> forest = myForest.binaryTreeToForest(resultRoot);

    // Output the resulting forest
    std::cout << "The resulting forest contains " << forest.size() << " trees:" << std::endl;
    for (size_t i = 0; i < forest.size(); ++i) {
        std::cout << "Tree " << i + 1 << ":\n" << forest[i] << std::endl;
    }

    std::cout << "End of Program.\n";
    return 0;
}



/*********************************Forest.h************************************/
#ifndef FOREST_H
#define FOREST_H

#include <vector>
#include <stdexcept>
#include <stack>
#include <queue>
#include <iostream>

template <typename T>
class TreeNode {
public:
    T data;
    TreeNode* left;
    TreeNode* right;

    TreeNode(const T& value);
};

template <typename T>
class BinaryTree;

template <typename T>
class Forest;

template <typename T>
std::ostream& operator<<(std::ostream& os, const BinaryTree<T>& tree);

template <typename T>
class BinaryTree {
private:
    TreeNode<T>* root;

    void destroyTree(TreeNode<T>*& node);
    TreeNode<T>* copyTree(const TreeNode<T>* node);
    
    void preOrderWorkhorse(const TreeNode<T>* node) const;
    void inOrderWorkhorse(const TreeNode<T>* node) const;
    void postOrderWorkhorse(const TreeNode<T>* node) const;
    void levelOrderWorkhorse(TreeNode<T>* node) const;
    
public:
    BinaryTree();
    BinaryTree(const BinaryTree& other);
    BinaryTree(BinaryTree&& other) noexcept;
    BinaryTree& operator=(const BinaryTree& other);
    BinaryTree& operator=(BinaryTree&& other) noexcept;
    ~BinaryTree();

    TreeNode<T>* getRoot() const;
    void setRoot(TreeNode<T>* newRoot);
    
    void preOrder() const;
    void inOrder() const;
    void postOrder() const;
    void levelOrder() const;

    void postOrderIterative() const;
    void levelOrderIterative() const;

    friend std::ostream& operator<< <T>(std::ostream& os, const BinaryTree<T>& tree);
    friend class Forest<T>;
};

template <typename T>
class Forest {
private:
    std::vector<BinaryTree<T>> trees;

    void destroyTree(TreeNode<T>*& node);
    TreeNode<T>* copyTree(const TreeNode<T>* node);
    TreeNode<T>* copySubtree(TreeNode<T>* node);

    void preOrderTree(const TreeNode<T>* root) const;
    void preOrderForestHelper(size_t index) const;
    
    void inOrderTree(const TreeNode<T>* root) const;
    void inOrderForestHelper(size_t index) const;
    
    void postOrderTree(const TreeNode<T>* root) const;
    void postOrderForestHelper(size_t index) const;
    
    void levelOrderTree(const BinaryTree<T>& tree) const;

public:
    Forest();
    ~Forest();

    void addTree(BinaryTree<T>&& tree);
    BinaryTree<T>& getTree(size_t index);
    void removeTree(size_t index);
    size_t size() const;
    void clear();

    TreeNode<T>* forestToBinaryTree();
    TreeNode<T>* restructureTree(TreeNode<T>* node);
    std::vector<BinaryTree<T>> binaryTreeToForest(TreeNode<T>* root);
    
    void preOrderForest() const;
    void inOrderForest() const;
    void postOrderForest() const;
    void levelOrderForest() const;
    
    void postOrderForestIter() const;
};

#include "Forest.tpp"

#endif // FOREST_H



/*********************************Forest.tpp***********************************/
#include "Forest.h"

// TreeNode implementation
template <typename T>
TreeNode<T>::TreeNode(const T& value) : data(value), left(nullptr), right(nullptr) {}

// BinaryTree implementation
template <typename T>
BinaryTree<T>::BinaryTree() : root(nullptr) {}

template <typename T>
BinaryTree<T>::BinaryTree(const BinaryTree& other) : root(copyTree(other.root)) {}

template <typename T>
BinaryTree<T>::BinaryTree(BinaryTree&& other) noexcept : root(other.root) {
    other.root = nullptr;
}

template <typename T>
BinaryTree<T>& BinaryTree<T>::operator=(const BinaryTree& other) {
    if (this != &other) {
        destroyTree(root);
        root = copyTree(other.root);
    }
    return *this;
}

template <typename T>
BinaryTree<T>& BinaryTree<T>::operator=(BinaryTree&& other) noexcept {
    if (this != &other) {
        destroyTree(root);
        root = other.root;
        other.root = nullptr;
    }
    return *this;
}

template <typename T>
BinaryTree<T>::~BinaryTree() {
    destroyTree(root);
}

template <typename T>
void BinaryTree<T>::destroyTree(TreeNode<T>*& node) {
    if (node) {
        destroyTree(node->left);
        destroyTree(node->right);
        delete node;
        node = nullptr;
    }
}

template <typename T>
TreeNode<T>* BinaryTree<T>::copyTree(const TreeNode<T>* node) {
    if (node == nullptr) return nullptr;
    TreeNode<T>* newNode = new TreeNode<T>(node->data);
    newNode->left = copyTree(node->left);
    newNode->right = copyTree(node->right);
    return newNode;
}

template <typename T>
void BinaryTree<T>::preOrderWorkhorse(const TreeNode<T>* node) const {
    if (node == nullptr) {
        return;
    }
    // Visit the current node
    std::cout << node->data << " ";
    // Traverse the left subtree
    preOrderWorkhorse(node->left);
    // Traverse the right subtree
    preOrderWorkhorse(node->right);
}

template <typename T>
void BinaryTree<T>::inOrderWorkhorse(const TreeNode<T>* node) const {
    if (node == nullptr) {
        return;
    }
    // Traverse the left subtree
    inOrderWorkhorse(node->left);
    // Visit the current node
    std::cout << node->data << " ";
    // Traverse the right subtree
    inOrderWorkhorse(node->right);
}

template <typename T>
void BinaryTree<T>::postOrderWorkhorse(const TreeNode<T>* node) const {
    if (node == nullptr) {
        return;
    }
    // Traverse the left subtree
    postOrderWorkhorse(node->left);
    // Traverse the right subtree
    postOrderWorkhorse(node->right);
    // Visit the current node
    std::cout << node->data << " ";

}

template <typename T>
void BinaryTree<T>::levelOrderWorkhorse(TreeNode<T>* node) const {
    if (node == nullptr) {
        return; // If the node is null, return
    }

    std::queue<TreeNode<T>*> q; // Queue to hold nodes at each level
    q.push(node); // Start with the root node

    while (!q.empty()) {
        TreeNode<T>* current = q.front(); // Get the front node
        q.pop(); // Remove the front node from the queue

        // Visit the current node
        std::cout << current->data << " ";

        // Add left child to the queue if it exists
        if (current->left) {
            q.push(current->left);
        }

        // Add right child to the queue if it exists
        if (current->right) {
            q.push(current->right);
        }
    }
}

// Post-order traversal (non-recursive)
template <typename T>
void BinaryTree<T>::postOrderIterative() const {
    if (root == nullptr) {
        return; // Empty tree
    }
    std::stack<TreeNode<T>*> stack;
    TreeNode<T>* current = root;
    TreeNode<T>* lastVisited = nullptr;

    while (!stack.empty() || current != nullptr) {
        if (current != nullptr) {
            stack.push(current);
            current = current->left; // Go left
        } else {
            TreeNode<T>* peekNode = stack.top(); // Look at the top node
            // If the right child is null or has been visited, visit the node
            if (peekNode->right == nullptr || peekNode->right == lastVisited) {
                std::cout << peekNode->data << " "; // Visit node
                lastVisited = stack.top(); // Mark it as last visited
                stack.pop(); // Remove from stack
            } else {
                current = peekNode->right; // Move to right child
            }
        }
    }
}

// Level-order traversal without using stacks or queues
template <typename T>
void BinaryTree<T>::levelOrderIterative() const {
    if (root == nullptr) return; // Empty tree

    // Start with the root
    std::vector<TreeNode<T>*> currentLevel;
    currentLevel.push_back(root); // Add root to the current level

    while (!currentLevel.empty()) {
        // Vector to hold the next level
        std::vector<TreeNode<T>*> nextLevel;

        // Traverse nodes in the current level
        for (TreeNode<T>* node : currentLevel) {
            if (node) {
                std::cout << node->data << " "; // Print the current node's data

                // Add the children of the current node to the next level
                if (node->left) {
                    nextLevel.push_back(node->left);
                }
                if (node->right) {
                    nextLevel.push_back(node->right);
                }
            }
        }
        // Move to the next level
        currentLevel = nextLevel;
    }
}


template <typename T>
TreeNode<T>* BinaryTree<T>::getRoot() const {
    return root;
}

template <typename T>
void BinaryTree<T>::setRoot(TreeNode<T>* newRoot) {
    destroyTree(root);
    root = newRoot;
}

template <typename T>
void BinaryTree<T>::preOrder() const {
    preOrderWorkhorse(root);
}

template <typename T>
void BinaryTree<T>::inOrder() const {
    inOrderWorkhorse(root);
}

template <typename T>
void BinaryTree<T>::postOrder() const {
    postOrderWorkhorse(root);
}

template <typename T>
void BinaryTree<T>::levelOrder() const {
    levelOrderWorkhorse(root);
}

// Forest implementation
template <typename T>
Forest<T>::Forest() {}

template <typename T>
Forest<T>::~Forest() {
    clear();
}

template <typename T>
void Forest<T>::addTree(BinaryTree<T>&& tree) {
    trees.push_back(std::move(tree));
}

template <typename T>
BinaryTree<T>& Forest<T>::getTree(size_t index) {
    if (index >= trees.size()) {
        throw std::out_of_range("Tree index out of range.\n");
    }
    return trees[index];
}

template <typename T>
void Forest<T>::removeTree(size_t index) {
    if (index < trees.size()) {
        trees.erase(trees.begin() + index);
    }
}

template <typename T>
size_t Forest<T>::size() const {
    return trees.size();
}

template <typename T>
void Forest<T>::clear() {
    for (auto& tree : trees) {
        destroyTree(tree.root);
        tree.root = nullptr;
    }
    trees.clear();
}

template <typename T>
void Forest<T>::destroyTree(TreeNode<T>*& node) {
    if (node) {
        destroyTree(node->left);
        destroyTree(node->right);
        delete node;
        node = nullptr;
    }
}

template <typename T>
TreeNode<T>* Forest<T>::copyTree(const TreeNode<T>* node) {
    if (node == nullptr) return nullptr;
    TreeNode<T>* newNode = new TreeNode<T>(node->data);
    newNode->left = copyTree(node->left);
    newNode->right = copyTree(node->right);
    return newNode;
}

template <typename T>
TreeNode<T>* Forest<T>::copySubtree(TreeNode<T>* node) {
    if (node == nullptr) return nullptr;

    TreeNode<T>* newNode = new TreeNode<T>(node->data);
    newNode->left = copySubtree(node->left);

    return newNode;
}

// Recursive function to perform pre-order traversal on a single tree
template <typename T>
void Forest<T>::preOrderTree(const TreeNode<T>* root) const {
    if (root == nullptr) {
        return; // Base case: If the node is null, return
    }
    std::cout << root->data << " "; // Visit the root
    preOrderTree(root->left);        // Traverse left subtree
    preOrderTree(root->right);       // Traverse right subtree
}

// Recursive function to traverse the forest in pre-order
template <typename T>
void Forest<T>::preOrderForestHelper(size_t index) const {
    if (index >= trees.size()) {
        return; // Base case: If index is out of bounds, return
    }
        
    // Visit the root of the current tree
    const TreeNode<T>* root = trees[index].root;
    preOrderTree(root); // Visit the root and traverse its subtrees

    // Traverse the remaining trees in the forest
    preOrderForestHelper(index + 1);
}

// Recursive function to perform in-order traversal on a single tree
template <typename T>
void Forest<T>::inOrderTree(const TreeNode<T>* root) const {
    if (root == nullptr) {
        return; // Base case: If the node is null, return
    }
    inOrderTree(root->left);        // Traverse left subtree
    std::cout << root->data << " "; // Visit the root
    inOrderTree(root->right);       // Traverse right subtree
}

// Recursive function to traverse the forest in in-order
template <typename T>
void Forest<T>::inOrderForestHelper(size_t index) const {
    if (index >= trees.size()) {
        return; // Base case: If index is out of bounds, return
    }
        
    // Visit the root of the current tree
    const TreeNode<T>* root = trees[index].root;
    inOrderTree(root); // Visit the root and traverse its subtrees

    // Traverse the remaining trees in the forest
    inOrderForestHelper(index + 1);
}

// Recursive function to perform post-order traversal on a single tree
template <typename T>
void Forest<T>::postOrderTree(const TreeNode<T>* root) const {
    if (root == nullptr) {
        return; // Base case: If the node is null, return
    }
    postOrderTree(root->left);        // Traverse left subtree
    postOrderTree(root->right);       // Traverse right subtree
    std::cout << root->data << " "; // Visit the root
}

// Recursive function to traverse the forest in post-order
template <typename T>
void Forest<T>::postOrderForestHelper(size_t index) const {
    if (index >= trees.size()) {
        return; // Base case: If index is out of bounds, return
    }
        
    // Visit the root of the current tree
    const TreeNode<T>* root = trees[index].root;
    postOrderTree(root); // Visit the root and traverse its subtrees

    // Traverse the remaining trees in the forest
    postOrderForestHelper(index + 1);
}

// Level-order traversal for a single tree
template <typename T>
void Forest<T>::levelOrderTree(const BinaryTree<T>& tree) const {
    TreeNode<T>* levelroot = tree.root;
    if (levelroot == nullptr) {
        std::cout << "Empty tree" << std::endl;
        return;
    }

    std::queue<TreeNode<T>*> q; // Queue to hold nodes
    q.push(levelroot); // Start with the levelroot node

    while (!q.empty()) {
        TreeNode<T>* current = q.front(); // Get the front node
        q.pop(); // Remove the front node from the queue

        // Visit the current node
        std::cout << current->data << " ";

        // Add left child to the queue if it exists
        if (current->left) {
            q.push(current->left);
        }

        // Add right child to the queue if it exists
        if (current->right) {
            q.push(current->right);
        }
    }
}

// Level-order traversal for the entire forest
template <typename T>
void Forest<T>::levelOrderForest() const {
    if (trees.empty()) {
        std::cout << "Empty forest." << std::endl;
        return;
    }

    for (const auto& tree : trees) {
        levelOrderTree(tree); // Traverse each tree in level order
    }
}

template <typename T>
TreeNode<T>* Forest<T>::forestToBinaryTree() {
    if (trees.empty()) {
        return nullptr;
    }

    TreeNode<T>* root = copyTree(restructureTree(trees[0].root));
    TreeNode<T>* current = root;

    for (size_t i = 1; i < trees.size(); ++i) {
        TreeNode<T>* nextTreeRoot = copyTree(restructureTree(trees[i].root));
        if (nextTreeRoot) {
            TreeNode<T>* rightmost = current;
            while (rightmost->right != nullptr) {
                rightmost = rightmost->right;
            }
            rightmost->right = nextTreeRoot;
        }
    }

    return root;
}

template <typename T>
TreeNode<T>* Forest<T>::restructureTree(TreeNode<T>* node) {
    if (node == nullptr) return nullptr;

    node->left = restructureTree(node->left);
    node->right = restructureTree(node->right);

    if (node->right != nullptr) {
        TreeNode<T>* rightChild = node->right;
        node->right = nullptr;

        TreeNode<T>* leftmost = node->left;
        if (leftmost == nullptr) {
            node->left = rightChild;
        } else {
            while (leftmost->left != nullptr) {
                leftmost = leftmost->left;
            }
            leftmost->left = rightChild;
        }
    }

    return node;
}

template <typename T>
std::vector<BinaryTree<T>> Forest<T>::binaryTreeToForest(TreeNode<T>* root) {
    std::vector<BinaryTree<T>> forest;

    while (root != nullptr) {
        BinaryTree<T> newTree;
        TreeNode<T>* newRoot = copySubtree(root);
        newTree.setRoot(newRoot);
        forest.push_back(std::move(newTree));

        TreeNode<T>* nextRoot = root->right;
        root->right = nullptr;
        root = nextRoot;
    }

    return forest;
}

template <typename T>
void Forest<T>::preOrderForest() const {
    preOrderForestHelper(0); // Start traversal from the first tree
}

template <typename T>
void Forest<T>::inOrderForest() const {
    inOrderForestHelper(0); // Start traversal from the first tree
}

template <typename T>
void Forest<T>::postOrderForest() const {
    postOrderForestHelper(0); // Start traversal from the first tree
}

// Post-order traversal of the entire forest
template <typename T>
void Forest<T>::postOrderForestIter() const {
    if (trees.empty()) {
        std::cout << "Empty forest" << std::endl;
        return;
    }

    for (const auto& tree : trees) {
        tree.postOrderIterative(); // Traverse each tree in post-order
    }
}

// Overloaded operator<< for BinaryTree
template <typename T>
std::ostream& operator<<(std::ostream& os, const BinaryTree<T>& tree) {
    if (tree.root == nullptr) {
        os << "Empty tree";
        return os;
    }

    std::queue<TreeNode<T>*> q;
    q.push(tree.root);
    while (!q.empty()) {
        int levelSize = q.size();
        for (int i = 0; i < levelSize; i++) {
            TreeNode<T>* node = q.front();
            q.pop();

            if (node == nullptr) {
                os << "0 ";
            } else {
                os << node->data << " ";
                q.push(node->left);
                q.push(node->right);
            }
        }
        os << std::endl;
    }
    return os;
}
