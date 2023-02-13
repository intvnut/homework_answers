#include <stdio.h>

void print_number_and_its_square(const char *s) {
  if (!*s) return;

  puts(s);
  putchar('\n');

  const char *e = s;
  do e++; while(*e);

  const int end = --e - s;
  printf("%s%c\n", s, *s);
  if (end) do {
    printf("%c%*s%c\n", *e--, end, "", *++s);
  } while (s - e != end);

  putchar(*e);
  do putchar(*s); while (s-- != e);
  putchar('\n');
  putchar('\n');
}

int main() {
  print_number_and_its_square("1");
  print_number_and_its_square("27");
  print_number_and_its_square("42");
  print_number_and_its_square("2.71828182845904523536");
  print_number_and_its_square("6.022*10^24");
  print_number_and_its_square("1234567890");
  print_number_and_its_square("867-5309/Jenny");
  print_number_and_its_square("22/7");
  print_number_and_its_square("pi");
  print_number_and_its_square("number");
}
