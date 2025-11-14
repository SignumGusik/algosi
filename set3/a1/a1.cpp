#import <iostream>
#import <vector>
#import <random>
#import <algorithm>
#import <limits>
#import <iomanip>

class Circle {
  double x = 0;
  double y = 0;
  double radius = 0;
public:
  Circle(double x, double y, double r) : x(x), y(y), radius(r) {}
  bool contains(double px, double py) const {
    const double dx = px - x;
    const double dy = py - y;
    return dx * dx + dy * dy <= radius * radius;
  }
  double left() const {
    return x - radius;
  }
  double right() const {
    return x + radius;
  }
  double bottom() const {
    return y - radius;
  }
  double top() const {
    return y + radius;
  }

};

double MonteCarloWide(const int n, const std::vector<Circle>& circles) {
  double xmin =  std::numeric_limits<double>::infinity();
  double xmax = -std::numeric_limits<double>::infinity();
  double ymin =  std::numeric_limits<double>::infinity();
  double ymax = -std::numeric_limits<double>::infinity();

  for (const auto& circle : circles) {
    xmin = std::min(xmin, circle.left());
    xmax = std::max(xmax, circle.right());
    ymin = std::min(ymin, circle.bottom());
    ymax = std::max(ymax, circle.top());
  }

  const double s = (xmax - xmin) * (ymax - ymin);

  std::mt19937_64 gen(std::random_device{}());
  std::uniform_real_distribution<double> dx(xmin, xmax);
  std::uniform_real_distribution<double> dy(ymin, ymax);

  int inside = 0;
  for (int i = 0; i < n; ++i) {
    const double x = dx(gen);
    const double y = dy(gen);

    bool ok = true;
    for (const auto& c : circles) {
      if (!c.contains(x, y)) {
        ok = false;
        break;
      }
    }
    if (ok) {
      ++inside;
    }
  }
  return s * static_cast<double>(inside) / n;
}

double MonteCarloNarrow(const int n, const std::vector<Circle>& circles) {
  double xmin = -std::numeric_limits<double>::infinity();
  double xmax =  std::numeric_limits<double>::infinity();
  double ymin = -std::numeric_limits<double>::infinity();
  double ymax =  std::numeric_limits<double>::infinity();

  for (const auto& circle : circles) {
    xmin = std::max(xmin, circle.left());
    xmax = std::min(xmax, circle.right());
    ymin = std::max(ymin, circle.bottom());
    ymax = std::min(ymax, circle.top());
  }

  if (xmin >= xmax || ymin >= ymax) {
    return 0.0;
  }

  const double s = (xmax - xmin) * (ymax - ymin);

  std::mt19937_64 gen(std::random_device{}());
  std::uniform_real_distribution<double> dx(xmin, xmax);
  std::uniform_real_distribution<double> dy(ymin, ymax);

  int inside = 0;
  for (int i = 0; i < n; ++i) {
    const double x = dx(gen);
    const double y = dy(gen);

    bool ok = true;
    for (const auto& c : circles) {
      if (!c.contains(x, y)) {
        ok = false;
        break;
      }
    }
    if (ok) {
      ++inside;
    }
  }
  return s * static_cast<double>(inside) / n;
}


int main() {
  std::vector<Circle> circles;
  double x = 0;
  double y = 0;
  double radius = 0;
  int n = 2000000;
  for (int i = 0; i < 3; ++i) {
    std::cin >> x >> y >> radius;
    circles.push_back(Circle(x, y, radius));
  }
  const double S2 = MonteCarloNarrow(n, circles);
  std::cout.setf(std::ios::fixed);
  std::cout << std::setprecision(20) << S2 << '\n';
}
/*
int main() {
  std::vector<Circle> circles = {
    {1.0, 1.0, 1.0},
    {1.5, 2.0, sqrt(5.0) / 2.0},
    {2.0, 1.5, sqrt(5.0) / 2.0}
  };
  constexpr double PI = 3.14159265358979323846;
  const double S_exact =
      0.25 * PI + 1.25 * std::asin(0.8) - 1.0;

  std::cout << "N " << "S_wide " << "S_narrow " << "err_wide" << "err_narrow" << "\n";

  for (int n = 100; n <= 100000; n += 500) {
    const double S1 = MonteCarloWide(n, circles);
    const double S2 = MonteCarloNarrow(n, circles);
    const double rel1 = std::fabs(S1 - S_exact) / S_exact;
    const double rel2 = std::fabs(S2 - S_exact) / S_exact;
    std::cout << n << " " << S1 << " " << S2 << " " << rel1 << " " << rel2 << "\n";
  }

}
*/
