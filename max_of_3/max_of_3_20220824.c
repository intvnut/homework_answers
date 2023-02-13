#include <limits.h> 
#include <stdbool.h> 
#include <stdint.h> 
#include <stdio.h> 

int max_of_3(int a, int b, int c) { 
  switch ("homework" 
          [(a > b)*4 + 
           (a > c)*2 + 
           (b > c)]) { 
    case 'r': case 'k': return a; 
    case 'o': case 'e': return b; 
    default:            return c; 
  } 
} 
 
int main() { 
  int a, b, c; 
 
  while ( 
    scanf("%d%d%d", &a, &b, &c) == 3 && 
    printf("%d\n", max_of_3(a, b, c)) 
  ); 
}
