/**
 * Solutions to Chapter 2, Section 3, Exercise 3 and 4 of Horowitz's
 * Data Structures in C++.
 */

#include <iostream>
#include <stdexcept>

const int MAX_SIZE = 1000; // Define a maximum size for the space array

int space[MAX_SIZE];
int front[MAX_SIZE];
int rear[MAX_SIZE];
int n; // Number of lists

void initialize(int numLists, int m) {
    n = numLists;
    front[0] = -1;
    rear[0] = -1;
    for (int i = 1; i < n; ++i) {
        front[i] = rear[i-1];
        rear[i] = front[i];
    }
    front[n] = m - 1;
}

//Exercise 3.
bool insert(int i, int j, int item) {
    if (rear[i] >= front[i+1]) {
        std::cerr << "Insertion failed: not enough space" << std::endl;
        return false;
    }

    if (i < 0 || i >= n) {
        std::cerr << "Insertion failed: invalid list index" << std::endl;
        return false;
    }

    int insertPos = front[i] + j + 1;

    if (insertPos > rear[i] + 1) {
        std::cerr << "Insertion failed: invalid position" << std::endl;
        return false;
    }

    for (int k = rear[i]; k >= insertPos; --k) {
        space[k + 1] = space[k];
    }

    space[insertPos] = item;
    ++rear[i];

    return true;
}


//Exercise 4.
//The term 'delete' is already taken as a built-in operator name in C++.
int deleteElement(int i, int j) {
    if (i < 0 || i >= n) {
        throw std::out_of_range("Deletion failed: invalid list index");
    }

    if (j <= 0 || j > (rear[i] - front[i])) {
        throw std::out_of_range("Deletion failed: invalid position");
    }

    int deletePos = front[i] + j;

    int deletedItem = space[deletePos];

    for (int k = deletePos; k < rear[i]; ++k) {
        space[k] = space[k + 1];
    }

    --rear[i];

    return deletedItem;
}

int main() {
    const int m = 20; // Example size for space array
    const int numLists = 3; // Example number of lists

    initialize(numLists, m);

    // Example usage
    insert(0, 0, 10);
    insert(0, 1, 20);
    insert(1, 0, 30);
    insert(1, 1, 40);

    std::cout << "Deleted item: " << deleteElement(0, 1) << std::endl;

    return 0;
}
