#include <algorithm>
#include <cmath>
#include <iostream>
#include <iterator>
#include <vector>

int main() {
    auto v = std::vector<double> { 1., 42., NAN, -3., 7., 3.14159, NAN };

    std::sort(std::begin(v), std::end(v),
      [](double d1, double d2) {
        return std::isnan(d1) || (!std::isnan(d2) && d1 < d2);
      });

    for (const auto d : v) {
        std::cout << d << '\n';
    }
}

