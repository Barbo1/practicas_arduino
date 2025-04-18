#include "../Display.hpp"

void Display4::display (int miliseconds, bool masked) {
  Timer timer(miliseconds);

  timer.init ();
  if (masked) {
    int i = 0;

    while (this->num.get (i) == 0 && i < 3) { i++; }
    while (timer.finished ()) {
      for (int j = i; j <= 3; j++) {
        display_dig (num_mask (this->num.get (j)), j);
      }
    }
  } else {
    while (timer.finished ()) {
      display_dig (num_mask (this->num.get (0)), 0);
      display_dig (num_mask (this->num.get (1)), 1);
      display_dig (num_mask (this->num.get (2)), 2);
      display_dig (num_mask (this->num.get (3)), 3);
    }
  }
  timer.close ();
  clear_display ();
}
