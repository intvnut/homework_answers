#include <cassert>

// The following is only defined over the domain of integers 0..5
// and has a range of integer outputs 0..10.
int sum_with_switch(const int a, const int b) {
  assert(a >= 0 && a <= 5);
  assert(b >= 0 && b <= 5);
  switch (a) {
    case 0: {
      switch (b) {
         case 0: return 0;
         case 1: return 1;
         case 2: return 2;
         case 3: return 3;
         case 4: return 4;
         case 5:
         default: return 5;
      }
    }
    case 1: {
      switch (b) {
         case 0: return 1;
         case 1: return 2;
         case 2: return 3;
         case 3: return 4;
         case 4: return 5;
         case 5:
         default: return 6;
      }
    }
    case 2: {
      switch (b) {
         case 0: return 2;
         case 1: return 3;
         case 2: return 4;
         case 3: return 5;
         case 4: return 6;
         case 5:
         default: return 7;
      }
    }
    case 3: {
      switch (b) {
         case 0: return 3;
         case 1: return 4;
         case 2: return 5;
         case 3: return 6;
         case 4: return 7;
         case 5:
         default: return 8;
      }
    }
    case 4: {
      switch (b) {
         case 0: return 4;
         case 1: return 5;
         case 2: return 6;
         case 3: return 7;
         case 4: return 8;
         case 5:
         default: return 9;
      }
    }
    case 5:
    default: {
      switch (b) {
         case 0: return 5;
         case 1: return 6;
         case 2: return 7;
         case 3: return 8;
         case 4: return 9;
         case 5:
         default: return 10;
      }
    }
  }
}
