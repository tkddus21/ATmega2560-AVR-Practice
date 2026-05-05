#define F_CPU 16000000L

#include <avr/io.h>
#include "millis2560.h"

#define set_bit(value, bit)   ((value) |= (1 << (bit)))
#define clear_bit(value, bit) ((value) &= ~(1 << (bit)))

int main(void) {
  // PE4 = Arduino 2번 LED
  set_bit(DDRE, 4);
  clear_bit(PORTE, 4);

  // PE5 = Arduino 3번 LED
  set_bit(DDRE, 5);
  clear_bit(PORTE, 5);

  uint8_t state_PE4 = 0;
  uint8_t state_PE5 = 0;

  unsigned long previous_PE4;
  unsigned long previous_PE5;
  unsigned long current_time;

  millis2560_init();

  previous_PE4 = millis2560();
  previous_PE5 = millis2560();

  while (1) {
    current_time = millis2560();

    // PE4: 0.5초 간격 점멸
    if ((current_time - previous_PE4) >= 500) {
      previous_PE4 = current_time;

      state_PE4 = !state_PE4;

      if (state_PE4) {
        set_bit(PORTE, 4);
      } else {
        clear_bit(PORTE, 4);
      }
    }

    // PE5: 1초 간격 점멸
    if ((current_time - previous_PE5) >= 1000) {
      previous_PE5 = current_time;

      state_PE5 = !state_PE5;

      if (state_PE5) {
        set_bit(PORTE, 5);
      } else {
        clear_bit(PORTE, 5);
      }
    }
  }

  return 0;
}