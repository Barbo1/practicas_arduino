#include <avr/io.h>
#include <util/delay.h>


/* * * * * * * * * * * * * * * * * * * * * * * *
 * functions to convert from number to digits. *
 * * * * * * * * * * * * * * * * * * * * * * * */

using _digits = struct {int n1; int n2; int n3; int n4;};

_digits cast_to_digits (int num) {
  static constexpr int dig_pos_summer_1[14] = {1,2,4,8,6,2,4,8,6,2,4,8,6,2};
  static constexpr int dig_pos_summer_2[14] = {0,0,0,0,1,3,6,2,5,1,2,4,9,9};
  static constexpr int dig_pos_summer_3[14] = {0,0,0,0,0,0,0,1,2,5,0,0,0,1};
  static constexpr int dig_pos_summer_4[14] = {0,0,0,0,0,0,0,0,0,0,1,2,4,8};

  _digits ret = {0,0,0,0};
  int i = 0;
  bool b;
  while (i < 14) {
    if ((1 << i) & num) {
      ret.n1 += dig_pos_summer_1[i];
      ret.n2 += dig_pos_summer_2[i];
      ret.n3 += dig_pos_summer_3[i];
      ret.n4 += dig_pos_summer_4[i];

      b = ret.n1 >= 10;
      ret.n2 += b;
      ret.n1 -= 10 * b;

      b = ret.n2 >= 10;
      ret.n3 += b;
      ret.n2 -= 10 * b;

      b = ret.n3 >= 10;
      ret.n4 += b;
      ret.n3 -= 10 * b;
    }
    i += 1; 
  }

  return ret;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * defining the type Timer that count in miliseconds(aproximatelly). *
 * This class only use the timer 2 and the ASSR register.            *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class Timer {
  private:
    int bound;

  public: 
    Timer (int bound) {
      if (bound > 255) {
        this->bound = 255;
      } else {
        this->bound = bound;
      }
    }

    void init () {
      ASSR |= 1 << EXCLK;
      ASSR |= 1 << AS2;
      while (ASSR & ((1 << AS2) | (1 << TCN2UB) | (1 << OCR2AUB) | (1 << TCR2AUB)) != 0);
      TCNT2 = 0;
      TCCR2A |= (1 << COM2A0) | (1 << WGM20);
      TCCR2B |= (1 << CS20) |(1 << CS21) | (1 << CS22);
      OCR2A = this->bound;
      ASSR ^= 1 << EXCLK;
    }

    bool finished () {
      return (TIFR2 & (1 << OCF2A)) == 0;
    }

    void close () {
      TIFR2 |= (1 << OCF2A);
    }
};


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * defining the type Display4Number to manipulate the 4 digit screen.                    *
 * This class use the following bits:                                                    *
 *   PINB5, PINB4, PINB3, PINB2, PINB1, PINB0, PIND7, PIND6, PIND5, PIND4, PIND3, PIND2  *
 *     D4     D3     D2     D1     DP     G      F      E      D      C      B      A    *
 *                                                                                       *
 *  This class is meant to show integer numbers, and counting the number as a whole. It  *
 *  cannot split up the screen to show various numbers.                                  *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

enum _display_type {
  WHOLE,
  D_31,
  D_13,
  D_22,
  D_1111,
};

class Display4Number {
  private:
    _digits nums;
    _display_type t;

    int num_mask (int number) {
      switch (number) {
        case 0: return 0b00111111;
        case 1: return 0b00000110;
        case 2: return 0b01011011;
        case 3: return 0b01001111;
        case 4: return 0b01100110;
        case 5: return 0b01101101;
        case 6: return 0b01111101;
        case 7: return 0b00000111;
        case 8: return 0b01111111;
        case 9: return 0b01101111;
      }
      return 0;
    }

    void display_dig (int numero, int posicion) {
      PORTD = (0x3F & numero) << 2;
      PORTB = (0xC0 & numero) >> 6;
      PORTB |= (0x0F ^ 1 << posicion) << 2;
      _delay_ms (6);
    }

    void config () {
      DDRD = 0xFC;
      DDRB = 0x3F;
    }

    void clear_display () {
      PORTB |= 0x3C;
    }

  public:
    Display4Number (int n) {
      this->nums = (n <= 9999 ? cast_to_digits (n) : _digits {0,0,0,0});
      this->t = WHOLE;
      config ();
    }

    Display4Number (int n2, int n1);
    Display4Number (int n3, int n2, int n1, bool order);
    Display4Number (int n4, int n3, int n2, int n1);

    void set_n1 (int n) {
      this->nums.n1 = n;
    }

    void set_n2 (int n) {
      this->nums.n2 = n;
    }

    void set_n3 (int n) {
      this->nums.n3 = n;
    }

    void set_n4 (int n) {
      this->nums.n4 = n;
    }

    void count_sum (int addend) {
      _digits result = (addend <= 9999 ? cast_to_digits (addend) : _digits {0,0,0,0});
      this->nums.n1 += result.n1;
      this->nums.n2 += result.n2;
      this->nums.n3 += result.n3;
      this->nums.n4 += result.n4;

      bool b = this->nums.n1 >= 10;
      this->nums.n2 += b;
      this->nums.n1 -= 10 * b;

      b = this->nums.n2 >= 10;
      this->nums.n3 += b;
      this->nums.n2 -= 10 * b;

      b = this->nums.n3 >= 10;
      this->nums.n4 += b;
      this->nums.n3 -= 10 * b;
      
      b = this->nums.n4 >= 10;
      this->nums.n4 -= 10 * b;
    }

    void display (int miliseconds, bool masked = false) {
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
};

int main () {
  Display4Number dis = Display4Number(32);
  while (1) {
    dis.display (200, true);
    dis.count_sum (1);
  }
}
