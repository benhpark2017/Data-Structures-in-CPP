/**
 * Solutions to Chapter 7, Section 7, Exercise 4 of Horowitz, Sahni, and Mehta's
 * Fundamentals of Data Structures in C++, 1st Edition.
 * 
 * Task 4:
 * Write a sort function to sort R_1, R_2, ..., R_n lexically on keys
 * (K^1, ..., K^r) for the case when the range of each key is much larger than 
 * n? In this case, the bin-sort scheme used in RadixSort() to sort within each
 * key becomes inefficient (why?). What scheme would you use to sort within a
 * key if we desired a function with (a) good worst-case behavior, (b) good 
 * average behavior, and (c) small n, say n < 15?
 * 
 * 
 * Answers to conceptual questions:
 * 
 * Why bin-sort is inefficient in this scenario:
 * Bin-sort relies on key ranges. When the range of each key is much larger than
 * n, the number of bins becomes excessive, leading to increased memory usage
 * and inefficiency in time due to handling a sparse bin array.
 * 
 * Proposed sorting schemes for different scenarios:
 * For good worst-case behavior, use merge sort or quick sort, with time
 * complexity of O(n log (n)). Merge sort is stable while quick sort is
 * unstable.
 * For good average behavior, use quick sort; consider randomized or median
 * -of-three pivot selection for optimized efficiency.
 * For small n, where n < 15, use insertion sort. In the worst case, it 
 * exhibits O(n^2) time complexity and O(n) in the best case.
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

class Record {
public:
    std::vector<int> keys; // Keys for lexical sorting
    std::string data;      // Associated data

    Record(std::vector<int> k, std::string d) : keys(std::move(k)), data(std::move(d)) {}
};

// Comparator for lexical sorting
bool lexicalCompare(const Record& a, const Record& b) {
    for (size_t i = 0; i < a.keys.size(); ++i) {
        if (a.keys[i] != b.keys[i]) {
            return a.keys[i] < b.keys[i];
        }
    }
    return false; // Equal keys
}

// Lexical Sort Function
void lexicalSort(std::vector<Record>& records) {
    size_t n = records.size();

    if (n < 15) {
        // Use Insertion Sort for small n
        for (size_t i = 1; i < n; ++i) {
            Record key = records[i];
            size_t j = i;
            while (j > 0 && lexicalCompare(key, records[j - 1])) {
                records[j] = records[j - 1];
                --j;
            }
            records[j] = key;
        }
    } else if (n > 1000) {
        // For large n, consider Merge Sort for good worst-case behavior
        std::stable_sort(records.begin(), records.end(), lexicalCompare);
    } else {
        // Use Quick Sort for moderate-sized lists with good average-case performance
        std::sort(records.begin(), records.end(), lexicalCompare);
    }
}

// Print records
void printRecords(const std::vector<Record>& records) {
    for (const auto& record : records) {
        for (int key : record.keys) {
            std::cout << key << " ";
        }
        std::cout << ": " << record.data << "\n";
    }
}

int main() {
    std::vector<Record> records = {
        {{3, 1, 4}, "Data1"},
        {{1, 5, 9}, "Data2"},
        {{2, 6, 5}, "Data3"},
        {{3, 5, 8}, "Data4"},
        {{1, 5, 7}, "Data5"}
    };

    std::cout << "Original records:\n";
    printRecords(records);

    lexicalSort(records);

    std::cout << "\nSorted records:\n";
    printRecords(records);

    return 0;
}
