#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cmath>
#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <unordered_set>
#include <vector>

#include "HashFuncGen.h"
#include "RandomStreamGen.h"

class HyperLogLog {
public:
  explicit HyperLogLog(std::uint32_t hashSeed, int indexBitCount = 12): indexBitCount_(indexBitCount),
        registerCount_(static_cast<std::uint32_t>(1u << indexBitCount_)),
        registers_(registerCount_, 0),
        hasher_(hashSeed),
        alpha_(ComputeAlpha(registerCount_)) {}

  void Add(const std::string& value) {
    const std::uint32_t hashValue = hasher_(value);

    const std::uint32_t registerIndex = hashValue & (registerCount_ - 1u);

    const std::uint32_t remaining = hashValue >> indexBitCount_;
    const std::uint8_t rhoValue = static_cast<std::uint8_t>(
        CountLeadingZerosPlusOne(remaining, 32 - indexBitCount_));

    registers_[registerIndex] = std::max(registers_[registerIndex], rhoValue);
  }

  double Estimate() const {
    double harmonicSum = 0.0;
    for (std::uint8_t reg : registers_) {
      harmonicSum += std::ldexp(1.0, -static_cast<int>(reg));
    }

    const double m = static_cast<double>(registerCount_);
    double estimate = alpha_ * m * m / harmonicSum;

    if (estimate <= 2.5 * m) {
      const std::size_t zeroRegisters = static_cast<std::size_t>(
          std::count(registers_.begin(), registers_.end(), 0u));
      if (zeroRegisters > 0) {
        estimate = m * std::log(m / static_cast<double>(zeroRegisters));
      }
    }

    return estimate;
  }

private:
  int indexBitCount_;
  std::uint32_t registerCount_;
  std::vector<std::uint8_t> registers_;
  HashFuncGen hasher_;
  double alpha_;

  static double ComputeAlpha(std::uint32_t m) {
    if (m == 16u) return 0.673;
    if (m == 32u) return 0.697;
    if (m == 64u) return 0.709;
    return 0.7213 / (1.0 + 1.079 / static_cast<double>(m));
  }

  static int CountLeadingZerosPlusOne(std::uint32_t x, int maxBits) {
    if (maxBits <= 0) return 1;
    if (x == 0u) return maxBits + 1;

    uint32_t w = x << (32 - maxBits);
    int leadingZeros = __builtin_clz(w);
    int rho = leadingZeros + 1;
    const int maxRho = maxBits + 1;
    if (rho > maxRho) rho = maxRho;
    return rho;
  }
};



class HyperLogLogUltraMax {
public:
  explicit HyperLogLogUltraMax(std::uint64_t hashSeed, int indexBitCount = 12)
      : indexBitCount_(indexBitCount),
        registerCount_(static_cast<std::uint32_t>(1u << indexBitCount_)),
        registers_(registerCount_, 0),
        hasher_(hashSeed),
        alpha_(ComputeAlpha(registerCount_)) {}

  void Add(const std::string& value) {
    const std::uint64_t hashValue = hasher_(value);

    const std::uint32_t registerIndex = static_cast<std::uint32_t>(hashValue & (registerCount_ - 1u));

    const std::uint64_t remaining = hashValue >> indexBitCount_;
    const std::uint8_t rhoValue = static_cast<std::uint8_t>(
        CountLeadingZerosPlusOne(remaining, 64 - indexBitCount_));

    registers_[registerIndex] = std::max(registers_[registerIndex], rhoValue);
  }

  double Estimate() const {
    double harmonicSum = 0.0;
    for (std::uint8_t reg : registers_) {
      harmonicSum += std::ldexp(1.0, -static_cast<int>(reg));
    }

    const double m = static_cast<double>(registerCount_);
    double estimate = alpha_ * m * m / harmonicSum;

    if (estimate <= 2.5 * m) {
      const std::size_t zeroRegisters = static_cast<std::size_t>(
          std::count(registers_.begin(), registers_.end(), 0u));
      if (zeroRegisters > 0) {
        estimate = m * std::log(m / static_cast<double>(zeroRegisters));
      }
    } else if (estimate < 5 * m) {
      estimate -= EstimateBias(estimate, m);
    }
    const double two64 = std::pow(2.0, 64.0);
    if (estimate > two64 / 30.0) {
      estimate = -two64 * std::log(1.0 - estimate / two64);
    }

    return estimate;
  }

private:
  int indexBitCount_;
  std::uint32_t registerCount_;
  std::vector<std::uint8_t> registers_;
  HashFuncGen64 hasher_;
  double alpha_;

  static double ComputeAlpha(std::uint32_t m) {
    if (m == 16u) return 0.673;
    if (m == 32u) return 0.697;
    if (m == 64u) return 0.709;
    return 0.7213 / (1.0 + 1.079 / static_cast<double>(m));
  }

  static int CountLeadingZerosPlusOne(std::uint64_t x, int maxBits) {
    if (maxBits <= 0) return 1;
    if (x == 0u) return maxBits + 1;

    uint64_t w = x << (64 - maxBits);
    int leadingZeros = __builtin_clzll(w);
    int rho = leadingZeros + 1;
    const int maxRho = maxBits + 1;
    if (rho > maxRho) rho = maxRho;
    return rho;
  }

  static double EstimateBias(double estimate, double m) {
    static const double rawEstimateData12[200] = {2954, 3003.4782, 3053.3568, 3104.3666, 3155.324, 3206.9598, 3259.648, 3312.539, 3366.1474, 3420.2576, 3474.8376, 3530.6076, 3586.451, 3643.38, 3700.4104, 3757.5638, 3815.9676, 3875.193, 3934.838, 3994.8548, 4055.018, 4117.1742, 4178.4482, 4241.1294, 4304.4776, 4367.4044, 4431.8724, 4496.3732, 4561.4304, 4627.5326, 4693.949, 4761.5532, 4828.7256, 4897.6182, 4965.5186, 5034.4528, 5104.865, 5174.7164, 5244.6828, 5316.6708, 5387.8312, 5459.9036, 5532.476, 5604.8652, 5679.6718, 5753.757, 5830.2072, 5905.2828, 5980.0434, 6056.6264, 6134.3192, 6211.5746, 6290.0816, 6367.1176, 6447.9796, 6526.5576, 6606.1858, 6686.9144, 6766.1142, 6847.0818, 6927.9664, 7010.9096, 7091.0816, 7175.3962, 7260.3454, 7344.018, 7426.4214, 7511.3106, 7596.0686, 7679.8094, 7765.818, 7852.4248, 7936.834, 8022.363, 8109.5066, 8200.4554, 8288.5832, 8373.366, 8463.4808, 8549.7682, 8642.0522, 8728.3288, 8820.9528, 8907.727, 9001.0794, 9091.2522, 9179.988, 9269.852, 9362.6394, 9453.642, 9546.9024, 9640.6616, 9732.6622, 9824.3254, 9917.7484, 10007.9392, 10106.7508, 10196.2152, 10289.8114, 10383.5494, 10482.3064, 10576.8734, 10668.7872, 10764.7156, 10862.0196, 10952.793, 11049.9748, 11146.0702, 11241.4492, 11339.2772, 11434.2336, 11530.741, 11627.6136, 11726.311, 11821.5964, 11918.837, 12015.3724, 12113.0162, 12213.0424, 12306.9804, 12408.4518, 12504.8968, 12604.586, 12700.9332, 12798.705, 12898.5142, 12997.0488, 13094.788, 13198.475, 13292.7764, 13392.9698, 13486.8574, 13590.1616, 13686.5838, 13783.6264, 13887.2638, 13992.0978, 14081.0844, 14189.9956, 14280.0912, 14382.4956, 14486.4384, 14588.1082, 14686.2392, 14782.276, 14888.0284, 14985.1864, 15088.8596, 15187.0998, 15285.027, 15383.6694, 15495.8266, 15591.3736, 15694.2008, 15790.3246, 15898.4116, 15997.4522, 16095.5014, 16198.8514, 16291.7492, 16402.6424, 16499.1266, 16606.2436, 16697.7186, 16796.3946, 16902.3376, 17005.7672, 17100.814, 17206.8282, 17305.8262, 17416.0744, 17508.4092, 17617.0178, 17715.4554, 17816.758, 17920.1748, 18012.9236, 18119.7984, 18223.2248, 18324.2482, 18426.6276, 18525.0932, 18629.8976, 18733.2588, 18831.0466, 18940.1366, 19032.2696, 19131.729, 19243.4864, 19349.6932, 19442.866, 19547.9448, 19653.2798, 19754.4034, 19854.0692, 19965.1224, 20065.1774, 20158.2212, 20253.353, 20366.3264};

    static const double biasData12[200] = {2953, 2900.4782, 2848.3568, 2796.3666, 2745.324, 2694.9598, 2644.648, 2595.539, 2546.1474, 2498.2576, 2450.8376, 2403.6076, 2357.451, 2311.38, 2266.4104, 2221.5638, 2176.9676, 2134.193, 2090.838, 2048.8548, 2007.018, 1966.1742, 1925.4482, 1885.1294, 1846.4776, 1807.4044, 1768.8724, 1731.3732, 1693.4304, 1657.5326, 1621.949, 1586.5532, 1551.7256, 1517.6182, 1483.5186, 1450.4528, 1417.865, 1385.7164, 1352.6828, 1322.6708, 1291.8312, 1260.9036, 1231.476, 1201.8652, 1173.6718, 1145.757, 1119.2072, 1092.2828, 1065.0434, 1038.6264, 1014.3192, 988.5746, 965.0816, 940.1176, 917.9796, 894.5576, 871.1858, 849.9144, 827.1142, 805.0818, 783.9664, 763.9096, 742.0816, 724.3962, 706.3454, 688.018, 667.4214, 650.3106, 633.0686, 613.8094, 597.818, 581.4248, 563.834, 547.363, 531.5066, 520.455400000001, 505.583199999999, 488.366, 476.480799999999, 459.7682, 450.0522, 434.328799999999, 423.952799999999, 408.727000000001, 399.079400000001, 387.252200000001, 373.987999999999, 360.852000000001, 351.6394, 339.642, 330.902400000001, 322.661599999999, 311.662200000001, 301.3254, 291.7484, 279.939200000001, 276.7508, 263.215200000001, 254.811400000001, 245.5494, 242.306399999999, 234.8734, 223.787200000001, 217.7156, 212.0196, 200.793, 195.9748, 189.0702, 182.449199999999, 177.2772, 170.2336, 164.741, 158.613600000001, 155.311, 147.5964, 142.837, 137.3724, 132.0162, 130.0424, 121.9804, 120.451800000001, 114.8968, 111.585999999999, 105.933199999999, 101.705, 98.5141999999996, 95.0488000000005, 89.7880000000005, 91.4750000000004, 83.7764000000006, 80.9698000000008, 72.8574000000008, 73.1615999999995, 67.5838000000003, 62.6263999999992, 63.2638000000006, 66.0977999999996, 52.0843999999997, 58.9956000000002, 47.0912000000008, 46.4956000000002, 48.4383999999991, 47.1082000000006, 43.2392, 37.2759999999998, 40.0283999999992, 35.1864000000005, 35.8595999999998, 32.0998, 28.027, 23.6694000000007, 33.8266000000003, 26.3736000000008, 27.2008000000005, 21.3245999999999, 26.4115999999995, 23.4521999999997, 19.5013999999992, 19.8513999999996, 10.7492000000002, 18.6424000000006, 13.1265999999996, 18.2436000000016, 6.71860000000015, 3.39459999999963, 6.33759999999893, 7.76719999999841, 0.813999999998487, 3.82819999999992, 0.826199999999517, 8.07440000000133, -1.59080000000176, 5.01780000000144, 0.455399999998917, -0.24199999999837, 0.174800000000687, -9.07640000000174, -4.20160000000033, -3.77520000000004, -4.75179999999818, -5.3724000000002, -8.90680000000066, -6.10239999999976, -5.74120000000039, -9.95339999999851, -3.86339999999836, -13.7304000000004, -16.2710000000006, -7.51359999999841, -3.30679999999847, -13.1339999999982, -10.0551999999989, -6.72019999999975, -8.59660000000076, -10.9307999999983, -1.8775999999998, -4.82259999999951, -13.7788, -21.6470000000008, -10.6735999999983};

    const double* it = std::lower_bound(rawEstimateData12, rawEstimateData12 + 200, estimate);
    int idx = static_cast<int>(it - rawEstimateData12);
    int k = 6;
    int left = std::max(0, idx - k/2);
    int right = std::min(200, left + k);
    if (right - left < k) {
      left = std::max(0, right - k);
    }

    double sum_bias = 0.0;
    int count = right - left;
    for (int i = left; i < right; ++i) {
      sum_bias += biasData12[i];
    }

    return sum_bias / static_cast<double>(count);
  }
};

static double ComputeMean(const std::vector<double>& values) {
  if (values.empty()) return 0.0;
  const double sum = std::accumulate(values.begin(), values.end(), 0.0);
  return sum / static_cast<double>(values.size());
}

static double ComputePopulationStdDev(const std::vector<double>& values, double mean) {
  if (values.empty()) return 0.0;
  double squaredSum = 0.0;
  for (double x : values) {
    const double diff = x - mean;
    squaredSum += diff * diff;
  }
  const double variance = squaredSum / static_cast<double>(values.size());
  return std::sqrt(variance);
}

int main() {
  const std::uint64_t baseSeed = 42ULL;
  const int streamCount = 10;

  const std::size_t streamLength = 1000000;
  const std::size_t checkpointStep = 10000;
  const std::size_t checkpointCount = streamLength / checkpointStep;

  const int indexBitCount = 12;

  std::vector<std::vector<double>> allEstimates(checkpointCount + 1);
  std::vector<std::size_t> exampleExact(checkpointCount + 1, 0);
  std::vector<double> exampleEstimate(checkpointCount + 1, 0.0);

  std::vector<std::vector<double>> allEstimatesUltra(checkpointCount + 1);
  std::vector<std::size_t> exampleExactUltra(checkpointCount + 1, 0);
  std::vector<double> exampleEstimateUltra(checkpointCount + 1, 0.0);

  std::uint64_t hashSeed = 123ULL;
  for (int stream = 0; stream < streamCount; ++stream) {
    RandomStreamGen gen(baseSeed + static_cast<std::uint64_t>(stream));
    std::vector<std::string> streamData = gen.generate(streamLength);
    std::unordered_set<std::string> exactSet;
    HyperLogLog hll(static_cast<std::uint32_t>(hashSeed), indexBitCount);

    std::size_t currentExact = 0;
    for (std::size_t i = 0; i < streamLength; ++i) {
      const std::string& value = streamData[i];
      if (exactSet.insert(value).second) {
        ++currentExact;
      }
      hll.Add(value);

      if ((i + 1) % checkpointStep == 0) {
        std::size_t c = (i + 1) / checkpointStep;
        double est = hll.Estimate();
        allEstimates[c].push_back(est);
        if (stream == 0) {
          exampleExact[c] = currentExact;
          exampleEstimate[c] = est;
        }
      }
    }
  }
  for (int stream = 0; stream < streamCount; ++stream) {
    RandomStreamGen gen(baseSeed + static_cast<std::uint64_t>(stream));
    std::vector<std::string> streamData = gen.generate(streamLength);
    std::unordered_set<std::string> exactSet;
    HyperLogLogUltraMax hllUltra(hashSeed, indexBitCount);

    std::size_t currentExact = 0;
    for (std::size_t i = 0; i < streamLength; ++i) {
      const std::string& value = streamData[i];
      if (exactSet.insert(value).second) {
        ++currentExact;
      }
      hllUltra.Add(value);

      if ((i + 1) % checkpointStep == 0) {
        std::size_t c = (i + 1) / checkpointStep;
        double est = hllUltra.Estimate();
        allEstimatesUltra[c].push_back(est);
        if (stream == 0) {
          exampleExactUltra[c] = currentExact;
          exampleEstimateUltra[c] = est;
        }
      }
    }
  }

  {
    std::ofstream out("graph1.csv");
    out << "t,Ft0,Nt\n";
    out << "0,0,0\n";
    for (std::size_t c = 1; c <= checkpointCount; ++c) {
      const std::size_t t = c * checkpointStep;
      out << t << "," << exampleExact[c] << "," << exampleEstimate[c] << "\n";
    }
  }

  {
    std::ofstream out("graph2.csv");
    out << "t,E_Nt,sigma_Nt\n";
    out << "0,0,0\n";
    for (std::size_t c = 1; c <= checkpointCount; ++c) {
      const std::size_t t = c * checkpointStep;
      const std::vector<double>& estimates = allEstimates[c];
      const double mean = ComputeMean(estimates);
      const double sigma = ComputePopulationStdDev(estimates, mean);
      out << t << "," << mean << "," << sigma << "\n";
    }
  }

  {
    std::ofstream out("graph3.csv");
    out << "t,Ft0,Nt\n";
    out << "0,0,0\n";
    for (std::size_t c = 1; c <= checkpointCount; ++c) {
      const std::size_t t = c * checkpointStep;
      out << t << "," << exampleExactUltra[c] << "," << exampleEstimateUltra[c] << "\n";
    }
  }

  {
    std::ofstream out("graph4.csv");
    out << "t,E_Nt,sigma_Nt\n";
    out << "0,0,0\n";
    for (std::size_t c = 1; c <= checkpointCount; ++c) {
      const std::size_t t = c * checkpointStep;
      const std::vector<double>& estimates = allEstimatesUltra[c];
      const double mean = ComputeMean(estimates);
      const double sigma = ComputePopulationStdDev(estimates, mean);
      out << t << "," << mean << "," << sigma << "\n";
    }
  }

  std::cout << "CSV files generated:\n";
  return 0;
}