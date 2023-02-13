# Any: Write 1 to 1000 in a Program Without A Loop

Initially, I pointed out they could just write a really long comment.  That's
what's in `ver1.c`.

I suggested that maybe they wanted to _output_ the numbers,
in which case they could use a really long string.  That's what's in `ver2.c`.

That version has a caveat that `puts()` itself a loop in it.  Ultimately,
the only way to be free of loops in software is to drive the output device
directly.

The result was a prototype C version that drives the data out of a serial port.
That's `ver3.c`.

And eventually I wrote a PIC24H assembly language version (untested) that is in
`ver4.s`.  The `RCALL`s might need to become `CALL` if the size is too big.

The two UART versions require the UART to drain as fast or faster than the code
writes characters to it.  Otherwise, the TX FIFO will overrun.  Ordinarily,
you would check that the TX FIFO isn't full; however, that would require a
loop.

Original: [How do I write 1 to 1000 in a programme without using any loop?](https://www.quora.com/How-do-I-write-1-to-1000-in-a-programme-without-using-any-loop/answer/Joe-Zbiciak)

____

Copyright © 2023, Joe Zbiciak <joe.zbiciak@leftturnonly.info>  
`SPDX-License-Identifier:  CC-BY-SA-4.0`

