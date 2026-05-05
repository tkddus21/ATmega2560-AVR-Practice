#include <avr/io.h>
#include <util/delay.h>
#include "UART0.h"

#define set_bit(value, bit)   (value |= (1 << bit))
#define clear_bit(value, bit) (value &= ~(1 << bit))

int main(void) {
    UART0_init(); // UART 시리얼 통신 초기화
    clear_bit(DDRD, 0);   // PD0, Arduino 21번 버튼 입력
    set_bit(PORTD, 0);    // PD0 내장 풀업 저항 ON

    set_bit(DDRE, 3);     // PE3, Arduino 5번 LED 출력

    while (1) {
        uint8_t state = PIND & (1 << 0);

        if (!state) {
            set_bit(PORTE, 3);     // 버튼 누르면 LED ON
        } else {
            clear_bit(PORTE, 3);   // 버튼 떼면 LED OFF
        }
        UART0_print(!state); //시리얼 모니터로 LED 상태 출력
        UART0_write('\n');

        _delay_ms(500);
    }

    return 0;
}