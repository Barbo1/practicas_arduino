#include "./Timer.hpp"
#include "./Number10.hpp"

#include <avr/io.h>
#include <util/delay.h>

/* * * * * * * * * * * * * * * * * * * * * * * *
 * functions to convert from number to digits. *
 * * * * * * * * * * * * * * * * * * * * * * * */

class Display4 {
  private:
    Number10 num;

    uint8_t num_mask (uint8_t num);

    void display_dig (uint8_t num, uint8_t pos);
    void clear_display ();

    void config ();

  public:
    Display4 (int n);

    void count_sum (int addend);

    void set (int16_t newn);
    void set (int8_t newn, uint8_t pos);

    void display (int miliseconds, bool masked = false);
};
