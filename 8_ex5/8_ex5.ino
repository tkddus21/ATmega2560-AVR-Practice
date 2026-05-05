#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "ADC.h"

// Arduino UNO 기준: D13 = PB7
#define LED_PIN PB7

void delay_variable_ms(unsigned int ms)
{
    while (ms--)
    {
        _delay_ms(1);
    }
}

int main(void)
{
    // ADC 초기화
    ADC_init(0, SINGLE_CONVERSION);

    // LED 핀 출력 설정
    DDRB |= (1 << LED_PIN);

    while (1)
    {
        int read = read_ADC();

        // 200ms ~ 2000ms 범위
        unsigned int delay_time = 200 + ((unsigned long)read * 1800) / 1023;

        // LED ON
        PORTB |= (1 << LED_PIN);
        delay_variable_ms(delay_time);

        // LED OFF
        PORTB &= ~(1 << LED_PIN);
        delay_variable_ms(delay_time);
    }

    return 0;
}