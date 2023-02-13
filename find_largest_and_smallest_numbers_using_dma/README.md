# C: Find Largest and Smallest Numbers Using DMA

This question used the acronym DMA in a highly non-standard way.  To most of
the computer-savvy world, DMA means Direct Memory Access.

DMA normally refers to a hardware engine that accesses memory directly, as
opposed to having the CPU do memory accesses on its behalf.  Generic DMA
devices perform memory copy and fill operations at the CPU's request.
Peripherals with DMA capability will read and write memory as needed to perform
their assigned function.

This poor student's teacher most likely meant Dynamic Memory Allocation.  I
have only ever encountered that use of the acronym DMA on misguided homework
questions.

In order to answer this question _properly,_ I modeled a generic DMA engine
with `memcpy()` and `memset()`.  In this particular program, I only needed
`memset()`.

Original: [What is a program in C to find the largest and smallest number in a list of N numbers using DMA?](https://www.quora.com/What-is-a-program-in-C-to-find-the-largest-and-smallest-number-in-a-list-of-N-numbers-using-DMA/answers/347709518)

____

Copyright © 2023, Joe Zbiciak <joe.zbiciak@leftturnonly.info>  
`SPDX-License-Identifier:  CC-BY-SA-4.0`
