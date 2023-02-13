#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

const int dtbl[CHAR_MAX] = {
 ['0'] = 0, ['1'] = 1, ['2'] = 2, ['3'] = 3,
 ['4'] = 4, ['5'] = 5, ['6'] = 6, ['7'] = 7
};

const char stbl[2][8][8] = {
  {"01234567", "12345670", "23456701", "34567012",
   "45670123", "56701234", "67012345", "70123456"},
  {"12345670", "23456701", "34567012", "45670123",
   "56701234", "67012345", "70123456", "01234567"},
};

const bool ctbl[2][8][8] = {
 {{ false, false, false, false, false, false, false, false },
  { false, false, false, false, false, false, false, true  },
  { false, false, false, false, false, false, true,  true  },
  { false, false, false, false, false, true,  true,  true  },
  { false, false, false, false, true,  true,  true,  true  },
  { false, false, false, true,  true,  true,  true,  true  },
  { false, false, true,  true,  true,  true,  true,  true  },
  { false, true,  true,  true,  true,  true,  true,  true  }},

 {{ false, false, false, false, false, false, false, true  },
  { false, false, false, false, false, false, true,  true  },
  { false, false, false, false, false, true,  true,  true  },
  { false, false, false, false, true,  true,  true,  true  },
  { false, false, false, true,  true,  true,  true,  true  },
  { false, false, true,  true,  true,  true,  true,  true  },
  { false, true,  true,  true,  true,  true,  true,  true  },
  { true,  true,  true,  true,  true,  true,  true,  true  }}
};

bool add_recurse(const char *a, const char *b, char *sum) {
  if (*a) {
    bool c = add_recurse(&a[1], &b[1], &sum[1]);
    *sum = stbl[(int)c][dtbl[(int)*a]][dtbl[(int)*b]];
    return ctbl[(int)c][dtbl[(int)*a]][dtbl[(int)*b]];
  } else {
    *sum = 0;
    return false;
  }
}

unsigned long add(unsigned long a, unsigned long b) {
  char bufa[43], bufb[43], bufs[43];
  snprintf(bufa, sizeof(bufa), "%042lo", a);
  snprintf(bufb, sizeof(bufb), "%042lo", b);
  add_recurse(bufa, bufb, bufs);
  return strtoul(bufs, NULL, 8);
}

int main() {
  printf("12345 + 67890 = %lu\n", add(12345ul, 67890ul));
  return 0;
}
