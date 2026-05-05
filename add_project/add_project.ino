#include <UART0.h>

#define F_CPU 16000000L
#include <avr/io.h>
#include <stdio.h>    // sscanf 함수 사용을 위해 필요
#include "UART0.h"

#define TERMINATOR '\n'

int main(void) {
    char buffer[64];      // 수신 문자열 저장용
    int index = 0;
    char data;
    
    int num1, num2, result;
    char op;              // 연산자 (+, -, *, /)

    UART0_init();         // 9600bps 초기화
    UART0_print("--- Arduino Calculator ---\n");
    UART0_print("Enter expression (ex: 10+4):\n");

    while (1) {
        data = UART0_read(); // 1바이트 읽기

        if (data == TERMINATOR) {
            buffer[index] = '\0'; // 문자열 종료
            
            // sscanf를 이용해 문자열에서 숫자-연산자-숫자 분리
            // 성공적으로 3개의 인자를 읽었는지 확인
            if (sscanf(buffer, "%d %c %d", &num1, &op, &num2) == 3) {
                switch (op) {
                    case '+': result = num1 + num2; break;
                    case '-': result = num1 - num2; break;
                    case '*': result = num1 * num2; break;
                    case '/': 
                        if (num2 != 0) result = num1 / num2; 
                        else {
                            UART0_print("Error: Divide by zero!\n");
                            index = 0; continue;
                        }
                        break;
                    default:
                        UART0_print("Invalid Operator!\n");
                        index = 0; continue;
                }

                // 결과 출력
                UART0_print(num1);
                UART0_write(' ');
                UART0_write(op);
                UART0_write(' ');
                UART0_print(num2);
                UART0_print(" = ");
                UART0_print(result);
                UART0_write('\n');
            } 
            else {
                UART0_print("Usage: [Number][Op][Number]\n");
            }

            index = 0; // 버퍼 초기화
        } 
        else {
            if (index < 63 && data != '\r') {
                buffer[index++] = data;
            }
        }
    }
    return 0;
}