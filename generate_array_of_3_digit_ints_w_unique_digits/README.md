# C: Generate an Array of Three Digit Integers With Unique Digits

This one comes from here:

[How do I form an array of three-digit random numbers, in each of which all three digits must be different?](https://www.quora.com/How-do-I-form-an-array-of-three-digit-random-numbers-in-each-of-which-all-three-digits-must-be-different-Use-C-language)

My answer is anything but straightforard. I decided to implement a _bitslice_
implementation of the algorithm.  Bitslice implementations put different
iterations of a loop in separate _bits_ within most of the words involved.

A single `AND`, `OR`, `XOR`, or `NOT` on a 64-bit value applies that same
operation on a single bit across 64 loop iterations.

Since most of the operations involved in this code only require 3 to 4 bits,
and a few require 7 to 10 bits, there's a big potential performancewin by
pursuing a bitslice implementation.

And... bitslice implementations tend to be completely opaque to someone who
has no idea what's going on.

In this directory, the `quora_r3db.c` file has what I posted to Quora.  The
`r3db.c` file adds a ton of comments, attempting to explain what's going on.
The `r3db_opt.c` file represents my attempts at optimizing the code for raw
performance.  It may have some rough edges, but I did try to clean up the parts
that count most for understanding what's going on. 

The file "OLD-README.txt" is the file I kept notes in while developing.

____

Copyright © 2023, Joe Zbiciak <joe.zbiciak@leftturnonly.info>  
`SPDX-License-Identifier:  CC-BY-SA-4.0`
