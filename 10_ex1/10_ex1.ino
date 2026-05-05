#define F_CPU 16000000UL

#include <avr/io.h>
#include "millis2560.h"

#define set_bit(value, bit)   ((value) |= (1 << (bit)))
#define clear_bit(value, bit) ((value) &= ~(1 << (bit)))

int main(void) {
  // Arduino Mega 13번 핀 = PB7 = 내장 LED
  set_bit(DDRB, 7);
  clear_bit(PORTB, 7);

  uint8_t state = 0;

  millis2560_init();

  unsigned long time_previous;
  unsigned long time_current;

  time_previous = millis2560();

  while (1) {
    time_current = millis2560();

    if ((time_current - time_previous) > 1000) {
      time_previous = time_current;

      state = !state;

      if (state) {
        set_bit(PORTB, 7);
      } else {
        clear_bit(PORTB, 7);
      }
    }
  }

  return 1;
}