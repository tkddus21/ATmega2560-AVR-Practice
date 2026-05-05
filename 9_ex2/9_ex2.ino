#include <avr/io.h>
#include <avr/interrupt.h>

#define set_bit(value, bit)   (value |= (1 << bit))
#define clear_bit(value, bit) (value &= ~(1 << bit))

volatile uint8_t state = 0;   // 0~3 상태

// 버튼 눌렀을 때 인터럽트 발생
ISR(INT0_vect) {
    _delay_ms(500); // 50ms 정도 기다려서 떨림(Bouncing)이 끝날 때까지 대기

    state++;
    if (state >= 4) state = 0;   // 0~3 반복

    // ⚠️ 중요: 기다리는 동안 발생해서 EIFR에 쌓인 가짜 깃발들을 강제로 지워줌
    EIFR |= (1 << INTF0);
}

// INT0 초기화
void INT0_init(void) {
    EIMSK |= (1 << INT0);     // INT0 허용

    // 하강 에지 (버튼 누를 때만) 10
    EICRA |= (1 << ISC01); 
    EICRA &= ~(1 << ISC00);

    sei();                    // 전역 인터럽트 허용
}

int main(void) {

    // 버튼 (PD0 = Arduino 21번)
    clear_bit(DDRD, 0);
    set_bit(PORTD, 0);    // 풀업

    // LED 4개 출력 설정
    set_bit(DDRE, 4);   // PE4 (2번)
    set_bit(DDRE, 5);   // PE5 (3번)
    set_bit(DDRG, 5);   // PG5 (4번)
    set_bit(DDRE, 3);   // PE3 (5번)

    INT0_init();

    while (1) {

        // 전부 OFF 먼저
        clear_bit(PORTE, 4);
        clear_bit(PORTE, 5);
        clear_bit(PORTG, 5);
        clear_bit(PORTE, 3);

        // state에 따라 하나만 ON
        switch (state) {
            case 0: set_bit(PORTE, 4); break; // LED1
            case 1: set_bit(PORTE, 5); break; // LED2
            case 2: set_bit(PORTG, 5); break; // LED3
            case 3: set_bit(PORTE, 3); break; // LED4
        }
    }

    return 0;
}