#ifndef MILLIS2560_H_
#define MILLIS2560_H_

#include <avr/io.h>

// 함수 선언
void millis2560_init(void);    // 타이머 설정 및 인터럽트 허용
unsigned long millis2560(void); // 현재 경과 시간(ms) 반환

#endif /* MILLIS2560_H_ */