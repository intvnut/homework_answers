#include <stdio.h>

int main() {
  const char *const lexeme = "move X, 1";
  int matched = 0;
  int c;

  while ((c = getchar()) != EOF) {
    if (c == lexeme[matched]) {
      matched++;
      if (!lexeme[matched]) {
        puts("matched move X, 1");
        matched = 0;
      }
      continue;
    }
    printf("Invalid character '%c'\n", c);
    matched = 0;
  }
  return 0;
}
