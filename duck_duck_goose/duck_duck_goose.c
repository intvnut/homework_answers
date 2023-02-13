// Counts the number of times "duck" entered on input.
// Stops at "goose."
#include <stdio.h>

static int ns(const int s, const int n) {
  static const unsigned long long t[9] = {
    20496, 536891408, 20499, 282640, 20496, 6311952, 7360528,
    134238224, 22800
  };
  return (t[s] >> n) % 16;
}

static int c2n(const char c) {
  static const unsigned long long l = 11284783271425117322ull;
  static const unsigned long long m = 72340172838076673ull;
  static const unsigned long long n = 1161981756646125696ull;
  const unsigned long long t0 = c * m;
  const unsigned long long t1 = t0 ^ l;
  const unsigned long long t2 = t1 & (t1 / 2);
  const unsigned long long t3 = t2 & (t2 / 4);
  const unsigned long long t4 = t3 & (t3 / 16);
  const unsigned long long t5 = t4 & m;
  const unsigned long long t6 = n * t5;
  return 32 - (t6 >> 58);
}

int main() {
  int c, d = 0, s = 0;
  while ((c = getchar()) != EOF && s != 9) {
    s = ns(s, c2n(c));
    d += s == 4;
  }
  if (s != 9) {
    puts("Short input! No goose!");
  }
  printf("Ducks seen: %d\n", d);
}
