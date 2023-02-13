# C++: Print Nucleotide Names Given Their Letters

The OP was asked to accept 5 single letters representing nucleotides (e.g.
from a DNA sequence), and to expand those to the full nucleotide names.
I generated some polynomials that, when provided the characters as input,
would evaluate to the appropriate character to output.

In this directory you'll find the quick-and-dirty Perl script I used to
compute those coefficients.  You'll also find two versions of the C++ program
I wrote.  One version is ASCII specific, and the other is not.

The non-ASCII-specific  C++ program outputs some amusing names for other input
letters.

* Adenine
* Bhdnine
* Cytosine
* Dplpjine
* Eztqfine
* Ftjpkine
* Guanine
* Hvnjcine
* Icberine
* Jrsvsine
* Kqqnbine
* Lheepine
* Mine
* Nfdqgine
* Olulwine
* Pinjmine
* Qpilsine
* Raeuaine
* Sogjine
* Thymine
* Uagxgine
* Vbxukine
* Wrkhoine
* Xemnxine
* Yaine
* Zuwmvine

As I said in a comment:

> I have a bright future ahead of me naming pharmaceuticals.

Someone suggested I should extend the program to output Uracil for U, so it
works for RNA.  I left that as an exercise for the reader.  If you look at the
code you'll see why.

Original: [What is a program that reads 5 single letter nucleotides in an array and prints their full name separately?](https://www.quora.com/What-is-a-program-that-reads-5-single-letter-nucleotides-in-an-array-and-prints-their-full-name-separately/answer/Joe-Zbiciak)

____

Copyright © 2023, Joe Zbiciak <joe.zbiciak@leftturnonly.info>  
`SPDX-License-Identifier:  CC-BY-SA-4.0`
