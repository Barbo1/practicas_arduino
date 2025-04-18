#include <avr/io.h>
#include <util/delay.h>

/* utilizacion de los puertos B y D, de la siguiente manera:
 *    PINB1, PINB0, PIND7, PIND6, PIND5, PIND4, PIND3, PIND2
 *      DP     G      F      E      D      C      B      A
 *  
 *  de esta manera los numeros dentro del arreglo sec son arreglos de bits en el mismo orden, 
 *  que luego se utilizan para enviarlos por los puertos. 
 * */


void imprimir_numero (int a) {
  PORTD = (0x3F & a) << 2;
  PORTB = (0xC0 & a) >> 6;
}

int const SEQTOP = 22;

int sec[22] = {
  0b00001000, 0b00010000, 0b00100000, 0b00000001, 0b00000010, 0b00000100, 
  0b00001000, 0b00010000, 0b01000000, 0b00000010, 0b00000001, 0b00100000, 
  0b01000000, 0b00000100, 0b00001000, 0b00010000, 0b01000000, 0b00000010, 
  0b00000001, 0b00100000, 0b00010000, 0b00001000
};

int main () {
  DDRD = 0xFC;
  DDRB = 0x03;

  while (1) {
    imprimir_numero (0b10000000);
    _delay_ms(600);
    for (int i = 0; i < SEQTOP; i++) {
      imprimir_numero (sec [i]);
      _delay_ms(100);
    } 
  }
  return 0;
}
