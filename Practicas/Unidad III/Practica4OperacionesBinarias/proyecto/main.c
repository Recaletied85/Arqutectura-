#include <avr/io.h>     // Librería para registros de I/O
#include <util/delay.h> // Librería de AVR para retardos temporales
#define F_CPU 16000000UL


int main(void) {
    //salida de leds, (prender)
    DDRB |= ((1 << DDB0) | (1 << DDB1) | (1 << DDB2));
    DDRD |= ((1 << DDD7));

    //Entradas par D y C
    DDRD &= ~((1 << DDD0) | (1 << DDD1));
    DDRC &= ~((1 << DDC0) | (1 << DDC1) | (1 << DDC2) | (1 << DDC3) | (1 << DDC4) | (1 << DDC5));

    // Activar pull-up en PD0 y PD1:
    PORTD |= (1 << PORTD0) | (1 << PORTD1);
    // Activar pull-up en PC0 a PC5:
    PORTC |= (1 << PORTC0) | (1 << PORTC1) | (1 << PORTC2) | 
    (1 << PORTC3) | (1 << PORTC4) | (1 << PORTC5);

    while (1) {
        uint8_t d = PIND;
        uint8_t c = PINC;

        // Nibble 1: PC5 (bit 0), PC4 (bit 1), PC3 (bit 2), PC2 (bit 3)
        uint8_t num1 = (((d >> PINC5) & 1) << 0)
                     | (((d >> PIND4) & 1) << 1)
                     | (((c >> PINC3) & 1) << 2)
                     | (((c >> PINC2) & 1) << 3);

        // Nibble 2: PC1 (bit 0), PC0 (bit 1), PD0 (bit 2), PD1 (bit 3)
        uint8_t num2 = (((c >> PINC1) & 1) << 0)
                     | (((c >> PINC0) & 1) << 1)
                     | (((c >> PIND0) & 1) << 2)
                     | (((c >> PIND1) & 1) << 3);

        uint8_t res_and = (num1 & num2) & 0x0F;
        uint8_t res_or  = (num1 | num2) & 0x0F;
        uint8_t res_xor = (num1 ^ num2) & 0x0F;

        PORTB = (PORTB & 0xF0) | res_and;
        _delay_ms(500);

        PORTB = (PORTB & 0xF0) | res_or;
        _delay_ms(1000);

        // Operación 3: XOR
        PORTB = (PORTB & 0xF0) | res_xor;
        _delay_ms(500);
    }

    return 0;
}