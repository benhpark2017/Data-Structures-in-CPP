/**
 * Solutions to Chapter 7, Section 9, Exercise 4 of Horowitz, Sahni, and Mehta's
 * Fundamentals of Data Structures in C++, 1st Edition.
 * 
 * Task 4: 
 * The objective of this assignment is to come up with a composite sorting
 * function that is good on the worst-time criterion. The candidate sort methods
 * are (a) insertion sort, (b) quick sort, (c) merge sort, and (d) heap sort.
 * 
 * To begin with, program these sort methods in C++. In each case, assume that
 * n integers are to be sorted. In the case of quick sort, use the
 * median-of-three method. In the case of merge sort, use the iterative method
 * (as a separate exercise, you might wish to compare the run times of the
 * iterative and recursive versions of merge sort and determine what the
 * recursion penalty is in your favorite language using your favorite compiler).
 * Check out the correctness of the programs using some test data. Since quite 
 * detailed and working functions are given in the book, this part of the 
 * assignment should take little effort. In any case, no points are earned until
 * after this step.
 * 
 * To obtain reasonably accurate run times, you need to know the accuracy of the
 * clock or timer you are using. Determine this by reading the appropriate
 * manual. Let the clock accuracy be 0. Now, run a pilot test to determine
 * ballpark times for your four sorting functions for n = 5, 10, 20, 30, 40, 50,
 * and 100. You will notice times of 0 for many of these values of n. The other
 * times may not be much larger than the clock accuracy.
 * 
 * To time an event that is smaller than or near the clock accuracy, repeat it
 * many times and divide the overall time by the number of repetitions. You 
 * should obtain times that are accurate to within 1%.
 * 
 * We need worst-case data for each of the four sort methods. The worst-case
 * data for insertion sort are easy to generate. Just use the sequence n, n - 1,
 * n − 2, ..., 1. Worst-case data for merge sort can be obtained by working
 * backward. Begin with the last merge your function will perform and make this 
 * work hardest. Then look at the second-to-last merge, and so on. Use this
 * logic to obtain a program that will generate worst-case data for merge sort
 * for each of the above values of n. 
 * 
 * Generating worst-case data for heap sort is the hardest, so, here we shall
 * use a random permutation generator (one is provided in Program 7.19). We 
 * shall generate random permutations of the desired size, clock heap sort on 
 * each of these, and use the max of these times to approximate to the 
 * worst-case time. You will be able to use more random permutations for smaller
 * values of n than for larger. For no value of n should you use fewer than 10
 * permutations. Use the same technique to obtain worst-case times for quick
 * sort.
 * 
 * Having settled on the test data, we are ready to perform our experiment.
 * Obtain the worst-case times. From these times you will get a rough idea when 
 * one function performs better than the other. Now, narrow the scope of your 
 * experiments and determine the exact value of n when one sort method 
 * outperforms another. For some methods, this value may be 0. For instance, 
 * each of the other three methods may be faster than quick sort for all values 
 * of n. 
 * 
 * Plot your findings on a single sheet of graph paper. Do you see the n^2 
 * behavior of insertion sort and quick sort and the n log(n) behavior of the 
 * other two methods for suitably large n (about n > 20)? If not, there is 
 * something wrong with the clock or with both. For each value of n, determine 
 * the sort function that is fastest (simply look at your graph). Write a 
 * composite function with the best possible performance for all n. Clock this
 * function and plot the times on the same graph sheet you used earlier.
 * 
 * A word of CAUTION: If you are using a multiprocess computer, make all your
 * final runs at about the same time. On these computers, the clocked time will
 * vary significantly with the amount of computer work load. Comparing the run
 * times of an insertion sort run made at 2:00 p.m. with the run times of a
 * merge sort run made at 2:00 a.m. will not be very meaningful.
 * 
 * WHAT TO TURN IN:
 * You are required to submit a report that states the clock accuracy, the
 * number of random permutations tried for heap sort, the worst case data for 
 * merge sort and how you generated it, a table of times for the above values 
 * of n, the times for the narrowed ranges, the graph, and a table of times for 
 * the composite function. In addition, your report must be accompanied by a 
 * complete listing of the program used by you (this includes the sorting 
 * functions and the main program for timing and test-data generation)
 * 
 */

#include <iostream>
#include <vector>
#include <chrono>
#include <random> // For the Mersenne Twister engine
#include <iomanip> // For std::setw()

class Element {
  private:
    int key;
    
  public:
    Element() : key(0) {}
    Element(int k) : key(k) {}
    
    // Getter methods
    int getKey() const { return key; }

    // Setter methods
    void setKey(int k) { key = k; }
};

// Helper function to print the vector of Elements
void printElements(const std::vector<Element>& elements) {
    for (const auto& elem : elements) {
        std::cout << elem.getKey() << " ";
    }
    std::cout << std::endl;
}

// Insertion Sort
void insertionSort(std::vector<Element>& elements) {
    for (size_t i = 1; i < elements.size(); ++i) {
        Element key = elements[i];
        int j = i - 1;
        while (j >= 0 && elements[j].getKey() > key.getKey()) {
            elements[j + 1] = elements[j];
            j--;
        }
        elements[j + 1] = key;
    }
}

// Quick Sort (using median-of-three method)
int medianOfThree(std::vector<Element>& elements, int low, int high) {
    int mid = low + (high - low) / 2;
    if (elements[low].getKey() > elements[mid].getKey())
        std::swap(elements[low], elements[mid]);
    if (elements[low].getKey() > elements[high].getKey())
        std::swap(elements[low], elements[high]);
    if (elements[mid].getKey() > elements[high].getKey())
        std::swap(elements[mid], elements[high]);
    return mid; // Return the index of the median
}

int partition(std::vector<Element>& elements, int low, int high) {
    int pivotIndex = medianOfThree(elements, low, high);
    std::swap(elements[pivotIndex], elements[high]); // Move pivot to the end
    Element pivot = elements[high];
    int i = low - 1;

    for (int j = low; j < high; ++j) {
        if (elements[j].getKey() <= pivot.getKey()) {
            i++;
            std::swap(elements[i], elements[j]);
        }
    }
    std::swap(elements[i + 1], elements[high]);
    return i + 1;
}

void quickSortHelper(std::vector<Element>& elements, int low, int high) {
    if (low < high) {
        int pi = partition(elements, low, high);
        quickSortHelper(elements, low, pi - 1);
        quickSortHelper(elements, pi + 1, high);
    }
}

void quickSort(std::vector<Element>& elements) {
    quickSortHelper(elements, 0, elements.size() - 1);
}

// Iterative Merge Sort
void merge(std::vector<Element>& elements, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    std::vector<Element> L(n1), R(n2);

    for (int i = 0; i < n1; ++i)
        L[i] = elements[left + i];
    for (int j = 0; j < n2; ++j)
        R[j] = elements[mid + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i].getKey() <= R[j].getKey()) {
            elements[k] = L[i];
            i++;
        } else {
            elements[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        elements[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        elements[k] = R[j];
        j++;
        k++;
    }
}

void mergeSort(std::vector<Element>& elements) {
    int n = elements.size();
    for (int currSize = 1; currSize <= n - 1; currSize = 2 * currSize) {
        for (int left = 0; left < n - 1; left += 2 * currSize) {
            int mid = std::min(left + currSize - 1, n - 1);
            int right = std::min(left + 2 * currSize - 1, n - 1);
            merge(elements, left, mid, right);
        }
    }
}

// Heap Sort
void heapify(std::vector<Element>& elements, int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && elements[left].getKey() > elements[largest].getKey())
        largest = left;

    if (right < n && elements[right].getKey() > elements[largest].getKey())
        largest = right;

    if (largest != i) {
        std::swap(elements[i], elements[largest]);
        heapify(elements, n, largest);
    }
}

void heapSort(std::vector<Element>& elements) {
    int n = elements.size();

    // Build heap (rearrange array)
    for (int i = n / 2 - 1; i >= 0; --i)
        heapify(elements, n, i);

    // Extract elements from heap one by one
    for (int i = n - 1; i > 0; --i) {
        std::swap(elements[0], elements[i]);
        heapify(elements, i, 0);
    }
}

// Do NOT use 0 as the number of permutations! Otherwise, the program will return
// a segmentation fault.
// Function to generate a random permutation of the first n elements in the vector
void permute(std::vector<Element>& elements, int n) {
    // Ensure n does not exceed the size of the vector
    if (n > elements.size()) {
        std::cerr << "Error: n exceeds the size of the vector." << std::endl;
        return;
    }

    // Initialize the Mersenne Twister random number generator
    std::random_device rd; // Seed for the random number engine
    std::mt19937 gen(rd()); // Mersenne Twister engine

    // Perform the Fisher-Yates shuffle on the first n elements
    for (size_t i = n - 1; i > 0; --i) {
        // Generate a random index between 0 and i (inclusive)
        std::uniform_int_distribution<size_t> dist(0, i);
        size_t j = dist(gen);

        // Swap elements[i] and elements[j]
        std::swap(elements[i], elements[j]);
    }
}

// Function to print the vector
void printVector(const std::vector<Element>& vec) {
    for (const auto& elem : vec) {
        std::cout << elem.getKey() << " ";
    }
    std::cout << std::endl;
}

// Function to generate worst-case data for Insertion Sort
std::vector<Element> generateWorstCaseInsertionSort(int n) {
    std::vector<Element> worstCaseData;
    for (int i = n; i >= 1; --i) {
        worstCaseData.push_back(Element(i));
    }
    return worstCaseData;
}

// Helper function to recursively generate worst-case input for merge sort
void generateWorstCaseMergeSortHelper(std::vector<int>& data, int start, int end) {
    if (end - start <= 1) return; // Base case: only one element, no further splitting needed

    int mid = (start + end) / 2;

    // Create temporary vectors to hold interleaved elements
    std::vector<int> left, right;

    // Interleave the elements for worst-case merging
    for (int i = start; i < end; i++) {
        if (i % 2 == 0) {
            left.push_back(data[i]);
        } else {
            right.push_back(data[i]);
        }
    }

    // Copy interleaved elements back to the original data array
    for (int i = 0; i < left.size(); i++) {
        data[start + i] = left[i];
    }
    for (int i = 0; i < right.size(); i++) {
        data[start + left.size() + i] = right[i];
    }

    // Recursively apply the same process to the two halves
    generateWorstCaseMergeSortHelper(data, start, mid);
    generateWorstCaseMergeSortHelper(data, mid, end);
}

// Function to generate worst-case input for merge sort
std::vector<Element> generateWorstCaseMergeSort(int n) {
    std::vector<int> rawData(n);

    // Initialize the data with sorted keys
    for (int i = 0; i < n; ++i) {
        rawData[i] = i + 1;
    }

    // Rearrange the data to produce the worst-case for merge sort
    generateWorstCaseMergeSortHelper(rawData, 0, n);

    // Convert rawData into a vector of Element objects
    std::vector<Element> worstCaseData;
    for (int key : rawData) {
        worstCaseData.push_back(Element(key));
    }

    return worstCaseData;
}

// Function to generate worst-case data for Quick Sort
std::vector<Element> generateWorstCaseQuickSort(int n) {
    std::vector<Element> worstCaseData;
    int low = 1, high = n;
    for (int i = 0; i < n; ++i) {
        if (i % 2 == 0) {
            worstCaseData.push_back(Element(low++));
        } else {
            worstCaseData.push_back(Element(high--));
        }
    }
    return worstCaseData;
}

// Function to generate worst-case data for Heap Sort (Randomized by mt19337)
std::vector<Element> generateWorstCaseHeapSort(int n) {
    std::vector<int> worstHeapSortData(n);  // Initialize the vector with size n

    // Populate the data with sorted keys
    for (int i = 0; i < n; ++i) {
        worstHeapSortData[i] = i + 1;
    }

    // Convert the vector of ints to a vector of Elements
    std::vector<Element> worstCaseData;
    for (int key : worstHeapSortData) {
        worstCaseData.push_back(Element(key));
    }
    
    // Permute the data to create a worst-case scenario for Heap Sort
    permute(worstCaseData, n);
    
    return worstCaseData;
}

void runSortingExperiment() {
    std::vector<int> sizes = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100,
                              200, 300, 400, 500, 600, 700, 800, 900, 1000};
    
    // Print table header
    std::cout << std::setw(10) << "Size" 
              << std::setw(15) << "Insertion" 
              << std::setw(15) << "Merge" 
              << std::setw(15) << "Quick" 
              << std::setw(15) << "Heap" << std::endl;
    std::cout << std::string(70, '-') << std::endl;

    for (int n : sizes) {
        std::cout << std::setw(10) << n;
        
        // Test Insertion Sort
        std::vector<Element> insertionData = generateWorstCaseInsertionSort(n);
        auto startIns = std::chrono::high_resolution_clock::now();
        insertionSort(insertionData);
        auto endIns = std::chrono::high_resolution_clock::now();
        auto timeIns = std::chrono::duration_cast<std::chrono::microseconds>(endIns - startIns).count();
        std::cout << std::setw(15) << timeIns;

        // Test Merge Sort
        std::vector<Element> mergeData = generateWorstCaseMergeSort(n);
        auto startMer = std::chrono::high_resolution_clock::now();
        mergeSort(mergeData);
        auto endMer = std::chrono::high_resolution_clock::now();
        auto timeMer = std::chrono::duration_cast<std::chrono::microseconds>(endMer - startMer).count();
        std::cout << std::setw(15) << timeMer;

        // Test Quick Sort
        std::vector<Element> quickData = generateWorstCaseQuickSort(n);
        auto startQck = std::chrono::high_resolution_clock::now();
        quickSort(quickData);
        auto endQck = std::chrono::high_resolution_clock::now();
        auto timeQck = std::chrono::duration_cast<std::chrono::microseconds>(endQck - startQck).count();
        std::cout << std::setw(15) << timeQck;

        // Test Heap Sort
        std::vector<Element> heapData = generateWorstCaseHeapSort(n);
        auto startHeap = std::chrono::high_resolution_clock::now();
        heapSort(heapData);
        auto endHeap = std::chrono::high_resolution_clock::now();
        auto timeHeap = std::chrono::duration_cast<std::chrono::microseconds>(endHeap - startHeap).count();
        std::cout << std::setw(15) << timeHeap << std::endl;
    }
}

int main() {
    runSortingExperiment();
    return 0;
}
