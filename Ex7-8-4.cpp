/**
 * Solutions to Chapter 7, Section 8, Exercise 4 of Horowitz, Sahni, and Mehta's
 * Fundamentals of Data Structures in C++, 1st Edition.
 * 
 * Task 4:
 * Write a table sort version of quick sort. Now during the sort, records are
 * not physically moved. Instead, t[i] is the index of the record that would
 * have been in position i if records were physically moved around as in
 * QuickSort (Program 7.6). Begin with t[i] = i, 1 <= i <= n. At the end of the 
 * sort, t[i] is the index of the record that should be in the i-th position in 
 * the sorted list. So now function table may be used to rearrange the records 
 * into the sorted order specified by t. Note that this reduces the amount of 
 * data movement taking place when compared to QuickSort for the case of large 
 * records.
 * 
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip> // For std::setw

class Element {
public:
    int key;
    int link;
    Element() : key(0), link(0) {}
    Element(int k, int l) : key(k), link(l) {}
    int getKey() const { return key; }
};

// Function to display the current state of the list
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

void QuickSortTable(std::vector<Element>& list, std::vector<int>& t, int left, int right) {
    if (left < right) {
        int i = left, j = right + 1;
        int pivot = list[t[left]].getKey();
        do {
            do ++i; while (list[t[i]].getKey() < pivot);
            do --j; while (list[t[j]].getKey() > pivot);
            if (i < j) std::swap(t[i], t[j]);
        } while (i < j);
        std::swap(t[left], t[j]);
        QuickSortTable(list, t, left, j - 1);
        QuickSortTable(list, t, j + 1, right);
    }
}

void table(std::vector<Element>& list, const std::vector<int>& t) {
    std::vector<Element> temp(list.size());
    for (size_t i = 0; i < list.size(); ++i) {
        temp[i] = list[t[i]];
    }
    list = temp;
}

int main() {
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

    // Perform the table sort version of QuickSort
    QuickSortTable(list, t, 0, list.size() - 1);

    // Rearrange the list according to the sorted indices in t
    table(list, t);

    // Display the final state of the list
    displayList(list, "Final State (Sorted)");

    return 0;
}
