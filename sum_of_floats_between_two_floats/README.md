# C: Find the Sum of Floats Between Two Floats

This one was a lot of fun.  I came up with an _O(1)_ way to find the sum of all
`float` values for the closed interval `[a, b]`, where `a` and `b` are floats.
Technically speaking, that's not _between_ the values _a_ and _b_. That should
be the open interval `(a, b)`.  In any case, you could tweak my code a bit to
get there.

The original answer and the code itself offer a nice explanation of how it all
works.  I won't repeat it here.

Original: [How do I write a C program that displays the sum of floating numbers between any given two floating numbers?](https://www.quora.com/How-do-I-write-a-C-program-that-displays-the-sum-of-floating-numbers-between-any-given-two-floating-numbers/answer/Joe-Zbiciak)

_Noie:_  The C code includes [Jerry Coffin's version](https://www.quora.com/How-do-I-write-a-C-program-that-displays-the-sum-of-floating-numbers-between-any-given-two-floating-numbers/answer/Jerry-Coffin-2) which is far more compact, but
not _O(1)._ 

Jerry correctly implemented "between" as an open interval `(a,b)`, whereas I
incorrectly implemented "between" as a closed interval `[a,b]`.  Therefore his
answer and mine have slight differences.


____

Copyright © 2023, Joe Zbiciak <joe.zbiciak@leftturnonly.info>  
`SPDX-License-Identifier:  CC-BY-SA-4.0`

