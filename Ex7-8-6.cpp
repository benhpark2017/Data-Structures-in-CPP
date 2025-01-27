/**
 * Solutions to Chapter 7, Section 8, Exercise 6 of Horowitz, Sahni, and Mehta's
 * Fundamentals of Data Structures in C++, 1st Edition.
 * 
 * Task 6:
 * Do Exercise 4 for the case of merge sort.
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

void displayTable(const std::vector<Element>& list, const std::vector<int>& t, const std::string& title) {
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
    for (size_t i = 0; i < list.size(); i++) {
        std::cout << std::setw(4) << list[t[i]].getKey() << " ";
    }
    std::cout << "\n";
    
    // Link values
    std::cout << "Link | ";
    for (size_t i = 0; i < list.size(); i++) {
        std::cout << std::setw(4) << list[t[i]].getLink() << " ";
    }
    std::cout << "\n";
}

void merge(const std::vector<Element>& initList, std::vector<int>& t, int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;
    std::vector<int> L(n1), R(n2);
    
    for (int i = 0; i < n1; i++)
        L[i] = t[l + i];
    for (int j = 0; j < n2; j++)
        R[j] = t[m + 1 + j];
    
    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        if (initList[L[i]].getKey() <= initList[R[j]].getKey()) {
            t[k] = L[i];
            i++;
        } else {
            t[k] = R[j];
            j++;
        }
        k++;
    }
    
    while (i < n1) {
        t[k] = L[i];
        i++;
        k++;
    }
    
    while (j < n2) {
        t[k] = R[j];
        j++;
        k++;
    }
}

void mergeSort(const std::vector<Element>& initList, std::vector<int>& t, int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        mergeSort(initList, t, l, m);
        mergeSort(initList, t, m + 1, r);
        merge(initList, t, l, m, r);
    }
}

void table(std::vector<Element>& list, const std::vector<int>& t) {
    std::vector<Element> tempList(list.size());
    for (size_t i = 0; i < list.size(); i++) {
        tempList[i] = Element(list[t[i]].getKey(), list[t[i]].getLink());
    }
    list = tempList;
}

int main() {
    std::vector<Element> list = {
        {26, 9}, {5, 6}, {77, 0}, {1, 2}, {61, 3},
        {11, 8}, {59, 5}, {15, 10}, {48, 7}, {19, 1}
    };
    
    std::vector<int> t(list.size());
    for (size_t i = 0; i < t.size(); i++) {
        t[i] = i;
    }
    
    displayTable(list, t, "Initial State");
    mergeSort(list, t, 0, list.size() - 1);
    displayTable(list, t, "Final State (Sorted)");
    
    table(list, t);
    
    std::cout << "\nFinal Rearranged List:\n";
    for (const auto& elem : list) {
        std::cout << "Key: " << elem.getKey() << ", Link: " << elem.getLink() << "\n";
    }
    
    return 0;
}
