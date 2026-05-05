#include "ADC.h"
static unsigned char adc_mode = SINGLE_CONVERSION;

void ADC_init(unsigned char channel, unsigned char mode) {
    adc_mode = mode;
    ADMUX = (1 << REFS0);
    ADMUX = (ADMUX & 0xE0) | (channel & 0x07);

    if (channel > 7) {
        ADCSRB |= (1 << MUX5);
    }
    else {
        ADCSRB &= ~(1 << MUX5);
    }
    ADCSRA = (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);

    if (adc_mode == FREE_RUNNING) {
        ADCSRA |= (1 << ADATE);
        ADCSRB &= ~((1 << ADTS2) | (1 << ADTS1) | (1 << ADTS0));
    }
    else {
        ADCSRA &= ~(1 << ADATE);
    }

    ADCSRA |= (1 << ADEN);

    if (adc_mode == FREE_RUNNING) {
        ADCSRA |= (1 << ADSC);
    }
}

int read_ADC(void) {
    if (adc_mode == SINGLE_CONVERSION) {
        ADCSRA |= (1 << ADSC);
    }
    while (!(ADCSRA & (1 << ADIF)));

    ADCSRA |= (1 << ADIF);

    return ADC;
}