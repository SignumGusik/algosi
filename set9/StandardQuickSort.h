#ifndef STANDARD_QUICK_SORT_H
#define STANDARD_QUICK_SORT_H

#include <vector>
#include <string>

class StandardQuickSort {
private:
    static int CompareStrings(const std::string& first, const std::string& second, long long& charComparisons) {
        int i = 0;
        while (i < (int)first.size() && i < (int)second.size()) {
            charComparisons++;
            if (first[i] < second[i]) {
                return -1;
            }
            if (first[i] > second[i]) {
                return 1;
            }
            i++;
        }

        if (first.size() < second.size()) {
            return -1;
        }
        if (first.size() > second.size()) {
            return 1;
        }
        return 0;
    }

    static void Swap(std::vector<std::string>& array, int first, int second) {
        std::string temp = array[first];
        array[first] = array[second];
        array[second] = temp;
    }

    static void Sort(std::vector<std::string>& array, int left, int right, long long& charComparisons) {
        if (left >= right) {
            return;
        }

        int i = left;
        int j = right;
        std::string pivot = array[left];
        while (i <= j) {
            while (CompareStrings(array[i], pivot, charComparisons) < 0) {
                i++;
            }
            while (CompareStrings(array[j], pivot, charComparisons) > 0) {
                j--;
            }
            if (i <= j) {
                Swap(array, i, j);
                i++;
                j--;
            }
        }

        if (left < j) {
            Sort(array, left, j, charComparisons);
        }
        if (i < right) {
            Sort(array, i, right, charComparisons);
        }
    }

public:
    static void Sort(std::vector<std::string>& array, long long& charComparisons) {
        if (array.size() == 0) {
            return;
        }
        Sort(array, 0, (int)array.size() - 1, charComparisons);
    }
};

#endif
