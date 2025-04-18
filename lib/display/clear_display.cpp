#include "../Display.hpp"

void Display4::clear_display () {
  PORTB |= 0x3C;
}
