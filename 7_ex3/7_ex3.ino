#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>
#include "UART0.h"

int main(void) {
    char data;

    // 1. 사용할 핀(2, 3, 4, 5번)을 출력 모드로 설정
    // Pin 2: PE4, Pin 3: PE5, Pin 4: PG5, Pin 5: PE3
    DDRE |= (1 << PE4) | (1 << PE5) | (1 << PE3); 
    DDRG |= (1 << PG5);

    UART0_init(); // UART 초기화 (9600bps)

    while (1) {
        data = UART0_read(); // 시리얼로부터 데이터 수신

        // 2. 줄바꿈 문자(\n, \r)가 들어오면 아무것도 하지 않고 통과
        // 시리얼 모니터 설정에 따라 엔터 입력 시 이 문자들이 같이 들어올 수 있습니다.
        if (data == '\n' || data == '\r') {
            continue; 
        }

        // 3. 새로운 유효 데이터가 들어왔을 때만 일단 모든 LED를 끕니다.
        PORTE &= ~((1 << PE4) | (1 << PE5) | (1 << PE3));
        PORTG &= ~(1 << PG5);

        // 4. 입력된 값에 따라 해당 LED만 점등
        switch (data) {
            case '2':
                PORTE |= (1 << PE4);
                UART0_print("LED on Pin 2 is ON.\n");
                break;
            
            case '3':
                PORTE |= (1 << PE5);
                UART0_print("LED on Pin 3 is ON.\n");
                break;

            case '4':
                PORTG |= (1 << PG5);
                UART0_print("LED on Pin 4 is ON.\n");
                break;

            case '5':
                PORTE |= (1 << PE3);
                UART0_print("LED on Pin 5 is ON.\n");
                break;

            default:
                // '2'~'5' 이외의 문자가 들어오면 모든 LED가 꺼진 상태 유지
                UART0_print("All LEDs are OFF.\n");
                break;
        }
    }
    return 0;
}