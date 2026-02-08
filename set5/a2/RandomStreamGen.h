//
// Created by Диана on 08/02/2026.
//
#include <string>
#include <vector>
#include <random>
#include <cstddef>

class RandomStreamGen {
public:
  explicit RandomStreamGen(std::uint64_t seed): rng_(seed), lenDist(1, 30),
        charDist(0, static_cast<int>(alphabet.size()) - 1) {}

  std::string next() {
    const int len = lenDist(rng_);
    std::string s;
    s.reserve(static_cast<std::size_t>(len));
    for (int i = 0; i < len; ++i) {
      s.push_back(alphabet[charDist(rng_)]);
    }
    return s;
  }

  std::vector<std::string> generate(std::size_t N) {
    std::vector<std::string> v;
    v.reserve(N);
    for (std::size_t i = 0; i < N; ++i) {
      v.push_back(next());
    }
    return v;
  }

  static std::size_t prefixByPercent(std::size_t N, int percent) {
    return (N * static_cast<std::size_t>(percent)) / 100;
  }

private:
  std::mt19937_64 rng_;
  std::uniform_int_distribution<int> lenDist;

  static inline const std::string alphabet =
      "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
      "abcdefghijklmnopqrstuvwxyz"
      "0123456789-";

  std::uniform_int_distribution<int> charDist;
};