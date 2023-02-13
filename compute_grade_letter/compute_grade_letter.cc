#include <array>
#include <cstdint>
#include <iostream>

namespace {

// Four possible grades:  B, C, D, or E.
// This array maps values 0 to 3 to the four possible letter grades.
constexpr auto grade_decode = std::array{ 'B', 'C', 'D', 'E' };

// Scoring ranges:
//   Range   Letter  Index
//  40 - 44    E       3
//  45 - 49    D       2
//  50 - 54    C       1
//  55 - 59    B       0
//
// Truth table:
//
// i5 i4 i3 i2 i1 i0  o1 o0
//  0  x  x  x  x  x   x  x
//  1  0  0  x  x  x   x  x
//
//  1  0  1  0  x  x   1  1
//  1  0  1  1  0  0   1  1
//
//  1  0  1  1  0  1   1  0
//  1  0  1  1  1  x   1  0
//  1  1  0  0  0  x   1  0
//
//  1  1  0  0  1  x   0  1
//  1  1  0  1  0  x   0  1
//  1  1  0  1  1  0   0  1
//
//  1  1  0  1  1  1   0  0
//  1  1  1  0  x  x   0  0
//
//  1  1  1  1  x  x   x  x
//
// Notice i5 doesn't matter for any of our defined outputs, so we can ignore
// it. That allows us to encode the truth table into a pair of 32- bit bitmaps.
// Note that the lookups omit input i5 as it becomes a don't-care.

constexpr std::uint32_t o0_lookup =
  1u << 0b01000 | 1u << 0b01001 | 1u << 0b01010 | 1u << 0b01011 |
  1u << 0b01100 | 1u << 0b10010 | 1u << 0b10011 | 1u << 0b10100 |
  1u << 0b10101 | 1u << 0b10110;

constexpr std::uint32_t o1_lookup =
  1u << 0b01000 | 1u << 0b01001 | 1u << 0b01010 | 1u << 0b01011 |
  1u << 0b01100 | 1u << 0b01101 | 1u << 0b01110 | 1u << 0b01111 |
  1u << 0b10000 | 1u << 0b10001;

constexpr char grade_letter_lookup(const std::uint8_t score) {
  const auto o0 = (o0_lookup >> (score & 0b11111)) & 1;
  const auto o1 = (o1_lookup >> (score & 0b11111)) & 1;
  return grade_decode[o0 + 2*o1];
}

}  // namespace

int main() {
  for (std::uint8_t score = 40; score <= 59; ++score) {
    std::cout << "Score = " << int(score)
              << " Grade = " << grade_letter_lookup(score) << '\n';
  }
}
