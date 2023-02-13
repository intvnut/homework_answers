#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>

enum {
  A, B, C, D = 1 << CHAR_BIT,
  E = '\\', F = '\'', G = '"',
  H = '/', I = '*', J = '\n',
  K = '\r'
};

int n(void) {
  static int a = A,
  b = G, c = A;
  a = b, b = c,
  c = getchar();
  return a == E && (
  b == J | b == K) ?
  c |= b == K & c ==
  J ? D : A, b |= D,
  a |= D : A, a;
}

int main() {
  int s = A, p = n();
  int q = n(), c;
  while (EOF != (c = n
    ())){c & D || (s =
    s == A ? p == H &&
    (c == H || c == I)
    ? q = D ^ (c += D)
    , C : (c == F || c
    == G) ? q = c, B :
    A : s == B ? (c ==
    q && p != E) ? A :
    B : (q == I & p ==
    q && c == H) || (q
    == H && (c == J ||
    c == K)) ? c ^= D,
    A : (c ^= tolower(
    c) ^ toupper(c), C
    ));p = c & D ? p :
    p == E && p == c ?
    A : c; putchar(c &
    ~     D    )     ;
  }
}
