#include "digit_adaptor.hh"
#include <algorithm>
#include <array>
#include <iostream>

template <typename T, int RADIX>
auto count_digits(const T& number) {
  auto counts = std::array<int, RADIX>{};
  for (const auto d : digit_adaptor<T, RADIX>(number)) {
    counts[d]++;
  }
  return counts;
}

template <typename T>
void display_counts(const T& counts) {
  std::cout << "Digit counts in radix " << counts.size() << '\n';
  for (auto digit = 0; digit != counts.size(); ++digit) {
    const char digit_char = digit < 10 ? '0' + digit : 'A' + digit - 10;
    std::cout << digit_char << " : " << counts[digit] << '\n';
  }
}

int main() {
  auto number = 0ull;

  std::cout << "Enter a number: ";
  std::cin  >> number;

  display_counts(count_digits<const unsigned long long, 2>(number));
  display_counts(count_digits<const unsigned long long, 3>(number));
  display_counts(count_digits<const unsigned long long, 4>(number));
  display_counts(count_digits<const unsigned long long, 5>(number));
  display_counts(count_digits<const unsigned long long, 6>(number));
  display_counts(count_digits<const unsigned long long, 7>(number));
  display_counts(count_digits<const unsigned long long, 8>(number));
  display_counts(count_digits<const unsigned long long, 9>(number));
  display_counts(count_digits<const unsigned long long, 10>(number));
  display_counts(count_digits<const unsigned long long, 11>(number));
  display_counts(count_digits<const unsigned long long, 12>(number));
  display_counts(count_digits<const unsigned long long, 13>(number));
  display_counts(count_digits<const unsigned long long, 14>(number));
  display_counts(count_digits<const unsigned long long, 15>(number));
  display_counts(count_digits<const unsigned long long, 16>(number));
}
