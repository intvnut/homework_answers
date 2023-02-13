#include <stdio.h>

x[1000];

/* x[N] nonzero means non-prime. */
mark_step;
mark_curr;

mark() {
  x[mark_curr] = 1;
  mark_curr += mark_step;
  mark_curr < 1000 ? mark() : 0;
}

iox_curr;

iterate_over_x() {
  mark_step = iox_curr;
  mark_curr = 2*iox_curr;
  x[iox_curr] == 0 ? mark() : 0;
  iox_curr++;
  iox_curr < 500 ? iterate_over_x() : 0;
}

print_curr;

print_prime() {
  printf("%d\n", print_curr);
}

print_primes() {
  x[print_curr] == 0 ? print_prime() : 0;
  print_curr++;
  print_curr < 1000 ? print_primes() : 0;
}

main() {
  x[0] = 1;
  x[1] = 1;

  iox_curr = 2;
  iterate_over_x();

  print_curr = 2;
  print_primes();
}
