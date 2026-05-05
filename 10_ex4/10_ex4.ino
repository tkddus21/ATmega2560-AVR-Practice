#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>

#define set_bit(value, bit)   ((value) |= (1 << (bit)))
#define clear_bit(value, bit) ((value) &= ~(1 << (bit)))

volatile int count0 = 0;   // Timer0 비교일치 횟수
volatile int count2 = 0;   // Timer2 비교일치 횟수

uint8_t state_PE4 = 0;
uint8_t state_PE5 = 0;

// Timer0 Compare Match A Interrupt
ISR(TIMER0_COMPA_vect) {
  count0++;
  TCNT0 = 0;   // Normal mode이므로 직접 초기화
}

// Timer2 Compare Match A Interrupt
ISR(TIMER2_COMPA_vect) {
  count2++;
  TCNT2 = 0;   // Normal mode이므로 직접 초기화
}

int main(void) {
  // PE4 = Arduino 2번 LED
  set_bit(DDRE, 4);
  clear_bit(PORTE, 4);

  // PE5 = Arduino 3번 LED
  set_bit(DDRE, 5);
  clear_bit(PORTE, 5);

  // -------------------------
  // Timer0 설정: 0.5초용
  // -------------------------
  TCCR0A = 0x00;
  TCCR0B = 0x00;
  TCNT0 = 0;

  OCR0A = 128;   // 비교일치 기준값

  // Timer0 분주비 1024
  TCCR0B |= (1 << CS02) | (1 << CS00);

  // Timer0 비교일치 A 인터럽트 허용
  TIMSK0 |= (1 << OCIE0A);

  // -------------------------
  // Timer2 설정: 1초용
  // -------------------------
  TCCR2A = 0x00;
  TCCR2B = 0x00;
  TCNT2 = 0;

  OCR2A = 128;   // 비교일치 기준값

  // Timer2 분주비 1024
  // Timer2는 CS22, CS21, CS20 = 111
  TCCR2B |= (1 << CS22) | (1 << CS21) | (1 << CS20);

  // Timer2 비교일치 A 인터럽트 허용
  TIMSK2 |= (1 << OCIE2A);

  // 전역 인터럽트 허용
  sei();

  while (1) {
    // Timer0: OCR0A=128, 64회 ≈ 0.5초
    if (count0 >= 64) {
      count0 = 0;
      state_PE4 = !state_PE4;

      if (state_PE4) {
        set_bit(PORTE, 4);
      } else {
        clear_bit(PORTE, 4);
      }
    }

    // Timer2: OCR2A=128, 128회 ≈ 1초
    if (count2 >= 128) {
      count2 = 0;
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