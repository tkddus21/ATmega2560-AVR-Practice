#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "ADC.h"

// 서보 모터 동작 범위 설정 (16MHz, 8분주 기준)
#define SERVO_MIN 2000  // 0도 (1ms 펄스) 
#define SERVO_MAX 4000  // 180도 (2ms 펄스) 

// PWM 초기화 (Timer1 사용 - PB5/D11 핀 출력)
void Servo_PWM_init(void) {
    DDRB |= (1 << PB5); // 아두이노 11번 핀(PB5) 출력 설정 [cite: 1630, 1657]

    // Fast PWM 모드 (Mode 14), ICR1을 TOP으로 사용
    TCCR1A |= (1 << COM1A1) | (1 << WGM11);
    TCCR1B |= (1 << WGM13) | (1 << WGM12) | (1 << CS11); // 프리스케일러 8

    ICR1 = 39999;  // 20ms 주기 설정 (50Hz) [cite: 1673]
    OCR1A = SERVO_MIN; // 초기 위치 0도
}

int main(void) {
    ADC_init(0, SINGLE_CONVERSION); // 가변저항 A0(PF0) 초기화 [cite: 1629, 1645]
    Servo_PWM_init();

    int current_pos = SERVO_MIN; // 현재 모터의 위치 기록
    int target_pos;              // 가변저항으로 읽은 목표 위치

    while (1) {
        // 1. ADC 값 읽기 및 목표 위치 계산 (0~1023 -> 2000~4000)
        int adc_val = read_ADC();
        target_pos = SERVO_MIN + (int)(adc_val * 1.95); // 매핑 연산 [cite: 1660]

        // 2. 부드러운 이동 (현재 위치를 목표 위치로 서서히 접근) 
        if (current_pos < target_pos) {
            current_pos++; // 조금씩 증가
        } 
        else if (current_pos > target_pos) {
            current_pos--; // 조금씩 감소
        }

        // 3. 실제 PWM 레지스터에 적용
        OCR1A = current_pos;

        // 4. 이동 속도 제어 (이 값이 클수록 더 천천히 움직임)
        // 2ms로 설정하면 매우 부드럽고, 10ms 이상이면 아주 천천히 움직입니다.
        _delay_ms(5); 
    }

    return 0;
}