Big Math in C
=============

A maths library in C focused on absurdly large numbers and the ability to calculate with them.

This library is currently in development with goals of adding more/better operations to the current BigInteger type as well as developing a large floating point number type.  
Feel free to find/fix problems, optimise and add operations/functions to allow for more uses of these big numbers!

Big Data Types (structs)
------------------------

*Exact memory allocation calculations may vary on the system*  

### BigInteger
Theoretical Range: $-2^{2^{64}}\le n\le 2^{2^{64}}$  
Minimum Memory Allocation: 21 Bytes  
Memory Allocation as integer $n$ increases: $21+16\lfloor 1+\log_{2^{64}} n\rfloor$ Bytes  