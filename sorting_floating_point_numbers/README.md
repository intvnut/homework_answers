# C++: Fast Sorting Algorithm For Small Number of Floating Point Numbers

For these kinds of questions, I defer to the standard library whenever
possible.  However, IEEE 754 floating point has a curveball with respect
to _NaN_ values.  Every comparison involving _NaN_ returns false, except
not-equals.  _NaN_ compare not-equal to everything.

The `std::sort` function, however, requires strict-weak ordering, and that
requires an `operator<` that behaves consistently.

Version 1 is the naive version that ignores _NaN._  Version 2 is my attempt
to correct this, but it has an error.  Version 3 gets that correct, AFAICT.  

Original: [How can I make a C program that takes 7 numbers as input and calculates the average?](https://www.quora.com/How-can-I-make-a-C-program-that-takes-7-numbers-as-input-and-calculates-the-average/answer/Joe-Zbiciak)

____

Copyright © 2023, Joe Zbiciak <joe.zbiciak@leftturnonly.info>  
`SPDX-License-Identifier:  CC-BY-SA-4.0`

