// Compute sum & product of
// a and b, returning sum in
// a and product in b, avoiding
// undefined behavior.
void homework(
  int& a, int& b
) {
  unsigned x[] = { b, a, 0 };

  while (x[0]--) {
    ++x[1];
    x[2] += a;
  }

  a = x[1];
  b = x[2];
}
