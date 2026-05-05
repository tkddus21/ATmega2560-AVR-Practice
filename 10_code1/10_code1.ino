#define F_CPU 16000000L
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <util/delay.h>

// 매크로 함수 정의
#define set_bit(value, bit) (value |= (1 << bit))
#define clear_bit(value, bit) (value &= ~(1 << bit))

volatile int count = 0; // 오버플로가 발생한 횟수
uint8_t state = 0;

ISR(TIMER0_OVF_vect) {
  count++;
}

int main(void) {
  set_bit(DDRB, 7); //PB7 핀을 출력으로 설정
  clear_bit(PORTB, 7); // LED는 끈 상태에서 시작

  TCCR0B |= (1 << CS02) | (1 << CS00); //분주비를 1024로 설정 101
  TIMSK0 |= (1 << TOIE0); // 오버플로 인터럽트 허용
  sei();  //전역적으로 인터럽트 허용

  while(1) {
    if(count == 32) { //오버플로 32회 발생 = 0.5초 경과
      count = 0; //인터럽트 발생 횟수 초기화
      state  = !state; // LED 상태 반전
      if (state) set_bit(PORTB, 7); // LED 켜기
      else clear_bit(PORTB, 7); //LED 끄기
    }
  }

  return 0;
}