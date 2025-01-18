/**
 * Solutions to Chapter 7, Section 7, Exercise 5 and 6 of Horowitz, Sahni, and
 * Mehta's Fundamentals of Data Structures in C++, 1st Edition.
 * 
 * Task 5:
 * If we have n records with integer keys in the range [0, n^2), then they may
 * be sorted in O(n * log(n)) time using heap or merge sort. Radix sort on a
 * single key (i.e. pass number d = 1 and radix = n^2) takes O(n^2) time. Show
 * how to interpret the keys as two sub-keys so that radix sort will take only 
 * O(n) time to sort n records. Hint: Each key K_i may be written as K_i = K_i^1
 * * n + K_i^2 with K_i^1 and K_i^2 being integers in the range [0, n).
 * 
 * Task 6:
 * Generalize the method of the previous exercise to the case of integer keys in
 * the range (0, n^p), obtaining an O(p*n) time sorting function in C++.
 */

#include <vector>
#include <iostream>
#include <cmath>

class Element {
private:
    int key;
    int link;
public:
    Element(int k = 0) : key(k), link(0) {}
    int getKey() const { return key; }
    void setKey(int k) { key = k; }
    int getLink() const { return link; }
    void setLink(int l) { link = l; }
};

void RadixSort(std::vector<Element>& list, int n, int p) {
    const int RADIX = n;  // Radix is equal to n for decomposition

    // Initialize the linked list
    for (int i = 1; i <= n; i++) {
        list[i].setLink(i + 1);
    }
    list[n].setLink(0);  // Terminate the chain

    int first = 1;  // Start of the chain

    // Process each sub-key (p passes for keys in [0, n^p))
    for (int pass = 0; pass < p; pass++) {
        // Initialize bins
        std::vector<int> f(RADIX, 0), e(RADIX, 0);
        
        // Distribute keys into bins based on the current sub-key
        int current = first;
        first = 0;  // Reset for next iteration

        while (current) {
            // Extract the sub-key for the current pass
            int key = (list[current].getKey() / static_cast<int>(std::pow(n, pass))) % n;
            int next = list[current].getLink();

            if (!f[key]) {
                f[key] = current;
            } else {
                list[e[key]].setLink(current);
            }
            e[key] = current;
            current = next;
        }

        // Collect records from bins
        int last = 0;  // Track last element processed

        // Find first non-empty bin and initialize first pointer
        for (int i = 0; i < RADIX; i++) {
            if (f[i] != 0) {
                if (!first) {
                    first = f[i];
                }
                if (last) {
                    list[last].setLink(f[i]);
                }
                last = e[i];
                list[last].setLink(0);  // Ensure proper termination
            }
        }
    }
}

int main() {
    int n = 10;  // Number of records
    int p = 2;   // Number of passes (for keys in [0, n^p))

    // List of elements to be sorted (1-indexed)
    std::vector<Element> list(n + 1);

    for (int i = 1; i <= n; i++) {
        std::cout << "Enter key for element " << i << ": ";
        int key;
        std::cin >> key;
        list[i].setKey(key);
    }

    RadixSort(list, n, p);

    std::cout << "Sorted keys:" << std::endl;
    int current = 1;
    while (current) {
        std::cout << list[current].getKey() << " ";
        current = list[current].getLink();
    }
    std::cout << std::endl;

    return 0;
}
