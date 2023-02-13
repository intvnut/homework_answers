#include <iostream>
#include <string>

int main() {
  for (auto i = 10000; i < 12400; i++) {
    if (i % 100 == 60) i += 40;
    auto s = std::to_string(i);
    s[0] = s[1];
    s[1] = s[2];
    s[2] = ':';
    std::cout << s << '\n';
  }
  return 0;
}
