#ifndef BitInteger_h
#define BitInteger_h

#include <stdint.h>

#define LOG2 0.30103
#define MAX(a,b) (((a) > (b)) ? (a) : (b))

/*head (most significant) -> 123456789 <- tail (least significat)*/

typedef struct BigInteger_Block {
    uint64_t integer;
    struct BigInteger_Block* more_significance;
    struct BigInteger_Block* less_significance;
} BigInteger_Block;

typedef struct BigInteger {
    BigInteger_Block* least_significant;
    BigInteger_Block* most_significant;

    short sign;
    unsigned int size;
} BigInteger;

BigInteger* define_big_integer(short sign, unsigned int size, uint64_t* arr);
void remove_big_integer(BigInteger *bigint);

void append_most_significant(BigInteger *bigint, uint64_t integer);
void append_least_significant(BigInteger *bigint, uint64_t integer);
void remove_most_significant(BigInteger *bigint);
void remove_least_significant(BigInteger *bigint);

int big_integer_compare(BigInteger *greaterint, BigInteger *lesserint);
void zero_big_integer(BigInteger *bigint);
int bigint_is_zero(BigInteger *bigint);

void add_big_integers(BigInteger *resultint, BigInteger *addingint);
void add_big_integer(BigInteger *resultint, uint64_t addingint);
void sub_big_integers(BigInteger *resultint, BigInteger *subtractingint);
void sub_big_integer(BigInteger *resultint, uint64_t subint);
void mul_big_integers(BigInteger *resultint, BigInteger *multiplierint);
void exp_big_integer(BigInteger *bigint, uint64_t exp);
void fact_big_integers(BigInteger *bigint);

uint64_t div_big_integer(BigInteger *bigint, uint64_t divisor);
uint64_t mod_big_integer(BigInteger *bigint, uint64_t divisor);

char* bigint_to_string(BigInteger* bigint);
BigInteger* string_to_bigint(const char *str);
BigInteger* scientific_to_bigint(uint64_t mantissa, uint64_t exponent);

#endif