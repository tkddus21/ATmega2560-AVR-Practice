#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>

#define set_bit(value, bit)   ((value) |= (1 << (bit)))
#define clear_bit(value, bit) ((value) &= ~(1 << (bit)))

volatile int count = 0;
uint8_t state_PE4 = 0;

// Timer0 Compare Match A 인터럽트
ISR(TIMER0_COMPA_vect) {
  count++;
}

int main(void) {
  // PB7 = Arduino 13번 = OC0A = 빠르게 점멸
  set_bit(DDRB, 7);
  clear_bit(PORTB, 7);

  // PE4 = Arduino 2번 = 0.5초 간격 점멸
  set_bit(DDRE, 4);
  clear_bit(PORTE, 4);

  // Timer0 초기화
  TCCR0A = 0x00;
  TCCR0B = 0x00;
  TCNT0 = 0;

  OCR0A = 255;

  // 비교일치 발생 시 OC0A(PB7) 자동 반전
  TCCR0A |= (1 << COM0A0);

  // CTC 모드 설정
  TCCR0A |= (1 << WGM01);

  // 비교일치 A 인터럽트 허용
  TIMSK0 |= (1 << OCIE0A);

  // 분주비 1024 설정
  TCCR0B |= (1 << CS02) | (1 << CS00);

  sei();

  while (1) {
    if (count >= 32) {
      count = 0;
      state_PE4 = !state_PE4;

      if (state_PE4) {
        set_bit(PORTE, 4);
      } else {
        clear_bit(PORTE, 4);
      }
    }
  }

  return 0;
}