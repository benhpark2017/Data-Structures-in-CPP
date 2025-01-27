/**
 * Solutions to Chapter 7, Section 8, Exercise 5 of Horowitz, Sahni, and Mehta's
 * Fundamentals of Data Structures in C++, 1st Edition.
 * 
 * Task 5:
 * Do Exercise 4 for the case of insertion sort.
 */

#include <iostream>
#include <vector>
#include <iomanip>

class Element {
private:
    int key;
    int link;

public:
    Element() : key(0), link(0) {}
    Element(int k, int l) : key(k), link(l) {}
    
    // Getters
    int getKey() const { return key; }
    int getLink() const { return link; }
    
    // Setters
    void setKey(int k) { key = k; }
    void setLink(int l) { link = l; }
};

void displayList(const std::vector<Element>& list, const std::string& title) {
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
        std::cout << std::setw(4) << elem.getKey() << " ";
    }
    std::cout << "\n";
    
    // Link values
    std::cout << "Link | ";
    for (const auto& elem : list) {
        std::cout << std::setw(4) << elem.getLink() << " ";
    }
    std::cout << "\n";
}

void insert(int eIndex, const std::vector<Element>& list, std::vector<int>& t, int i) {
    int eKey = list[eIndex].getKey();
    while (i >= 0 && eKey < list[t[i]].getKey()) {
        t[i + 1] = t[i]; // Shift indices to the right
        i--;
    }
    t[i + 1] = eIndex; // Insert the current index in the correct position
}

void InsertionSortTable(const std::vector<Element>& list, std::vector<int>& t) {
    t[0] = 0; // Start with the first element
    for (size_t j = 1; j < list.size(); j++) {
        insert(j, list, t, j - 1);
    }
}

void table(std::vector<Element>& list, const std::vector<int>& t) {
    std::vector<Element> temp(list.size());
    for (size_t i = 0; i < list.size(); ++i) {
        // Create new Element with the same key and link values
        int key = list[t[i]].getKey();
        int link = list[t[i]].getLink();
        temp[i] = Element(key, link);
    }
    list = temp;
}

int main() {
    // Initialize the list using initializer list syntax
    std::vector<Element> list = {
        {26, 9}, {5, 6}, {77, 0}, {1, 2}, {61, 3},
        {11, 8}, {59, 5}, {15, 10}, {48, 7}, {19, 1}
    };
    
    // Display the initial state of the list
    displayList(list, "Initial State");
    
    // Initialize the index array t
    std::vector<int> t(list.size());
    for (size_t i = 0; i < t.size(); ++i) {
        t[i] = i;
    }
    
    // Perform the table sort version of Insertion Sort
    InsertionSortTable(list, t);
    
    // Rearrange the list according to the sorted indices in t
    table(list, t);
    
    // Display the final state of the list
    displayList(list, "Final State (Sorted)");
    
    return 0;
}
