#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>
#include "UART0.h"

int main(void) {
    uint8_t count = 0;      // 카운트 변수
    uint8_t buttonState = 0; // 현재 버튼 상태
    uint8_t lastButtonState = 0; // 이전 버튼 상태

    // 1. 14번 핀(PJ1)을 입력 모드로 설정
    DDRJ &= ~(1 << PJ1); 

    UART0_init(); // UART 초기화 (9600bps)

    while (1) {
        // 2. 버튼 상태 읽기 (PINJ 레지스터 사용)
        if (PINJ & (1 << PJ1)) {
            buttonState = 1; // 눌림 (HIGH)
        } else {
            buttonState = 0; // 안 눌림 (LOW)
        }

        // 3. 버튼이 눌리는 순간(Rising Edge) 감지
        if (buttonState == 1 && lastButtonState == 0) {
            count++; // 카운트 증가
            
            // 4. UART로 상태 전송 (슬라이드 출력 양식)
            UART0_print("Count is ");
            UART0_print(count);
            UART0_write('\n');
            
            _delay_ms(50); // 버튼 채터링(떨림) 방지를 위한 짧은 지연
        }

        lastButtonState = buttonState; // 상태 업데이트
    }

    return 0;
}