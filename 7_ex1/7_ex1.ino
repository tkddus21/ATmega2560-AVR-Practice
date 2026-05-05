#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>
#include "UART0.h"

int main(void) {
    UART0_init();                 // UART0 초기화
    UART0_print("Decimal:");   // 문자열 출력
    UART0_print(312);             // 정수를 10진수로 출력
    UART0_write('\n');            // 문자 출력

    UART0_print("Binary:0b");
    UART0_print(312, 2);          // 정수를 2진수로 출력
    UART0_write('\n');

    while (1) {
    }

    return 0;
}