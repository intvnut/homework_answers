# C++: Compute Grade Letter Given a Grade Value

This particular question asked to map four grade ranges to the grades B, C,
D, and E.  I decided to use a "bitmap lookup table", since the valid input
range was constrained to 20 integer values in the range 40 - 59.  Outside
that range, there's no defined grade letter.

One micro-optimization you could apply to this program:  Preshift the
`o1_lookup` by one bit.

```
constexpr std::uint32_t o0_lookup =
  1u << 0b01000 | 1u << 0b01001 | 1u << 0b01010 | 1u << 0b01011 |
  1u << 0b01100 | 1u << 0b10010 | 1u << 0b10011 | 1u << 0b10100 |
  1u << 0b10101 | 1u << 0b10110;

constexpr std::uint32_t o1_lookup =
  2u << 0b01000 | 2u << 0b01001 | 2u << 0b01010 | 2u << 0b01011 |
  2u << 0b01100 | 2u << 0b01101 | 2u << 0b01110 | 2u << 0b01111 |
  2u << 0b10000 | 2u << 0b10001;

constexpr char grade_letter_lookup(const std::uint8_t score) {
  const auto o0 = (o0_lookup >> (score & 0b11111)) & 1;
  const auto o1 = (o1_lookup >> (score & 0b11111)) & 2;
  return grade_decode[o1 | o0];
}
```

On a 64-bit machine, you could use pairs of bits in a 64-bit constant:

```
constexpr std::uint64_t o10_lookup =
  0b11u << 0b010000 | 0b11u << 0b010010 | 0b11u << 0b010100 |
  0b11u << 0b010110 | 0b11u << 0b011000 | 0b10u << 0b011010 |
  0b10u << 0b011100 | 0b10u << 0b011110 | 0b10u << 0b100000 |
  0b10u << 0b100010 | 0b00u << 0b100100 | 0b00u << 0b100110 |
  0b00u << 0b101000 | 0b00u << 0b101010 | 0b00u << 0b101100;

constexpr char grade_letter_lookup(const std::uint8_t score) {
  return grade_decode[(o10_lookup >> ((2 * score) & 0b111110)) & 0b11];
}
```

Original: [What is a C++ program to compute the grade of an examination score for one subject? Hint: grade, 40-44=E, 45-49=D, 50-54=C, 55-59=B.](https://www.quora.com/What-is-a-C-program-to-compute-the-grade-of-an-examination-score-for-one-subject-Hint-grade-40-44-E-45-49-D-50-54-C-55-59-B/answer/Joe-Zbiciak)

____

Copyright © 2023, Joe Zbiciak <joe.zbiciak@leftturnonly.info>  
`SPDX-License-Identifier:  CC-BY-SA-4.0`
