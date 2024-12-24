/**
 * Solutions to Chapter 7, Section 7, Exercise 1 of Horowitz, Sahni, and Mehta's
 * Fundamentals of Data Structures in C++, 1st Edition.
 * 
 * Task 1:
 * Write the status of the list F = {12, 2, 16, 30, 8, 28, 4, 10, 20, 6, 18} at
 * the end of each phase of RadixSort(). Use 10 as the radix.
 */


#include <iostream>
#include <cmath>

const int radix = 10;

class Element {
public:
    int getKey() const { return key; }
    void setKey(int k) { key = k; }
    int getLink() const { return link; }
    void setLink(int l) { link = l; }

    Element(int k = 0, int l = 0) : key(k), link(l) {}

private:
    int key;
    int link;
};

void RadixSort(Element* list, const int d, const int n) {
    int e[radix], f[radix];

    for (int pass = 0; pass < d; pass++) {
        for (int j = 0; j < radix; j++) {
            f[j] = e[j] = 0; // Reset pointers for each bucket
        }

        int current = list[0].getLink(); // Start from first actual element

        while (current != 0) {
            int digit = (list[current].getKey() / static_cast<int>(pow(radix, pass))) % radix;
            if (f[digit] == 0) {
                f[digit] = current; // Start of the bucket
            } else {
                list[e[digit]].setLink(current); // Append to the bucket
            }
            e[digit] = current; // Update end of the bucket
            current = list[current].getLink();
        }

        // Rebuild the linked list in order of buckets
        int last = 0; // Start from dummy head
        for (int j = 0; j < radix; j++) {
            if (f[j] != 0) {
                list[last].setLink(f[j]); // Connect last element to the start of this bucket
                last = e[j];              // Update last to the end of this bucket
            }
        }
        list[last].setLink(0); // Terminate the list

        // Print the status of the list after this pass
        std::cout << "After pass " << pass + 1 << ": ";
        int currentPrint = list[0].getLink(); // Start from the first element
        while (currentPrint != 0) {
            std::cout << list[currentPrint].getKey() << " ";
            currentPrint = list[currentPrint].getLink();
        }
        std::cout << std::endl;
    }
}

int main() {
    int d = 2; // Number of digits in the largest number
    int n = 11; // Number of elements in the list
    Element list[12]; // 1-based indexing with a dummy node at index 0

    // Initialize input keys and links
    int input[] = {12, 2, 16, 30, 8, 28, 4, 10, 20, 6, 18};
    for (int i = 1; i <= n; i++) {
        list[i].setKey(input[i - 1]);
        list[i].setLink(i + 1);
    }
    list[0].setLink(1); // Dummy head links to the first element
    list[n].setLink(0); // End of the list

    std::cout << "Original list: ";
    int currentPrint = list[0].getLink(); // Start from the first element
    while (currentPrint != 0) {
        std::cout << list[currentPrint].getKey() << " ";
        currentPrint = list[currentPrint].getLink();
    }
    std::cout << std::endl;

    RadixSort(list, d, n);

    std::cout << "Sorted list: ";
    currentPrint = list[0].getLink(); // Start from the first element
    while (currentPrint != 0) {
        std::cout << list[currentPrint].getKey() << " ";
        currentPrint = list[currentPrint].getLink();
    }
    std::cout << std::endl;

    return 0;
}
