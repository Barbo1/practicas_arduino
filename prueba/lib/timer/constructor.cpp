#include "../Timer.hpp"

Timer::Timer (int bound) {
  this->bound = bound & 0xFF;
}
