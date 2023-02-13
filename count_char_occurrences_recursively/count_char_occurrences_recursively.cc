#include <iostream>
#include <iterator>
#include <string_view>

// Returns a recursive function for counting occurrences of a character in a
// string.  Pass in the character to search for.
auto make_counter(char ch) {
  using std::begin, std::end;
  using sv = std::string_view;

  auto car = [](sv s) { return s[0]; };
  auto cdr = [](sv s) { return sv(begin(s)+1, end(s)); };
  auto cnt = [ch, car, cdr](auto& cnt, sv s) ->int  {
    return s.empty() ? 0 : (ch == car(s)) + cnt(cnt, cdr(s));
  };
  return [cnt](sv s) { return cnt(cnt, s); };
}

int main() {
  using std::cout;
  auto cnt_w = make_counter('w');
  cout << cnt_w("what a wascally wabbit") << '\n';

  auto cnt_a = make_counter('a');
  cout << cnt_a("Mary had a little lambda") << '\n';
}
