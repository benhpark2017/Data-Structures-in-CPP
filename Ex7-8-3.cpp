/**
 * Solutions to Chapter 7, Section 8, Exercise 3 of Horowitz, Sahni, and Mehta's
 * Fundamentals of Data Structures in C++, 1st Edition.
 * 
 * Task 3:
 * Write a version of selection sort (see Chapter 1) that works on a linked
 * chain of records.
 */

#include <iostream>
#include <iomanip> // for std::setw()
#include <vector>
#include <algorithm> // for std::find

class Element {
public:
    int key;
    int link;

    Element() : key(0), link(0) {}
    Element(int k, int l) : key(k), link(l) {}

    // Define equality operator for std::find and similar algorithms
    bool operator==(const Element& other) const {
        return key == other.key && link == other.link;
    }
};

// Function to display the current state of the chain
void displayChain(const std::vector<Element>& list, int first) {
    std::cout << "\nCurrent chain configuration (first = " << first << "):\n";

    // Header with position indicators
    std::cout << std::setw(3) << "Pos" << "  | ";
    for (int i = 0; i < list.size(); i++) {
        std::cout << std::setw(4) << i + 1 << " ";
    }
    std::cout << "\n";

    // Separator line
    std::cout << "-----+";
    for (size_t i = 0; i < list.size(); i++) {
        std::cout << "-----";
    }
    std::cout << "\n";

    // Key values
    std::cout << "Key  | ";
    for (const auto& elem : list) {
        std::cout << std::setw(4) << elem.key << " ";
    }
    std::cout << "\n";

    // Link values
    std::cout << "Link | ";
    for (const auto& elem : list) {
        std::cout << std::setw(4) << elem.link << " ";
    }
    std::cout << "\n";
}

// Chain selection sort algorithm
int chainSelectionSort(std::vector<Element>& list, int first) {
    int n = list.size();
    if (n <= 1) return first;

    std::cout << "\n=== Initial configuration ===";
    displayChain(list, first);

    int minIdx = first - 1; // Current position of `first`
    int minPrev = -1;       // Previous node in chain for minimum
    int current = first - 1;
    int prev = -1;

    // Find the minimum element in the chain
    while (current != -1) {
        if (list[current].key < list[minIdx].key) {
            minIdx = current;
            minPrev = prev;
        }
        prev = current;
        current = list[current].link - 1;
    }

    // If the minimum is not already the first element
    if (minIdx != first - 1) {
        // Rearrange the chain
        if (minPrev != -1) {
            list[minPrev].link = list[minIdx].link;
        }
        list[minIdx].link = first;
        first = minIdx + 1; // Update `first`
    }

    // Rebuild the chain by rearranging elements in order
    std::vector<Element> updatedList;
    current = first - 1;
    while (current != -1) {
        updatedList.push_back(list[current]);
        current = list[current].link - 1;
    }
    for (const auto& elem : list) {
        if (std::find(updatedList.begin(), updatedList.end(), elem) == updatedList.end()) {
            updatedList.push_back(elem);
        }
    }
    list = updatedList;

    // Display the state of the chain after each iteration
    std::cout << "\n=== Final configuration ===";
    displayChain(list, first);

    return first;
}

int main() {
    std::vector<Element> list = {
        {26, 9}, {5, 6}, {77, 0}, {1, 2}, {61, 3},
        {11, 8}, {59, 5}, {15, 10}, {48, 7}, {19, 1}
    };

    int first = 4;  // Start with element at index 3 (value 1)

    std::cout << "Selection Sort on Linked Chain\n";
    std::cout << "==============================\n";
    chainSelectionSort(list, first);

    return 0;
}
