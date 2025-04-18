/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * defining the type Timer that count in miliseconds(aproximatelly). *
 * This class only use the timer 1.                                  *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <avr/io.h>

class Timer {
  private:
    int bound;

  public: 
    Timer (int bound);

    void init ();
    bool finished ();
    void close ();
};
