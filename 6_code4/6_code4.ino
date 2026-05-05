#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>

int main(void) {
  //아두이노 Serial 클래스를 사용하여 UART 시리얼 통신을 수행
  init();
  Serial.begin(9600); // UART 시리얼 통신 초기화 -> baud rate 9600 설정

  // 이전과 현재 버튼의 상태
  uint8_t state_previous = 0, state_current;
  int count = 0;  //버튼이 눌린 횟수

  DDRJ &= ~0x01;  // 버튼이 연결된 핀을 입력으로 설정

  while(1) {  
    state_current = PINJ & 0x01; //버튼 상태 읽기

    if(state_current == 1) {      //버튼이 눌린 경우
      if(state_previous == 0) {   // 버튼의 이전 상태가 눌리지 않은 경우
        count++;                  // 버튼을 누른 횟수 증가
        Serial.println(count);    // 시리얼 모니터로 버튼을 누른 횟수 출력
      }
      _delay_ms(50);              // 디바운싱
    }
    state_previous = state_current; // 버튼의 이전 상태 갱신

  }

  return 0;
}