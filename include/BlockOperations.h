#include <stdint.h>

uint64_t divmod128(uint64_t divisor, uint64_t hi, uint64_t lo, uint64_t *out_quotient_lo);
uint64_t mul64(uint64_t a, uint64_t b, uint64_t *hi_out);