/**
 * Solutions to Chapter 7, Section 9, Exercise 5 of Horowitz, Sahni, and Mehta's
 * Fundamentals of Data Structures in C++, 1st Edition.
 * 
 * Task 5:
 * Repeat the previous exercise for the case of average run times. Average-case
 * data are usually very difficult to create, so use random permutations. This
 * time, however, do not repeat a permutation many times to overcome clock 
 * inaccuracies. Instead, use each permutation once and clock the overall time
 * (for a fixed n).
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

// Function to generate worst-case data for Heap Sort (Randomized by mt19337)
std::vector<Element> generateAverageData(int n) {
    std::vector<int> averageTimeData(n);  // Initialize the vector with size n

    // Populate the data with sorted keys
    for (int i = 0; i < n; ++i) {
        averageTimeData[i] = i + 1;
    }

    // Convert the vector of ints to a vector of Elements
    std::vector<Element> averageCaseData;
    for (int key : averageTimeData) {
        averageCaseData.push_back(Element(key));
    }
    
    // Permute the data to create a worst-case scenario for Heap Sort
    permute(averageCaseData, n);
    
    return averageCaseData;
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
        std::vector<Element> insertionData = generateAverageData(n);
        auto startIns = std::chrono::high_resolution_clock::now();
        insertionSort(insertionData);
        auto endIns = std::chrono::high_resolution_clock::now();
        auto timeIns = std::chrono::duration_cast<std::chrono::microseconds>(endIns - startIns).count();
        std::cout << std::setw(15) << timeIns;

        // Test Merge Sort
        std::vector<Element> mergeData = generateAverageData(n);
        auto startMer = std::chrono::high_resolution_clock::now();
        mergeSort(mergeData);
        auto endMer = std::chrono::high_resolution_clock::now();
        auto timeMer = std::chrono::duration_cast<std::chrono::microseconds>(endMer - startMer).count();
        std::cout << std::setw(15) << timeMer;

        // Test Quick Sort
        std::vector<Element> quickData = generateAverageData(n);
        auto startQck = std::chrono::high_resolution_clock::now();
        quickSort(quickData);
        auto endQck = std::chrono::high_resolution_clock::now();
        auto timeQck = std::chrono::duration_cast<std::chrono::microseconds>(endQck - startQck).count();
        std::cout << std::setw(15) << timeQck;

        // Test Heap Sort
        std::vector<Element> heapData = generateAverageData(n);
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
