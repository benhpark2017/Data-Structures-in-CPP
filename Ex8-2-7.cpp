/**
 * Solutions to Chapter 8, Section 2, Exercise 7 and 8 of Horowitz, Sahni, and 
 * Mehta's Fundamentals of Data Structures in C++, 1st Edition.
 * 
 * Assumptions:
 * For both exercises, it is assumed that the sets S and T are already sorted
 * in non-decreasing order.
 * 
 * Task 7:
 * Let S = {x_1, x_2, ..., x_n} and T = {y_1, y_2, ..., y_r} be two sets. Assume 
 * 1<=x_i<=m, 1<=i<=n, and 1<=y_i<=m, 1<=i<=r. Using the idea of writing the 
 * Find(), Insert(), and Delete() functions in O(1) time, write a function to 
 * determine if S is a subset of T. Your function should work in O(r + n) time. 
 * Since S is an equivalent set to T if and only if S is a subset of T and T is 
 * a subset of S, one can determine in linear time whether two sets are 
 * equivalent. How much space is needed by this function?
 * 
 * Time Complexity:
 * Storing elements of T takes O(r) time.
 * Checking elements of S takes O(n) time.
 * Total time complexity: O(r+n).
 * 
 * Space Complexity:
 * The direct access table requires O(m) space.
 * 
 * 
 * Task 8:
 * Using the idea of writing the Find(), Insert(), and Delete() functions in
 * O(1) time, write an O(n + m) time algorithm to carry out the function of
 * algorithm Verify2 (Program 7.3). How much space does your algorithm need?
 * 
 * SetChecker Class:
 * The Verify2() function is added to the SetChecker class.
 * It uses the present array to mark elements of T and then checks elements of S
 * against T.
 * It also checks elements of T against S to find discrepancies.
 * 
 * Time Complexity:
 * The Verify2() function runs in O(n + m) time, where n is the size of S and m
 * is the size of T.
 * 
 * Space Complexity:
 * The present array requires O(m) space, where m is the maximum possible value
 * in the sets.
 */
 
#include <vector>
#include <iostream>

class Element {
  public:
    // Constructor to initialize key and record
    Element(int k, int r) : key(k), record(r) {}

    // Getter and setter for key
    int getKey() const { return key; }
    void setKey(int k) { key = k; }

    // Public member variable
    int getRecord() const { return record; };

  private:
    int key;
    int record;
};

class SetChecker {
private:
    std::vector<int> present; // Hash table to store record values
    int maxVal;               // Maximum possible value in sets

public:
    SetChecker(int m) : maxVal(m) {
        present.resize(m + 1, 0); // Initialize with 0s, size m+1 to handle 1-based indices
    }

    // Function to check if S is a subset of T
    bool isSubset(const std::vector<Element>& S, const std::vector<Element>& T) {
        // First, mark all elements from set T
        for (const Element& y : T) {
            present[y.getKey()] = 1;
        }

        // Check if all elements from S are present in T
        bool isSubset = true;
        for (const Element& x : S) {
            if (present[x.getKey()] == 0) {
                isSubset = false;
                break;
            }
        }

        // Clean up - reset the present array for future use
        for (const Element& y : T) {
            present[y.getKey()] = 0;
        }

        return isSubset;
    }

    // Function to check if two sets are equivalent
    bool areEquivalent(const std::vector<Element>& S, const std::vector<Element>& T) {
        return isSubset(S, T) && isSubset(T, S);
    }

    void Verify2(const std::vector<Element>& S, const std::vector<Element>& T) {
        // Manually reset the present array to 0
        for (int i = 0; i <= maxVal; ++i) {
            present[i] = 0;
        }
    
        // Store records of elements in T
        for (const Element& y : T) {
            present[y.getKey()] = y.getRecord();
        }
    
        // Check elements of S against T
        for (const Element& x : S) {
            if (present[x.getKey()] == 0) {
                std::cout << x.getKey() << " is not in set T." << std::endl;
            } else if (x.getRecord() != present[x.getKey()]) {
                std::cout << "Discrepancy in " << x.getKey() << ": "
                          << x.getRecord() << " and " << present[x.getKey()] << std::endl;
            }
        }
    
        // Manually reset the present array to 0 again
        for (int i = 0; i <= maxVal; ++i) {
            present[i] = 0;
        }
    
        // Store records of elements in S
        for (const Element& x : S) {
            present[x.getKey()] = x.getRecord();
        }
    
        // Check elements of T against S
        for (const Element& y : T) {
            if (present[y.getKey()] == 0) {
                std::cout << y.getKey() << " is not in set S." << std::endl;
            }
        }
    
        // Manually reset the present array to 0 for future use
        for (int i = 0; i <= maxVal; ++i) {
            present[i] = 0;
        }
    }
};

// Helper function to print a vector of Elements
void printVector(const std::vector<Element>& vec, const std::string& name) {
    std::cout << name << " = { ";
    for (size_t i = 0; i < vec.size(); ++i) {
        std::cout << vec[i].getKey() << "(" << vec[i].getRecord() << ")";
        if (i < vec.size() - 1) std::cout << ", ";
    }
    std::cout << " }" << std::endl;
}

int main() {
    std::cout << "\nTesting Class SetChecker:" << std::endl;

    // Create some test sets
    std::vector<Element> S1 = {{1, 10}, {2, 20}, {3, 30}, {6, 60}};
    std::vector<Element> S2 = {{1, 10}, {2, 15}, {3, 20}, {4, 40}, {5, 50}};
    std::vector<Element> S3 = {{1, 10}, {2, 20}, {3, 30}};
    std::vector<Element> S4 = {{1, 10}, {2, 20}, {3, 30}, {6, 60}};
    std::vector<Element> S5 = {{1, 10}, {2, 20}, {3, 30}, {6, 60}, {8, 80}};

    // Initialize SetChecker with maximum possible value
    SetChecker checker(10);  // Assuming maximum value is 10

    // Print test sets
    printVector(S1, "S1");
    printVector(S2, "S2");
    printVector(S3, "S3");
    printVector(S4, "S4");
    printVector(S5, "S5");    

    // Test isSubset() function
    std::cout << "\nTesting if Set 3 is a subset of Set 1:" << std::endl;
    std::cout << std::boolalpha << checker.isSubset(S3, S1) << std::endl;

    std::cout << "\nTesting if Set 1 is a subset of Set 3:" << std::endl;
    std::cout << std::boolalpha << checker.isSubset(S1, S3) << std::endl;
    
    // Test areEquivalent() function
    std::cout << "\nTesting if Sets 1 and 4 are equivalent:" << std::endl;
    std::cout << std::boolalpha << checker.areEquivalent(S1, S4) << std::endl;

    // Test Verify2 function
    std::cout << "\nTesting Verify2 function:" << std::endl;
    checker.Verify2(S2, S1);

    return 0;
}
