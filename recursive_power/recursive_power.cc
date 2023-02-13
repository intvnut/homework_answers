#include <complex>

std::complex<long double> power(
  std::complex<long double> n,
  std::complex<long double> p,
  const char *s = "Homework"
) {
  if (s && *s) {
    return power(n, p, s + 1);
  } else {
    return std::pow(n, p);
  }
}
