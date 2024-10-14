/**
 * Solutions to Chapter 5, Section 8, Exercise 1 through 5 and 7 of Horowitz's
 * Fundamentals of Data Structures in C++.
 * 
 * Task 1:
 * Write C++ class definitions for winner and loser trees.
 * 
 * Task 2:
 * Write a function to construct winner trees for records R_i, 1<=i<k, with
 * key values K_i, 1<=i<k. Let the tree nodes T_i, 1<=i<k, with T_1,  
 * 1<=i<k, a pointer to the winner of the tournament. Assume that k is a
 * power of 2. Show that this construction can be carried out in time O(k).
 * 
 * Task 3:
 * Do Exercise 2 for the case when k is not restricted to being a power of 2.
 * 
 * Task 4:
 * Write a function to construct loser trees for records R_i, 1<=i<k, with
 * key values K_i, 1<=i<k. Let the tree nodes T_i, 1<=i<k, with T_1,  
 * 1<=i<k, a pointer to the winner of the tournament and T_0 a pointer to the
 * overall winner.  Show that this construction can be carried out in time O(k).
 * Assume that k is a power of 2.
 * 
 * Task 5:
 * Do Exercise 4 for the case when k is not restricted to being a power of 2.
 * 
 * Task 7: 
 * Write an algorithm, using a tree of winners, to carry out a k-way merge of
 * k runs, k>=2. Use the data structures of Exercise 3 and assume the existence
 * of a function to initialize the winner tree. This initialization function
 * runs in linear time. Show that if there are n records in k runs together,
 * then the computing time is O(n*log_2(k)).
 */
/*********************************main.cpp*************************************/
#include "WinnerTree.h"
#include "LoserTree.h"
#include <iostream>

int main() {
    std::cout << "Instantiating Winner and Loser Trees...\n";
    WinnerTree winnerTree(8);
    LoserTree loserTree(8);
    
    std::vector<std::vector<int>> testData = {
        {10, 15, 16}, {9, 20, 38}, {20, 20, 30}, {6, 25, 28},
        {8, 15, 50}, {9, 11, 16}, {90, 95, 99}, {17, 18, 20}
    };
    
    winnerTree.Initialize(testData);
    loserTree.Initialize(testData);

    std::cout << "\nRunning Winner Tree tournaments:\n";
    winnerTree.runTournaments(testData);
    
    std::cout << "\nRunning Loser Tree tournaments:\n";
    loserTree.runTournaments(testData);
    
    winnerTree.addToRun(testData, 4, 15);

    std::cout << "\nRunning New Winner Tree Tournaments.\n";
    winnerTree.runTournaments(testData);

    std::cout << "\nRunning the tournament with a merged data set:\n";
    std::vector<std::vector<int>> mergedData = winnerTree.mergeRuns(testData);
    
    if (!mergedData.empty()) {
        std::cout << "Merged and remaining runs:\n";
        for (const auto& run : mergedData) {
            std::cout << "{ ";
            for (int val : run) {
                std::cout << val << " ";
            }
            std::cout << "}\n";
        }
    } else {
        std::cout << "No valid runs were merged.\n";
    }
    
    std::cout << "End of program.\n";
    return 0;
}



/******************************SelectionTree.h*********************************/

#ifndef SELECTION_TREE_H
#define SELECTION_TREE_H

#include <vector>
#include <queue>
#include <cmath>
#include <limits>
#include <stdexcept>

class Element {
public:
    int key;
    int record;

    Element(int k = 0, int r = 0);
    bool operator<(const Element& other) const;
    bool operator>(const Element& other) const;
};

class SelectionTree {
protected:
    std::vector<Element> tree;
    std::vector<std::queue<int>> runs;
    int height;

    static constexpr int SENTINEL_VALUE = std::numeric_limits<int>::max();

    virtual ~SelectionTree() = default;

public:
    explicit SelectionTree(int numRuns);
    virtual void runTournaments(const std::vector<std::vector<int>>& data) = 0;
};

#endif // SELECTION_TREE_H



/*****************************SelectionTree.cpp********************************/

#include "SelectionTree.h"

Element::Element(int k, int r) : key(k), record(r) {}

bool Element::operator<(const Element& other) const {
    return key < other.key;
}

bool Element::operator>(const Element& other) const {
    return key > other.key;
}

SelectionTree::SelectionTree(int numRuns) {
    if (numRuns <= 0) throw std::invalid_argument("Number of runs must be positive");

    height = static_cast<int>(std::ceil(std::log2(numRuns)));
    int treeSize = (1 << (height + 1)) - 1;

    tree.resize(treeSize);
    runs.resize(numRuns);
}



/********************************WinnerTree.h**********************************/

#ifndef WINNER_TREE_H
#define WINNER_TREE_H

#include "SelectionTree.h"
#include <algorithm>

class WinnerTree : public SelectionTree {
private:
    void buildTreeForTournament(const std::vector<int>& contestants);

public:
    explicit WinnerTree(int numRuns);
    void Initialize(const std::vector<std::vector<int>>& data);
    void addToRun(std::vector<std::vector<int>>& data, int runIndex, int value);
    void runTournaments(const std::vector<std::vector<int>>& data) override;
    std::vector<std::vector<int>> mergeRuns(const std::vector<std::vector<int>>& runs);
};

#endif // WINNER_TREE_H



/*******************************WinnerTree.cpp*********************************/

#include "WinnerTree.h"
#include <iostream>

WinnerTree::WinnerTree(int numRuns) : SelectionTree(numRuns) {}

void WinnerTree::Initialize(const std::vector<std::vector<int>>& data) {
    int numRuns = data.size();
    height = static_cast<int>(std::ceil(std::log2(numRuns)));
    int treeSize = (1 << (height + 1)) - 1;

    tree.resize(treeSize);

    std::cout << "Winner Tree initialized with " << treeSize << " nodes.\n";
}

void WinnerTree::addToRun(std::vector<std::vector<int>>& data, int runIndex, int value) {
    if (runIndex < 1 || runIndex > data.size()) {
        throw std::out_of_range("Run index must be between 1 and " + std::to_string(data.size()));
    }
    data[runIndex - 1][0] = value;
}

void WinnerTree::runTournaments(const std::vector<std::vector<int>>& data) {
    Initialize(data);
    
    int numTournaments = data[0].size();
    int numRuns = data.size();
    
    for (int t = 0; t < numTournaments; ++t) {
        std::vector<int> contestants;
        for (int r = 0; r < numRuns; ++r) {
            contestants.push_back(data[r][t]);
        }
        buildTreeForTournament(contestants);
    }
}

void WinnerTree::buildTreeForTournament(const std::vector<int>& contestants) {
    int k = contestants.size();

    // Find the next power of 2 greater than or equal to k
    int powerOfTwo = 1 << static_cast<int>(std::ceil(std::log2(k)));

    // Add padding with dummy nodes if k is not a power of 2
    std::vector<Element> paddedContestants;
    for (int i = 0; i < k; ++i) {
        paddedContestants.emplace_back(contestants[i], i + powerOfTwo);  // Add contestants with record
    }
    if (k < powerOfTwo) {
        for (int i = 0; i < powerOfTwo - k; ++i) {
            paddedContestants.emplace_back(std::numeric_limits<int>::max(), -1);  // Sentinel value
        }
    }

    // Create the tree structure to hold internal nodes and contestants
    int numInternalNodes = powerOfTwo - 1;
    tree.resize(2 * powerOfTwo - 1);

    // Copy contestants to the bottom level of the tree
    int leafStart = numInternalNodes;
    for (int i = 0; i < paddedContestants.size(); ++i) {
        tree[leafStart + i] = paddedContestants[i];
    }

    // Now, build the internal nodes (winners) by comparing children
    for (int i = leafStart - 1; i >= 0; --i) {
        int leftChild = 2 * i + 1;
        int rightChild = 2 * i + 2;

        if (tree[leftChild].key == std::numeric_limits<int>::max()) {
            tree[i] = tree[rightChild];  // If left child is a sentinel, take right child
            tree[i].record /= 2;
        } else if (tree[rightChild].key == std::numeric_limits<int>::max()) {
            tree[i] = tree[leftChild];   // If right child is a sentinel, take left child
            tree[i].record /= 2; 
        } else {
            if (tree[leftChild] < tree[rightChild]) {
                tree[i] = tree[leftChild];
                tree[i].record /= 2;
            } else {
                tree[i] = tree[rightChild];
                tree[i].record /= 2;
            }
        }
    }

    // Output the winner of this tournament
    std::cout << "Winner: Key = " << tree[0].key << ", Record = " << tree[0].record << std::endl;
    std::cout << "Finalist Key = " << tree[1].key << ", Record = " << tree[1].record << std::endl;
    std::cout << "Finalist Key = " << tree[2].key << ", Record = " << tree[2].record << std::endl;
    std::cout << "Semifinalist Key = " << tree[3].key << ", Record = " << tree[3].record << std::endl;
    std::cout << "Semifinalist Key = " << tree[4].key << ", Record = " << tree[4].record << std::endl;
    std::cout << "Semifinalist Key = " << tree[5].key << ", Record = " << tree[5].record << std::endl;
    std::cout << "Semifinalist Key = " << tree[6].key << ", Record = " << tree[6].record << std::endl;
    std::cout << std::endl; 
}

std::vector<std::vector<int>> WinnerTree::mergeRuns(const std::vector<std::vector<int>>& runs) {
    int numRuns = runs.size();
    if (numRuns < 2) {
        std::cout << "At least two runs are needed for merging.\n";
        return {};  // Return empty result for invalid input
    }

    std::vector<int> selectedRuns;  // Store user-specified run indices
    int runIndex;

    // Ask user to specify which runs to merge
    std::cout << "Specify the runs to merge (Enter -1 to stop):\n";
    while (true) {
        std::cin >> runIndex;
        if (runIndex == -1) break;  // Stop input when user enters -1
        if (runIndex >= 0 && runIndex < numRuns) {
            selectedRuns.push_back(runIndex);  // Add valid run index
        } else {
            std::cout << "Invalid run number. Please try again.\n";
        }
    }

    if (selectedRuns.size() < 2) {
        std::cout << "You must select at least two runs to merge.\n";
        return {};  // Return empty result for invalid input
    }

    // Initialize the merged result with the first selected run
    std::vector<int> mergedRun = runs[selectedRuns[0]];

    // Manually merge the selected runs within this function
    for (size_t i = 1; i < selectedRuns.size(); ++i) {
        const std::vector<int>& run = runs[selectedRuns[i]];  // Get the next selected run
        std::vector<int> temp;  // Temporary vector for merging

        size_t idx1 = 0, idx2 = 0;  // Index pointers for merging two runs

        // Manually merge two sorted runs
        while (idx1 < mergedRun.size() && idx2 < run.size()) {
            if (mergedRun[idx1] < run[idx2]) {
                temp.push_back(mergedRun[idx1]);
                ++idx1;
            } else {
                temp.push_back(run[idx2]);
                ++idx2;
            }
        }

        // Add any remaining elements from either run
        while (idx1 < mergedRun.size()) {
            temp.push_back(mergedRun[idx1]);
            ++idx1;
        }
        while (idx2 < run.size()) {
            temp.push_back(run[idx2]);
            ++idx2;
        }

        // Move the merged result back to mergedRun
        mergedRun = std::move(temp);
    }

    // Prepare the final result
    std::vector<std::vector<int>> result;
    result.push_back(mergedRun);  // The first vector is the merged run

    // Add unselected runs to the result
    for (int i = 0; i < numRuns; ++i) {
        if (std::find(selectedRuns.begin(), selectedRuns.end(), i) == selectedRuns.end()) {
            result.push_back(runs[i]);  // Add runs that were not selected
        }
    }

    return result;  // Return the merged run and remaining unselected runs
}



/*********************************LoserTree.h**********************************/

#ifndef LOSER_TREE_H
#define LOSER_TREE_H

#include "SelectionTree.h"

class LoserTree : public SelectionTree {
private:
    void buildTreeForTournament(const std::vector<int>& contestants);

public:
    explicit LoserTree(int numRuns);
    void Initialize(const std::vector<std::vector<int>>& data);
    void addToRun(std::vector<std::vector<int>>& data, int runIndex, int value);
    void runTournaments(const std::vector<std::vector<int>>& data) override;
};

#endif // LOSER_TREE_H



/********************************LoserTree.cpp*********************************/

#include "LoserTree.h"
#include <iostream>

LoserTree::LoserTree(int numRuns) : SelectionTree(numRuns) {}

void LoserTree::Initialize(const std::vector<std::vector<int>>& data) {
    int numRuns = data.size();
    height = static_cast<int>(std::ceil(std::log2(numRuns)));
    int treeSize = (1 << (height + 1)) - 1;

    tree.resize(treeSize);

    std::cout << "Loser Tree initialized with " << treeSize << " nodes.\n";
}

void LoserTree::addToRun(std::vector<std::vector<int>>& data, int runIndex, int value) {
    if (runIndex < 1 || runIndex > data.size()) {
        throw std::out_of_range("Run index must be between 1 and " + std::to_string(data.size()));
    }
    data[runIndex - 1][0] = value;
}

void LoserTree::runTournaments(const std::vector<std::vector<int>>& data) {
    Initialize(data);
    
    int numTournaments = data[0].size();
    int numRuns = data.size();
    
    for (int t = 0; t < numTournaments; ++t) {
        std::vector<int> contestants;
        for (int r = 0; r < numRuns; ++r) {
            contestants.push_back(data[r][t]);
        }
        buildTreeForTournament(contestants);
    }
}

void LoserTree::buildTreeForTournament(const std::vector<int>& contestants) {
    int k = contestants.size();

    // Find the next power of 2 greater than or equal to k
    int powerOfTwo = 1 << static_cast<int>(std::ceil(std::log2(k)));

    // Create the tree structure to hold internal nodes and contestants
    int numInternalNodes = powerOfTwo - 1;
    tree.resize(2 * powerOfTwo - 1);  // This is the Loser Tree (final output)

    // Step 1: Build the Winner Tree first
    std::vector<Element> winnerTree(2 * powerOfTwo - 1);
    
    // Copy contestants to the bottom level of the winner tree
    int leafStart = numInternalNodes;
    for (int i = 0; i < k; ++i) {
        winnerTree[leafStart + i] = Element(contestants[i], i + powerOfTwo);  // Assign contestants
    }
    for (int i = k; i < powerOfTwo; ++i) {
        winnerTree[leafStart + i] = Element(std::numeric_limits<int>::max(), -1);  // Sentinel
    }

    // Step 2: Build the Winner Tree (to get winners for the matchups)
    for (int i = leafStart - 1; i >= 0; --i) {
        int leftChild = 2 * i + 1;
        int rightChild = 2 * i + 2;

        // Compare keys to determine the winner for each match
        if (winnerTree[leftChild].key <= winnerTree[rightChild].key) {
            winnerTree[i] = winnerTree[leftChild];  // Winner moves up the tree
            winnerTree[i].record /= 2;
        } else {
            winnerTree[i] = winnerTree[rightChild]; // Winner moves up the tree
            winnerTree[i].record /= 2;
        }
    }

    // Step 3: Populate the Loser Tree by storing the loser of each match
    for (int i = leafStart - 1; i >= 0; --i) {
        int leftChild = 2 * i + 1;
        int rightChild = 2 * i + 2;

        // Fetch the losers from the winnerTree
        if (winnerTree[leftChild].key <= winnerTree[rightChild].key) {
            tree[i] = winnerTree[rightChild];  // Loser goes into the Loser Tree
            tree[i].record /= 2;
        } else {
            tree[i] = winnerTree[leftChild];   // Loser goes into the Loser Tree
            tree[i].record /= 2;
        }
    }

    // Step 4: Output the overall winner from the Winner Tree
    Element winner = winnerTree[0];
    winner.record = 0;

    // Output results for both trees
    std::cout << "Overall Winner: Key = " << winner.key << ", Record = " << winner.record << std::endl;
    
    // Output Loser Tree
    for (int i = 0; i < numInternalNodes; ++i) {
        std::cout << "Loser Tree Node " << i << ": Key = " << tree[i].key << ", Record = " << tree[i].record << std::endl;
    }
    std::cout << std::endl;
}
