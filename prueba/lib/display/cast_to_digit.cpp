#include "../Display.hpp"

template <_display_type T>
_digits cast_to_digits (int num) {
  static constexpr int dig_pos_summer_1[14] = {1,2,4,8,6,2,4,8,6,2,4,8,6,2};
  static constexpr int dig_pos_summer_2[14] = {0,0,0,0,1,3,6,2,5,1,2,4,9,9};
  static constexpr int dig_pos_summer_3[14] = {0,0,0,0,0,0,0,1,2,5,0,0,0,1};
  static constexpr int dig_pos_summer_4[14] = {0,0,0,0,0,0,0,0,0,0,1,2,4,8};

  _digits ret = {0,0,0,0};
  int i = 0;
  bool b;
  while (i < 14) {
    if ((1 << i) & num) {
      ret.n1 += dig_pos_summer_1[i];
      ret.n2 += dig_pos_summer_2[i];
      ret.n3 += dig_pos_summer_3[i];
      ret.n4 += dig_pos_summer_4[i];

      b = ret.n1 >= 10;
      ret.n2 += b;
      ret.n1 -= 10 * b;

      b = ret.n2 >= 10;
      ret.n3 += b;
      ret.n2 -= 10 * b;

      b = ret.n3 >= 10;
      ret.n4 += b;
      ret.n3 -= 10 * b;
    }
    i += 1; 
  }

  return ret;
}
