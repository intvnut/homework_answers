# C++:  Writing Initials With Asterisks

Some lazy homework inquiries arrive as _demands_ rather than requests.  In
this case, the question ends with "please write the full program for me."

Original: [How do you write a C++ program to print the 1st two letters of your name in linear order using a for loop with asterisk “\*"? And first two letters of my name is U & N please write the full program for me](https://www.quora.com/How-do-you-write-a-C-program-to-print-the-1st-two-letters-of-your-name-in-linear-order-using-a-for-loop-with-asterisk-And-first-two-letters-of-my-name-is-U-N-please-write-the-full-program-for-me/answer/Joe-Zbiciak)

OK, so I will.  But, I will make sure its provenance is difficult to disguise.
I'm pretty sure anyone capable enough to remove this program's in-built
confession is capable enough to have written the program themselves originally.

## How It Works

In this case, I put the "confession" in a `std::array` of `char` that I use as
seed material for a Mersenne Twister random number generator.  I use a separate
table of _discard counts_ to say how many random numbers to skip.

The random numbers _between_ skips have the necessary bit-patterns to generate
the OP's initials as a pattern of asterisks.  Modify the seed string and, well,
you're [fbxrd.](https://www.urbandictionary.com/define.php?term=fbxrd)

## How I Did It

Someone asked me if I used a tool to obfuscate the answer:

![I do all my own stunts](../Images/i_do_all_my_own_stunts.png?raw=true)

I experimented with a couple different modulos until I settled on 16 and a
4-bit wide bitmap for the ASCII art.  I used a simple loop to inform me of what
drop counts I needed to achieve the necessary result:

```
  int x[] = { 9, 9, 9, 15, 0, 9, 11, 13, 9 };
  for (const auto c : x) {
    for (int d = 0; ; d++) {
      auto v = r() % 16;
      if (d && v == c) {
        std::cout << d << ", ";
        break;
      }
    }
  }
```

"Simple," he says.  `:-D`

And yes, I really did use decimal for the bitmap, and single letter variable
names.  Because, why not? `:-D`  I honestly didn't think anyone would see it
someday.  It was literally throwaway code, as far as I was concerned.

____

Copyright © 2023, Joe Zbiciak <joe.zbiciak@leftturnonly.info>  
`SPDX-License-Identifier:  CC-BY-SA-4.0`
