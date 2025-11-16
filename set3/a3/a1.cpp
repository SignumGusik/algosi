#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <chrono>
#include <fstream>
#include <string>
#include <map>
#include <cmath>

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
        int swapNum = std::max(1, int(size * 0.01));
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
    if (left >= right) {
      return;
    }

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

void heapify_subarray(std::vector<double>& a, int left, int heapSize, int i) {
    int largest = i;
    while (true) {
        int l = 2 * i + 1;
        int r = 2 * i + 2;
        largest = i;

        if (l < heapSize && a[left + l] > a[left + largest])
            largest = l;
        if (r < heapSize && a[left + r] > a[left + largest])
            largest = r;

        if (largest != i) {
            std::swap(a[left + i], a[left + largest]);
            i = largest;
        } else {
            break;
        }
    }
}

void heapSort(std::vector<double>& a, int left, int right) {
    int n = right - left + 1;
    if (n <= 1) {
      return;
    }

    for (int i = n / 2 - 1; i >= 0; --i) {
        heapify_subarray(a, left, n, i);
    }

    for (int i = n - 1; i > 0; --i) {
        std::swap(a[left], a[left + i]);
        heapify_subarray(a, left, i, 0);
    }
}

int partitionRandom(std::vector<double>& a, int left, int right, std::mt19937& gen) {
    std::uniform_int_distribution<int> dist(left, right);
    int pivotIndex = dist(gen);
    double pivot = a[pivotIndex];
    std::swap(a[pivotIndex], a[right]);

    int i = left - 1;
    for (int j = left; j < right; ++j) {
        if (a[j] <= pivot) {
            ++i;
            std::swap(a[i], a[j]);
        }
    }
    std::swap(a[i + 1], a[right]);
    return i + 1;
}

void quickSortRecursive(std::vector<double>& a, int left, int right, std::mt19937& gen) {
    if (left >= right) return;
    int p = partitionRandom(a, left, right, gen);
    quickSortRecursive(a, left, p - 1, gen);
    quickSortRecursive(a, p + 1, right, gen);
}

void introSortRecursive(std::vector<double>& a,int left, int right, int depthLimit,
                        std::mt19937& gen) {
    int n = right - left + 1;
    if (n <= 1) {
      return;
    }

    if (n < 16) {
        insertionSort(a, left, right);
        return;
    }

    if (depthLimit == 0) {
        heapSort(a, left, right);
        return;
    }


    int p = partitionRandom(a, left, right, gen);
    introSortRecursive(a, left, p - 1, depthLimit - 1, gen);
    introSortRecursive(a, p + 1, right, depthLimit - 1, gen);
}

void introSort(std::vector<double>& a) {
    int n = (int)a.size();
    if (n <= 1) {
      return;
    }
    int depthLimit = 2 * (int)std::log2(std::max(1, n));
    static std::mt19937 gen(123456);
    introSortRecursive(a, 0, n - 1, depthLimit, gen);
}

class SortTester {
public:
    static double testQuickSort(std::vector<double> arr) {
        if (arr.empty()) return 0.0;
        static std::mt19937 gen(987654321);
        auto start = std::chrono::high_resolution_clock::now();
        quickSortRecursive(arr, 0, (int)arr.size() - 1, gen);
        auto end = std::chrono::high_resolution_clock::now();
        return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    }

    static double testHybridQuickSort(std::vector<double> arr) {
        auto start = std::chrono::high_resolution_clock::now();
        introSort(arr);
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

    std::vector<double> bigRandomArray  = testGenerator.generateRandomArray(MAX_SIZE);
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

            double totalQuick = 0.0;
            for (int j = 0; j < REPEATS; j++) {
                std::vector<double> arrCopy = testArray;
                totalQuick += SortTester::testQuickSort(arrCopy);
            }
            double averageQuick = totalQuick / REPEATS;
            csvData.push_back({
                std::to_string(size),
                dataType,
                "QuickSort",
                std::to_string(averageQuick)
            });

            double totalHybrid = 0.0;
            for (int j = 0; j < REPEATS; j++) {
                std::vector<double> arrCopy = testArray;
                totalHybrid += SortTester::testHybridQuickSort(arrCopy);
            }
            double averageHybrid = totalHybrid / REPEATS;
            csvData.push_back({
                std::to_string(size),
                dataType,
                "HybridQuickSort",
                std::to_string(averageHybrid)
            });
        }
    }

    writeToCSV("quick_sorting_results.csv", csvData);
    std::cout << "Results saved to quick_sorting_results.csv\n";

    return 0;
}