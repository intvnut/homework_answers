#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

uint32_t pow_2(uint32_t bit) {
  uint32_t p2 = 1;
  while (bit-- > 0) {
    p2 *= 2;
  }
  return p2;
}

bool get_bit(const uint32_t value, const uint32_t bit) {
  return (value / pow_2(bit)) % 2;
}

uint32_t homework_answer(uint32_t y) {
  y = y % 2147483648 - y % 1048576 + y % 524288 - y % 131072 + y % 65536 - y % 8 + y % 4 + 589824;
  return y;
}

void test_the_homework_answer(const uint32_t y) {
  printf("Before: y = %10u, bit 31 = %d, 19 == %d, 16 = %d, 2 = %d  ",
         y, get_bit(y, 31), get_bit(y, 19), get_bit(y, 16), get_bit(y, 2));

  const uint32_t z = homework_answer(y);

  printf("After: z = %10u, 31 = %d, 19 == %d, 16 = %d, 2 = %d\n",
         z, get_bit(z, 31), get_bit(z, 19), get_bit(z, 16), get_bit(z, 2));
}

int main() {
  srand(0);  // Not a great seed, but adequate.

  for (int i = 0; i < 30; ++i) {
    uint32_t input = 0;
    // Ensure we have 32 pseudorandom bits, regardless of RAND_MAX.
    for (int j = 0; j < 32; j++) {
      input = (input * 2) + rand();
    }
    test_the_homework_answer(input);
  }
}
