# C++: Return 39 For 13, and 13 For 39 Without an If-Statement

This is where the gloves came off. I believe this is my third true homework
trolling answer to a programming question.  I actually offered a mega-fun-pack
of potential solutions.

Since all of them are short, I will just include them here.  That said, I
recommend you visit (the original answer)[https://www.quora.com/What-is-the-C++-code-for-this-question-without-an-if-else-condition-If-input-13-the-answer-will-be-39-and-if-input-39-the-answer-will-be-13/answer/Joe-Zbiciak]
because I include a great war story and lesson at the end of it.

## What is the C++ code for this question without an if/else condition, "If input 13 the answer will be 39 and if input 39 the answer will be 13"?

```
// Return 39 if given 13, or 13 if given 39.
// The "| 1" prevents crashes when the input is 0.
constexpr unsigned do_it(const unsigned input) {
  return 507 / (input | 1);
}
```
…
```
#include <array>

// Return 39 if given 13, or 13 if given 39.
constexpr unsigned do_it(const unsigned input) {
  constexpr auto lookup = std::array<unsigned, 2>{ 13, 39 };
  return lookup[(input & 2) == 0];
}
```
…
```
#include <string>

// Return 39 if given 13, or 13 if given 39.
unsigned do_it(const unsigned input) {
  using namespace std::literals::string_literals;
  const auto s1 = "abcdefghijklm39pqrstuvwxyzABCDEFGHIJKLM13"s;
  const auto s2 = std::to_string(input);
  return s1.find(s2);
}
```
…
```
// Return 39 if given 13, or 13 if given 39.
constexpr unsigned do_it(const unsigned input) {
  return (0x000006800004E000ull >> (input & 0x3F)) & 0xFF;
}
```
…
```
#include <cmath>
// Return 39 if given 13, or 13 if given 39.
constexpr long do_it(const unsigned input) {
  return std::lround(std::cos(input) * 42.0 + std::sin(input) * 2.0);
}
```
…

```
#include <random>

// Return 39 if given 13, or 13 if given 39.
unsigned do_it(const unsigned input) {
  std::mt19937 rng(input);
  std::uniform_int_distribution<unsigned> uniform_dist(7, 47);
  rng.discard(829);
  return uniform_dist(rng);
}
```
…
```
// Return 39 if given 13, or 13 if given 39.
constexpr unsigned do_it(const unsigned input) {
  return (input * input * input * input - 2 * 13 * 1097) % 87881;
}
```
…
```
// Return 39 if given 13, or 13 if given 39.
constexpr unsigned do_it(const unsigned input) {
  return (871 / ((2 * (input | input >> 1 | input >> 2) | 1) & 65)) % 64;
}
```
____

Copyright © 2023, Joe Zbiciak <joe.zbiciak@leftturnonly.info>  
`SPDX-License-Identifier:  CC-BY-SA-4.0`
