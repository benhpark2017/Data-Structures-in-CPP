/** Solutions to Chapter 1, Section 5, Exercise 11 of Horowitz's Data
 * Structures in C++. */

#include <iostream>
#include <vector>

// Recursive Ackermann function
unsigned long recursiveAckermann(int m, int n) {
    if (m == 0) {
        return n + 1;
    } else if (n == 0) {
        return recursiveAckermann(m - 1, 1);
    } else {
        return recursiveAckermann(m - 1, recursiveAckermann(m, n - 1));
    }
}

// Iterative Ackermann function
unsigned long iterativeAckermann(int m, int n) {
    // Use a vector to simulate the "right" list
    std::vector<int> right(m + 1);
    int result = n;
    int i = 0;

    right[0] = m;

    while (true) {
        if (right[i] > 0 && result > 0) {
            if (i + 1 >= right.size()) {
                right.resize(right.size() + 1);
            }
            right[i + 1] = right[i];
            right[i] -= 1;
            result -= 1;
            i += 1;
        } else if (right[i] > 0 && result == 0) {
            right[i] -= 1;
            result = 1;
        } else if (right[i] == 0) {
            result += 1;
            i -= 1;
        }

        if (i < 0) {
            break;
        }
    }

    return result;
}

int main() {
    int m, n;

    std::cout << "Please specify the integer m of A(m, n).\n";
    std::cin >> m;

    std::cout << "Please specify the integer n of A(m, n).\n";
    std::cin >> n;

    std::cout << "Recursive: A(" << m << ", " << n << ") = " << recursiveAckermann(m, n) << std::endl;
    std::cout << "Iterative: A(" << m << ", " << n << ") = " << iterativeAckermann(m, n) << std::endl;

    return 0;
}

