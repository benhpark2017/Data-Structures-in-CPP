/**
 * Solutions to Chapter 7, Section 5, Exercise 7 of Horowitz, Sahni, and Mehta's
 * Fundamentals of Data Structures in C++, 1st Edition.
 * 
 * Task 7:
 * Write an iterative merge sort function using linked lists. How much time does
 * this function take on an initially sorted list? Note that merge sort takes
 * O(n log(n)) on such an input list. What is the worst-case computing time of 
 * the new function? How much additional space is needed?
 */

#include <iostream>

class Element {
public:
    int getKey() const { return key; }
    void setKey(int k) { key = k; }
    
    int getLink() const { return link; }
    void setLink(int l) { link = l; }
    
    Element(int k = 0) : key(k), link(-1) {}

private:
    int key;
    int link;
};

class LinkedList {
private:
    Element* list;
    int size;

    // Find the next naturally sorted sequence
    int findNextRun(int start, int& runEnd) {
        runEnd = start;
        while (runEnd < size - 1 && list[runEnd].getKey() <= list[runEnd + 1].getKey()) {
            runEnd++;
        }
        
        return runEnd;
    }

    // Merge two sorted runs
    void merge(Element* mergedList, int left, int mid, int right) {
        int i1 = left, i2 = mid + 1, iResult = left;
        
        while (i1 <= mid && i2 <= right) {
            if (list[i1].getKey() <= list[i2].getKey()) {
                mergedList[iResult++] = list[i1++];
            } else {
                mergedList[iResult++] = list[i2++];
            }
        }
        
        while (i1 <= mid) {
            mergedList[iResult++] = list[i1++];
        }
        
        while (i2 <= right) {
            mergedList[iResult++] = list[i2++];
        }
    }

public:
    LinkedList(int listSize) : size(listSize) {
        list = new Element[size];
    }

    void setElement(int index, int key) {
        if (index >= 0 && index < size) {
            list[index].setKey(key);
        }
    }

    void setupLinks() {
        // Explicitly set links for all 11 elements
        for (int i = 0; i < size; i++) {
            list[i].setLink(i + 1 < size ? i + 1 : size);
        }
    }

    void naturalMergeSort() {
        Element* tempList = new Element[size];
        bool sorted = false;
        
        while (!sorted) {
            sorted = true;
            int left = 0;
            
            while (left < size) {
                // Find first run
                int runEnd1;
                int nextStart = findNextRun(left, runEnd1);
                
                // If only one run remains or reached the end, we're done
                if (runEnd1 == size - 1) break;
                
                // Find second run
                int runEnd2;
                int rightEnd = findNextRun(runEnd1 + 1, runEnd2);
                
                // Merge the two runs
                merge(tempList, left, runEnd1, rightEnd);
                
                // Copy merged section back
                for (int i = left; i <= rightEnd; i++) {
                    list[i] = tempList[i];
                }
                
                sorted = false;
                left = rightEnd + 1;
            }
            
            if (!sorted) {
                printList();
            }
        }
        
        // Reestablish links after sorting
        setupLinks();
        
        delete[] tempList;
    }

    void printList() {
        static int count = 0;
        std::cout << "After phase " << count++ << ": ";
        for (int i = 0; i < size; i++) {
            std::cout << list[i].getKey() << " ";
        }
        std::cout << std::endl;
    }

    // Print links to demonstrate link functionality
    void printLinks() {
        std::cout << "Links:\n";
        for (int i = 0; i < size; i++) {
            std::cout << "Element " << i 
                      << " key: " << list[i].getKey() 
                      << ", Link index: " << list[i].getLink() 
                      << std::endl;
        }
    }

    ~LinkedList() {
        delete[] list;
    }
};

int main() {
    const int size = 11;
    LinkedList list(size);
    
    // Initialize the list
    int values[size] = {12, 2, 16, 30, 8, 28, 4, 10, 20, 6, 18};
    for (int i = 0; i < size; i++) {
        list.setElement(i, values[i]);
    }
    
    // Setup links between elements
    list.setupLinks();
    
    // Print initial state
    std::cout << "Initial list:" << std::endl;
    list.printList();
    list.printLinks();
    
    // Perform natural merge sort
    list.naturalMergeSort();
    
    // Print final state and links
    std::cout << "Final sorted list:" << std::endl;
    list.printList();
    list.printLinks();

    return 0;
}
