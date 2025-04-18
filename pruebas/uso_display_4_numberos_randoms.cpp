#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>


/* * * * * * * * * * * * * * * * * * * * * * * *
 * Functions to make calculations.             *
 * * * * * * * * * * * * * * * * * * * * * * * */

void multiply_32 (uint16_t a, uint16_t b, uint16_t &high, uint16_t &low) {
  uint16_t r = 1, al = a, ah = 0; 
  low = 0; 
  high = 0;
  bool transmit;
  for (int i = 0; i < 16; i++) {
    if (r & b) {
      transmit = low > 0 && low > 0xFFFF - al;
      low = low + al;
      high = high + ah + transmit;
    }
    transmit = 0x8000 & al;
    al <<= 1;
    ah <<= 1;
    if (transmit) {
      ah |= 1;
    }
    r <<= 1;
  }
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * defining the type Number10 that represents numbers base 10.       *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class Number10 {
  private: 
    uint16_t res;

    Number10 () {}
    
    uint16_t cast_to_digits (int16_t num) {
      uint16_t unum;
      if (num < 0) unum = -num;
      else unum = num;

      uint16_t low, high;
      multiply_32 (13422, unum, high, low);
      unum -= 10000 * (high >> 11);

      uint16_t n1;
      multiply_32 (10486, unum, n1, low);
      n1 >>= 4;

      uint16_t n2 = unum - 100 * n1;

      uint16_t n3;
      multiply_32 (6554, n2, n3, low);

      uint16_t n4;
      multiply_32 (6554, n1, n4, low);

      uint16_t nf;
      if (num < 0) nf = 0xF;
      else nf = n4;

      return (n2 - 10*n3) << 12 | n3 << 8 | (n1 - 10*n4) << 4 | nf;
    }

  public:
    Number10 (uint16_t num) {
      this->res = cast_to_digits (num);
    }

    void set (uint16_t num) {
      this->res = cast_to_digits (num);
    }

    void set (uint8_t num, uint8_t pos) {
      if (num < 10 && pos < 4) {
        uint8_t calc_pos = 16 - pos * 4;
        this->res = this->res & ~(0xF << calc_pos) | num << calc_pos;
      }
      this->res = cast_to_digits (num);
    }

    uint8_t get (uint8_t pos) {
      uint8_t fin_pos = (pos & 3) * 4;
      return (this->res >> fin_pos) & 0xF;
    }

    Number10 operator+ (Number10 num) {
      uint16_t t1 = this->res + 0x6666;
      uint16_t t2 = t1 + num.res;
      uint16_t t3 = t1 ^ num.res;
      uint16_t t4 = t3 ^ t2;
      uint16_t t5 = ~t4 ^ 0x1110;
      uint16_t t6 = (t5 << 3) | (t5 << 2);
      Number10 res = Number10 ();
      res.res = t2 - t6;
      return res;
    }
};


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * defining the type Random that is used to generate a sequence of   *
 * random numbers. The sequence is calculates using the linear       *
 * congruential method, where the seed is the modulus.               *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class Random { 
  private: 
    uint32_t c, a, xN;

    static constexpr uint32_t primes[2] {
      641, 6700417
    };
    
    static constexpr uint32_t relatives[32] {
      19, 8191, 337, 431, 89, 257, 2833, 353, 4649, 673, 193, 449, 9901, 97, 
      1777, 2113, 65537, 113, 953, 683, 1321, 26317, 37171, 1013, 1657, 1103, 
      13367, 5419, 1429, 20857, 241, 101
    };
    
  public:
    Random (uint32_t seed) {
      /* Calculating c... */
      this->c = 1;
      for (int i = 0; i < 16; i++) {
        if (seed & (1 << i)) {
          this->c *= relatives[i];
        }
      }

      /* Calculating a... */
      this->a = 2;
      seed = (~seed >> 5) + 0x0000FFFF;
      for (int i = 16; i < 32; i++) {
        if (seed & (1 << i)) {
          this->a *= relatives[i];
        }
      }
      this->a += 1;
      
      /* Set xN to 0... */
      this->xN = 0;
    }

    uint32_t generate () {
      this->xN = this->a * this->xN + this->c;
      return this->xN;
    }
};


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * defining the type Timer that count in miliseconds(aproximatelly). *
 * This class only use the timer 1.                                  *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class Timer {
  private:
    int bound;

  public: 
    Timer (int bound) {
      this->bound = bound & 0xFF;
    }

    void init () {
      OCR1A = this->bound * 67;
      TCCR1B = (1 << WGM12) | (1 << CS12) | (1 << CS10);
    }

    bool finished () {
      return (TIFR1 & (1 << OCF1A)) == 0;
    }

    void close () {
      TIFR1 |= (1 << OCF1A);
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

class Display4 {
  private:
    Number10 num;

    uint8_t num_mask (uint8_t num) {
      switch (num) {
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
        case 15: return 0b01000000;
        default: return 0;
      }
    }

    void display_dig (uint8_t num, uint8_t pos) {
      PORTD = (0x3F & num) << 2;
      PORTB = (0xC0 & num) >> 6;
      PORTB |= (0x0F ^ 1 << pos) << 2;
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
    Display4 (int n): num (n) {
      config ();
    }

    void count_sum (int addend) {
      this->num = this->num + Number10(addend);
    }

    void set (int16_t newn) {
      this->num = Number10 (newn);
    }

    void set (int8_t newn, uint8_t pos) {
      this->num.set (newn, pos);
    }

    void display (int miliseconds, bool masked = false) {
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
};

int main () {
  Random rand = Random (19824);
  Display4 dis = Display4 (0);
  while (1) {
    dis.set (rand.generate() & 0x7FFF);
    dis.display (1000, true);
  }
}
