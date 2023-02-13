# ASM:  Add 10 Numbers in Assembly Language

Like so many other assembly language questions, I played _Dealer's Choice._

This time, I picked
[CP-1600X](../pdf/Locutus_CP-1600X_Instruction_Set_Extensions_20191203a.pdf),
which is an extended instruction set I created for the already obscure CP-1600
architecture.  This instruction set isn't just an "on paper" design.  I
actually implemented it, such that you can execute these new instructions on
an actual Intellivision video game unit.

In this directory you'll find three versions that correspond to the three
versions in my original answer.  These aren't complete programs.  They're
_just_ the inner loop.  These are in `ver1.asm`, `ver2.asm`, and `ver3.asm`.

The file `lto_isa.mac` includes the macro package that actually assembles the
extended instructions, using AS1600 from jzIntv.

And the PDF link above goes to the specification for CP-1600X.

Enjoy!

Original:  [What will the program be to add 10 numbers in the assembly language?](https://www.quora.com/What-will-the-program-be-to-add-10-numbers-in-the-assembly-language/answer/Joe-Zbiciak)


____

Copyright © 2023, Joe Zbiciak <joe.zbiciak@leftturnonly.info>  
`SPDX-License-Identifier:  CC-BY-SA-4.0`

