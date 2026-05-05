#define F_CPU 16000000L //동작주파수 (16MHZ)설정, "L"long형
#include <avr/io.h>
#include <util/delay.h>

int main(void) {
  DDRE |= 0x30;

  while(1) {
    PORTE |= 0x30;
    _delay_ms(500);
    PORTE &= ~0x30;
    _delay_ms(500);
  }

  return 0;
}