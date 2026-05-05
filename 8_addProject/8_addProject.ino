#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "ADC.h"   // 추가된 라이브러리 사용

// PWM 초기화 (Timer0 사용)
void PWM_init(void) {
    DDRB |= (1 << PB7);  // LED 출력

    // Fast PWM 모드
    TCCR0A |= (1 << WGM00) | (1 << WGM01);

    // 비반전 모드
    TCCR0A |= (1 << COM0A1);

    // 프리스케일러 64
    TCCR0B |= (1 << CS01) | (1 << CS00);
}

int main(void) {

    ADC_init(0, SINGLE_CONVERSION);   // 채널 0, 싱글 변환 모드
    PWM_init();                       // PWM 시작

    while (1) {
        int adc_value = read_ADC();

        // 10bit ADC → 8bit PWM 변환
        uint8_t pwm_value = adc_value / 4;

        OCR0A = pwm_value;  // LED 밝기 설정

        _delay_ms(10);
    }

    return 0;
}