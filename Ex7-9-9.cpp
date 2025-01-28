/**
 * Solutions to Chapter 7, Section 9, Exercise 9 of Horowitz, Sahni, and Mehta's
 * Fundamentals of Data Structures in C++, 1st Edition.
 * 
 * Task 9:
 * Redo the previous exercise beginning with an unsorted chain of records and
 * ending with a sorted chain.
 * 
 */

#include <iostream>
#include <vector>
#include <iomanip> // For std::setw

class Element {
private:
    int key;
    int link; // Acts as a pointer to the next element in the chain

public:
    Element() : key(0), link(-1) {} // -1 indicates no link (end of chain)
    Element(int k, int l) : key(k), link(l) {}

    // Getters
    int getKey() const { return key; }
    int getLink() const { return link; }

    // Setters
    void setKey(int k) { key = k; }
    void setLink(int l) { link = l; }
};

void displaySortedList(const std::vector<Element>& list, int start, const std::string& title) {
    std::cout << "\n" << title << ":\n";
    
    // Header with position indicators
    std::cout << std::setw(3) << "Pos" << "  | ";
    for (size_t i = 0; i < list.size(); i++) {
        std::cout << std::setw(4) << i + 1 << " ";
    }
    std::cout << "\n-----+";
    for (size_t i = 0; i < list.size(); i++) {
        std::cout << "-----";
    }
    std::cout << "\n";
    
    // Store both keys and links in sorted order
    std::vector<int> sortedKeys(list.size());
    std::vector<int> sortedLinks(list.size());
    int current = start;
    int pos = 0;
    
    // Follow the chain to store both keys and links
    while (current != -1) {
        sortedKeys[pos] = list[current].getKey();
        sortedLinks[pos] = list[current].getLink();
        current = list[current].getLink();
        pos++;
    }
    
    // Display keys in sorted order
    std::cout << "Key  | ";
    for (int key : sortedKeys) {
        std::cout << std::setw(4) << key << " ";
    }
    std::cout << "\n";
    
    // Display links in sorted order
    std::cout << "Link | ";
    for (int link : sortedLinks) {
        std::cout << std::setw(4) << link << " ";
    }
    std::cout << "\n";
}

// Function to perform Bubble Sort on a chain of Elements
void bubbleSortChain(std::vector<Element>& elements, int& start) {
    if (start == -1 || elements.empty()) return;

    bool swapped;
    do {
        swapped = false;
        int current = start;
        int prev = -1;

        while (current != -1) {
            int next = elements[current].getLink();
            if (next != -1 && elements[current].getKey() > elements[next].getKey()) {
                // Perform the swap
                if (prev == -1) {
                    // Update start if the first two elements are swapped
                    start = next;
                } else {
                    // Link the previous element to the new "first" element of the swapped pair
                    elements[prev].setLink(next);
                }

                // Update links for the swapped pair
                elements[current].setLink(elements[next].getLink());
                elements[next].setLink(current);

                // Mark a swap occurred
                swapped = true;

                // Update prev to the new "first" element of the swapped pair
                prev = next;
            } else {
                // Move to the next pair if no swap
                prev = current;
                current = next;
            }
        }
    } while (swapped);
}

// Function to print the chain of elements
void printChain(const std::vector<Element>& elements, int start) {
    int current = start;
    while (current != -1) {
        std::cout << "Key: " << elements[current].getKey() 
                  << ", Link: " << elements[current].getLink() << std::endl;
        current = elements[current].getLink();
    }
}

int main() {
    // Initialize a longer chain of elements
    std::vector<Element> elements = {
        {5, 1},  // Index 0: Key = 5, Link = 1
        {3, 2},  // Index 1: Key = 3, Link = 2
        {8, 3},  // Index 2: Key = 8, Link = 3
        {2, 4},  // Index 3: Key = 2, Link = 4
        {7, 5},  // Index 4: Key = 7, Link = 5
        {4, 6},  // Index 5: Key = 4, Link = 6
        {1, -1}  // Index 6: Key = 1, Link = -1 (end of chain)
    };

    int start = 0; // Index of the first element in the chain

    std::cout << "Original chain:" << std::endl;
    printChain(elements, start);

    displaySortedList(elements, start, "Original List");

    bubbleSortChain(elements, start);

    std::cout << "\nSorted chain:" << std::endl;
    printChain(elements, start);

    displaySortedList(elements, start, "Sorted List");

    return 0;
}
