/**
 * Solutions to Chapter 7, Section 5, Exercise 6 of Horowitz, Sahni, and Mehta's
 * Fundamentals of Data Structure in C++.
 * 
 * Task 6:
 * Write an iterative natural merge sort function using arrays as in function
 * MergeSort(). How much time does this function take on an initially sorted
 * list? Note that MergeSort() takes O(n log(n)) on such an input list. What is
 * the worst-case computing time of the new function? How much additional space
 * is needed?
 */

#include <iostream>
#include <stack>
#include <utility>
#include <vector>

class Element {
  public:
    int getKey() const { return key; }
    void setKey(int k) { key = k; }
    Element(int k = 0) : key(k), link(0) {}
  private:
    int key;
    int link;
};

void merge(Element *initList, Element *mergedList, const int l, const int m, const int n) {
    int i1 = l, i2 = m + 1, iResult = l;
    while (i1 <= m && i2 <= n) {
        if (initList[i1].getKey() <= initList[i2].getKey()) {
            mergedList[iResult++] = initList[i1++];
        } else {
            mergedList[iResult++] = initList[i2++];
        }
    }
    while (i1 <= m) mergedList[iResult++] = initList[i1++];
    while (i2 <= n) mergedList[iResult++] = initList[i2++];
}

void MergePass(Element *initList, Element *resultList, const int n, const int l) {
    int i;
    for (i = 0; i <= n - 2 * l; i += 2 * l) {
        merge(initList, resultList, i, i + l - 1, i + 2 * l - 1);
    }
    if (i + l - 1 < n) merge(initList, resultList, i, i + l - 1, n);
    else for (int t = i; t <= n; t++) resultList[t] = initList[t];
}

void MergeSort(Element *list, const int n) {
    Element *tempList = new Element[n];
    for (int l = 1; l < n; l *= 2) {
        MergePass(list, tempList, n - 1, l);
        l *= 2;
        MergePass(tempList, list, n - 1, l);
    }
    delete[] tempList;
}

void printList(Element *list, int size) {
    std::cout << "After sorting: ";
    for (int i = 0; i < size; i++) {
        std::cout << list[i].getKey() << " ";
    }
    std::cout << std::endl;
}

int findNextRun(Element* list, int start, const int n) {
    if (start >= n - 1) return n - 1;
    
    while (start < n - 1 && list[start].getKey() <= list[start + 1].getKey()) {
        start++;
    }
    return start;
}

void naturalMergeSort(Element* list, const int n) {
    Element* tempList = new Element[n];
    bool sorted = false;
    
    while (!sorted) {
        sorted = true;  // Assume array is sorted until we find a merge is needed
        int left = 0;   // Start of first run
        
        while (left < n - 1) {
            // Find end of first run
            int mid = findNextRun(list, left, n);
            
            // If this run extends to the end of the array, we're done with this pass
            if (mid == n - 1) break;
            
            // Find end of second run
            int right = findNextRun(list, mid + 1, n);
            
            // Merge the two runs
            merge(list, tempList, left, mid, right);
            
            // Copy merged sequence back to original array
            for (int i = left; i <= right; i++) {
                list[i] = tempList[i];
            }
            
            sorted = false;  // We had to merge, so array wasn't fully sorted
            left = right + 1;  // Move to start of next run
        }
    }
    
    delete[] tempList;
}


int main() {
    const int size = 11;
    
    Element F[size] = {12, 2, 16, 30, 8, 28, 4, 10, 20, 6, 18};
    Element G[size] = {12, 2, 16, 30, 8, 28, 4, 10, 20, 6, 18};
    
    //Iterative merge sort
    MergeSort(F, size);
    printList(F, size);
    
    //iterative natural merge sort
    naturalMergeSort(G, size);
    printList(G, size);
    
    return 0;
}
