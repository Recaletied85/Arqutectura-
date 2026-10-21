#define F_CPU 16000000UL //
#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>

// Rojo PD7, PB1
// Peaton PB2
// Verde PB0, PB3


// Paso 1: Almacena 4 velocidades en la memoria Flash
const uint16_t velocidades[3] PROGMEM = {
    
};

uint8_t estados[] = {



};

void ledRojo_on() {
    PORTB |= (1 << PB1); // Enciende el LED rojo
    PORTD |= (1 << PD7); // Enciende el LED rojo

    

}

// aplicar un retardo variable 
void retardo_variable_ms(uint16_t ms) {
    while (ms--) {
        _delay_ms(1);
    }
}

int main(void) {
    // Configura el pin PB0 como salida
    DDRB |= (1 << PB0) | (1 << PB1) | (1 << PB2); // Configura PB0,PB2 y PB1 como salidas
    DDRD |= (1 << PD7); // Configura PD7 como salida




   
}

