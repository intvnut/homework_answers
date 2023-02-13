# ASM: Compute Triangle Area With Heron's Formula

Initially, I suggested OP write up a C version, compile it, _et voila!_ That
would yield their requested assembly code.  This is the C code:

```
#include <math.h>

double heron(double a, double b, double c) {
  double s = (a + b + c) / 2.;
  return sqrt(s * (s - a) * (s - b) * (s - c));
}
```

Easy peasy, right?

Of course, I went back and wrote a complete Intellivision assembly language
program for it, which you can find in this directory.  Here's the `README.txt`
I originally posted with the assembly:

```
A silly implmementation of Heron's Formula to find the area of a triangle.

The classic formula:

A = sqrt(s * (s - a) * (s - b) * (s - c)), where s = (a + b + c) / 2.

Alternate formulation:

A = (1/4) * sqrt((a + b + c) * (-a + b + c) * (a - b + c) * (a + b - c))

This is an unsigned 8q8 implementation, reworked even further.  To limit the
dynamic range of intermediate results, we perform the square roots early and
then multiply.  This is somewhat more expensive.

First, sort so that a >= b && a >= c.

t1 = sqrt((a + b + c) / 4)  ; note: Sum a + b + c carred out at higher prec.
t2 = sqrt(-a + b + c)
t3 = sqrt((a - b + c) / 2)  ; note: Sum a - b + c carred out at higher prec.
t4 = sqrt((a + b - c) / 2)  ; note: Sum a + b - c carred out at higher prec.
rslt = t1 * t2 * t3 * t4


Note:  The sums (-a + b + c), (a - b + c), and (a + b - c) are never negative
for valid triangles, thanks to the triangle inequality:  a <= b + c.
```

Original: [Who can write an assembly language program to compute the area of a triangle using Heron's formula?](https://www.quora.com/Who-can-write-an-assembly-language-program-to-compute-the-area-of-a-triangle-using-Herons-formula/answer/Joe-Zbiciak)

____

Copyright © 2023, Joe Zbiciak <joe.zbiciak@leftturnonly.info>  
`SPDX-License-Identifier:  CC-BY-SA-4.0`

