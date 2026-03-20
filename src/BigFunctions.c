#include "BigFunctions.h"

/*Returns the value of bigint! (in place)*/
void fact_big_integers(BigInteger *bigint) {
    BigInteger *count = deep_copy_big_integer(bigint);
    count->sign = 0;
    sub_big_integer(count, 1);

    while (count->size != 0) {
        mul_big_integers(bigint, count);
        sub_big_integer(count, 1);
    }

    remove_big_integer(count);
}