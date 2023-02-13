# C: Count Number of Ducks, Stopping at Goose

Quoting:

> This short C program will count the number of times “duck” was seen in the
> input. It will stop reading input once it sees “goose”.

That's exactly what this program does.  How it does it, however, isn't exactly
straightforward to see.  I used a few fun techniques, including some SIMD
wizardry, and a state machine to recognize `"duck"` and `"goose"`.

I'll leave the reader to pull apart how those pieces work.

Original: [Write a program with a loop that repeatedly reads a string as input, where the string is either "duck" or "goose". The loop terminates when "goose" is entered. After the loop, your code should print out the number of "duck" strings that were read.?](https://www.quora.com/Write-a-program-with-a-loop-that-repeatedly-reads-a-string-as-input-where-the-string-is-either-duck-or-goose-The-loop-terminates-when-goose-is-entered-After-the-loop-your-code-should-print-out-the-number-of-duck/answer/Joe-Zbiciak)

____

Copyright © 2023, Joe Zbiciak <joe.zbiciak@leftturnonly.info>  
`SPDX-License-Identifier:  CC-BY-SA-4.0`
