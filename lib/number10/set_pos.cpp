#include "../Number10.hpp"

void Number10::set (uint8_t num, uint8_t pos) {
  if (num < 10 && pos < 4) {
    uint8_t calc_pos = 16 - pos * 4;
    this->res = this->res & ~(0xF << calc_pos) | num << calc_pos;
  }
  this->res = cast_to_digits (num);
}
