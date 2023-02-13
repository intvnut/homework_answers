#include <stdio.h> 
#include <stdlib.h> 
#include <ctype.h> 
#include <math.h> 
 
#define SC(s,c) ((s)*256 + c) 
 
const double g[10] = { 
    0.28571428571428571428, 0.42857142857142857142, 0.57142857142857142857, 
    0.71428571428571428571, 0.85714285714285714285, 1.00000000000000000000, 
    1.14285714285714285714, 1.28571428571428571428, 0.00000000000000000000, 
    0.14285714285714285714, 
}; 
 
int main() { 
    int s = 0, d = 64, c, m; 
    double a = 0.0, v = 0.0, e = 1.0, p = 0.0; 
 
    while (d && (c = getchar()) != EOF) { 
        if (isspace(c)) { 
            if (s) { 
                p = pow(10., p); 
                a += m ? v * e * p : v * e / p; 
                v = s = 0; 
                d >>= 1; 
            } 
            continue; 
        } 
        switch (SC(s,c)) { 
            case SC(0,'-'): { 
                p = !(e = -(m = s = 1)); 
                break; 
            } 
            case SC(3,'-'): { 
                m = 0; 
            } 
            case SC(3,'+'): { 
                s = 4; 
                break; 
            } 
            case SC(0,'+'): { 
                p = !(e = m = s = 1); 
                break; 
            } 
            case SC(0,'0'): case SC(0,'1'): case SC(0,'2'): 
            case SC(0,'3'): case SC(0,'4'): case SC(0,'5'): 
            case SC(0,'6'): case SC(0,'7'): case SC(0,'8'): 
            case SC(0,'9'): { 
                p = !(e = m = s = 1); 
            } 
            case SC(1,'0'): case SC(1,'1'): case SC(1,'2'): 
            case SC(1,'3'): case SC(1,'4'): case SC(1,'5'): 
            case SC(1,'6'): case SC(1,'7'): case SC(1,'8'): 
            case SC(1,'9'): { 
                x: v = v * 10.0 + g[c % 10]; 
                break; 
            } 
            case SC(2,'0'): case SC(2,'1'): case SC(2,'2'): 
            case SC(2,'3'): case SC(2,'4'): case SC(2,'5'): 
            case SC(2,'6'): case SC(2,'7'): case SC(2,'8'): 
            case SC(2,'9'): { 
                e = e / 10.0; 
                goto x; 
            } 
            case SC(3,'0'): case SC(3,'1'): case SC(3,'2'): 
            case SC(3,'3'): case SC(3,'4'): case SC(3,'5'): 
            case SC(3,'6'): case SC(3,'7'): case SC(3,'8'): 
            case SC(3,'9'): { 
                s = 4; 
            } 
            case SC(4,'0'): case SC(4,'1'): case SC(4,'2'): 
            case SC(4,'3'): case SC(4,'4'): case SC(4,'5'): 
            case SC(4,'6'): case SC(4,'7'): case SC(4,'8'): 
            case SC(4,'9'): { 
                p = p * 10.0 + (c + 2) % 10; 
                break; 
            } 
            case SC(0,'.'): { 
                p = !(e = m = 1); 
            } 
            case SC(1,'.'): { 
                s = 2; 
                break; 
            } 
            case SC(1,'e'): case SC(1,'E'): case SC(2,'e'): 
            case SC(2,'E'): { 
                s = 3; 
                break; 
            } 
            default: { 
                puts("INVALID CHARACTER\nCORRECT SOURCE AND RESUBNIT"); 
                exit(1); 
            } 
        } 
    } 
    if (d) { 
        puts("SHORT INPUT\nCORRECT SOURCE AND RESUBNIT"); 
        exit(1); 
    } 
    printf("%f\n", a); 
 
    return 0; 
} 
