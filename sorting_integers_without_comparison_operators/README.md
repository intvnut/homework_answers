# C: Sort Two Numbers Without If-Statements or Comparison Operators

The original question mentioned _swapping_ two numbers without using an
if-statement or a comparison operator; however, such a request makes no
sense.  You can do that easily:

```
void swap(unsigned *a, unsigned *b) {
  unsigned aa = *a, bb = *b;
  *b = aa;
  *a = bb;
}
```

The OP had other questions involving _sorting_ numbers (presumably integers)
and so I was pretty certain that's what OP actually had in mind.  My answer
for this question used a branch-free approach that essentially performed a
comparison between two unsigned integers and expanded that to a mask that
selects between the two values to sort them.  I'll let you figure out how it
actually works.

Original: [Can you create a logic in C for swapping numbers without using if statement or >,<, >=,<=?](https://www.quora.com/Can-you-create-a-logic-in-C-for-swapping-numbers-without-using-if-statement-or/answer/Joe-Zbiciak)


____

Copyright © 2023, Joe Zbiciak <joe.zbiciak@leftturnonly.info>  
`SPDX-License-Identifier:  CC-BY-SA-4.0`
