#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>

void setup()
{
    DDRE |= (1<<PE3) | (1<<PE4) | (1<<PE5);
    DDRG |= (1<<PG5);
}

void loop()
{
    PORTE = (1<<PE4);
    PORTG = 0;
    _delay_ms(1000);

    PORTE = (1<<PE5);
    PORTG = 0;
    _delay_ms(1000);

    PORTE = 0;
    PORTG = (1<<PG5);
    _delay_ms(1000);

    PORTE = (1<<PE3);
    PORTG = 0;
    _delay_ms(1000);
}