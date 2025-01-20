/**
 * Solutions to Chapter 7, Section 8, Exercise 1 of Horowitz, Sahni, and Mehta's
 * Fundamentals of Data Structures in C++, 1st Edition.
 * 
 * Task 1:
 * Complete Example 7.9.
 */

#include <iostream>
#include <iomanip>
#include <vector>

class Element {
public:
    int key;
    int link;
    int linkb;
    
    Element() : key(0), link(0), linkb(0) {}
    Element(int k) : key(k), link(0), linkb(0) {}
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
        std::cout << std::setw(3) << list[i].key << " ";
    }
    std::cout << "\n";
    
    // Link values
    std::cout << "link | ";
    for (int i = 0; i < list.size(); i++) {
        std::cout << std::setw(3) << list[i].link << " ";
    }
    std::cout << "\n";
    
    // Linkb values
    std::cout << "linkb| ";
    for (int i = 0; i < list.size(); i++) {
        std::cout << std::setw(3) << list[i].linkb << " ";
    }
    std::cout << "\n";
}

void list1(std::vector<Element>& list, const int n, int first) {
    int iteration = 0;
    displayConfiguration(list, first, iteration);
    
    for (int i = 1; i <= n; i++) {
        if (first != i) {
            int firstIndex = first - 1;
            int iIndex = i - 1;
            
            if (list[iIndex].link != 0) {
                list[list[iIndex].link - 1].linkb = first;
            }
            if (list[iIndex].linkb != 0) {
                list[list[iIndex].linkb - 1].link = first;
            }
            std::swap(list[firstIndex], list[iIndex]);
        }
        first = list[i - 1].link;
        iteration++;
        displayConfiguration(list, first, iteration);
    }
}

std::vector<Element> initializeList(int& first) {
    const int n = 10;
    std::vector<Element> list(n);
    
    // Initialize with given values
    std::vector<int> keys = {26, 5, 77, 1, 61, 11, 59, 15, 48, 19};
    std::vector<int> links = {9, 6, 0, 2, 3, 8, 5, 10, 7, 1};
    std::vector<int> linkbs = {10, 4, 5, 0, 7, 2, 9, 6, 1, 8}; // Updated linkb values
    
    for (int i = 0; i < n; i++) {
        list[i].key = keys[i];
        list[i].link = links[i];
        list[i].linkb = linkbs[i];
    }
    
    first = 4;
    return list;
}

int main() {
    int first;
    std::vector<Element> list = initializeList(first);
    
    std::cout << "Initial configuration:\n";
    displayConfiguration(list, first, 0);
    
    list1(list, list.size(), first);
    
    return 0;
}
