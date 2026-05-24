#ifndef STANDARD_MERGE_SORT_H
#define STANDARD_MERGE_SORT_H

#include <vector>
#include <string>

class StandardMergeSort {
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

    static void Merge(std::vector<std::string>& array, int left, int middle, int right, long long& charComparisons) {
        std::vector<std::string> leftPart;
        std::vector<std::string> rightPart;
        for (int i = left; i <= middle; i++) {
            leftPart.push_back(array[i]);
        }
        for (int i = middle + 1; i <= right; i++) {
            rightPart.push_back(array[i]);
        }
        int i = 0;
        int j = 0;
        int k = left;

        while (i < (int)leftPart.size() && j < (int)rightPart.size()) {
            if (CompareStrings(leftPart[i], rightPart[j], charComparisons) <= 0) {
                array[k] = leftPart[i];
                i++;
            } else {
                array[k] = rightPart[j];
                j++;
            }
            k++;
        }

        while (i < (int)leftPart.size()) {
            array[k] = leftPart[i];
            i++;
            k++;
        }

        while (j < (int)rightPart.size()) {
            array[k] = rightPart[j];
            j++;
            k++;
        }
    }

    static void Sort(std::vector<std::string>& array, int left, int right, long long& charComparisons) {
        if (left >= right) {
            return;
        }

        int middle = left + (right - left) / 2;
        Sort(array, left, middle, charComparisons);
        Sort(array, middle + 1, right, charComparisons);
        Merge(array, left, middle, right, charComparisons);
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
