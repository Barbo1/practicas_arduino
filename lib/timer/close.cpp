#include "../Timer.hpp"

Timer::close () {
  TIFR1 |= (1 << OCF1A);
}
