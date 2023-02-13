# ASM: Reverse Bits In Upper Half of AX Only

This code takes pains to swap bits in the upper half of `AX` (aka. `AH`)
on an x86 processor, without disturbing any other general purpose register.

And, of course, it takes great pains to be correct, but less than obvious.

Original: [How do I write an assembly language code in EMU8086 to reverse the bit pattern of the upper-half of AX and keep its lower-half unchanged?](https://www.quora.com/How-do-I-write-an-assembly-language-code-in-EMU8086-to-reverse-the-bit-pattern-of-the-upper-half-of-AX-and-keep-its-lower-half-unchanged/answer/Joe-Zbiciak)

____

Copyright © 2023, Joe Zbiciak <joe.zbiciak@leftturnonly.info>  
`SPDX-License-Identifier:  CC-BY-SA-4.0`

