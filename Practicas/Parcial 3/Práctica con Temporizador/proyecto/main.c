#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <util/delay.h>


#define F_CPU 16 000 000
volatile uint_16;
contador_ms=0;

ISR(TIMER1_COMPA_vect){
    contador_ms++;
    if(contador_ms>=00){
        PORTB|=(1<<PB1);
        contador_ms=0;
    }
}

int main(){

    DDRB |=(1<<DDB1);
    TCCR1A=0;
    TCCR1B=0;
    TCNT1=0;
    OCR1A=249;
    TCCR1B|=(1<<WGM12);
    TCCR1B|=(1<<CS11)|(1<<CS10);
    TIMSK1|=(1<<OCIE1A);
    sei();
    
    while(1);
}