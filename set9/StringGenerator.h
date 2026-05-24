//
// Created by Диана on 24/05/2026.
//

#ifndef STRING_GENERATOR_H
#define STRING_GENERATOR_H

#include <vector>
#include <string>
#include <random>
#include <algorithm>

class StringGenerator {
private:
    std::string symbols;
    std::mt19937 generator;

public:
    StringGenerator() {
        symbols = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        symbols += "abcdefghijklmnopqrstuvwxyz";
        symbols += "0123456789";
        symbols += "!@#%:;^&*()-.";
        std::random_device rd;
        generator = std::mt19937(rd());
    }

    std::string GenerateString() {
        std::uniform_int_distribution<int> lengthDistribution(10, 200);
        std::uniform_int_distribution<int> symbolDistribution(0, (int)symbols.size() - 1);
        int length = lengthDistribution(generator);
        std::string result = "";
        for (int i = 0; i < length; i++) {
            int index = symbolDistribution(generator);
            result += symbols[index];
        }
        return result;
    }

    std::vector<std::string> GenerateRandomArray(int size) {
        std::vector<std::string> array;
        for (int i = 0; i < size; i++) {
            array.push_back(GenerateString());
        }
        return array;
    }

    std::vector<std::string> GenerateReverseSortedArray(int size) {
        std::vector<std::string> array = GenerateRandomArray(size);
        std::sort(array.begin(), array.end());
        std::reverse(array.begin(), array.end());
        return array;
    }

    std::vector<std::string> GenerateAlmostSortedArray(int size) {
        std::vector<std::string> array = GenerateRandomArray(size);
        std::sort(array.begin(), array.end());
        int swapsCount = size / 20;
        if (swapsCount < 1) {
            swapsCount = 1;
        }
        std::uniform_int_distribution<int> indexDistribution(0, size - 1);
        for (int i = 0; i < swapsCount; i++) {
            int firstIndex = indexDistribution(generator);
            int secondIndex = indexDistribution(generator);

            std::string temp = array[firstIndex];
            array[firstIndex] = array[secondIndex];
            array[secondIndex] = temp;
        }

        return array;
    }

    std::vector<std::string> GetSubArray(std::vector<std::string>& array, int size) {
        std::vector<std::string> result;
        for (int i = 0; i < size && i < (int)array.size(); i++) {
            result.push_back(array[i]);
        }
        return result;
    }

    std::vector<std::string> GenerateArrayWithSamePrefix(int size, std::string prefix) {
        std::vector<std::string> array;
        for (int i = 0; i < size; i++) {
            std::string current = prefix + GenerateString();
            array.push_back(current);
        }
        return array;
    }
};

#endif
