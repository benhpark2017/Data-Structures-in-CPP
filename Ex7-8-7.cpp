/**
 * Solutions to Chapter 7, Section 8, Exercise 7 of Horowitz, Sahni, and Mehta's
 * Fundamentals of Data Structures in C++, 1st Edition.
 * 
 * Task 7:
 * Do Exercise 4 for the case of heap sort.
 */

#include <iostream>
#include <vector>
#include <iomanip> // For std::setw

class Element {
public:
    int key;
    int link;
    Element() : key(0), link(0) {}
    Element(int k, int l) : key(k), link(l) {}
    int getKey() const { return key; }
};

void adjust(std::vector<Element>& list, int root, int n) {
    int rootKey = list[root].getKey();
    Element rootElement = list[root];
    int child = 2 * root + 1;
    while (child < n) {
        // Find larger child
        if (child + 1 < n && list[child + 1].getKey() > list[child].getKey()) {
            child++;
        }
        // If root is larger, stop
        if (rootKey >= list[child].getKey()) {
            break;
        }
        // Move child up
        list[root] = list[child];
        root = child;
        child = 2 * root + 1;
    }
    list[root] = rootElement;
}

void heapSort(std::vector<Element>& list) {
    int n = list.size();
    // Build max heap
    for (int i = n / 2 - 1; i >= 0; i--) {
        adjust(list, i, n);
    }
    // Sort
    for (int i = n - 1; i > 0; i--) {
        std::swap(list[0], list[i]);
        adjust(list, 0, i);
    }
}

void displayTable(const std::vector<Element>& list, const std::string& title) {
    std::cout << "\n" << title << ":\n";
    // Header with position indicators
    std::cout << std::setw(3) << "Pos" << "  | ";
    for (size_t i = 0; i < list.size(); i++) {
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

int main() {
    std::vector<Element> list = {
        {26, 9}, {5, 6}, {77, 0}, {1, 2}, {61, 3},
        {11, 8}, {59, 5}, {15, 10}, {48, 7}, {19, 1}
    };

    displayTable(list, "Initial State");
    
    heapSort(list);
    
    displayTable(list, "Final State (Sorted)");

    std::cout << "\nFinal Rearranged List:\n";
    for (const auto& elem : list) {
        std::cout << "Key: " << elem.key << ", Link: " << elem.link << "\n";
    }

    return 0;
}
