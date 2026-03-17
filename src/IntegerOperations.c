#include "IntegerOperations.h"
#include <stdint.h>

/*Divide a 128 bit number by 10, returns remainder and writes quotient as out_quotient_lo*/
uint64_t divmod128(uint64_t divisor, uint64_t hi, uint64_t lo, uint64_t *out_quotient_lo) {
  uint64_t q_hi = hi / divisor;
  uint64_t r_hi = hi % divisor;

  uint64_t lo_hi = lo >> 32;
  uint64_t lo_lo = lo & 0xFFFFFFFF;

  uint64_t mid = (r_hi << 32) | lo_hi;
  uint64_t q_mid = mid / divisor;
  uint64_t r_mid = mid % divisor;

  uint64_t low = (r_mid << 32) | lo_lo;
  uint64_t q_low = low / divisor;
  uint64_t r_low = low % divisor;

  *out_quotient_lo = (q_mid << 32) | q_low;

  (void)q_hi; //not needed
  return r_low;
}

/*Multiplies two 64 bit integers together, retuns the low and assigns 'hi_out' as the high.*/
uint64_t mul64(uint64_t a, uint64_t b, uint64_t *hi_out) {
  uint64_t a_lo = a & 0xFFFFFFFF,  a_hi = a >> 32;
  uint64_t b_lo = b & 0xFFFFFFFF,  b_hi = b >> 32;

  uint64_t ll = a_lo * b_lo;
  uint64_t lh = a_lo * b_hi;
  uint64_t hl = a_hi * b_lo;
  uint64_t hh = a_hi * b_hi;

  uint64_t mid = (ll >> 32) + (lh & 0xFFFFFFFF) + (hl & 0xFFFFFFFF);
  *hi_out = hh + (lh >> 32) + (hl >> 32) + (mid >> 32);

  return (mid << 32) | (ll & 0xFFFFFFFF);
}