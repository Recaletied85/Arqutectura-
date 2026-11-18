#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>


volatile uint16_t contador_ms = 0;
volatile uint16_t limite_ms = 1000; 

ISR(TIMER1_COMPA_vect) {
    contador_ms++;
    // Cuando el contador alcanza el límite definido por los DIP switches, entra al if
    if(contador_ms >= limite_ms) {
        PORTB ^= (1<<PB1); // La compuerta XOR (^) invierte el estado actual del pin
        contador_ms = 0;
    }
}

int main() {
    // salida para el LED
    DDRB |= (1<<DDB1);
    
    // PC5 y PC4 como entradas para dip switch
    DDRC &= ~((1<<DDC5) | (1<<DDC4)); 
    // el pull up interno en PC5 y PC4
    PORTC |= (1<<PORTC5) | (1<<PORTC4);

    // configutamos los timers para las interrupciones
    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1 = 0;
    OCR1A = 249; 
    
    TCCR1B |= (1<<WGM12);            // Activar CTC
    TCCR1B |= (1<<CS11) | (1<<CS10); // Prescaler = 64
    TIMSK1 |= (1<<OCIE1A);           // Habilitar interrupción de comparación A
    
    sei(); // Habilitar interrupciones globales
    
    while(1) {
        // senal de los switches
        uint8_t sw1 = !(PINC & (1<<PINC5));
        uint8_t sw2 = !(PINC & (1<<PINC4));

        // 4 casos para el parpadeo
        if (sw1 == 0 && sw2 == 0) {
            limite_ms = 1000; // velocidad 1 parpadea cada 1 segundo
        } 
        else if (sw1 == 1
             && sw2 == 0) {
            limite_ms = 3000;  // velocidad 2 parpadea cada medio segundo
        } 
        else if (sw1 == 0 && sw2 == 1) {
            limite_ms = 100;  // velocidad 3 parpadea cada 100 ms
        } 
        else {
            limite_ms = 2000; // 1 y 2 activos parpadea cada 2 segundos
        }
    }
}