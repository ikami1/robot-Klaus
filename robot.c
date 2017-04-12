#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

char flaga = 0;

int main(void){
	DDRB |= (1<<PB1)|(1<<PB2);	//ustawienie PB1 i PB2 jako wyjscie
    DDRD |= (0<<PD3);   //ustawienie PD3 jako wejscie

    GICR |= (1<<INT1);   //wlaczenie przerwan na INT1. (1<<INT0) wlaczy przerwania na INT0
    //MCRU &= ~15; niepoczebne
    MCUCR |= (1<<ISC11)|(1<<ISC10); //The rising edge of INT1 generates an interrupt request. Negacja na ISC10 powoduje zmiane wyzwalajacego zbocza
    sei();	// enable global interrupts

    while(1)
    {
		PB2 = 1;
    	_delay_ms(1000);
		PB2 = 0;
		_delay_ms(1000000);
    }

}


ISR(INT1_vect){//przerwanie na INT1
    
	if(!flaga){
		OC1A = 100;
		MCUCR ^= ~(1<<ISC10);
		flaga = !flaga;
	}
	else{
		OC1A = 0;
		MCUCR |= (1<<ISC10);
		flaga = !flaga;
	}
}

