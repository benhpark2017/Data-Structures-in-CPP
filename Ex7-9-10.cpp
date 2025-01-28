/**
 * Solutions to Chapter 7, Section 9, Exercise 10 of Horowitz, Sahni, and 
 * Mehta's Fundamentals of Data Structures in C++, 1st Edition.
 * 
 * Task 10:
 * The objective of this exercise is to study the effect of the size of an array
 * element on the computational time of various sorting algorithms.
 * 
 * (a) Use templates to implement insertion sort, quick sort, iterative merge
 *     sort, and heap sort functions. 
 * (b) Use these functions to sort arrays of (i) characters (char), (ii) 
 *     integers (int), (iii) floating point numbers (float), and (iv) rectangles
 *     (Assume that a rectangle is represented by the coordinates of its bottom
 *     left point and its height and width, all of which are of type float.
 *     Assume, also, that rectangles are to be sorted in non-decreasing order of
 *     their areas.)
 * (c) Obtain a set of run times for each algorithm-data type pair specified 
 *     above. (There should be sixteen such pairs.) To obtain a set of run times
 *     of an algorithm-data type pair, you should run the algorithm on at least
 *     four arrays of different sizes containing elements of the appropriate 
 *     data type. The elements in an array should be generated using a random
 *     number generator.
 * (d) Draw tables and graphs displaying your experimental results. What do you
 *     conclude from the experiments?
 */

/**********************************main.cpp************************************/
#include "SortingTest.h"

int main() {
    SortingTest test;
    test.runAllTests();
    return 0;
}



/**********************************Timer.h************************************/
#ifndef TIMER_H
#define TIMER_H

#include <chrono>

class Timer {
private:
    using Clock = std::chrono::high_resolution_clock;
    using TimePoint = Clock::time_point;
    TimePoint start_time;

public:
    Timer();
    double elapsed() const;
};

#endif // TIMER_H



/**********************************Timer.cpp***********************************/
#include "Timer.h"

Timer::Timer() : start_time(Clock::now()) {}

double Timer::elapsed() const {
    auto end_time = Clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
    return duration.count() / 1000.0; // Convert to milliseconds
}



/*********************************Rectangle.h**********************************/
#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <iostream>

class Rectangle {
private:
    float x, y;    // coordinates of lower-left vertex
    float height;
    float width;
    float area;    // store area to avoid recalculation

public:
    Rectangle(float x = 0, float y = 0, float h = 0, float w = 0);
    float getArea() const;
    
    bool operator<(const Rectangle& other) const;
    bool operator>(const Rectangle& other) const;
    bool operator<=(const Rectangle& other) const;
    bool operator>=(const Rectangle& other) const;
    
    friend std::ostream& operator<<(std::ostream& os, const Rectangle& rect);
};

#endif // RECTANGLE_H



/********************************Rectangle.cpp*********************************/
#include "Rectangle.h"

Rectangle::Rectangle(float x, float y, float h, float w)
    : x(x), y(y), height(h), width(w) {
    area = height * width;
}

float Rectangle::getArea() const { 
    return area; 
}

bool Rectangle::operator<(const Rectangle& other) const {
    return area < other.area;
}

bool Rectangle::operator>(const Rectangle& other) const {
    return area > other.area;
}

bool Rectangle::operator<=(const Rectangle& other) const {
    return area <= other.area;
}

bool Rectangle::operator>=(const Rectangle& other) const {
    return area >= other.area;
}

std::ostream& operator<<(std::ostream& os, const Rectangle& rect) {
    os << "Rectangle((" << rect.x << "," << rect.y << "), h=" << rect.height 
       << ", w=" << rect.width << ", area=" << rect.area << ")";
    return os;
}



/*********************************Randomizer.h*********************************/
#ifndef RANDOMIZER_H
#define RANDOMIZER_H

template <typename T>
T* generateRandomArray(int n);

#include "Randomizer.tpp"
#endif // RANDOMIZER_H



/********************************Randomizer.tpp********************************/
#include <random>
#include "Rectangle.h"

template <>
char* generateRandomArray<char>(int n) {
    char* arr = new char[n];
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(33, 126);
    
    for(int i = 0; i < n; i++) {
        arr[i] = static_cast<char>(dis(gen));
    }
    return arr;
}

template <>
int* generateRandomArray<int>(int n) {
    int* arr = new int[n];
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 1000);
    
    for(int i = 0; i < n; i++) {
        arr[i] = dis(gen);
    }
    return arr;
}

template <>
float* generateRandomArray<float>(int n) {
    float* arr = new float[n];
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0.0f, 100.0f);
    
    for(int i = 0; i < n; i++) {
        arr[i] = dis(gen);
    }
    return arr;
}

template <>
Rectangle* generateRandomArray<Rectangle>(int n) {
    Rectangle* arr = new Rectangle[n];
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> coordDist(-100.0f, 100.0f);
    std::uniform_real_distribution<float> sizeDist(1.0f, 50.0f);
    
    for(int i = 0; i < n; i++) {
        arr[i] = Rectangle(coordDist(gen), coordDist(gen), 
                          sizeDist(gen), sizeDist(gen));
    }
    return arr;
}



/********************************SortingAlgo.h*********************************/
#ifndef SORTING_ALGO_H
#define SORTING_ALGO_H

#include <vector>
#include "Timer.h"

template <typename T>
class SortingAlgorithms {
private:
    static int partition(T arr[], int low, int high);
    static void merge(T arr[], int left, int mid, int right);
    static void heapify(T arr[], int n, int i);

public:
    static void insertionSort(T arr[], int n);
    static void quickSort(T arr[], int low, int high);
    static void mergeSort(T arr[], int n);
    static void heapSort(T arr[], int n);
    static void printArray(T arr[], int n);
    static double measureSortingTime(void (*sortingFunction)(T[], int), T arr[], int n);
    static double measureQuickSortTime(T arr[], int n);
};

#include "SortingAlgo.tpp"

#endif // SORTING_ALGO_H



/*******************************SortingAlgo.tpp********************************/
#include <iostream>
#include <algorithm>
#include <vector>

template <typename T>
int SortingAlgorithms<T>::partition(T arr[], int low, int high) {
    T pivot = arr[high];
    int i = low - 1;
    
    for(int j = low; j < high; j++) {
        if(arr[j] < pivot) {
            i++;
            std::swap(arr[i], arr[j]);
        }
    }
    std::swap(arr[i + 1], arr[high]);
    return i + 1;
}

template <typename T>
void SortingAlgorithms<T>::merge(T arr[], int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    
    std::vector<T> L(arr + left, arr + left + n1);
    std::vector<T> R(arr + mid + 1, arr + mid + 1 + n2);
    
    int i = 0, j = 0, k = left;
    
    while (i < n1 && j < n2) {
        if(L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
    
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }
    
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

template <typename T>
void SortingAlgorithms<T>::heapify(T arr[], int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    
    if (left < n && arr[left] > arr[largest])
        largest = left;
        
    if (right < n && arr[right] > arr[largest])
        largest = right;
        
    if (largest != i) {
        std::swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}

template <typename T>
void SortingAlgorithms<T>::insertionSort(T arr[], int n) {
    for (int i = 1; i < n; i++) {
        T key = arr[i];
        int j = i - 1;
        
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

template <typename T>
void SortingAlgorithms<T>::quickSort(T arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

template <typename T>
void SortingAlgorithms<T>::mergeSort(T arr[], int n) {
    for (int curr_size = 1; curr_size <= n-1; curr_size = 2*curr_size) {
        for (int left_start = 0; left_start < n-1; left_start += 2*curr_size) {
            int mid = std::min(left_start + curr_size - 1, n-1);
            int right_end = std::min(left_start + 2*curr_size - 1, n-1);
            merge(arr, left_start, mid, right_end);
        }
    }
}

template <typename T>
void SortingAlgorithms<T>::heapSort(T arr[], int n) {
    for (int i = n/2 - 1; i >= 0; i--)
        heapify(arr, n, i);
        
    for (int i = n-1; i > 0; i--) {
        std::swap(arr[0], arr[i]);
        heapify(arr, i, 0);
    }
}

template <typename T>
void SortingAlgorithms<T>::printArray(T arr[], int n) {
    for (int i = 0; i < n; i++) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;
}

template <typename T>
double SortingAlgorithms<T>::measureSortingTime(void (*sortingFunction)(T[], int), T arr[], int n) {
    // Create a copy of the array to preserve original
    T* arrCopy = new T[n];
    std::copy(arr, arr + n, arrCopy);
    
    Timer timer;
    sortingFunction(arrCopy, n);
    double time = timer.elapsed();
    
    delete[] arrCopy;
    return time;
}

template <typename T>
double SortingAlgorithms<T>::measureQuickSortTime(T arr[], int n) {
    T* arrCopy = new T[n];
    std::copy(arr, arr + n, arrCopy);
    
    Timer timer;
    quickSort(arrCopy, 0, n-1);
    double time = timer.elapsed();
    
    delete[] arrCopy;
    return time;
}



/********************************SortingTest.h*********************************/
#ifndef SORTING_TEST_H
#define SORTING_TEST_H

#include <string>
#include "SortingAlgo.h"
#include "Randomizer.h"

class SortingTest {
private:
    int arraySizes[18];
    static const int numSizes = 18;
    
    template<typename T>
    void runTestForType(const std::string& typeName);

public:
    SortingTest();
    void runAllTests();
};

#include "SortingTest.tpp"

#endif // SORTING_TEST_H



/*******************************SortingTest.tpp********************************/
#ifndef SORTINGTEST_TPP
#define SORTINGTEST_TPP

#include <iostream>
#include <iomanip>
#include <string>
#include "SortingAlgo.h"
#include "Randomizer.h"
#include "Timer.h"

// Constructor implementation
SortingTest::SortingTest() {
    // Initialize arraySizes in the constructor
    int tempSizes[] = {1, 5, 10, 20, 30, 40, 50,
                       100, 200, 300, 400, 500, 1000,
                       2000, 3000, 4000, 5000, 10000};
    for (int i = 0; i < numSizes; ++i) {
        arraySizes[i] = tempSizes[i];
    }
}

// Method to run all tests
void SortingTest::runAllTests() {
    std::cout << "Starting sorting algorithm performance tests...\n";
    std::cout << "Times are in milliseconds\n";

    runTestForType<char>("char");
    runTestForType<int>("int");
    runTestForType<float>("float");
    runTestForType<Rectangle>("Rectangle");
}

// Template method to run a test for a specific type
template <typename T>
void SortingTest::runTestForType(const std::string& typeName) {
    std::cout << "\nTesting sorting algorithms for " << typeName << ":\n";
    std::cout << std::setw(8) << "Size"
              << std::setw(15) << "Insertion"
              << std::setw(15) << "Quick"
              << std::setw(15) << "Merge"
              << std::setw(15) << "Heap" << std::endl;
    std::cout << std::string(68, '-') << std::endl;

    for (int size : arraySizes) {
        // Generate a random array of type T
        T* arr = generateRandomArray<T>(size);

        // Measure time for each sorting algorithm
        double insertionTime = SortingAlgorithms<T>::measureSortingTime(
            SortingAlgorithms<T>::insertionSort, arr, size);
        double quickTime = SortingAlgorithms<T>::measureQuickSortTime(arr, size);
        double mergeTime = SortingAlgorithms<T>::measureSortingTime(
            SortingAlgorithms<T>::mergeSort, arr, size);
        double heapTime = SortingAlgorithms<T>::measureSortingTime(
            SortingAlgorithms<T>::heapSort, arr, size);

        // Print results
        std::cout << std::fixed << std::setprecision(3)
                  << std::setw(8) << size
                  << std::setw(15) << insertionTime
                  << std::setw(15) << quickTime
                  << std::setw(15) << mergeTime
                  << std::setw(15) << heapTime << std::endl;

        // Clean up allocated memory
        delete[] arr;
    }
}

#endif // SORTINGTEST_TPP
