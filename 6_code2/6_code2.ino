#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>

int main(void) {
  DDRE |= 0x30; // LED 연결 핀을 출력으로 설정
  PORTE &= ~0x10; // PE4번 핀에 연결된 LED 끄기
  PORTE |= 0x20; // PE5번 핀에 연결된 LED 켜기

  while(1) {
    PORTE ^= 0x30;  // 2개 LED 상태 반전
    _delay_ms(500);
  }

  return 0;
}