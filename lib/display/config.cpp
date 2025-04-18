#include "../Display.hpp"

void Display4::config () {
  DDRD = 0xFC;
  DDRB = 0x3F;
}
