#include <avr/io.h>
#include <avr/interrupt.h>

#define set_bit(value, bit)   (value |= (1 << bit))
#define clear_bit(value, bit) (value &= ~(1 << bit))

volatile uint8_t state = 0;   // LED 상태

// INT0 인터럽트 발생 시
ISR(INT0_vect) {
    state = !state;   // 상태 반전
}

// INT0 초기화
void INT0_init(void) {
    EIMSK |= (1 << INT0);     // INT0 허용

    // 모든 변화(상승 + 하강 에지)에서 인터럽트 발생
    EICRA |= (1 << ISC00);
    EICRA &= ~(1 << ISC01);

    sei();                    // 전역 인터럽트 허용
}

int main(void) {

    // 버튼 (PD0 = Arduino 21번)
    clear_bit(DDRD, 0);   // 입력
    set_bit(PORTD, 0);    // 풀업

    // LED 4개 출력 설정
    set_bit(DDRE, 4);   // PE4 (Arduino 2)
    set_bit(DDRE, 5);   // PE5 (Arduino 3)
    set_bit(DDRG, 5);   // PG5 (Arduino 4)
    set_bit(DDRE, 3);   // PE3 (Arduino 5)

    INT0_init();

    while (1) {
        if (state) {
            // 모두 ON
            set_bit(PORTE, 4);
            set_bit(PORTE, 5);
            set_bit(PORTG, 5);
            set_bit(PORTE, 3);
        } else {
            // 모두 OFF
            clear_bit(PORTE, 4);
            clear_bit(PORTE, 5);
            clear_bit(PORTG, 5);
            clear_bit(PORTE, 3);
        }
    }

    return 0;
}