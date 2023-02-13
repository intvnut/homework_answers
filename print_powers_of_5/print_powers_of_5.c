#include <stdio.h> 
 
static const char homework_digits[] = "12526"; 
static const int homework_indices[][2] = { 
  { 0, 1 }, { 2, 3 }, { 1, 3 }, { 0, 3 }, { 5, 1 }, { -1, -1 } 
}; 
 
int main() { 
  int i = -1; 
  while (homework_indices[++i][0] >= 0) { 
    int j = homework_indices[i][0]; 
    const int k = homework_indices[i][1]; 
    const int d = 1 - 2 * (j > k); 
    while (j != k) { 
      putchar(homework_digits[j]); 
      j += d; 
    } 
    putchar('\n'); 
  } 
} 
