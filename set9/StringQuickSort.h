//
// Created by Диана on 24/05/2026.
//

#ifndef STRING_QUICK_SORT_H
#define STRING_QUICK_SORT_H

#include <vector>
#include <string>

class StringQuickSort {
private:
  static int GetChar(const std::string& s, int index, long long& charComparisons) {
    if (index >= (int)s.size()) {
      return -1;
    }

    charComparisons++;
    return (int)s[index];
  }

  static void Swap(std::vector<std::string>& array, int first, int second) {
    std::string temp = array[first];
    array[first] = array[second];
    array[second] = temp;
  }

  static void Sort(
      std::vector<std::string>& array,
      int left,
      int right,
      int charIndex,
      long long& charComparisons
  ) {
    if (left >= right) {
      return;
    }

    int less = left;
    int greater = right;

    int pivot = GetChar(array[left], charIndex, charComparisons);

    int i = left + 1;

    while (i <= greater) {
      int current = GetChar(array[i], charIndex, charComparisons);

      if (current < pivot) {
        Swap(array, less, i);
        less++;
        i++;
      } else if (current > pivot) {
        Swap(array, i, greater);
        greater--;
      } else {
        i++;
      }
    }

    Sort(array, left, less - 1, charIndex, charComparisons);

    if (pivot >= 0) {
      Sort(array, less, greater, charIndex + 1, charComparisons);
    }

    Sort(array, greater + 1, right, charIndex, charComparisons);
  }

public:
  static void Sort(std::vector<std::string>& array, long long& charComparisons) {
    if (array.size() == 0) {
      return;
    }

    Sort(array, 0, (int)array.size() - 1, 0, charComparisons);
  }

  static void Sort(std::vector<std::string>& array) {
    long long charComparisons = 0;
    Sort(array, charComparisons);
  }
};

#endif
