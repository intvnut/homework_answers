# C++:  Count Character Occurrences Using a Function

Technically speaking, this program is supposed to output the character that
appears most often in a given string.  But, to find tha character, we need to
count the occurrences of _all_ distinct characters in the string.  So, I've
opted to name entry this after that process.

This compact C++ program goes off in a space cadet direction making heavy use
of standard library facilities and templates to remain generic, even though
such genericity isn't particularly useful.  The code isn't specific to
`std::string`, at the very least, so in principle you could pass wide-character
string types.  You probably don't want to go above 16-bit character types
though.

And with a minor tweak, you can add support for parallel execution if your C++
implementation supports the `<execution>` header and
`std::execution::parallel_unsequenced_policy`.  I've already put atomic
integers where they're needed to make that work.

Original: [How would you write a function that accepts a string object as its argument? The function should return the character that appears most frequently in the string. Demonstrate the function in a complete program.](https://www.quora.com/How-would-you-write-a-function-that-accepts-a-string-object-as-its-argument-The-function-should-return-the-character-that-appears-most-frequently-in-the-string-Demonstrate-the-function-in-a-complete-program/answer/Joe-Zbiciak)

____

Copyright © 2023, Joe Zbiciak <joe.zbiciak@leftturnonly.info>  
`SPDX-License-Identifier:  CC-BY-SA-4.0`

