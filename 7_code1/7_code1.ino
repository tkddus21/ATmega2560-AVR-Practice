#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>

void UART0_init(void);
void UART0_write(uint8_t data);
uint8_t UART0_read(void);

void UART0_init(void) {
  UBRR0H = 0x00; //9600 Baud rate로 설정
  UBRR0L = 207;

  UCSR0A |= _BV(U2X0); //2배속 모드
  // 비동기, 8비트 데이터, 패리티 없음, 1비트 정지 모드
  UCSR0C |= 0x06;

  UCSR0B |= _BV(RXEN0); //송수신 가능
  UCSR0B |= _BV(TXEN0); 
}
uint8_t UART0_read(void) {
  while(!(UCSR0A & (1 << RXC0)));  //데이터 수신 대기
  return UDR0;
}
void UART0_write(uint8_t data) {
  while(!(UCSR0A & (1 << UDRE0)) ); //데이터 송신 가능 대기
  UDR0 =data;
}

int main(void) {
  UART0_init(); //UART0 초기화
  while(1) {
    //UART0 포트로 수신된 데이터를 컴퓨터로 돌려보냄
    UART0_write(UART0_read());
  }
  return 0;
}