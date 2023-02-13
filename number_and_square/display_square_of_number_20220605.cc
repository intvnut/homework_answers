#include <iostream>
#include <string>
#include <string_view>

namespace {
using std::string;
using std::string_view;

string square(string_view num) {
  string numsq{num};

  if (num.empty()) {
    return numsq;
  }

  auto f = num.begin();
  auto r = num.rbegin();
  auto s = string(num.length() - 1, ' ');

  do {
    numsq += *f++;
    numsq += '\n';
    numsq += *r++;
    numsq += s;
  } while (f != num.end());

  numsq.resize(numsq.length() - s.length());

  r = num.rbegin();
  do {
    numsq += *r++;
  } while (r != num.rend());

  numsq += '\n';
  return numsq;
}

} // namespace

int main() {
  std::string s;

  std::cin >> s;
  std::cout << square(s);
}
