#include "../Timer.hpp"

bool Timer::finished () {
  return (TIFR1 & (1 << OCF1A)) == 0;
}
