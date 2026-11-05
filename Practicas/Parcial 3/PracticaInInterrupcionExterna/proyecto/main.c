#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

// interrupción para int0
ISR(INT0_vect) {
    
    // invertimos el led
    PORTB ^= (1 << PORTB1);

    // PARA el bouncing o sin bouncing, comento esta lnia 
    _delay_ms(50);

    // limpiar bandera
    EIFR |= (1 << INTF0);
}



int main(void) {
    // Configuración de Puertos y Pines
    DDRB |= (1 << DDB1);          // salida led
    DDRD &= ~(1 << DDD2);         // Pin PD2 (INT0) como entrada
    PORTD |= (1 << PORTD2);       // Habilitar resistencia Pull-up interna en PD2

    // Cambiar el caso de int0 
//-----------------------------------------------
    // bajo
    EICRA = 0; 

    // logico
    //EICRA = (1 << ISC00); 

    // flanco bajo
    //EICRA = (1 << ISC01); 

    
    // flanco arriba
    //EICRA = (1 << ISC01) | (1 << ISC00); 
//-----------------------------------------
    // limpiamos el int0
    EIFR = (1 << INTF0);          
    
    // se prende la interrupcion
    EIMSK = (1 << INT0);          
    
    // activar interrupciones
    sei();                        

    while (1) {
        
    }
}