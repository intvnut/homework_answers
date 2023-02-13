# ASM: Replace Value in AX With its Absolute Value

This one is written in Intel syntax for the original 8086.  It's not wrapped
into a function, though.  It's obviously much longer and more drawn out than
necessary, because of course it is.

But it's branchless.  As I say in the original answer, branchless is _always_
better, right?  `;-)`

I also wrote a much more compact version involving `CWD`:

```
   CWD
   XOR AX, DX
   SUB AX, DX
```

That may in fact be the most efficient way to perform this operation on the
classic 8086, at a total of 11 cycles.  `CWD` is surprsingly expensive at 5
cycles.

Original: [What is the assembly code for this question: "Replace the number in AX by its absolute value"?](https://www.quora.com/What-is-the-assembly-code-for-this-question-Replace-the-number-in-AX-by-its-absolute-value/answer/Joe-Zbiciak)

____

Copyright © 2023, Joe Zbiciak <joe.zbiciak@leftturnonly.info>  
`SPDX-License-Identifier:  CC-BY-SA-4.0`

