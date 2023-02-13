#include <algorithm>
#include <array>
#include <atomic>
#include <limits>
#include <numeric>
#include <iostream>
#include <iterator>
#include <string>
#include <type_traits>

template <typename T>
auto find_most_frequent_character(const T &s) {
  using uchar = typename std::make_unsigned<typename T::value_type>::type;
  auto h =
    std::array<std::atomic_uint, std::numeric_limits<uchar>::max() + 1ull> { };
  std::for_each(std::cbegin(s), std::cend(s), [&h](const uchar c) {
    h[c].fetch_add(1, std::memory_order_relaxed);
  });
  const auto m = std::max_element(std::cbegin(h), std::cend(h));
  return static_cast<typename T::value_type>(std::distance(std::cbegin(h), m));
}

int main() {
  using namespace std::literals::string_literals;
  const auto s = "I should really, really do my own homework"s;
  const auto c = find_most_frequent_character(s);
  std::cout << "The most frequent character is: '" << c << "'\n";
}
