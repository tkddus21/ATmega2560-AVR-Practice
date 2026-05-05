#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>
#include "ADC.h"

int main(void) {
    // 1. ADC 및 PWM 초기화
    ADC_init(0, SINGLE_CONVERSION); // 가변저항(A0) 초기화 [cite: 423]
    
    // PWM 설정 (Timer1 사용 - 서보 모터는 보통 50Hz 주기가 필요합니다)
    TCCR1A |= (1 << COM1A1) | (1 << WGM11);
    TCCR1B |= (1 << WGM13) | (1 << WGM12) | (1 << CS11); // 프리스케일러 8
    ICR1 = 39999; // 20ms 주기 설정 (50Hz)

    DDRB |= (1 << PB5); // 아두이노 11번 핀 출력 설정

    while (1) {
        // 2. 가변저항 값 읽기 (0 ~ 1023) [cite: 416]
        int adc_value = read_ADC();

        // 3. ADC 값을 서보 모터 펄스 폭으로 매핑 [cite: 616]
        // 보통 1ms(0도) ~ 2ms(180도) 사이의 펄스가 필요합니다.
        // 계산식: OCR1A = 2000 + (adc_value * 2000 / 1024)
        OCR1A = 2000 + (adc_value * 2); 

        _delay_ms(20); // 안정적인 제어를 위한 짧은 지연 [cite: 603]
    }
    return 0;
}