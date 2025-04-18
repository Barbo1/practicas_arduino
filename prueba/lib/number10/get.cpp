#include "../Number10.hpp"

uint8_t Number10::get (uint8_t pos) {
  uint8_t fin_pos = (pos & 3) * 4;
  return (this->res >> fin_pos) & 0xF;
}
