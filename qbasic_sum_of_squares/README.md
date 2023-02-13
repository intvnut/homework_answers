# QBASIC: Find the Sum of Squares of the First 50 Odd Numbers

OK, so apparently there are some schools out there that actually still teach
programming in QBASIC?  At least, that was true in 2018 when I wrote this
answer.

This solution takes a _graphical_ approach.

N.B. The statement...

```
   LINE (x1%,y1%)-(x2%,y2%),c%,BF
```

...draws a filled rectangle whose corners are (x1%,y1%) and (x2%,y2%). The
suffix `BF` means "box, filled."  The argument `c%` specifies the color.

The function `POINT(x%,y%)` returns the color of the pixel at (x%,y%).

In this case, I've drawn all the pixels within the box in color 1.  

Original: [Write a QBASIC program to find the sum of squares of first 50 odd numbers?](https://www.quora.com/Write-a-QBASIC-program-to-find-the-sum-of-squares-of-first-50-odd-numbers/answer/Joe-Zbiciak)

____

Copyright © 2023, Joe Zbiciak <joe.zbiciak@leftturnonly.info>  
`SPDX-License-Identifier:  CC-BY-SA-4.0`
