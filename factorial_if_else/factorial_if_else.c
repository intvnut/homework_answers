#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  double d;
  if (argc != 2) {
    (void)fprintf(stderr, "Must provide exactly one argument.\n");
    return EXIT_FAILURE;
  } else if (sscanf(argv[1], "%lf", &d) != 1) {
    (void)fprintf(stderr, "Argument must be numeric.\n");
    return EXIT_FAILURE;
  } else if (d < 0) {
    (void)fprintf(stderr, "Argument must be non-negative.\n");
    return EXIT_FAILURE;
  } else {
    (void)printf("%g! = %g\n", d, tgamma(d + 1));
  }
  return EXIT_SUCCESS;
}
