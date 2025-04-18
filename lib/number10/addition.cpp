#include "../Number10.hpp"

Number10 Number10::operator+ (Number10 num) {
  uint16_t t1 = this->res + 0x6666;
  uint16_t t2 = t1 + num.res;
  uint16_t t3 = t1 ^ num.res;
  uint16_t t4 = t3 ^ t2;
  uint16_t t5 = ~t4 ^ 0x1110;
  uint16_t t6 = (t5 << 3) | (t5 << 2);
  Number10 res = Number10 ();
  res.res = t2 - t6;
  return res;
}
