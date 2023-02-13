# C++: Insert a Substring Into a String Without Any Library Function

Ah, the lulz you can have answering what someone asked, rather than what
they meant to ask.  In this case, the OP wanted to insert one string into
another without using _any_ library function.  They probably didn't realize
just how restrictive that actually is. 

I get it: their teacher didn't want them to call some basic substring and 
concatenation APIs.  But, by cutting off access to the _entire_ standard
library, we can't even allocate storage for the result!

I've put the complete, working version of the code in the directory.  Below
the cut is a copy of my original answer with the purposefully incomplete
version of the code.


---

Oooh, tricky. _No_ library function?

That also implies you’ll be using C-style strings, as `std::string` is a
library class chock full of library methods.

Let’s try to do this in a typesafe manner, shall we? Since you can’t return
arrays directly from functions, you’ll need to wrap the return value in a
`struct`. The caller can always unwrap it. Something like this:

```
template <std::size_t N>  
struct wrapped_string { 
  char data[N]; 
}; 
```

(Note: Be sure to `#include <cstddef>` to get a definition of `std::size_t`.
It’s a type, not a function, so it’s OK.)

Since we cannot call `new` or `malloc`, we need to size the output buffer for
the worst case input buffer size, regardless of the actual lengths of the
strings contained.

We can discover the size of the input buffers with template argument deduction. Something like this:

```
template <std::size_t N1, std::size_t N2> 
auto insert_substring( 
    const char (&haystack)[N1],   // What to insert into. 
    const char (&needle)[N2],     // What to insert. 
    const std::size_t offset      // Where to insert. 
) { 
  // .. details 
} 
```


Now, what are the details?

I don’t actually want to hand you your homework on a platter. I _did_ write up
a working version of the code, though, so I’ll just leave the skeleton here
and let you work out what you need to fill in.

```
#include <cstddef> 
   
template <std::size_t N> 
struct wrapped_string { 
  char data[N]; 
}; 
 
template <std::size_t N1, std::size_t N2> 
auto insert_substring( 
    const char (&haystack)[N1],     // What to insert into. 
    const char (&needle)[N2],       // What to insert. 
    const std::size_t offset        // Where to insert. 
) { 
  auto result = wrapped_string<N1 + N2 + 1>{}; 
  auto haystack_idx = std::size_t{0}; 
  auto needle_idx = std::size_t{0}; 
  auto result_idx = std::size_t{0}; 
 
  // Copy from the haystack into the result up to the  
  // point of insertion. 
... 
 
  // Insert the needle into the haystack. 
... 
 
  // Copy any remaining haystack to the result. 
... 
 
  // Ensure null termination. 
... 
 
  return result; 
} 
```

You might ask yourself, why is it `wrapped_string<N1 + N2 + 1>` instead of
`wrapped_string<N1 + N2 - 1>`?

You forbade me standard library functions, and I wasn’t sure whether that
included exceptions. I know it definitely excludes `std::optional`. That’s why
I went this route.

Consider what happens when someone passes in a `char` array that isn’t null
terminated. Or, an `offset` beyond the end of a string.

We don’t have a way to report an error!

So, I just went for a crappier design. If you give me two arrays of characters
which may or may not be null-terminated, make sure I can always succeed,
copying both into my result buffer _and_ null terminating. And for the version
I implemented, if you give me an out-of-range offset, it just concatenates the
strings.

I’ll just paper over incorrect inputs without crashing.

_This isn’t great design!_ But, then, forbidding me the use of _any_ standard
library facility isn’t great either.

Take away the tools necessary to do a decent job, and guess what? You’ll get a
crappy result.

---

Original: [How can I write a C++ program to insert a substring in a string without using any library function?](https://www.quora.com/How-can-I-write-a-C-program-to-insert-a-substring-in-a-string-without-using-any-library-function/answer/Joe-Zbiciak)

____

Copyright © 2023, Joe Zbiciak <joe.zbiciak@leftturnonly.info>  
`SPDX-License-Identifier:  CC-BY-SA-4.0`

