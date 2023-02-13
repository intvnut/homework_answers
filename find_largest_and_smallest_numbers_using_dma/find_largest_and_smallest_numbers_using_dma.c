#include <stdint.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
 
typedef enum { 
   kMax, kMin 
} MaxMin; 
  
int find_extremum(const uint8_t *d, int len, MaxMin max_min ) { 
  const uint8_t mask = max_min == kMin ? 255 : 0; 
 
  uint8_t t[511] = {0}; 
  uint8_t *m = t + 255; 
 
  for (int i = 0; i < len; ++i) { 
    uint8_t v = d[i] ^ mask; 
    memset(m - *m, v, v); 
  } 
 
  return *m ^ mask; 
} 
 
int main() { 
  srand(time(0)); 
 
  uint8_t a[20]; 
 
  printf("Input: "); 
  for (int i = 0; i < 20; ++i) { 
    a[i] = rand() % 256; 
    printf("%d ", a[i]); 
  } 
 
  printf("\nMax: %d\n", find_extremum(a, 20, kMax)); 
  printf("Min: %d\n", find_extremum(a, 20, kMin)); 
}
