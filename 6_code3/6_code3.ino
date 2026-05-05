#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>

#define set_bit(value, bit)   (_SFR_BYTE(value) |= _BV(bit))
#define clear_bit(value, bit) (_SFR_BYTE(value) &= ~_BV(bit))


int main(void) {
  DDRJ &= ~0x01; // 풀다운 저항이 연결된 버튼을 입력으로 설정
  DDRH &= ~0x02; // 풀업 저항이 연결된 버튼을 입력으로 설정
  DDRE |= 0x20; // LED가 연결된 포트 E번 핀을 출력으로 설정
  DDRG |= 0x20; // LED가 연결된 포트 G번 핀을 출력으로 설정

  while(1) {
    // 풀다운 저항이 연결된 버튼 상태 읽기
    uint8_t btn_pulldown = PINJ & 0x01;
    // 풀업 저항이 연결된 버튼 상태 읽기
    uint8_t btn_pullup = (PINH >> 1) & 0x01;

    // 풀다운 저항이 연결된 버튼 상태 LED에 나타내기
    if (btn_pulldown) set_bit(PORTE, 5);
    else clear_bit(PORTE, 5);

    // 풀업 저항이 연결된 버튼 상태 LED에 나타내기
    if(btn_pullup) set_bit(PORTG, 5);
    else clear_bit(PORTG, 5);

  }
  return 0;
}