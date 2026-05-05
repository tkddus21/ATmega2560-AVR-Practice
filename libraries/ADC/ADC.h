#ifndef ADC_H
#define ADC_H

#include <avr/io.h>

// ADC 동작 모드 정의
#define FREE_RUNNING       0
#define SINGLE_CONVERSION  1

// 함수 원형 선언
void ADC_init(unsigned char channel, unsigned char mode);
int read_ADC(void);

#endif