# C: C Code That Isn't Valid C++

The OP asked for C code that isn't valid C++.  The two files in this directory
are meant to work together.  They make use of:

* C++ keywords that aren't keywords in C.  
* K &amp; R function syntax.  
* Tentative declarations at file scope.  
* C's definition of `auto`. 
* Designated initializers, for both array indices and structure members, in opposite order of declaration.  
* Recursive invocation of `main()`.  
* Different behavior for `sizeof('x')`.  
* Non-prototype function declarations.  
* String literal as initializer for a non-const `char *`.

Features I didn't work in:

* VLAs. 
* Compound literals.
* flexible array members.  
* Imaginary numbers.  
* C-style complex numbers.
* The `register` keyword (gone in C++17).
* implicit `int`.  

I suppose I could have made one file C89 and the other C99 to work implicit
`int`.  The rest of the code is C99 through C18 compliant (I believe), but not
C23 due to K &amp; R function definitions.

Original: [How can I make a C program that takes 7 numbers as input and calculates the average?](https://www.quora.com/How-can-I-make-a-C-program-that-takes-7-numbers-as-input-and-calculates-the-average/answer/Joe-Zbiciak)

## Bonus Round

I had answered [another version of this question](https://www.quora.com/What-are-the-things-which-we-can-do-in-C-but-not-in-C/answer/Joe-Zbiciak)
somewhat earlier that did incorporate some C89-isms.

```
#include <stdio.h>
#include <stdlib.h>

virtual(public, private, protected) {
    printf("%d, %d, %d\n", public, private, protected);
}

static template = 17, throw, catch, try;

int main() {
    auto new = 10, *delete = &new;
    register class = 42;

    throw = class + 1;
    catch = template - class;
    try = throw * catch - *delete * new;

    virtual(try, throw, catch);
    return 0;
}
```

It's not quite as fancy as the files in this directory though.


____

Copyright © 2023, Joe Zbiciak <joe.zbiciak@leftturnonly.info>  
`SPDX-License-Identifier:  CC-BY-SA-4.0`

