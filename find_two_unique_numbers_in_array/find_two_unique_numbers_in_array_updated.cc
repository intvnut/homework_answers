#include <algorithm>
#include <iterator>
#include <type_traits>
#include <utility>

namespace {

using std::declval;

template <typename T, typename = void>
struct is_equality_comparable : std::false_type {};

template <typename T>
struct is_equality_comparable<T, decltype(declval<T>() == declval<T>(), void())>
: std::true_type {};

template <typename T, typename = void>
struct is_less_than_comparable : std::false_type {};

template <typename T>
struct is_less_than_comparable<T, decltype(declval<T>() < declval<T>(), void())>
: std::true_type {};



template <typename T>
bool equivalent(const T& lhs, const T& rhs) {
  static_assert(
    is_equality_comparable<T>::value || is_less_than_comparable<T>::value,
    "Type must be equality or less-than comparable"
  );

  if constexpr (is_equality_comparable<T>::value) {
    return lhs == rhs;
  }

  if constexpr (is_less_than_comparable<T>::value) {
    return !(lhs < rhs) && !(rhs < lhs);
  }
}

template <typename InputIt>
auto find_first_two_unique(InputIt first, InputIt after_last)
  -> std::pair<decltype(*first), decltype(*first)> {
  const auto& first_unique = *first;

  std::advance(first, 1);

  const auto next_unique_it =
    std::find_if(first, after_last,
      [&first_unique](auto second) {
        return !equivalent(first_unique, second);
      });

  if (next_unique_it == after_last) {
    // What do you want to do here?  One option: Return first_unique twice.
    return {first_unique, first_unique};
  }

  return {first_unique, *next_unique_it};
}

}  // namespace


// ---- demo program ----

#include <array>
#include <complex>
#include <iostream>

// This is really just a wrapper around an integer, but we only let you
// perform less-than comparison on it.
class StrictWeakVal {
 public:
  StrictWeakVal(int x) : x_(x) {};

  operator int() const {
    return x_;
  }

 private:
  int x_;
};

template <typename T>
auto& operator<<(std::basic_ostream<T> &os, StrictWeakVal v) {
  return os << int(v);
}

bool operator<(StrictWeakVal lhs, StrictWeakVal rhs) {
  return int(lhs) < int(rhs);
}

// This class can't be used with our function because it defines neither
// equality or less-than relations.
class NotComparable {
 public:
  NotComparable() = default;
};

int main() {
  using namespace std::complex_literals;

  std::array<int, 5> ai =  { 1, 1, 2, 3, 4 };
  std::array<std::complex<double>, 5> ac = {
    1.0 + 1.0i, 1.0 + 1.0i, 1.0 + 2.0i, 3.0 + 1.0i, 1.0 + 4.0i
  };
  std::array<StrictWeakVal, 5> as = { 1, 1, 2, 3, 4};
  std::array<NotComparable, 5> an = {};

  auto f2u_i = find_first_two_unique(ai.cbegin(), ai.cend());
  auto f2u_c = find_first_two_unique(ac.cbegin(), ac.cend());
  auto f2u_s = find_first_two_unique(as.cbegin(), as.cend());
//  auto f2u_n = find_first_two_unique(an.cbegin(), an.cend());

  std::cout << f2u_i.first << ' ' << f2u_i.second << '\n'
            << f2u_c.first << ' ' << f2u_c.second << '\n'
            << f2u_s.first << ' ' << f2u_s.second << '\n';
}

