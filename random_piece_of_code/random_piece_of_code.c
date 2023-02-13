#include <stdio.h>

int main() {
  const char n = '\n', q ='"', b = '\\', *s = "#include <stdio.h>%c%cint main() {%c  const char n = '%cn', q = '%c', b = '%c%c', *s = %c%s%c;%c  printf(s, n, n, n, b, q, b, b, q, s, q, n, n, n);%c}%c";
  printf(s, n, n, n, b, q, b, b, q, s, q, n, n, n);
}
