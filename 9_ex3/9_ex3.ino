#include <avr/io.h>
#include <avr/interrupt.h>

#define set_bit(value, bit)   (value |= (1 << bit))
#define clear_bit(value, bit) (value &= ~(1 << bit))

volatile uint8_t state = 0;   // LED 상태

ISR(PCINT1_vect) {
    state = !state;           // 어느 버튼이든 변화하면 LED 2개 상태 반전
}

void PCINT1_init(void) {
    PCICR |= (1 << PCIE1);       // Pin Change Interrupt 1 허용

    PCMSK1 |= (1 << PCINT9);     // PJ0, Arduino 15번
    PCMSK1 |= (1 << PCINT10);    // PJ1, Arduino 14번

    sei();                       // 전역 인터럽트 허용
}

int main(void) {
    // 버튼 입력 설정
    clear_bit(DDRJ, 0);          // PJ0 입력, Arduino 15번
    clear_bit(DDRJ, 1);          // PJ1 입력, Arduino 14번

    // 외부 풀다운 저항 사용이므로 PORTJ 풀업은 켜지 않음
    clear_bit(PORTJ, 0);
    clear_bit(PORTJ, 1);

    // LED 출력 설정
    set_bit(DDRE, 4);            // PE4, Arduino 2번
    set_bit(DDRE, 5);            // PE5, Arduino 3번

    PCINT1_init();

    while (1) {
        if (state) {
            set_bit(PORTE, 4);   // LED1 ON
            set_bit(PORTE, 5);   // LED2 ON
        } else {
            clear_bit(PORTE, 4); // LED1 OFF
            clear_bit(PORTE, 5); // LED2 OFF
        }
    }

    return 0;
}