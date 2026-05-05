#include <avr/io.h>
#include <avr/interrupt.h>

#define set_bit(value, bit)   (value |= (1 << bit))
#define clear_bit(value, bit) (value &= ~(1 << bit))

volatile uint8_t state = 1;   // LED 상태 저장

ISR(INT0_vect) {
    state = !state;           // 인터럽트 발생 시 LED 상태 반전
}

void INT0_init(void) {
    EIMSK |= (1 << INT0);     // INT0 인터럽트 허용
    // 하강 에지에서 인터럽트 발생
    // 버튼이 풀업 상태라서 누르면 HIGH -> LOW가 됨
    EICRA |= (1 << ISC01); // 하강 에지에서 인터럽트 요청을 발생시키므로, 10 값을 가져야함.
    //EICRA는 외부인터럽트가 발생하는 시점을 결정 INT0~3번 외부 인터렙트 제어 ISCn0과 ISCn1 두 비트 
    //조합으로 발생 시점 결정
    sei();                    // 전역 인터럽트 허용
}

int main(void) {
    // 버튼: Arduino 21번 = PD0 = INT0
    clear_bit(DDRD, 0);       // PD0 입력 설정
    set_bit(PORTD, 0);        // PD0 내장 풀업 저항 ON

    // LED: Arduino 5번 = PE3
    set_bit(DDRE, 3);         // PE3 출력 설정

    INT0_init();

    while (1) {
        if (!state) {
            set_bit(PORTE, 3);    // LED ON
        } else {
            clear_bit(PORTE, 3);  // LED OFF
        }
    }

    return 0;
}