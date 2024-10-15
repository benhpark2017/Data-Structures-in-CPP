/**
 * Solutions to Chapter 5, Section 9, Exercises 1 and 2 of Horowitz's
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

public:
    BinaryTree();
    BinaryTree(const BinaryTree& other);
    BinaryTree(BinaryTree&& other) noexcept;
    BinaryTree& operator=(const BinaryTree& other);
    BinaryTree& operator=(BinaryTree&& other) noexcept;
    ~BinaryTree();

    TreeNode<T>* getRoot() const;
    void setRoot(TreeNode<T>* newRoot);

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
TreeNode<T>* BinaryTree<T>::getRoot() const {
    return root;
}

template <typename T>
void BinaryTree<T>::setRoot(TreeNode<T>* newRoot) {
    destroyTree(root);
    root = newRoot;
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
