#include "UART0.h"

void UART0_init() {
    UBRR0H = 0;
    UBRR0L = 207; 
    UCSR0A |= (1 << U2X0); 
    UCSR0B |= (1 << RXEN0) | (1 << TXEN0);
    UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);
}

void UART0_write(uint8_t data) {
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = data;
}

uint8_t UART0_read() {
    while (!(UCSR0A & (1 << RXC0)));
    return UDR0;
}

void UART0_print(char *str) {
    for (int i = 0; str[i]; i++) UART0_write(str[i]);
}

void UART0_print(const char *str) {
    for (int i = 0; str[i]; i++) UART0_write(str[i]);
}

void UART0_print(int no, int radix) {
    char buffer[20];
    itoa(no, buffer, radix);
    UART0_print(buffer);
}

void UART0_print(long no, int radix) {
    char buffer[33];
    ltoa(no, buffer, radix);
    UART0_print(buffer);
}

void UART0_print(unsigned long no, int radix) {
    char buffer[33];
    ultoa(no, buffer, radix);
    UART0_print(buffer);
}

void UART0_print(float no, int precision) {
    char buffer[20]; 
    dtostrf(no, 0, precision, buffer);

    UART0_print(buffer); 
}