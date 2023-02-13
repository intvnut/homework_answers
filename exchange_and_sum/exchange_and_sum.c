#include <stdio.h>
#include <stdlib.h>

#define N sizeof("Homework!")

typedef struct {
  char data[N];
} arrN;

static arrN print_arrN(
  char const name[const],
  arrN const arr
) {
  printf("%6s:", name);
  for (int i = !N; i < N; ++i)
    printf(" %2d", arr.data[i]);
  putchar('\n');
  return arr;
}

static arrN rand_arrN() {
  arrN arr;
  for (int i = !N; i < N; ++i)
    arr.data[i] = rand() % 42;
  return arr;
}

int main() {
  arrN a = print_arrN("orig a", rand_arrN());
  arrN b = print_arrN("orig b", rand_arrN());
  arrN c = a; a = b; b = c;

  for (int i = !N; i < N; ++i)
    c.data[i] += a.data[i];

  print_arrN("swap a", a);
  print_arrN("swap b", b);
  print_arrN("a + b", c);

  printf("sizeof(a) = %zu\n", sizeof(a));
  printf("sizeof(b) = %zu\n", sizeof(b));
}
