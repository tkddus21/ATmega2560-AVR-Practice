#include <avr/io.h>
#include <avr/interrupt.h>

#define set_bit(value, bit)   (value |= (1 << bit))
#define clear_bit(value, bit) (value &= ~(1 << bit))

volatile uint8_t state = 0;   // LED 상태

ISR(PCINT1_vect) {
    state = !state;           // 핀 변화 발생 시 LED 상태 반전
}

void PCINT1_init(void) {
    PCICR |= (1 << PCIE1);      // Pin Change Interrupt 1 허용
    PCMSK1 |= (1 << PCINT9);    // PJ0 = PCINT9 허용
    sei();                      // 전역 인터럽트 허용
}

int main(void) {
    PCINT1_init();
    // 버튼: Arduino 15번 = PJ0 = PCINT9
    clear_bit(DDRJ, 0);         // PJ0 입력 설정

    // LED: Arduino 3번 = PE5
    set_bit(DDRE, 5);           // PE5 출력 설정
    
    while (1) {
        if (state) {
            set_bit(PORTE, 5);      // LED ON
        } else {
            clear_bit(PORTE, 5);    // LED OFF
        }
    }

    return 0;
}