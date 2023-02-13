# MS-DOS Batch and C: Add Two Numbers Without Arithmetic Operators

I decided to answer this question in _two_ programming languages.

One is a language that lacks arithmetic operators altogether:  MS-DOS Batch.
This isn't the batch language of `CMD.EXE` in Windows NT based versions of
Microsoft's current operating system.  This is the old-school `COMMAND.COM`
batch language of MS-DOS 6.22 and before.

As I said in my answer:

> Some years back, someone lamented that the MS-DOS batch language provided
> by `COMMAND.COM` had no math facilities. So, I figured out [how to make that
> happen with a collection of batch
> files.](http://spatula-city.org/~im14u2c/addbat/) It wasn’t pretty.

Now to be fair, I wrote that long before I wrote this answer.

If you're interested in those batch files, they're the ALL UPPERCASE source
files in this directory.

I also presented a way to represent integers up to 4096 as strings of
whitespace.  You can see that in `ws4096.c`.

And I presented a way to approach the problem with lookup tables and recursion,
representing integers as C strings.  You can see that in `lutrec.c`.

Original: [How do I add two numbers without using the addition operator or any other arithmetic operator?](https://www.quora.com/How-do-I-add-two-numbers-without-using-the-addition-operator-or-any-other-arithmetic-operator/answer/Joe-Zbiciak)


____

Copyright © 2023, Joe Zbiciak <joe.zbiciak@leftturnonly.info>  
`SPDX-License-Identifier:  CC-BY-SA-4.0`
