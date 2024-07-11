/**
 * Solutions to Chapter 2, Section 8, Exercise 9 of Horowitz's
 * Fundamentals of Data Structures in C++.
 */
 
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <iomanip> // For setw

bool completedMove(int** arr); // Check if all tiles have been stepped on
bool bugMove(int x, int& imove, int& jmove); // Bug movement
void initialize(int** arr); // Initialize the array
bool insideRoom(int imove, int jmove); // Check if inside room
bool display(int** arr, int counter); // Display the array

int m, n; // Global variables

int main() {
    srand((unsigned)time(NULL));
    
    int** arr;
    int x, imove, jmove, counter = 0;

    std::cout << "Please specify length (from 2 to 40): ";
    std::cin >> m;
    while (m > 40 || m < 2) {
        std::cout << "Re-enter value from 2 to 40: ";
        std::cin >> m;
    }

    std::cout << "Please specify height (from 2 to 20): ";
    std::cin >> n;
    while (n > 20 || n < 2) {
        std::cout << "Re-enter value from 2 to 20: ";
        std::cin >> n;
    }

    std::cout << "Enter starting point (x, y): ";
    std::cin >> imove >> jmove;

    arr = new int*[m];
    for (int i = 0; i < m; i++) {
        arr[i] = new int[n];
    }

    initialize(arr);

    while ((!completedMove(arr)) && (counter < 50000)) {
        do {
            x = rand() % 7;
        } while (!bugMove(x, imove, jmove));

        arr[imove][jmove]++;
        counter++;
    }

    std::cout << "Movement of cockroach: " << counter << std::endl;
    if (display(arr, counter)) {
        std::cout << "The cockroach traversed all tiles." << std::endl;
    } else {
        std::cout << "The cockroach failed to traverse all tiles." << std::endl;
    }

    for (int i = 0; i < m; i++) {
        delete[] arr[i];
    }
    delete[] arr;

    return 0;
}

bool completedMove(int** arr) {
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            if (arr[i][j] == 0) {
                return false;
            }
        }
    }
    return true;
}

bool bugMove(int x, int& imove, int& jmove) {
    int idirection[8] = {-1, 0, 1, 1, 1, 0, -1, -1};
    int jdirection[8] = {1, 1, 1, 0, -1, -1, -1, 0};
    int itotal = imove + idirection[x];
    int jtotal = jmove + jdirection[x];

    if (insideRoom(itotal, jtotal)) { // Movement must remain within the room
        imove = itotal;
        jmove = jtotal;
        return true;
    }
    return false;
}

void initialize(int** arr) {
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            arr[i][j] = 0;
        }
    }
}

bool insideRoom(int imove, int jmove) {
    return !(imove >= m || imove < 0 || jmove >= n || jmove < 0);
}

bool display(int** arr, int counter) {
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            std::cout << std::setw(4) << arr[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            if (arr[i][j] == 0) {
                return false;
            }
        }
    }
    return true;
}
