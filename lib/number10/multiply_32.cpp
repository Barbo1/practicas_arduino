#include "../Number10.hpp"

void multiply_32 (uint16_t a, uint16_t b, uint16_t &high, uint16_t &low) {
  uint16_t r = 1, al = a, ah = 0; 
  low = 0; 
  high = 0;
  bool transmit;
  for (int i = 0; i < 16; i++) {
    if (r & b) {
      transmit = low > 0 && low > 0xFFFF - al;
      low = low + al;
      high = high + ah + transmit;
    }
    transmit = 0x8000 & al;
    al <<= 1;
    ah <<= 1;
    if (transmit) {
      ah |= 1;
    }
    r <<= 1;
  }
}

