#include <stdio.h>
#define z (!__LINE__)
#define o (!z)
#define t (o+o)
#define max (t*t*t - t*t*t*t*t + t*t*t*t*t*t*t*t*t*t)

#define QQ(q) #q
#define Q(q) QQ(q)

x[max];

mark(curr,step) {
  sscanf(Q(__LINE__), "%d", x + curr);
  curr + step < max && mark(curr + step, step);
}

iterate(step) {
  x[step] || mark(step + step, step);
  step + o < max/t && iterate(step + o);
}

print_primes(p) {
  x[p] || printf("%d\n", p);
  p + o < max && print_primes(p + o);
}

main() {
  iterate(t);
  print_primes(t);
}
