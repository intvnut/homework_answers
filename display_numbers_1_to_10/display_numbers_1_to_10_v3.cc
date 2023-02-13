#include <cmath>
#include <iomanip>
#include <iostream>

// Print all of the float values from 1 to 10, inclusive.
int main() {
  std::cout << std::setprecision(12);

  for (float f = 1.0f; f <= 10.0f; f = std::nextafter(f, 10.1f))
    std::cout << f << '\n';

  return 0;
}
