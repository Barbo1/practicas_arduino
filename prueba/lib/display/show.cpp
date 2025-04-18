#include "../Display.hpp"

template <_display_type T>
void Display4<T>::display (int miliseconds, bool masked) {
  Timer timer(miliseconds);
  timer.init ();
  if (masked) {
    int i = 3;

    while (timer.finished ()) {
      while (*((int *)&this->nums + i) == 0 && i > 0) { i--; }
      while (i > -1) {
        display_dig (num_mask (*((int *)&this->nums + i)), i);
        i--;
      }
    }
  } else {
    while (timer.finished ()) {
      display_dig (num_mask (this->nums.n1), 0);
      display_dig (num_mask (this->nums.n2), 1);
      display_dig (num_mask (this->nums.n3), 2);
      display_dig (num_mask (this->nums.n4), 3);
    }
  }
  timer.close ();
  clear_display ();
}
