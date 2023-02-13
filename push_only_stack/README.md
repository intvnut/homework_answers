# C++: Implement Dynamic Stack With Linked List and Only Push Function

This request was weird:  They wanted a stack, built on a linked list,
that _only_ supports push operations.  No pop operations.

Now, you could get a fully featured (`push()`, `pop()`, `top()`, etc.) with
just a few lines of C++:

```
#include <list>
#include <stack>

template <typename T>
using homework_stack = std::stack<T, std::list<T>>;
```

But, if you want a stack that _only_ supports `push()`, I demonstrate a way
to radically simplify the implemetnation and save a lot of space to boot.

Original: [What is the implementation of dynamic stack with a linked list only using a push function in the C++ language?](https://www.quora.com/What-is-the-implementation-of-dynamic-stack-with-a-linked-list-only-using-a-push-function-in-the-C-language/answer/Joe-Zbiciak)


____

Copyright © 2023, Joe Zbiciak <joe.zbiciak@leftturnonly.info>  
`SPDX-License-Identifier:  CC-BY-SA-4.0`
