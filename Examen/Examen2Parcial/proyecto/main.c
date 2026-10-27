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
    1000, // Velocidad 1 en el switch 00
    500,  // Velocidad 2 en el switch 01
    2000   // Velocidad 3 en el switch 10
};

const uint8_t estados[6][2] PROGMEM = {
    //la 2 columna reoresenta si el 1 de peaton esta activado o no 
    {0b00001001, 0b00001001, }, // S0
    {0b00001001, 0b00001000}, // S1 
    {0b00000110, 0b00000110}, // S2
    {0b00000110, 0b00000010}, // S3
    {0b00001010, 0b00001010}, // S4 
    {0b00011010, 0b00011010}  // S5 
};

const uint8_t transiciones[6][2] PROGMEM = {
    //la 2 columna representa si el 1 de peaton esta activado o no
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

    DDRC &= ~((1 << PC5) | (1 << PC4) | (1 << PC3)); // Configura C como entrada
    PORTC |= (1 << PC5) | (1 << PC4) | (1 << PC3); // Activa el pull-up de PC

    uint8_t estado_actual = 0; // Estado inicial

    while (1) {
        // Invertimos la entrada de PC ya que usamos el pull-up interno
        uint8_t lectura = ~PINC;

        /*Configuramos el modo de velocidad que se pone en el switch y leemos unicamente 
        3 numeros del 0 al 3 */
        uint8_t modo = (lectura >> 4) & 0x03;

        /*/aventamos el bit 3 del peaton para saber si esta activo o no y leemos unicamente 
        si el peaton es 0 o 1*/
        uint8_t peaton = (lectura >> 3) & 0x01;

        /*/leemos el tiempo desde la memoria flash y lo dividimos en 4 partes para 
        que cada parte dure el mismo tiempo*/
        uint16_t total = pgm_read_word(&velocidades[modo]) / 4;

        // Ejecutamos el estado actual 
        for (uint8_t i = 0; i < 4; i++) {
            //primero el for es para hacer 4 subcilos
            // con el i%2 hacemos que se divida de entre 2 y se redonde, 
            // entonces en vez de tener 0,1,2,3, ahora tenemos 0,1,0,1 y asi alterna en la matriz de estados
            uint8_t salida = pgm_read_byte(&estados[estado_actual][i % 2]);
            
            // Escribimos la salida en los pines PB0-PB4 sin contar las otras entradas de PB
            PORTB = (PORTB & ~0x1F) | (salida & 0x1F);

            retardo_variable_ms(total);
        }
        // cambiamos al sigeunte estado dependiando del switch de peaton
        estado_actual = pgm_read_byte(&transiciones[estado_actual][peaton]);
    }
}

