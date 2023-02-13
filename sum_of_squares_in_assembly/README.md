# ASM: Compute the Sum of a Series of Squares

As per my usual policy, an assembly question that doesn't specify an assembly
language gets _Dealer's Choice!_  I happened to pick the CP-1600 that's in the
Intellivision for this one.

There's three versions of the answer here.

Version 1 uses an assembly-time computations to populate a table of squares.
It then just sums up entries in that table of squares to produce the result.

Version 2 recognizes that (x + 1)^2 = x^2 + 2x + 1, and computes the squares on
the fly.

Now if I really wanted to be cute, I'd write a version 3 which implements the
closed-form formula x(x + 1)(2x + 1)/6; however, the CP-1600 lacks multiply
and divide instructions.  Unlike my [Heron's Formula](../herons_formula) answer,
I wasn't really motivated to bring in my library functions for that for this
answer.

Instead I wrote a version 3 that reads the answer from a precomputed table.
That's even faster.  `:-)`


Original: [https://www.quora.com/How-do-I-print-the-sum-of-series-1-2-2-2-3-2-4-2-N-2-in-the-assembly-language-using-an-array](https://www.quora.com/How-do-I-print-the-sum-of-series-1-2-2-2-3-2-4-2-N-2-in-the-assembly-language-using-an-array/answer/Joe-Zbiciak)

____

Copyright © 2023, Joe Zbiciak <joe.zbiciak@leftturnonly.info>  
`SPDX-License-Identifier:  CC-BY-SA-4.0`

