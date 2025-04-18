#include <avr/io.h>
#include <util/delay.h>

/* utilizacion de los puertos B y D, de la siguiente manera:
 *    PINB1, PINB0, PIND7, PIND6, PIND5, PIND4, PIND3, PIND2
 *      DP     G      F      E      D      C      B      A
 *  
 *  de esta manera mostrar_numero retorna un arreglo de bits en el mismo orden, 
 *  que luego se utilizan para enviarlos por los puertos. 
 * */

int mostrar_numero (char let) {
  switch (let) {
    case '0': return 0b00111111;
    case '1': return 0b00000110;
    case '2': return 0b01011011;
    case '3': return 0b01001111;
    case '4': return 0b01100110;
    case '5': return 0b01101101;
    case '6': return 0b01111101;
    case '7': return 0b00000111;
    case '8': return 0b01111111;
    case '9': return 0b01101111;
  }
  return 0;
}

void imprimir_numero (int i) {
  int a = mostrar_numero ((char)(i + 48));
  PORTD = (0x3F & a) << 2;
  PORTB = (0xC0 & a) >> 6;
}

int main () {
  DDRD = 0xFC;
  DDRB = 0x03;

  int i;
  while (1) {
    i++;
    if (i > 9) i = 0;
    imprimir_numero (i);
    _delay_ms(500);
  }
  return 0;
}
