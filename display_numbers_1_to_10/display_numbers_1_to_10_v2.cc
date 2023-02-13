#include <cstdint>
#include <cstring>
#include <iomanip>
#include <iostream>

float add_ulps(const float input, const int ulp) {
  static_assert(sizeof(float) == sizeof(std::uint32_t),
                "This code requires float and std::uint32_t "
                "to be the same size\n");

  std::uint32_t tmp;
  float result;

  std::memcpy((void *)&tmp, (const void *)&input, sizeof(float));

  tmp += ulp;

  std::memcpy((void *)&result, (const void *)&tmp, sizeof(float));

  return result;
}

// Print all of the float values from 1 to 10, inclusive.
int main() {
  float f;

  std::cout << std::setprecision(12);

  for (f = 1.0f; f <= 10.0f; f = add_ulps(f, 1))
    std::cout << f << '\n';

  return 0;
}
