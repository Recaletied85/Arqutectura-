#define F_CPU 16000000UL //
#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>

// leds de salida
// Rojo  PB1, PB3
// Peaton PB4
// Verde PB0, PB2
// entrada de switch
// PC5, PC4, PC3





const uint16_t velocidades[3] PROGMEM = {
    1000, // Velocidad 1
    500,  // Velocidad 2
    2000   // Velocidad 3
};

const uint8_t estados[6][2] PROGMEM = {
    {0b00001001, 0b00001001, }, // S0
    {0b00001001, 0b00001000}, // S1 
    {0b00000110, 0b00000110}, // S2
    {0b00000110, 0b00000010}, // S3
    {0b00001010, 0b00001010}, // S4 
    {0b00011010, 0b00011010}  // S5 
};

const uint8_t transiciones[6][2] PROGMEM = {
    {1, 1}, 
    {2, 2}, // De S1 pasa a S2
    {3, 3}, // De S2 pasa a S3
    {4, 4}, // De S3 pasa a S4
    {0, 5}, // De S4 a S0 o S5 dependiendo del switch
    {0, 0}  // despues del peaton  da a s0 
};

// aplicar un retardo variable 
void retardo_variable_ms(uint16_t ms) {
    while (ms--) {
        _delay_ms(1);
    }
}

int main(void) {
    // Configura el pin PB como salida
    DDRB |= (1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB3) | (1 << PB4); // Configura PB0,PB2 y PB1 como salidas

    DDRC &= ~((1 << PC5) | (1 << PC4) | (1 << PC3)); // Configura PC como entrada
    PORTC |= (1 << PC5) | (1 << PC4) | (1 << PC3); // Activa el pull-up de PC5

    uint8_t estado_actual = 0; // Estado inicial

    while (1) {
        // 1. Leemos las entradas (invertidas con ~ porque usamos pull-up)
        uint8_t lectura = ~PINC;

        // Extraemos el modo de velocidad de PC5 y PC4 (valor entre 0 y 3)
        uint8_t modo = (lectura >> 4) & 0x03;

        // Extraemos si el switch de peatón (PC3) está activado (0 o 1)
        uint8_t peaton = (lectura >> 3) & 0x01;

        // 2. Leemos el tiempo total desde la Flash y lo dividimos entre 4
        // (para hacer 2 parpadeos completos dentro del tiempo total del modo)
        uint16_t tiempo_cuarto = pgm_read_word(&velocidades[modo]) / 4;

        // 3. Ejecutamos el estado actual en 4 sub-ciclos usando las 2 columnas de tu matriz
        for (uint8_t i = 0; i < 4; i++) {
            // i % 2 alterna entre columna 0 y columna 1: (0, 1, 0, 1)
            uint8_t salida = pgm_read_byte(&estados[estado_actual][i % 2]);
            
            // Escribimos la salida en los pines PB0-PB4 sin afectar PB5-PB7
            PORTB = (PORTB & ~0x1F) | (salida & 0x1F);

            retardo_variable_ms(tiempo_cuarto);
        }

        // 4. Cambiamos al siguiente estado leyendo la tabla de transiciones en Flash
        estado_actual = pgm_read_byte(&transiciones[estado_actual][peaton]);
    }
   
}

