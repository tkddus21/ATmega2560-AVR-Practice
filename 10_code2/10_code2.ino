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

ISR(TIMER0_COMPA_vect) {
  count++;
  TCNT0 = 0;  //카운터 값 초기화

}

int main(void) {
  set_bit(DDRB, 7); //PB7 핀을 출력으로 설정
  clear_bit(PORTB, 7); // LED는 끈 상태에서 시작

  TCCR0B |= (1 << CS02) | (1 << CS00); //분주비를 1024로 설정 101 -> 1024/ 16MHz = 0.064 ms
  TIMSK0 |= (1 << OCIE0A); // 오버플로 인터럽트 허용 비교일치 인터럽트 A 허용
  OCR0A = 128; // 비교일치 기준 값 -> 0.064 ms * 128 = 8.192ms

  sei();  //전역적으로 인터럽트 허용

  while(1) {
    if(count == 64) { //오버플로 64회 발생 = 0.5초 경과
      count = 0; //인터럽트 발생 횟수 초기화
      state  = !state; // LED 상태 반전
      if (state) set_bit(PORTB, 7); // LED 켜기
      else clear_bit(PORTB, 7); //LED 끄기
    }
  }

  return 0;
}