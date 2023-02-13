#include <array>
#include <cstdint>
#include <iostream>
#include <iterator>
#include <random>

int main() {
  auto i = std::array<const char, 231>{
    "https://tinyurl.com/17o929zf\n"
    "How do you write a C++ program to "
    "print the 1st two letters of your "
    "name in linear order using a for "
    "loop with asterisk? And first two "
    "letters of my name is U & N please "
    "write the full program for me.\n"
  };

  auto s =
    std::seed_seq(std::begin(i),
                  std::end(i));
  auto r = std::mt19937_64(s);
  auto a = std::array <const char, 9> {
    47, 4, 2, 3, 61, 44, 28, 20, 4
  };

  for (const auto d : a) {
    r.discard(d);
    auto x = r() % 16;
    while (x) {
      std::cout << (x % 2 ? '*' : ' ');
      x /= 2;
    }
    std::cout << '\n';
  }
}
