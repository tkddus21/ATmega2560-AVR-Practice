#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>

int state = 0;          // 현재 LED 위치
int last_button = 0;    // 이전 버튼 상태

void setup()
{
    // LED 출력 설정
    DDRE |= (1<<PE3) | (1<<PE4) | (1<<PE5);
    DDRG |= (1<<PG5);

    // 버튼 입력 설정 (PJ1)
    DDRJ &= ~(1<<PJ1);   // 입력으로 설정
}

void loop()
{
    int current_button = (PINJ & (1<<PJ1));

    // 버튼 눌림 감지 (0 → 1 변화)
    if (current_button && !last_button)
    {
        state++;
        if (state > 3) state = 0;

        _delay_ms(50); // 디바운싱
    }

    last_button = current_button;

    // LED 출력 제어
    PORTE = 0;
    PORTG = 0;

    switch(state)
    {
        case 0:
            PORTE = (1<<PE4); // 2번
            break;
        case 1:
            PORTE = (1<<PE5); // 3번
            break;
        case 2:
            PORTG = (1<<PG5); // 4번
            break;
        case 3:
            PORTE = (1<<PE3); // 5번
            break;
    }
}