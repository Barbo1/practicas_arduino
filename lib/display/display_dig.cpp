#include "../Display.hpp"

void Display4::display_dig (uint8_t num, uint8_t pos) {
  PORTD = (0x3F & num) << 2;
  PORTB = (0xC0 & num) >> 6;
  PORTB |= (0x0F ^ 1 << pos) << 2;
  _delay_ms (6);
}
