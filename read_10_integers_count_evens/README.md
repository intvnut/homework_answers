# C: Read 10 Integers And Count Evens

This one comes from here:

[What is an algorithm which reads an array of 10 integers and counts the even numbers and running time?](https://www.quora.com/What-is-an-algorithm-which-reads-an-array-of-10-integers-and-counts-the-even-numbers-and-running-time)

This answer explores some pointer arithmetic shenanigans to create a really
craptastic state machine whose mode of operation isn't exactly obvious.

Also, it explores the duality of `a[a[i]] == i[a][a]`.  Because, why not?

____

Copyright © 2023, Joe Zbiciak <joe.zbiciak@leftturnonly.info>  
`SPDX-License-Identifier:  CC-BY-SA-4.0`
