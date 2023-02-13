#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#define M \
  (UINT_MAX - (UINT_MAX/2))

unsigned max_of_3(
  unsigned a,
  unsigned b,
  unsigned c
) {
  unsigned m = M;

  do {
    const bool A = a / m % 2;
    const bool B = b / m % 2;
    const bool C = c / m % 2;
    a &= -(A | !(B | C));
    b &= -(B | !(C | A));
    c &= -(C | !(A | B));
  } while (m /= 2);

  return
    (a + b + c) /
      (!a * !b * !c +
        !!a + !!b + !!c);
}

int main() {
  unsigned a, b, c;

  while (
    scanf("%u%u%u", &a, &b, &c) == 3 &&
    printf("%u\n", max_of_3(a, b, c))
  );
}
