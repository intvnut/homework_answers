# C++: Find First Two Unique Numbers in an Array

For this problem, I interpreted "unique" as "distinct," as opposed to "having
no repeats."  For example, the array `{ 1, 1, 1, 2, 2, 3, 4, 4, 4 }` holds four
distinct values: `1, 2, 3, 4`.  It holds only one value with no repeats: `3`.

My original answer requires elements that are less-than comparable, using the
notion of _[strict weak order](https://en.wikipedia.org/wiki/Weak_ordering)_ to
identify elements that are _equivaleni._  Elements that aren't equivalent are
considered unique.

The goal here was to make my function generic with respect to numeric type.

Now, on revisiting this answer, I realize this won't really work for numbers
with types such as `std::complex<double>`, as those are not less-than
comparable.  I've added a version that genericizes the comparison so that
it can work with equality-comparable types that are not less-than comparable.

I could use C++20 concepts here. I made this compatible with C++17 instead.

Original: [https://www.quora.com/How-do-I-find-the-first-two-unique-numbers-in-an-array-using-C](https://www.quora.com/How-do-I-find-the-first-two-unique-numbers-in-an-array-using-C/answer/Joe-Zbiciak)

____

Copyright © 2023, Joe Zbiciak <joe.zbiciak@leftturnonly.info>  
`SPDX-License-Identifier:  CC-BY-SA-4.0`

