#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>

#define set_bit(value, bit)   ((value) |= (1 << (bit)))
#define clear_bit(value, bit) ((value) &= ~(1 << (bit)))

volatile int count = 0;
uint8_t state = 0;

// Timer2 오버플로 인터럽트
ISR(TIMER2_OVF_vect) {
  count++;
}

int main(void) {
  // PB7 = Arduino 13번 LED
  set_bit(DDRB, 7);
  clear_bit(PORTB, 7);

  // Timer2 초기화
  TCCR2A = 0x00;
  TCCR2B = 0x00;
  TCNT2 = 0;

  // 🔥 1024 분주 설정 (Timer2는 다름!)
  TCCR2B |= (1 << CS22) | (1 << CS21) | (1 << CS20);

  // 오버플로 인터럽트 허용
  TIMSK2 |= (1 << TOIE2);

  // 전역 인터럽트 허용
  sei();

  while (1) {
    if (count >= 32) {   // 약 0.5초
      count = 0;
      state = !state;

      if (state) {
        set_bit(PORTB, 7);
      } else {
        clear_bit(PORTB, 7);
      }
    }
  }

  return 0;
}