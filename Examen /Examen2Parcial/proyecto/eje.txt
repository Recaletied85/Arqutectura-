#define F_CPU 16000000UL //
#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>

// Paso 1: Almacena 8 velocidades en la memoria Flash
const uint16_t velocidades[8] PROGMEM = {
    50,   // 000: Velocidad máxima
    100,  // 001
    150,  // 010
    250,  // 011
    350,  // 100
    500,  // 101
    700,  // 110
    1000  // 111: Velocidad mínima
};

// aplicar un retardo variable 
void retardo_variable_ms(uint16_t ms) {
    while (ms--) {
        _delay_ms(1);
    }
}

int main(void) {
    // pines de salida para los LEDs
    
    DDRD |= (1 << DDD7);
    // PB0, PB1, PB2 
    DDRB |= (1 << DDB0) | (1 << DDB1) | (1 << DDB2);

    // Paso 3:  pines de entrada para  Switch PC5, PC4, PC3
    // Limpiar los bits para configurarlos como entradas
    DDRC &= ~((1 << DDC5) | (1 << DDC4) | (1 << DDC3));
    // Activar las resistencias Pull-Up internas
    PORTC |= (1 << PORTC5) | (1 << PORTC4) | (1 << PORTC3);

    uint8_t posicion_pendulo = 0;
    int8_t direccion = 1; 

    while (1) {
        
        // Invertimos la lectura (~) para que un switch encendido se lea como 1.
        uint8_t lectura = ~PINC;

        // Extraer los bits PC5, PC4 y PC3 y formar un número del 0 al 7.
        // PC3 será el bit menos significativo y PC5 el más significativo 
        uint8_t bit0 = (lectura >> PINC3) & 0x01;
        uint8_t bit1 = (lectura >> PINC4) & 0x01;
        uint8_t bit2 = (lectura >> PINC5) & 0x01;
        
        uint8_t indice_velocidad = bit0 | (bit1 << 1) | (bit2 << 2);

        //  Leer el valor de retardo  desde la memoria Flash
        uint16_t retardo_actual = pgm_read_word(&velocidades[indice_velocidad]);

        // Apagar todos los LEDs antes de encender el siguiente
        PORTD &= ~(1 << PORTD7);
        PORTB &= ~((1 << PORTB0) | (1 << PORTB1) | (1 << PORTB2));

        //  Encender el LED correspondiente a la posición actual del péndulo
        switch (posicion_pendulo) {
            case 0: PORTD |= (1 << PORTD7); break; // LED 1
            case 1: PORTB |= (1 << PORTB0); break; // LED 2
            case 2: PORTB |= (1 << PORTB1); break; // LED 3
            case 3: PORTB |= (1 << PORTB2); break; // LED 4
        }

        // siguiente posición del péndulo
        posicion_pendulo += direccion;
        
        // Invertir la dirección 
        if (posicion_pendulo == 3) {
            direccion = -1; // regresar
        } else if (posicion_pendulo == 0) {
            direccion = 1;  // avanzar
        }

        // retardo de la Flash
        retardo_variable_ms(retardo_actual);
    }
}