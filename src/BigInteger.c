#include "BigInteger.h"
#include "IntegerOperations.h"
#include <stdlib.h>
#include <stdint.h>

/*Defines a Big Integer with a sign, a given size and array of the initial value of the Integer
  The most significant part of the integer is in the first elements of the given array.
  The given array does not represent the actual number in denary (The leftmost element is equivalent to 2^(size*64) of the given value)*/
BigInteger* define_big_integer(short sign, unsigned int size, uint64_t* arr) {
    BigInteger* bigint = malloc(sizeof(BigInteger));
    bigint->least_significant = NULL;
    bigint->most_significant = NULL;
    bigint->size = 0;

    for (int i = 0; i < size; i++) {
        append_least_significant(bigint, arr[i]);
    }

    bigint->sign = sign;
    //bigint->size = size;

    return bigint;
}

/*Frees the memory from the Big Integer*/
void remove_big_integer(BigInteger *bigint) {
    while (bigint->size > 0) {
        remove_most_significant(bigint);
    }

    free(bigint);
}

/*Appends a 64 bit number to the least significant part of the current Big Integer*/
void append_least_significant(BigInteger* bigint, uint64_t integer) {
    BigInteger_Block* bigint_block = malloc(sizeof(BigInteger_Block));
    bigint_block->integer = integer;
    bigint_block->less_significance = NULL;

    if (bigint->size > 0) {
        bigint_block->more_significance = bigint->least_significant;
        bigint->least_significant->less_significance = bigint_block;
    } else {
        bigint_block->more_significance = NULL;
        bigint->most_significant = bigint_block;
    }

    bigint->least_significant = bigint_block;
    bigint->size++;
}

/*Appens a 64 bit number to the most significant part of the current Big Integer*/
void append_most_significant(BigInteger* bigint, uint64_t integer) {
    BigInteger_Block* bigint_block = malloc(sizeof(BigInteger_Block));
    bigint_block->integer = integer;
    bigint_block->more_significance = NULL;

    if (bigint->size > 0) {
        bigint_block->less_significance = bigint->most_significant;
        bigint->most_significant->more_significance = bigint_block;
    } else {
        bigint_block->less_significance = NULL;
        bigint->least_significant = bigint_block;
    }

    bigint->most_significant = bigint_block;
    bigint->size++;
}

/*Frees the current most significant part of the Big Integer*/
void remove_most_significant(BigInteger* bigint) {
    if (bigint->size == 0) {return;}

    BigInteger_Block* block_to_free = bigint->most_significant;

    if (bigint->size == 1) {
        bigint->most_significant = NULL;
        bigint->least_significant = NULL;
    } else {
        bigint->most_significant = block_to_free->less_significance;
        bigint->most_significant->more_significance = NULL;
    }

    bigint->size--;
    free(block_to_free);
}

/*Frees the current least significant part of the Big Integer*/
void remove_least_significant(BigInteger* bigint) {
    if (bigint->size == 0) {return;}

    BigInteger_Block* block_to_free = bigint->least_significant;

    if (bigint->size == 1) {
        bigint->most_significant = NULL;
        bigint->least_significant = NULL;
    } else {
        bigint->least_significant = block_to_free->more_significance;
        bigint->least_significant->less_significance = NULL;
    }

    bigint->size--;
    free(block_to_free);
}

/*Removes any and all leading zero blocks (where a whole integer block is zero)*/
void remove_leading_zeros(BigInteger *bigint) {
    while (bigint->size > 0 && bigint->most_significant->integer == 0) {
        remove_most_significant(bigint);
    }
}

/*Operations*/

/*Zeros out the Big Integer (frees all integer blocks)*/
void zero_big_integer(BigInteger *bigint) {
    while (bigint->size > 0) {
        remove_most_significant(bigint);
    }
}

/*Returns true if the first BigInteger (greaterint) is greater than the second BigInteger (lesserint)*/
int big_integer_compare(BigInteger *greaterint, BigInteger *lesserint) {
    if (greaterint->size == lesserint->size) {
        BigInteger_Block *current_greaterint_block = greaterint->most_significant;
        BigInteger_Block *current_lesserint_block = lesserint->most_significant;
        for (int i = 0; i < greaterint->size; i++) {
            if (current_greaterint_block->integer > current_lesserint_block->integer) {
                return 1;
            } else if (current_greaterint_block->integer < current_lesserint_block->integer) {
                return -1;
            }

            current_greaterint_block = current_greaterint_block->less_significance;
            current_lesserint_block = current_lesserint_block->less_significance;
        }

        return 0;
    } else {
        return greaterint->size > lesserint->size ? 1 : -1;
    }

    return 0;
}

/*Adds value into result at block offset, propagating carry towards more significance
  Used mainly for current multiplication algorithm*/
void add_at_offset(BigInteger *result, uint64_t value, unsigned int offset) {
    //Go to block at the given offset, appending zeroes if needed (goes from least significance to more significance)
    BigInteger_Block *block = result->least_significant;

    for (unsigned int i = 0; i < offset; i++) {
        if (block->more_significance == NULL)
            append_most_significant(result, 0);
        block = block->more_significance;
    }

    //propagate addition and carry towards more significance.
    while (value > 0) {
        if (block == NULL) {
            append_most_significant(result, value);
            break;
        }

        uint64_t prev = block->integer;
        block->integer += value;
        value = (block->integer < prev) ? 1 : 0;  // carry if wrapped
        block = block->more_significance;
    }
}

/*Deep copies a BigInteger (returns the new BigInteger)*/
BigInteger *deep_copy_big_integer(BigInteger *bigint) {
    BigInteger *new_bigint = define_big_integer(bigint->sign, 0, NULL);

    BigInteger_Block *bigint_block = bigint->least_significant;
    for (int i = 0; i < bigint->size; i++) {
        append_most_significant(new_bigint, bigint_block->integer);
        bigint_block = bigint_block->more_significance;
    }

    return new_bigint;
}

/*Adds two Big Integers together and returns the sum as the first parameter's value
  DOES NOT ignore sign*/
void add_big_integers(BigInteger *resultint, BigInteger *addingint) {
    if (resultint->sign == 0 && addingint->sign == 1) {
        addingint->sign = 0;
        sub_big_integers(resultint, addingint);
        addingint->sign = 1;
    } else if (resultint->sign == 1 && addingint->sign == 0) {
        resultint->sign = 0;
        sub_big_integers(resultint, addingint);
        resultint->sign = 1;
    } else {
        uint64_t carry = 0;
        unsigned int max_size = MAX(resultint->size, addingint->size);

        BigInteger_Block* resultint_block = resultint->least_significant;
        BigInteger_Block* addingint_block = addingint->least_significant;

        for (unsigned int i = 0; i < max_size; i++) {
            uint64_t result = carry;
            carry = 0;

            if (addingint->size > i) {
                result += addingint_block->integer;

                if (result < addingint_block->integer) { //number wrap
                    carry = 1;
                }

                addingint_block = addingint_block->more_significance;
            }

            if (resultint->size > i) {
                result += resultint_block->integer;

                if (result < resultint_block->integer) { //number wrap
                    carry = 1;
                }

                resultint_block->integer = result;

                resultint_block = resultint_block->more_significance;
            } else {
                append_most_significant(resultint, result);
            }
        }

        if (carry) {
            append_most_significant(resultint, 1);
        }
    }
}

/*Adds a BigInteger and 64 bit integer together*/
void add_big_integer(BigInteger *resultint, uint64_t addingint) {
    uint64_t carry = 0;

    BigInteger_Block* resultint_block = resultint->least_significant;

    for (unsigned int i = 0; i < resultint->size; i++) {
        uint64_t result = carry;
        carry = 0;

        if (i == 0) {
            result += addingint;

            if (result < addingint) { //number wrap
                carry = 1;
            }
        } else if (carry == 0) {
            break;
        }

        if (resultint->size > i) {
            result += resultint_block->integer;

            if (result < resultint_block->integer) { //number wrap
                carry = 1;
            }

            resultint_block->integer = result;

            resultint_block = resultint_block->more_significance;
        } else {
            append_most_significant(resultint, result);
        }
    }

    if (carry) {
        append_most_significant(resultint, 1);
    }
    
}

/*Subtracts two big integers from each other where the result becomes the 'resultint'.*/
void sub_big_integers(BigInteger *resultint, BigInteger *subtractingint) {
    if (resultint->sign == 0 && subtractingint->sign == 1) {
        subtractingint->sign = 0;
        add_big_integers(resultint, subtractingint);
        subtractingint->sign = 1;

    } else if (resultint->sign == 1 && subtractingint->sign == 0) {
        resultint->sign = 0;
        add_big_integers(resultint, subtractingint);
        resultint->sign = 1;

    } else {
        uint64_t borrow = 0;
        unsigned int max_size = MAX(resultint->size, subtractingint->size);

        BigInteger_Block *resultint_block = resultint->least_significant;
        BigInteger_Block *subtractingint_block = subtractingint->least_significant;

        for (int i = 0; i < max_size; i++) {
            uint64_t result = 0;

            if (i < resultint->size) {
                result = resultint_block->integer;

                if (result > 0 && borrow) {
                    result--;
                    borrow = 0;
                }
            }

            if (i < subtractingint->size) {
                if (subtractingint_block->integer > result) {
                    result = (UINT64_MAX - subtractingint_block->integer) + result + 1;
                    borrow = 1;
                } else {
                    result -= subtractingint_block->integer;
                }

                subtractingint_block = subtractingint_block->more_significance;
            }

            if (i < resultint->size) {
                resultint_block->integer = result;
                resultint_block = resultint_block->more_significance;
            }
        }

        if (borrow) {
            resultint->sign = 1;
        }
    }

    remove_leading_zeros(resultint);
}

/*Subtracts a 64 bit integer from the resultint BigInteger*/
void sub_big_integer(BigInteger *resultint, uint64_t subint) {
    uint64_t borrow = 0;

    BigInteger_Block *resultint_block = resultint->least_significant;

    for (int i = 0; i < resultint->size; i++) {
        uint64_t result = 0;

        if (i < resultint->size) {
            result = resultint_block->integer;

            if (result > 0 && borrow) {
                result--;
                borrow = 0;
            }
        }

        if (i == 0) {
            if (subint > result) {
                result = UINT64_MAX - subint + 1;
                borrow = 1;
            } else {
                result -= subint;
            }
        } else if (borrow == 0) {
            break;
        }

        if (i < resultint->size) {
            resultint_block->integer = result;
            resultint_block = resultint_block->more_significance;
        }
    }

    if (borrow) {
        resultint->sign = 1;
    }


    remove_leading_zeros(resultint);
}

/*Returns a new Big Integer equal to resultint multiplied by multiplierint.
  Result of operation is resultint.*/
void mul_big_integers(BigInteger *resultint, BigInteger *multiplierint) {
    BigInteger *temp = define_big_integer(0, 0, NULL);

    // Seed with a zero block
    append_most_significant(temp, 0);

    BigInteger_Block *resultint_block = resultint->least_significant;

    for (unsigned int i = 0; i < resultint->size; i++) {
        BigInteger_Block *multiplierint_block = multiplierint->least_significant;

        for (unsigned int j = 0; j < multiplierint->size; j++) {
            uint64_t hi, lo;
            lo = mul64(resultint_block->integer, multiplierint_block->integer, &hi);

            add_at_offset(temp, lo, i + j);

            if (hi > 0) {
                add_at_offset(temp, hi, i + j + 1);
            }
                
            multiplierint_block = multiplierint_block->more_significance;
        }

        resultint_block = resultint_block->more_significance;
    }

    zero_big_integer(resultint);
    resultint->most_significant = temp->most_significant;
    resultint->least_significant = temp->least_significant;
    resultint->sign = (resultint->sign == multiplierint->sign) ? 0 : 1;
    resultint->size = temp->size;

    temp->most_significant = NULL;
    temp->least_significant = NULL;
    temp->size = 0;
    remove_big_integer(temp);
}

/*Calculates the value of the BigInteger to the exp (bigint^exp)*/
void exp_big_integer(BigInteger *bigint, uint64_t exp) {
    if (exp==0) {
        zero_big_integer(bigint);
        append_most_significant(bigint, 1);
    } else if (exp != 1) {
        BigInteger *mul_val = deep_copy_big_integer(bigint);

        while (exp-- > 1) {
            mul_big_integers(bigint, mul_val);
        }
    }
}

/*Returns the value of bigint! (in place)*/
void fact_big_integers(BigInteger *bigint) {
    BigInteger *count = deep_copy_big_integer(bigint);
    count->sign = 0;
    sub_big_integer(count, 1);

    while (count->size != 0) {
        mul_big_integers(bigint, count);
        sub_big_integer(count, 1);
    }
}


/*Divides the whole BigInteger in place by divisor, returns the remainder, goes from most to least significant carrying the remainder into each block*/
uint64_t bigint_divmod_inplace(BigInteger *num, uint64_t divisor) {
    uint64_t remainder = 0;

    BigInteger_Block *block = num->most_significant;
    while (block != NULL) {
        uint64_t new_block_val;
        remainder = divmod128_by10(remainder, block->integer, &new_block_val);
        block->integer = new_block_val;
        block = block->less_significance;
    }

    return remainder;
}

/*Multiply entire BigInteger by 10 and add a single digit in one pass*/
static void bigint_mul10_add(BigInteger *bigint, uint64_t digit) {
    uint64_t carry = digit;

    BigInteger_Block *block = bigint->least_significant;
    while (block != NULL) {
        uint64_t x = block->integer;

        uint64_t lo = x & 0xFFFFFFFF;
        uint64_t hi = x >> 32;

        uint64_t lo10 = lo * 10;
        uint64_t hi10 = hi * 10;

        uint64_t new_carry = hi10 >> 32;
        uint64_t result = (hi10 << 32) + lo10;
        if (result < lo10) new_carry++;

        result += carry;
        if (result < carry) new_carry++;

        block->integer = result;
        carry = new_carry;
        block = block->more_significance;
    }

    if (carry > 0) {
        append_most_significant(bigint, carry);
    }
}

/*Returns true if the whole BigInteger is zero*/
int bigint_is_zero(BigInteger *bigint) {
    remove_leading_zeros(bigint);
    return bigint->size == 0;
}

/*Produces a heap allocated string for the BigInteger, caller is responsible for freeing the buffer*/
char* bigint_to_string(BigInteger* bigint) {
    BigInteger *deep_copy = deep_copy_big_integer(bigint);

    size_t max_digits = (size_t)(deep_copy->size * 64 * LOG2) + 3;
    char *buf = malloc(max_digits);
    if (!buf) return NULL;

    size_t len = 0;

    if (bigint_is_zero(deep_copy)) {
        buf[len++] = '0';
    } else {
        while (!bigint_is_zero(deep_copy)) {
            uint64_t digit = bigint_divmod_inplace(deep_copy, 10);
            buf[len++] = '0' + (char)digit;
        }
    }

    if (deep_copy->sign > 0) buf[len++] = '-';
    buf[len] = '\0';

    //reverse buffer
    for (size_t i = 0, j = len - 1; i < j; i++, j--) {
        char tmp = buf[i];
        buf[i] = buf[j];
        buf[j] = tmp;
    }

    remove_big_integer(deep_copy);

    return buf;
}

/*Converts a decimal string to a Big Integer.*/
BigInteger* string_to_bigint(const char *str) {
    BigInteger *result = define_big_integer(0, 0, NULL);

    if (*str == '-') {result->sign = 1; str++;}
    else if (*str == '+') {str++;}

    append_most_significant(result, 0);

    for (; *str != '\0'; str++) {
        if (*str < '0' || *str > '9') break;
        bigint_mul10_add(result, (uint64_t)(*str - '0'));
    }

    return result;
}

/*Converts a mantissa and exponent to a Big Integer*/
BigInteger* scientific_to_bigint(uint64_t mantissa, uint64_t exponent) {
    BigInteger *result = define_big_integer(0, 0, NULL);

    uint64_t lo = mantissa & 0xFFFFFFFF;
    uint64_t hi = mantissa >> 32;

    if (hi > 0) {
        append_most_significant(result, hi);
        append_least_significant(result, lo);
    } else {
        append_most_significant(result, lo);
    }

    for (uint64_t i = 0; i < exponent; i++) {
        bigint_mul10_add(result, 0);
    }

    return result;
}