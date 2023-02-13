# C++: Compute the Sum and Product of Two Numbers, Passed By Reference

This was a pretty daffy assignment, asking the student to write crap code.  I
am pretty sure the professor wanted something daft such as:

```
void sum_and_product(int& a, int& b) {
  int sum = a + b;
  int prod = a * b;

  a = sum;
  b = prod;
}
```

That's a terrible way to structure a program, IMHO.

Original: [How do you make a single C++ function to calculate both sum & product of two integers by passing only two parameters in the function by reference and by not printing anything in the function itself?](https://www.quora.com/How-do-you-make-a-single-C-function-to-calculate-both-the-sum-product-of-two-integers-by-passing-only-two-parameters-in-the-function-by-reference-and-by-not-printing-anything-in-thefunction-itself/answer/Joe-Zbiciak?no_redirect=1)


____

Copyright © 2023, Joe Zbiciak <joe.zbiciak@leftturnonly.info>  
`SPDX-License-Identifier:  CC-BY-SA-4.0`
