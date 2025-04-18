#include "../Timer.hpp"

void Timer::init () {
  OCR1A = this->bound * 67;
  TCCR1B = (1 << WGM12) | (1 << CS12) | (1 << CS10);
}
