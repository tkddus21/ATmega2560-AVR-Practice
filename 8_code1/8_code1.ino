// #define F_CPU 16000000L
// #include <avr/io.h>
// #include <util/delay.h>
// #include "UART0.h"

// void ADC_init(unsigned char channel) {
//     // 1. 기준 전압 설정: AVCC (5V) [cite: 2, 47]
//     ADMUX = (1 << REFS0); 

//     // 2. 채널 선택 (0~15) [cite: 3, 94]
//     ADMUX = ((ADMUX & 0xE0) | (channel & 0x07));
//     if (channel >= 8) ADCSRB |= (1 << MUX5);
//     else ADCSRB &= ~(1 << MUX5);

//     // 3. 분주비 설정: 128 (ADC 권장 주파수 확보) [cite: 51]
//     ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
    
//     // 4. 프리러닝 모드 및 자동 트리거 설정 [cite: 12, 14]
//     ADCSRA |= (1 << ADATE); 
//     ADCSRB &= ~((1 << ADTS2) | (1 << ADTS1) | (1 << ADTS0)); // 비트 마스킹 수정
    
//     // 5. ADC 활성화 및 변환 시작 [cite: 29]
//     ADCSRA |= (1 << ADEN); 
//     ADCSRA |= (1 << ADSC); 
// }

// int read_ADC(void) {
//     // 프리러닝 모드에서는 플래그 대기 없이 현재 레지스터 값을 반환 
//     return ADC; 
// }

// int main(void) {
//     int read_val;

//     UART0_init(); 
//     ADC_init(0); // 0번 채널(A0) 초기화 [cite: 49]
    
//     while (1) {
//         read_val = read_ADC(); // 10비트 결과값 읽기 [cite: 46]

//         UART0_print(read_val); 
//         UART0_write('\n');
        
//         _delay_ms(1000); // 1초 간격 모니터링 [cite: 41]
//     }
//     return 0;
// }


#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "UART0.h"
void ADC_init(unsigned char channel) {
    ADMUX = (1 << REFS0);   // 기준 전압 AVCC
    ADMUX = (ADMUX & 0xE0) | (channel & 0x07);
    if (channel > 8) {
        ADCSRB |= (1 << MUX5);
    } else {
        ADCSRB &= ~(1 << MUX5);
    }
    ADCSRA = (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);    // 분주비 128 설정
    // Free Running Mode 설정
    ADCSRA |= (1 << ADATE);
    ADCSRB &= ~((1 << ADTS2) | (1 << ADTS1) | (1 << ADTS0));
    ADCSRA |= (1 << ADEN);    // ADC 활성화
    ADCSRA |= (1 << ADSC);    // 변환 시작
}
int read_ADC(void) {
    while (!(ADCSRA & (1 << ADIF)));  // 변환 완료 대기
    ADCSRA |= (1 << ADIF);            // ADIF 클리어
    return ADC;                       // 10비트 ADC 값 반환
}
int main(void) {
    UART0_init();     // UART0 초기화
    ADC_init(0);      // ADC0 사용, Arduino Mega A0

    while (1) {
        int value = read_ADC();

        UART0_print(value);
        UART0_write('\n');

        _delay_ms(1000);
    }
    return 0;
}