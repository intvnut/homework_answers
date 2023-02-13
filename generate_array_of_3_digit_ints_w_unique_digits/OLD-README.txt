Need to generate random 3-digit decimal numbers with no repeated digits within
each 3-digit number.

This is a bitslice implementation.

Use a lagged Fibonacci implementation from TAOCP for random numbers with lags
30, 127.

Need to generate two random values 0..8, and one random value 0..7.  

For 0..8, we can take a 9-bit random value and partition it as follows:

 -- For values less than or equal to 56, assign 8.
 -- otherwise assign the last 3 bits for the random value.

This gives even coverage over all values, except 0, which occurs 56 times out
of 512 instead of 57.

The digit assignment works as follows:

 -- First random value is the most significant decimal digit, as-is.
 -- Second random value is the middle digit.  Add 1 if it's equal to the first.
 -- Third random value is the least signficant decimal digit.  Add 1 if it
    is equal to the first.  Add 1 to that if it's equal to the second.


 

