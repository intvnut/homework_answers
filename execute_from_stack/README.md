# C: Execute CPU Instructions From the Stack

OP asserted C doesn't let you execute instructions from the stack.  This was
a textbook case of "Oh yeah?  Hold mah beer!"

Granted, the C language standard doesn't give you any tools for this, but C
language implementations quite often do.  In this case, I make use of POSIX,
and lean on the fact that most everyone running this code is using x86-64.

This needs tweaks to run on something other than Linux, but that should
surprise nobody.

Original: [Why do people say that C can go as low as you want when it is not possible to execute CPU instructions from the stack?](https://www.quora.com/Why-do-people-say-that-C-can-go-as-low-as-you-want-when-it-is-not-possible-to-execute-CPU-instructions-from-the-stack/answer/Joe-Zbiciak)

____

Copyright © 2023, Joe Zbiciak <joe.zbiciak@leftturnonly.info>  
`SPDX-License-Identifier:  CC-BY-SA-4.0`
