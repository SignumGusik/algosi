//
// Created by Диана on 08/02/2026.
//
#include <cstdint>
#include <string>
// FNV
class HashFuncGen {
public:
  explicit HashFuncGen(std::uint32_t seed) : seed_(seed) {}

  std::uint32_t operator()(const std::string& s) const {
    std::uint32_t h = 2166136261u ^ seed_;
    for (unsigned char c : s) {
      h ^= static_cast<std::uint32_t>(c);
      h *= 16777619u;
    }
    h ^= h >> 16;
    h *= 0x7feb352dU;
    h ^= h >> 15;
    h *= 0x846ca68bU;
    h ^= h >> 16;
    return h;
  }

private:
  std::uint32_t seed_;
};

class HashFuncGen64 {
public:
  explicit HashFuncGen64(std::uint64_t seed) : seed_(seed) {}

  std::uint64_t operator()(const std::string& s) const {
    std::uint64_t h = 14695981039346656037ULL ^ seed_;
    const std::uint64_t prime = 1099511628211ULL;
    for (unsigned char c : s) {
      h ^= static_cast<std::uint64_t>(c);
      h *= prime;
    }
    return h;
  }

private:
  std::uint64_t seed_;
};