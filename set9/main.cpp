#include <fstream>
#include <vector>
#include <string>

#include "StringGenerator.h"
#include "StringSortTester.h"

void SaveArrayToFile(const std::string& fileName, std::vector<std::string>& array) {
  std::ofstream file(fileName.c_str());

  file << array.size() << "\n";

  for (int i = 0; i < (int)array.size(); i++) {
    file << array[i] << "\n";
  }

  file.close();
}

int main() {
  StringGenerator generator;

  std::vector<std::string> randomArray = generator.GenerateRandomArray(3000);
  std::vector<std::string> reverseArray = generator.GenerateReverseSortedArray(3000);
  std::vector<std::string> almostSortedArray = generator.GenerateAlmostSortedArray(3000);
  std::vector<std::string> prefixArray =
      generator.GenerateArrayWithSamePrefix(3000, "PREFIX_PREFIX_");

  SaveArrayToFile("source_random.txt", randomArray);
  SaveArrayToFile("source_reverse_sorted.txt", reverseArray);
  SaveArrayToFile("source_almost_sorted.txt", almostSortedArray);
  SaveArrayToFile("source_same_prefix.txt", prefixArray);

  StringSortTester tester(5);

  tester.RunTests(
      "results.csv",
      randomArray,
      reverseArray,
      almostSortedArray,
      prefixArray
  );

  return 0;
}
