#include <algorithm>
#include <cctype>
#include <iostream>
#include <iterator>
#include <locale>
#include <string>
#include <string_view>

namespace {

// Returns true if there is a '+' or '-' on the integer.
bool HasSign(const std::string& s) {
  return s.length() && (s[0] == '-' || s[0] == '+');
}

// Returns true if the integer is negative.
bool IsNegative(const std::string& s) {
  return s.length() && s[0] == '-';
}

// Returns true if the std::string holds a valid integer.
bool IsValidInteger(const std::string& s) {
  if (s.length() == 0) {
    return false;
  }

  const auto has_sign = HasSign(s);
  auto digits = std::string_view(s.data() + has_sign, s.length() - has_sign);

  for (const auto digit : digits) {
    if (!std::isdigit(digit)) {
      return false;
    }
  }

  return true;
}

// Adds two integer std::strings.  Assumes integers have already been validated.
std::string AddIntegers(const std::string& s1, const std::string& s2) {
  // Handle reverse-subtraction by swapping the arguments.
  if (IsNegative(s1) && !IsNegative(s2)) { return AddIntegers(s2, s1); }

  auto result = std::string{};
  result.reserve(std::max(s1.length(), s2.length()) + 2);

  const auto length = std::max(s1.length() - HasSign(s1),
                               s2.length() - HasSign(s2));

  auto next_s1_digit =
    [it = s1.crbegin(), e = s1.crend() - HasSign(s1)]() mutable {
    return it == e ? 0 : *it++ - '0';
  };

  auto next_s2_digit =
    [it = s2.crbegin(), e = s2.crend() - HasSign(s2)]() mutable {
    return it == e ? 0 : *it++ - '0';
  };

  // Perform addition if the signs match.
  if (IsNegative(s1) == IsNegative(s2)) {
    const auto result_is_negative = IsNegative(s1);

    auto carry = 0;

    for (auto i = std::size_t{0}; i != length; i++) {
      auto sum = next_s1_digit() + next_s2_digit() + carry;
      carry = sum > 9;

      if (carry) sum -= 10;

      result.push_back('0' + sum);
    }

    if (carry) {
      result.push_back('1');
    }

    if (result_is_negative) {
      result.push_back('-');
    }
  } else {
    // Otherwise, perform subtraction if the signs mismatch.
    // We're guaranteed that the first argument is the minuend, and the
    // second argument is the subtrahend.
    auto not_borrow = 1;
    auto result_is_negative = false;

    for (auto i = std::size_t{0}; i != length; i++) {
      auto sum = next_s1_digit() + 9 - next_s2_digit() + not_borrow;
      not_borrow = sum > 9;
      if (not_borrow) sum -= 10;
      result.push_back('0' + sum);
    }

    // 10's complement the result if negative, since the result is
    // sign-magnitude.
    if (!not_borrow) {
      if (result.length() != 1 || result[0] != '0') {
        auto carry = 1;
        for (auto& digit : result) {
          const auto digit_val = digit - '0';
          const auto digit_cmp = 9 - digit_val + carry;
          carry = digit_cmp > 9;
          digit = '0' + digit_cmp - (carry ? 10 : 0);
        }
        result_is_negative = true;
      }
    }
    while (result.length() > 1 && result.back() == '0') {
      result.pop_back();
    }
    if (result_is_negative &&
        (result.length() > 1 || result.back() != '0')) {
      result.push_back('-');
    }
  }

  std::reverse(std::begin(result), std::end(result));
  return result;
}

// Ensures we're using properly narrowed characters.  Replaces
// characters we don't know how to handle with '?'.
std::string NarrowString(const std::string& s) {
  auto& f = std::use_facet<std::ctype<char>>(std::locale());
  auto result = std::string{};
  result.reserve(s.length());

  for (const auto c : s) {
    result += f.narrow(c, '?');
  }

  return result;
}

// Prompts the user to enter an integer, and then reads, narrows,
// and validates the entered value.  Loops until the user enters
// a valid integer.
std::string GetInteger(const std::string& prompt, std::istream& in,
                       std::ostream& out) {
  auto result = std::string{};

  while (true) {
    auto input = std::string{};
    out << prompt;
    in >> input;
    result = NarrowString(input);
    if (IsValidInteger(result)) {
      break;
    }
    out << "Invalid integer: " << input << '\n';
  }

  return result;
}

}  // namespace

int main() {
  auto i1 = GetInteger("Enter first integer:  ", std::cin, std::cout);
  auto i2 = GetInteger("Enter second integer: ", std::cin, std::cout);
  auto i3 = GetInteger("Enter third integer:  ", std::cin, std::cout);

  auto s12 = AddIntegers(i1, i2);
  auto s123 = AddIntegers(s12, i3);

  std::cout << "Sum: " << s123 << '\n';
}
