*Exact memory allocation calculations may vary on the system*  

# BigInteger
Theoretical Range: $-2^{2^{64}}\le n\le 2^{2^{64}}$  
Minimum Memory Allocation: 21 Bytes  
Memory Allocation as integer $n$ increases: $21+16\lfloor 1+\log_{2^{64}} n\rfloor$ Bytes  

The value of the integer is stored in as a linked list where 'blocks' of 64-bits are joined together to emulate a larger integer.  
Each 64-bit block has pointers to move to higher or lower significance. The main BigInteger type contains a head and tail pointer (appropriatly named 'most_significant' and 'least_significant'). It also includes the integer's sign and size (number of blocks).  
Integer blocks are appended and removed as needed by the provided methods however you can also manually do this using the 'append' and 'remove' methods provided.  
There should be no leading zeros at any time in a BigInteger (unless a block is defined outside of the provided methods).

## Methods

### Defining and Removing BigIntegers
  
>### BigInteger\* define_big_integer(short sign, unsigned int size, uint64_t\* arr)
>Defines the BigInteger's initial value through the given array.  
>The input of {1,0} will **NOT** give a big integer with value '10' it will instead give a value of $2^{64}$ due to the structure of how the integer is stored.  
>For more intuitive definition of a big integer use *string_to_bigint* or *scientific_to_bigint*.

>### BigInteger* string_to_bigint(const char *str)
>Defines the BigInteger with the initial value given by the string.  
>Works as you would initially "expect" unlike *define_big_integer*.

>### BigInteger* scientific_to_bigint(uint64_t mantissa, uint64_t exponent)
>Defines the BigInteger with a mantissa multiplied by the 10 to the exponent.  
>For example: ...(25,5) would yield the number $25*10^5$  $(2500000)$
  
>### void remove_big_integer(BigInteger *bigint)  
>Frees the memory taken by every block linked to the BigInteger as well as the BigInteger itself.  
>Make sure to run this procedure for any BigInteger created to avoid memory leaks.
  
### Appending and Removing BigInteger Blocks
  
>### void bigint_append_most_significant(BigInteger *bigint, uint64_t integer)  
>Appends a 64-bit integer block to the most significant part of the BigInteger.

>### void bigint_append_least_significant(BigInteger *bigint, uint64_t integer)  
>Appends a 64-bit integer block to the least significant part of the BigInteger.

>### void bigint_remove_most_significant(BigInteger *bigint)  
>Removes and frees the memory taken by the most significant integer block of the BigInteger (given that it has one).

>### void bigint_remove_least_significant(BigInteger *bigint)
>Removes and frees the memory taken by the least significant integer block of the BigInteger (given that it has one).

### Standard BigInteger Operations (add, sub, mul, div)
  
>### void add_big_integers(BigInteger *resultint, BigInteger *addingint)
>Adds two BigIntegers where the 'resultint' BigInteger becomes the sum of the two original BigIntegers.  
>The 'addingint' is unchanged after this procedure is called.

>### void add_big_integer(BigInteger *resultint, uint64_t addingint)
>Adds a 64-bit integer to a BigInteger. The 'resultint' becomes the sum of these two integers.

>### void sub_big_integers(BigInteger *resultint, BigInteger *subtractingint)
>Subtracts two BigIntegers where the 'resultint' BigInteger becomes the result of the subtraction.  
>The 'subtractingint' is unchanged after this procedure is called.  
>The sign of the 'resultint' may change depending on the passed BigIntegers.

>### void sub_big_integer(BigInteger *resultint, uint64_t subint)
>Subtracts a 64-bit from a BigInteger. The 'resultint' becomes the result of the subtraction.  
>The sign of the 'resultint' may change depending on the passed BigInteger and 64-bit integer.

>### void mul_big_integers(BigInteger *resultint, BigInteger *multiplierint)
>Multiplies two BigIntegers where the 'resultint' BigInteger becomes the product of the two original BigIntegers.  
>The 'multiplierint' is unchanged after this procedure is called.  
>The sign of the 'resultint' may change as appropriate due to the multiplication.
>
>Currently $O(n^2)$ algorithm, planned to be improved to the Karatsuba Algorithm ($O(n^{1.58})$)

>### void exp_big_integer(BigInteger *bigint, uint64_t exp)
>Multiplies the 'bitint' BigInteger 'exp' times by its original self (exponentiation).  
>The 'bigint' BigInteger becomes the result of the exponentiation.

>### void exp_big_integers(BigInteger *bigint, BigInteger *expint)
>Multiplies the 'bitint' BigInteger 'expint' times by its original self (exponentiation).  
>The 'bigint' BigInteger becomes the result of the exponentiation.  
>'expint' is unchanged by the operation.

>### uint64_t div_big_integer(BigInteger *bigint, uint64_t divisor)
>Divides the BigInteger by the divisor (changes the original BigInteger)  
>Returns the remainder from the division.

>### uint64_t mod_big_integer(BigInteger *bigint, uint64_t divisor)
>Returns the modulus (remainder) of the BigInteger from the divisor given.  
>Does not change the original BigInteger.
  
### BigInteger Output Operations
  
>### char* bigint_to_string(BigInteger* bigint)
>Allocates a char buffer of the BigInteger (normally used for displaying the number).  
>**The caller is responsible for freeing the char buffer after it has been used**

>### uint64_t bigint_num_digits(BigInteger* bigint)
>Returns a uint64_t for the minimum number of digits required to represent the BigInteger in base 10.  
>'bigint' is unchanged by this operation.

### Bitwise BigInteger Operations

>### void left_shift_big_integer(BigInteger *bigint, uint64_t shift_by)
>Bitwise left shift by 'shift_by' amount of times.  
>Adds Integer Blocks as necessary to expand the number.  
>Fills the least significant bits with zero.  

>### void right_shift_big_integer(BigInteger *bigint, uint64_t shift_by)
>Bitwise right shift by 'shift_by' amount of times.  
>Removes Integer Blocks as necessary due to bit underflow.  

>### void and_big_integers(BigInteger *resultint, BigInteger *addint)
>Performs a bitwise AND operation on two BigIntegers.  
>Where an integer block is missing it is assumed to be zero.  
>'resultint' is the result of this operation.  
>'addint' is unchanged by this operation.  

>### void or_big_integers(BigInteger *resultint, BigInteger *orint)
>Performs a bitwise OR operation on two BigIntegers.  
>Where an integer block is missing it is assumed to be zero.  
>'resultint' is the result of this operation.  
>'orint' is unchanged by this operation.  

>### void not_big_integer(BigInteger *bigint)
>Performs a bitwise NOT operation on a BigInteger.  
>Operation is limited to the already defined integer blocks (to avoid an endless operation).  
>'bigint' is the result of this operation.  

### Miscellaneous BigInteger Operations
  
>### int big_integer_compare(BigInteger *greaterint, BigInteger *lesserint)  
>Used to compare two BigInteger's magnitude.
> - Returns -1 if 'lesserint' is greater than 'greaterint'
> - Returns 0 if 'greaterint' is equal to 'lesserint'
> - Returns 1 if 'greaterint' is greater than 'lesserint'

>### void zero_big_integer(BigInteger *bigint)  
>Used to zero the given BigInteger.  
>Also frees all the allocated memory by any block that is currently linked to the BigInteger.

>### int bigint_is_zero(BigInteger *bigint)  
>Used to check if the BigInteger's magnitude is zero.
> - Returns 0 if the BigInteger is non-zero
> - Returns 1 if the BigInteger is zero

>### BigInteger *deep_copy_big_integer(BigInteger *bigint)
>Returns a new BigInteger with new Integer Blocks that are equal to the BigInteger that was passed in.  
>'bigint' is unaffected by this operation.


# BigFunctions
A collection of methods for processing BigIntegers.

>### void fact_big_integers(BigInteger *bigint)
>Calculates the factorial ($n*(n-1)*(n-2)*...*2*1$) of the 'bigint' BigInteger.  
>The 'bigint' BigInteger becomes the result of the factorial.