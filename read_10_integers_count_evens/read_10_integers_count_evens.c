#include <ctype.h> 
#include <stdio.h> 
 
int main() { 
  int c; 
  char s[] = "0\00\0009\012345"; 
  char *p = 7 + s, *q = &1[s]; 
 
  while ( 
    *(q += 3) && 
    (c = getchar()) != EOF 
  ) { 
    *(p -= 2) = !isdigit(c); 
    *(p += 2) = *++q & !s[6]; 
    *++q = s[5]; 
    *(q -= 6) += 1[s][p] & 8[s]; 
    4[s] -= 6[++q]; 
    p[1] = c - s[3]; 
  } 
 
  q[-3] = -(2[s][s]  > 3[s]) & '0'; 
  q[-3] && (2[s][s] -= 3[s] - q[-3]); 
 
  puts(s); 
} 
