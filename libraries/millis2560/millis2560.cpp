#include "millis2560.h"
#include <avr/interrupt.h>

#define MILLIS_INCREMENT_PER_OVERFLOW (1)
#define MICROS_INCREMENT_PER_OVERFLOW (24)

// 전역 변수: 외부에서 직접 수정 못 하도록 static 선언 (캡슐화)
static volatile unsigned long timer0_millis = 0; //
static volatile int timer0_micros = 0;           //

// 타이머0 오버플로 ISR
ISR(TIMER0_OVF_vect) {
    unsigned long m = timer0_millis;
    int f = timer0_micros;

    m += MILLIS_INCREMENT_PER_OVERFLOW;
    f += MICROS_INCREMENT_PER_OVERFLOW;

    // 1000us가 쌓이면 1ms 올림
    if (f >= 1000) {
        f -= 1000;
        m += 1;
    }

    timer0_millis = m;
    timer0_micros = f;
}


// 안전한 시간 읽기 함수
unsigned long millis2560(void) {
    unsigned long m;
    uint8_t oldSREG = SREG; // 이전 상태(인터럽트 허용 여부 등) 저장

    cli(); // 읽는 도중 값이 바뀌지 않게 인터럽트 일시 정지
    m = timer0_millis;
    SREG = oldSREG; // 이전 상태로 복원

    return m;
}

// 타이머 초기화
void millis2560_init(void) {
    // 64분주 설정: CS01, CS00 셋
    TCCR0B |= (1 << CS01) | (1 << CS00);
    
    // 오버플로 인터럽트 허용
    TIMSK0 |= (1 << TOIE0);
    
    // 전역 인터럽트 활성화
    sei();
}