#define F_CPU 16000000L
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <util/atomic.h>

// 매크로 함수 정의
#define set_bit(value, bit) (value |= (1 << bit))
#define clear_bit(value, bit) (value &= ~(1 << bit))

// 모드 관리를 위한 휘발성 변수 선언
// mode 0: 대기, mode 1: 비상 점멸, mode 2: 순차 순환
volatile uint8_t mode = 0;

// 외부 인터럽트 0 서비스 루틴 (INT0)
ISR(INT0_vect) {
    mode = 1; // 비상 모드로 변경
}

// 핀 변화 인터럽트 1 서비스 루틴 (PCINT1)
ISR(PCINT1_vect) {
    mode = 2; // 순환 모드로 변경
}

// 초기화 함수: 레지스터 설정[cite: 1]
void init_systems(void) {
    // 1. LED 핀 설정 (출력 모드)[cite: 1]
    set_bit(DDRE, 4); // 아두이노 2번a
    set_bit(DDRE, 5); // 아두이노 3번
    set_bit(DDRG, 5); // 아두이노 4번
    set_bit(DDRE, 3); // 아두이노 5번

    // 2. 버튼 핀 설정 (입력 모드 및 풀업/풀다운)[cite: 1]
    clear_bit(DDRD, 0); // PD0 (INT0) 입력[cite: 1]
    set_bit(PORTD, 0);  // 내장 풀업 저항 활성화[cite: 1]
    clear_bit(DDRJ, 0); // PJ0 (PCINT9) 입력[cite: 1]

    // 3. 외부 인터럽트 0 설정 (INT0)[cite: 1]
    EIMSK |= (1 << INT0);   // INT0 활성화[cite: 1]
    EICRA |= (1 << ISC01);  // 하강 에지(Falling Edge)에서 발생[cite: 1]

    // 4. 핀 변화 인터럽트 1 설정 (PCINT1)[cite: 1]
    PCICR |= (1 << PCIE1);   // PCINT 그룹 1 활성화[cite: 1]
    PCMSK1 |= (1 << PCINT9); // PJ0 핀 개별 인터럽트 활성화[cite: 1]

    sei(); // 전역 인터럽트 허용[cite: 1]
}

// LED 상태를 모두 끄는 함수
void clear_all_leds(void) {
    clear_bit(PORTE, 4);
    clear_bit(PORTE, 5);
    clear_bit(PORTG, 5);
    clear_bit(PORTE, 3);
}

int main(void) {
    init_systems();
    uint8_t current_mode;

    while (1) {
        // 인터럽트로부터 안전하게 모드 값 복사 (무결성 확보)[cite: 1]
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
            current_mode = mode;
        }

        if (current_mode == 1) { // 모드 1: 비상 모드 (전체 깜빡)
            set_bit(PORTE, 4); set_bit(PORTE, 5); set_bit(PORTG, 5); set_bit(PORTE, 3);
            _delay_ms(200);
            clear_all_leds();
            _delay_ms(200);
        } 
        else if (current_mode == 2) { // 모드 2: 순환 모드 (하나씩 이동)
            clear_all_leds(); set_bit(PORTE, 4); _delay_ms(150);
            clear_all_leds(); set_bit(PORTE, 5); _delay_ms(150);
            clear_all_leds(); set_bit(PORTG, 5); _delay_ms(150);
            clear_all_leds(); set_bit(PORTE, 3); _delay_ms(150);
        }
    }
    return 0;
}