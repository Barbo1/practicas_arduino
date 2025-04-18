#include <avr/io.h>
#include <stdint.h>

void multiply_32 (uint16_t a, uint16_t b, uint16_t &high, uint16_t &low);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * defining the type Number10 that represents numbers base 10.       *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class Number10 {
  private: 
    uint16_t res;

    Number10 ();
    
    uint16_t cast_to_digits (int16_t num);

  public:
    Number10 (uint16_t num);

    void set (uint16_t num);

    void set (uint8_t num, uint8_t pos);

    uint8_t get (uint8_t pos);

    Number10 operator+ (Number10 num);
};
