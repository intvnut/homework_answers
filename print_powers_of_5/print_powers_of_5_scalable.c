#include <stdio.h> 
#define MAX_DIG (1024) 
 
char pow_5[MAX_DIG] = { 1 }; 
int last_dig = 1; 
 
void next_pow_5(void) { 
    int i = 1, prev = pow_5[0]; 
    if (pow_5[i] != 5) { 
        i = prev = 0; 
    } 
    while (i <= last_dig) { 
        int curr = pow_5[i]; 
        pow_5[i] = prev / 2 + pow_5[i] % 2 * 5; 
        prev = curr; 
        i++; 
    } 
    last_dig += !!pow_5[last_dig]; 
} 
 
void print_pow_5(void) { 
    int i = last_dig; 
    while (i > 0) { 
        putchar('0' + pow_5[--i]); 
    } 
    putchar('\n'); 
} 
 
int main() { 
    print_pow_5(); 
    while (last_dig < MAX_DIG - 1) { 
        next_pow_5(); 
        print_pow_5(); 
    } 
} 
