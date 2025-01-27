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
private:
    int key;
    int link;
    int linkb;

public:
    Element() : key(0), link(0), linkb(0) {}
    Element(int k) : key(k), link(0), linkb(0) {}
    
    // Getters
    int getKey() const { return key; }
    int getLink() const { return link; }
    int getLinkB() const { return linkb; }
    
    // Setters
    void setKey(int k) { key = k; }
    void setLink(int l) { link = l; }
    void setLinkB(int lb) { linkb = lb; }
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
    
    // Linkb values
    std::cout << "linkb| ";
    for (int i = 0; i < list.size(); i++) {
        std::cout << std::setw(3) << list[i].getLinkB() << " ";
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
            
            if (list[iIndex].getLink() != 0) {
                list[list[iIndex].getLink() - 1].setLinkB(first);
            }
            if (list[iIndex].getLinkB() != 0) {
                list[list[iIndex].getLinkB() - 1].setLink(first);
            }
            std::swap(list[firstIndex], list[iIndex]);
        }
        first = list[i - 1].getLink();
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
    std::vector<int> linkbs = {10, 4, 5, 0, 7, 2, 9, 6, 1, 8};
    
    for (int i = 0; i < n; i++) {
        list[i].setKey(keys[i]);
        list[i].setLink(links[i]);
        list[i].setLinkB(linkbs[i]);
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
