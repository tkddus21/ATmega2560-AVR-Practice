#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>

#define set_bit(value, bit)   ((value) |= (1 << (bit)))
#define clear_bit(value, bit) ((value) &= ~(1 << (bit)))

volatile unsigned long ms_count = 0;

volatile uint8_t speed_mode = 0;
volatile unsigned long last_button_time = 0;

uint8_t led_state = 0;
unsigned long previous_time = 0;

// 속도 모드별 LED 반전 시간(ms)
unsigned int interval_table[3] = {
  250,   // mode 0: 0.25초
  500,   // mode 1: 0.5초
  1000   // mode 2: 1초
};

// Timer2 비교일치 인터럽트
// 1ms마다 실행
ISR(TIMER2_COMPA_vect) {
  ms_count++;
}

// INT0 외부 인터럽트
// Arduino Mega 21번 핀 버튼
ISR(INT0_vect) {
  unsigned long now = ms_count;

  // 버튼 채터링 방지: 200ms 이내 중복 입력 무시
  if ((now - last_button_time) > 200) {
    speed_mode++;

    if (speed_mode >= 3) {
      speed_mode = 0;
    }

    last_button_time = now;
  }
}

// 현재 ms 시간 읽기
unsigned long millis_timer2(void) {
  unsigned long m;
  uint8_t oldSREG = SREG;

  cli();
  m = ms_count;
  SREG = oldSREG;

  return m;
}

void timer2_init(void) {
  // Timer2 초기화
  TCCR2A = 0x00;
  TCCR2B = 0x00;
  TCNT2 = 0;

  // CTC 모드
  // WGM21 = 1
  TCCR2A |= (1 << WGM21);

  // 1ms 만들기
  // 16MHz / 64 = 250kHz
  // 250카운트 = 1ms
  OCR2A = 249;

  // Timer2 비교일치 A 인터럽트 허용
  TIMSK2 |= (1 << OCIE2A);

  // Timer2 분주비 64 설정
  // Timer2에서 64분주 = CS22:CS21:CS20 = 100
  TCCR2B |= (1 << CS22);
}

void INT0_init(void) {
  // PD0 = Arduino Mega 21번 = INT0 입력
  clear_bit(DDRD, 0);

  // 내부 풀업 저항 ON
  set_bit(PORTD, 0);

  // INT0 하강 에지에서 인터럽트 발생
  EICRA |= (1 << ISC01);
  EICRA &= ~(1 << ISC00);

  // INT0 인터럽트 허용
  EIMSK |= (1 << INT0);
}

int main(void) {
  // PB7 = Arduino Mega 13번 LED
  set_bit(DDRB, 7);
  clear_bit(PORTB, 7);

  timer2_init();
  INT0_init();

  sei();

  previous_time = millis_timer2();

  while (1) {
    unsigned long current_time = millis_timer2();
    unsigned int interval = interval_table[speed_mode];

    if ((current_time - previous_time) >= interval) {
      previous_time = current_time;

      led_state = !led_state;

      if (led_state) {
        set_bit(PORTB, 7);
      } else {
        clear_bit(PORTB, 7);
      }
    }
  }

  return 0;
}