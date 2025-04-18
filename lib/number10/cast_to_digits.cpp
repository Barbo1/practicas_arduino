#include "../Number10.hpp"

uint16_t Number10::cast_to_digits (int16_t num) {
  uint16_t unum;
  if (num < 0) unum = -num;
  else unum = num;

  uint16_t low, high;
  multiply_32 (13422, unum, high, low);
  unum -= 10000 * (high >> 11);

  uint16_t n1;
  multiply_32 (10486, unum, n1, low);
  n1 >>= 4;

  uint16_t n2 = unum - 100 * n1;

  uint16_t n3;
  multiply_32 (6554, n2, n3, low);

  uint16_t n4;
  multiply_32 (6554, n1, n4, low);

  uint16_t nf;
  if (num < 0) nf = 0xF;
  else nf = n4;

  return (n2 - 10*n3) << 12 | n3 << 8 | (n1 - 10*n4) << 4 | nf;
}

