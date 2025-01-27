/**
 * Solutions to Chapter 7, Section 9, Exercises 1 and 2 of Horowitz, Sahni, and
 * Mehta's Fundamentals of Data Structures in C++, 1st Edition.
 * 
 * Task 1:
 * [Count Sort] The simplest known sorting method arises from the observation
 * that the position of a record in a sorted list depends on the number of
 * records with smaller keys. Associated with each record there is a count
 * field used to determine the number of records that must precede this one in
 * the sorted list. Write a function to determine the count of each record in
 * an unordered list. Show that if the list has n records, then all the counts
 * can be determined by making at most n(n-1)/2 key comparisons.
 * 
 * Task 2:
 * Write a function similar to table() (Program 7.18) to rearrange the records
 * of a list if, with each record, we have a count of the number of records
 * preceding it in the sorted list (see Exercise 1).
 * 
 */

#include <iostream>
#include <vector>

class Element {
private:
    int key;
    int link;  // This will store the count of smaller elements

public:
    Element() : key(0), link(0) {}
    Element(int k, int l) : key(k), link(l) {}
    
    // Getter methods
    int getKey() const { return key; }
    int getLink() const { return link; }
    
    // Setter methods
    void setKey(int k) { key = k; }
    void setLink(int l) { link = l; }
};

//Chapter 7, Section 9, Exercise 1
void showCount(std::vector<Element>& elements) {
    int n = elements.size();
    
    // Initialize the count for each element
    for (int i = 0; i < n; i++) {
        elements[i].setLink(0);  // Initialize count to 0
    }
    // Compare each element with every other element
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (elements[i].getKey() < elements[j].getKey()) {
                elements[j].setLink(elements[j].getLink() + 1);  // Increment count for elements[j]
            } else if (elements[i].getKey() > elements[j].getKey()) {
                elements[i].setLink(elements[i].getLink() + 1);  // Increment count for elements[i]
            }
        }
    }
}

//Chapter 7, Section 9, Exercise 2
void countSort(std::vector<Element>& elements) {
    int n = elements.size();
    std::vector<Element> temp(n);  // Temporary array for storing sorted elements
    
    // Adjust link values to ensure unique positions
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (elements[i].getKey() == elements[j].getKey() && 
                elements[i].getLink() == elements[j].getLink()) {
                elements[j].setLink(elements[j].getLink() + 1);  // Increment link for duplicates
            }
        }
    }
    
    // Copy elements to temp array based on their adjusted link values
    for (int i = 0; i < n; i++) {
        temp[elements[i].getLink()] = elements[i];
    }
    
    // Copy back to original array
    elements = temp;
}

int main() {
    
    std::vector<Element> elements = {
        {5, 0}, {2, 0}, {9, 0}, {1, 0}, {5, 0}
    };
    
    showCount(elements);
    // Print the counts
    for (const auto& elem : elements) {
        std::cout << "Element with key " << elem.getKey() << " has " << elem.getLink() 
                 << " smaller elements.\n";
    }
    
    countSort(elements);
    
    // Print the sorted list
    std::cout << "Sorted list:\n";
    for (const auto& elem : elements) {
        std::cout << "Element with key " << elem.getKey() << "\n";
    }
}
