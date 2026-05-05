#define F_CPU 1600000L
#include <avr/io.h>
#include <util.delay.h>
#include "UART0.h"

void ADC_init(unsigned char channel) {
  ADMUX |= (1 << REFS0); //AVCC를 기준 전압으로 선택
  // 채널 선택. 8~15번 채널은 ADMUX 이외에 ADCSRB 레지스터 설정 필요
  ADMUX = ((ADMUX & 0xE0) | (channel & 0x07));
  if (channel > 8 ) ADCSRB |= (1 << MUX5);

  // 분주비를 128로 설정
  ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
  ADCSRA |= (1 << ADATE); //자동 트리거 모드 활성화

  //프리러닝 모드 선택
  ADCSRB &= ~(1 << ADTS2) & ~(1 << ADTS1) & ~(1 << ADTS0);

  ADCSRA |= (1 << ADEN); //ADC 활성화
  ADCSRA |= (1 << ADSC); //변환 시작

}