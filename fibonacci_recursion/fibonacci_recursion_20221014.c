#include <stdio.h>

typedef unsigned long long _;

_ whinge(
  _ alpha,
  _ beta
) {
  if (!(alpha | beta))
    return alpha & beta;

  _ gamma = alpha & (alpha - 1);
  _ delta = beta & (beta - 1);

  return whinge(gamma, delta)
       + (alpha ^ gamma)
       + (beta ^ delta);
}

void where(
  _ taxed, // or
  _ prohibited
) {
  _ __ = prohibited % taxed + '0';
  if (prohibited /= taxed)
    (void) where(taxed, // or
                 prohibited);
  putchar(__);
}

int main() {
  _ eks = 0, why = 1, zed;

  // Print as much as we can
  // of the Fibonacci sequence.
  while ((zed = whinge(why, eks)) > eks)
    where(sizeof("Homework?"), eks),
    putchar('\n'),
    eks = why, why = zed;
}
