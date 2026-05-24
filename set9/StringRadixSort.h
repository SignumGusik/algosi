//
// Created by Диана on 24/05/2026.
//

#ifndef STRING_RADIX_SORT_H
#define STRING_RADIX_SORT_H

#include <vector>
#include <string>

class StringRadixSort {
private:
    static int GetChar(const std::string& s, int index, long long& charComparisons) {
        if (index >= (int)s.size()) {
            return 0;
        }

        charComparisons++;
        return (int)s[index] + 1;
    }

    static void Sort(
        std::vector<std::string>& array,
        std::vector<std::string>& temp,
        int left,
        int right,
        int charIndex,
        long long& charComparisons
    ) {
        if (left >= right) {
            return;
        }

        int alphabetSize = 129;

        std::vector<int> count(alphabetSize + 1, 0);

        for (int i = left; i <= right; i++) {
            int currentChar = GetChar(array[i], charIndex, charComparisons);
            count[currentChar + 1]++;
        }

        for (int i = 0; i < alphabetSize; i++) {
            count[i + 1] += count[i];
        }

        std::vector<int> startPositions = count;

        for (int i = left; i <= right; i++) {
            int currentChar = GetChar(array[i], charIndex, charComparisons);
            int position = count[currentChar];

            temp[left + position] = array[i];
            count[currentChar]++;
        }

        for (int i = left; i <= right; i++) {
            array[i] = temp[i];
        }

        for (int i = 1; i < alphabetSize; i++) {
            int groupLeft = left + startPositions[i];
            int groupRight = left + startPositions[i + 1] - 1;

            if (groupLeft < groupRight) {
                Sort(array, temp, groupLeft, groupRight, charIndex + 1, charComparisons);
            }
        }
    }

public:
    static void Sort(std::vector<std::string>& array, long long& charComparisons) {
        if (array.size() == 0) {
            return;
        }

        std::vector<std::string> temp(array.size());
        Sort(array, temp, 0, (int)array.size() - 1, 0, charComparisons);
    }

    static void Sort(std::vector<std::string>& array) {
        long long charComparisons = 0;
        Sort(array, charComparisons);
    }
};

#endif
