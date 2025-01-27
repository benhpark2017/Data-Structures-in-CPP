/**
 * Solutions to Chapter 7, Section 8, Exercise 2 of Horowitz, Sahni, and Mehta's
 * Fundamentals of Data Structures in C++, 1st Edition.
 * 
 * Task 2:
 * Complete Example 7.10.
 */

#include <iostream>
#include <iomanip>
#include <vector>

class Element {
private:
    int key;
    int link;

public:
    Element() : key(0), link(0) {}
    Element(int k) : key(k), link(0) {}
    
    // Getters
    int getKey() const { return key; }
    int getLink() const { return link; }
    
    // Setters
    void setKey(int k) { key = k; }
    void setLink(int l) { link = l; }
};

void displayConfiguration(const std::vector<Element>& list, int first, int iteration) {
    std::cout << "\nConfiguration after iteration " << iteration << ", first = " << first << "\n";
    
    // Header
    std::cout << std::setw(2) << "R_i" << "  |  ";
    for (int i = 0; i < list.size(); i++) {
        std::cout << "R" << i + 1 << "  ";
    }
    std::cout << "\n";
    
    // Key values
    std::cout << "key  | ";
    for (int i = 0; i < list.size(); i++) {
        std::cout << std::setw(3) << list[i].getKey() << " ";
    }
    std::cout << "\n";
    
    // Link values
    std::cout << "link | ";
    for (int i = 0; i < list.size(); i++) {
        std::cout << std::setw(3) << list[i].getLink() << " ";
    }
    std::cout << "\n";
}

void list2(std::vector<Element>& list, const int n, int first) {
    int iteration = 0;
    displayConfiguration(list, first - 1, iteration);  // Adjust for 0-based index display
    
    for (int i = 0; i < n - 1; i++) {
        // Convert first to 0-based index for array access
        int firstIdx = first - 1;
        
        // Find the correct record for the i-th position
        while (firstIdx < i) {
            firstIdx = list[firstIdx].getLink() - 1;  // Convert link to 0-based index
            first = firstIdx + 1;  // Keep first in 1-based form
        }
        
        int nextFirst = list[firstIdx].getLink();  // Save next first (1-based)
        
        if (firstIdx != i) {
            // Swap elements
            Element temp = list[i];
            list[i] = list[firstIdx];
            list[firstIdx] = temp;
            
            // Update links to maintain the chain
            // Convert the firstIdx back to 1-based for storing in link
            list[i].setLink(firstIdx + 1);
        }
        
        first = nextFirst;
        iteration++;
        displayConfiguration(list, first - 1, iteration);  // Adjust for 0-based index display
    }
}

std::vector<Element> initializeList(int& first) {
    const int n = 10;
    std::vector<Element> list(n);
    
    // Initialize with given values
    std::vector<int> keys = {26, 5, 77, 1, 61, 11, 59, 15, 48, 19};
    std::vector<int> links = {9, 6, 0, 2, 3, 8, 5, 10, 7, 1};
    
    for (int i = 0; i < n; i++) {
        list[i].setKey(keys[i]);
        list[i].setLink(links[i]);
    }
    
    first = 4;
    return list;
}

int main() {
    int first;
    std::vector<Element> list = initializeList(first);
    
    std::cout << "Initial configuration:\n";
    displayConfiguration(list, first, 0);
    
    list2(list, list.size(), first);
    
    return 0;
}
