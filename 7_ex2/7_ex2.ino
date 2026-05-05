#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>
#include <ctype.h>    // toupper, tolower 함수를 쓰기 위해 포함
#include "UART0.h"    // 직접 만든 라이브러리 (따옴표 확인!)

#define TERMINATOR '\n'

int main(void) {
    char buffer[100]; // 수신 문자열 저장 공간
    int index = 0;
    char data;

    UART0_init(); // UART 초기화

    while (1) {
        data = UART0_read(); // 1바이트 읽기

        if (data == TERMINATOR) {
            buffer[index] = '\0'; // 문자열 종료 처리

            // 1. 받은 원본 메시지 먼저 전송
            UART0_print(buffer);
            UART0_write('\n');

            // 2. 대소문자 변환 후 전송
            for (int i = 0; buffer[i] != '\0'; i++) {
                if (isupper(buffer[i])) {        // 대문자라면
                    UART0_write(tolower(buffer[i])); // 소문자로 변환해 전송
                } 
                else if (islower(buffer[i])) {   // 소문자라면
                    UART0_write(toupper(buffer[i])); // 대문자로 변환해 전송
                } 
                else {
                    UART0_write(buffer[i]);      // 그 외 문자는 그대로 전송
                }
            }
            UART0_write('\n'); // 변환 문장 끝 줄바꿈

            index = 0; // 버퍼 인덱스 초기화
        } 
        else {
            if (index < 99) {
                buffer[index++] = data; // 버퍼에 문자 저장
            }
        }
    }
    return 0;
}