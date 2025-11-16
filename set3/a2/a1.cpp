#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <chrono>
#include <fstream>
#include <string>
#include <map>

class ArrayGenerator {
public:
    std::random_device rd;
    std::mt19937 gen;

    ArrayGenerator() : gen(rd()) {}

    std::vector<double> generateRandomArray(int size) {
        std::vector<double> arr(size);
        std::uniform_real_distribution<> dist(0, 6000);
        for (int i = 0; i < size; i++) {
            arr[i] = dist(gen);
        }
        return arr;
    }

    std::vector<double> generateReversedSortedArray(int size) {
        std::vector<double> arr(size);
        double value = 6000;
        for (int i = 0; i < size; i++) {
            arr[i] = value;
            value -= 6000.0 / size;
        }
        return arr;
    }

    std::vector<double> generateAlmostSortedArray(int size) {
        std::vector<double> arr(size);
        for (int i = 0; i < size; i++) {
            arr[i] = i;
        }
        int swapNum = size * 0.01;
        std::uniform_int_distribution<int> dist(0, size - 1);
        for (int i = 0; i < swapNum; i++) {
            int j = dist(gen);
            int k = dist(gen);
            std::swap(arr[j], arr[k]);
        }
        return arr;
    }
};

void merge(std::vector<double>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    std::vector<double> L(n1), R(n2);

    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
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

void mergeSort(std::vector<double>& arr, int left, int right) {
    if (left >= right) return;

    int mid = left + (right - left) / 2;
    mergeSort(arr, left, mid);
    mergeSort(arr, mid + 1, right);
    merge(arr, left, mid, right);
}

void insertionSort(std::vector<double>& arr, int left, int right) {
    for (int i = left + 1; i <= right; i++) {
        double key = arr[i];
        int j = i - 1;
        while (j >= left && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

void hybridMergeSort(std::vector<double>& arr, int left, int right, int threshold) {
    if (left >= right) return;

    if (right - left + 1 <= threshold) {
        insertionSort(arr, left, right);
        return;
    }

    int mid = left + (right - left) / 2;
    hybridMergeSort(arr, left, mid, threshold);
    hybridMergeSort(arr, mid + 1, right, threshold);
    merge(arr, left, mid, right);
}

class SortTester {
public:
    static double testMergeSort(std::vector<double> arr) {
        auto start = std::chrono::high_resolution_clock::now();
        mergeSort(arr, 0, arr.size() - 1);
        auto end = std::chrono::high_resolution_clock::now();
        return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    }

    static double testHybridMergeSort(std::vector<double> arr, int threshold) {
        auto start = std::chrono::high_resolution_clock::now();
        hybridMergeSort(arr, 0, arr.size() - 1, threshold);
        auto end = std::chrono::high_resolution_clock::now();
        return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    }
};

void writeToCSV(const std::string& filename, const std::vector<std::vector<std::string>>& data) {
    std::ofstream file(filename);
    for (const auto& row : data) {
        for (size_t i = 0; i < row.size(); ++i) {
            file << row[i];
            if (i != row.size() - 1) file << ",";
        }
        file << "\n";
    }
    file.close();
}


int main() {
    ArrayGenerator testGenerator;
    const int REPEATS = 5;
    const int MAX_SIZE = 100000;
    int optimalThreshold = 50;

    std::vector<double> bigRandomArray = testGenerator.generateRandomArray(MAX_SIZE);
    std::vector<double> bigReversedArray = testGenerator.generateReversedSortedArray(MAX_SIZE);
    std::vector<double> bigAlmostSortedArray = testGenerator.generateAlmostSortedArray(MAX_SIZE);

    std::vector<std::vector<std::string>> csvData;
    csvData.push_back({"Size", "DataType", "Algorithm", "TimeMicroseconds"});

    for (int size = 500; size <= MAX_SIZE; size += 100) {
        std::cout << "Testing size: " << size << std::endl;
        std::vector<double> randomSlice(bigRandomArray.begin(), bigRandomArray.begin() + size);
        std::vector<double> reversedSlice(bigReversedArray.begin(), bigReversedArray.begin() + size);
        std::vector<double> almostSortedSlice(bigAlmostSortedArray.begin(), bigAlmostSortedArray.begin() + size);
        std::vector<std::pair<std::string, std::vector<double>>> testCases = {
            {"Random", randomSlice},
            {"Reversed", reversedSlice},
            {"AlmostSorted", almostSortedSlice}
        };

        for (const auto& testCase : testCases) {
            const std::string& dataType = testCase.first;
            const std::vector<double>& testArray = testCase.second;

            double totalTime = 0.0;
            for (int j = 0; j < REPEATS; j++) {
                std::vector<double> arrCopy = testArray;
                totalTime += SortTester::testMergeSort(arrCopy);
            }
            double averageTime = totalTime / REPEATS;
            csvData.push_back({
                std::to_string(size),
                dataType,
                "MergeSort",
                std::to_string(averageTime)
            });

            double totalHybridTime = 0.0;
            for (int j = 0; j < REPEATS; j++) {
                std::vector<double> arrCopy = testArray;
                totalHybridTime += SortTester::testHybridMergeSort(arrCopy, optimalThreshold);
            }
            double averageHybridTime = totalHybridTime / REPEATS;
            csvData.push_back({
                std::to_string(size),
                dataType,
                "HybridMergeSort",
                std::to_string(averageHybridTime)
            });
        }
    }
    writeToCSV("sorting_results.csv", csvData);
    std::cout << "Results saved to sorting_results.csv" << std::endl;
    std::cout << "Total records: " << csvData.size() - 1 << std::endl;
    std::cout << "Optimal threshold used: " << optimalThreshold << std::endl;

    return 0;
}