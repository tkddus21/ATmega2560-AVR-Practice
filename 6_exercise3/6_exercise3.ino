#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>

int state = 0;          // 현재 LED 위치 (0~3)
int dir = 1;            // 방향 (1: 정방향, -1: 역방향)

int last_button = 0;

void setup()
{
    // LED 출력 설정
    DDRE |= (1<<PE3) | (1<<PE4) | (1<<PE5);
    DDRG |= (1<<PG5);

    // 버튼 입력 설정 (PJ1)
    DDRJ &= ~(1<<PJ1);
}

void loop()
{
    // 버튼 읽기
    int current_button = (PINJ & (1<<PJ1));

    // 버튼 "눌린 순간" 감지
    if (current_button && !last_button)
    {
        dir = -dir;   // 방향 반전
        _delay_ms(50); // 디바운싱
    }
    last_button = current_button;

    // LED 출력 초기화
    PORTE = 0;
    PORTG = 0;

    // 현재 상태 LED 켜기
    switch(state)
    {
        case 0: PORTE = (1<<PE4); break; // 2번
        case 1: PORTE = (1<<PE5); break; // 3번
        case 2: PORTG = (1<<PG5); break; // 4번
        case 3: PORTE = (1<<PE3); break; // 5번
    }

    // 100ms 대기
    _delay_ms(100);

    // 상태 이동 (방향 적용)
    state += dir;

    //  범위 처리 (0~3 유지)
    if (state > 3) state = 0;
    if (state < 0) state = 3;
}