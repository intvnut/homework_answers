#include <iostream>
#include <iterator>
#include <set>

int main() {
  auto u = std::set<double>(std::istream_iterator<double>(std::cin), {});

  for (const auto k : u)
    std::cout << k << ' ';

  std::cout << '\n';
}
