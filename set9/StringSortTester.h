#ifndef STRING_SORT_TESTER_H
#define STRING_SORT_TESTER_H

#include <vector>
#include <string>
#include <fstream>
#include <chrono>

#include "StandardQuickSort.h"
#include "StandardMergeSort.h"
#include "StringQuickSort.h"
#include "StringMergeSort.h"
#include "StringRadixSort.h"
#include "StringRadixQuickSort.h"

class StringSortTester {
private:
    int repeatsCount;

    std::vector<std::string> GetSubArray(std::vector<std::string>& array, int size) {
        std::vector<std::string> result;

        for (int i = 0; i < size && i < (int)array.size(); i++) {
            result.push_back(array[i]);
        }

        return result;
    }

    bool IsSorted(const std::vector<std::string>& array) {
        for (int i = 1; i < (int)array.size(); i++) {
            if (array[i] < array[i - 1]) {
                return false;
            }
        }

        return true;
    }

    void SortByName(
        std::vector<std::string>& array,
        const std::string& algorithmName,
        long long& charComparisons
    ) {
        if (algorithmName == "standard_quick_sort") {
            StandardQuickSort::Sort(array, charComparisons);
        } else if (algorithmName == "standard_merge_sort") {
            StandardMergeSort::Sort(array, charComparisons);
        } else if (algorithmName == "string_quick_sort") {
            StringQuickSort::Sort(array, charComparisons);
        } else if (algorithmName == "string_merge_sort") {
            StringMergeSort::Sort(array, charComparisons);
        } else if (algorithmName == "msd_radix_sort") {
            StringRadixSort::Sort(array, charComparisons);
        } else if (algorithmName == "msd_radix_quick_sort") {
            StringRadixQuickSort::Sort(array, charComparisons);
        }
    }

    void TestOneAlgorithm(
        std::ofstream& resultsFile,
        std::vector<std::string>& sourceArray,
        const std::string& typeName,
        const std::string& algorithmName,
        int size
    ) {
        long long totalTime = 0;
        long long totalComparisons = 0;
        bool sortedCorrectly = true;

        for (int repeat = 0; repeat < repeatsCount; repeat++) {
            std::vector<std::string> array = sourceArray;
            long long charComparisons = 0;

            std::chrono::high_resolution_clock::time_point start =
                std::chrono::high_resolution_clock::now();

            SortByName(array, algorithmName, charComparisons);

            std::chrono::high_resolution_clock::time_point finish =
                std::chrono::high_resolution_clock::now();

            long long time =
                std::chrono::duration_cast<std::chrono::microseconds>(finish - start).count();

            totalTime += time;
            totalComparisons += charComparisons;

            if (!IsSorted(array)) {
                sortedCorrectly = false;
            }
        }

        long long averageTime = totalTime / repeatsCount;
        long long averageComparisons = totalComparisons / repeatsCount;

        resultsFile << typeName << ";";
        resultsFile << algorithmName << ";";
        resultsFile << size << ";";
        resultsFile << averageTime << ";";
        resultsFile << averageComparisons << ";";

        if (sortedCorrectly) {
            resultsFile << "yes";
        } else {
            resultsFile << "no";
        }

        resultsFile << "\n";
    }

public:
    StringSortTester() {
        repeatsCount = 5;
    }

    StringSortTester(int repeatsCount) {
        this->repeatsCount = repeatsCount;

        if (this->repeatsCount < 1) {
            this->repeatsCount = 1;
        }
    }

    void RunTests(
        const std::string& resultsFileName,
        std::vector<std::string>& randomArray,
        std::vector<std::string>& reverseArray,
        std::vector<std::string>& almostSortedArray,
        std::vector<std::string>& prefixArray
    ) {
        std::ofstream resultsFile(resultsFileName.c_str());

        resultsFile << "array_type;algorithm;size;average_time_microseconds;";
        resultsFile << "average_char_comparisons;sorted\n";

        std::vector<std::string> algorithmNames;

        algorithmNames.push_back("standard_quick_sort");
        algorithmNames.push_back("standard_merge_sort");
        algorithmNames.push_back("string_quick_sort");
        algorithmNames.push_back("string_merge_sort");
        algorithmNames.push_back("msd_radix_sort");
        algorithmNames.push_back("msd_radix_quick_sort");

        for (int size = 100; size <= 3000; size += 100) {
            std::vector<std::string> randomPart = GetSubArray(randomArray, size);
            std::vector<std::string> reversePart = GetSubArray(reverseArray, size);
            std::vector<std::string> almostPart = GetSubArray(almostSortedArray, size);
            std::vector<std::string> prefixPart = GetSubArray(prefixArray, size);

            for (int i = 0; i < (int)algorithmNames.size(); i++) {
                TestOneAlgorithm(resultsFile, randomPart, "random", algorithmNames[i], size);
                TestOneAlgorithm(resultsFile, reversePart, "reverse_sorted", algorithmNames[i], size);
                TestOneAlgorithm(resultsFile, almostPart, "almost_sorted", algorithmNames[i], size);
                TestOneAlgorithm(resultsFile, prefixPart, "same_prefix", algorithmNames[i], size);
            }
        }

        resultsFile.close();
    }
};

#endif
