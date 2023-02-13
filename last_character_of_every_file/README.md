# C and C++:  The Last Character of Every File

This question asked what the last character in every files was, in C/C++.
Well, ask you may know, I must point out that C and C++ are different languages
at every opportunity, and this was no different.

There wasn't any code associated with this answer, so I'll just reproduce the
answer itself here.  

> `<pedant>`C/C++ is not a language. C and C++ are different
> languages.`</pedant>`  
> 
> The answer to your obscure trivia for C can be found in the standard for C17
> ([or its last public draft](https://web.archive.org/web/20181230041359if_/http://www.open-std.org/jtc1/sc22/wg14/www/abq/c17_updated_proposed_fdis.pdf)),
> section 5.1.1.2. In practice, though, violating this particular constraint
> just results in a diagnostic (aka. a warning).  
>  
> Interestingly, the public draft I have for C++20
> ([N4835 from 2019–10–08](https://github.com/cplusplus/draft/raw/master/papers/n4835.pdf))
> does not quite match that behavior. See Section 5.2. That said, the
> prescribed behavior is to behave as if the condition required by C17
> 5.1.1.2 was satisfied.  
> 
> This isn’t new with C++20. C++17, C++14, and C++11 all have similar text to
> C++20 here, although the exact section number has moved around. Look for
> [\[lex.phases\]](http://eel.is/c++draft/lex.phases#1.2) instead.
> [C++03](https://cs.nyu.edu/courses/fall11/CSCI-GA.2110-003/documents/c++2003std.pdf)
> has a similar requirement to C, and presumably so does C++98.  
>  
> See, C and C++ are different languages!
>  
> And yes, I haven’t said what character is required. Go look in the standard.
>  
> ---
> 
> **EDIT:**  I read the question as “every _source_ file.” If that’s not what
> you meant, then your question _has no answer._  
> 
> Some (_but not all!_) C standard library functions return the value `EOF`
> when you try to read _past the end of a file._ C++ `istream` has
> [entirely different ways](https://en.cppreference.com/w/cpp/io/basic_ios/eof)
> of indicating end of file. (I linked to one of them.)  
> 
> The macro `EOF` is not a character, and has a value that is (in practice, at
> least) outside the execution character set. Furthermore, `EOF` is not a part
> of the file itself. It’s an indication you’ve tried to read beyond the end of
> the file.  
> 
> For all of these reasons, it makes no sense to call `EOF` the file’s last
> character. If you consider `EOF` to be the last character of the file, then
> you’re committing an [off-by-one error.](https://www.quora.com/Why-are-off-by-one-errors-common-in-programming/answer/Joe-Zbiciak)   
> 
> One last thought: a zero-length file has _no_ last character. It has no
> _first_ character. It has no characters _at all._


Original: [What is the last character of every file in C/C++?](https://www.quora.com/What-is-the-last-character-of-every-file-in-C-C/answer/Joe-Zbiciak)

____

Copyright © 2023, Joe Zbiciak <joe.zbiciak@leftturnonly.info>  
`SPDX-License-Identifier:  CC-BY-SA-4.0`

