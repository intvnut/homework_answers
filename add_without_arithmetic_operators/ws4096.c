#include <assert.h>
#include <stdio.h>
#include <string.h>

// Represent numbers up to 4096 as strings of whitespace.
#define WS_16 "                "
#define WS_256 WS_16 WS_16 WS_16 WS_16 WS_16 WS_16 WS_16 WS_16 \
               WS_16 WS_16 WS_16 WS_16 WS_16 WS_16 WS_16 WS_16
#define WS_4096 WS_256 WS_256 WS_256 WS_256 WS_256 WS_256 WS_256 WS_256 \
                WS_256 WS_256 WS_256 WS_256 WS_256 WS_256 WS_256 WS_256
const char *ws = WS_4096;

typedef const char* wsint;

wsint convert_int_to_wsint(int i) {
  assert(i >= 0 && i < 4096 && "Number must be 0..4095");
  return &ws[strlen(&ws[i])];
}

int convert_wsint_to_int(wsint w) {
  return strlen(w);
}

wsint add_wsint(wsint a, wsint b) {
  return convert_int_to_wsint(snprintf(NULL, 0, "%s%s", a, b));
}

int main() {
  wsint a = convert_int_to_wsint(1234);
  wsint b = convert_int_to_wsint(456);
  wsint c = add_wsint(a, b);
  printf("the sum of a and b is %d\n", convert_wsint_to_int(c));
}
