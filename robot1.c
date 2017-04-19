#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

char flaga = 0;
uint16_t distance;

int main(void){
	DDRB |= (1<<PB1)|(1<<PB2);	//ustawienie PB1 i PB2 jako wyjscie
    DDRB |= (0<<PB0);   //ustawienie PB0 jako wejscie
	DDRD |= (1<<PD7);

    TCCR1B |= (1<<ICES1);   //rosnace zbocze wyzwala przerwania
    TCCR1B |= (1<<CS10);    //no prescaler, 1 cykl zegara to 1 mikrosekunda (przy 1 MHz)
    sei();	// enable global interrupts

    while(1)
    {
		PB2 = 1;
    	_delay_ms(1000);
		PB2 = 0;
		_delay_ms(1000000);
    }

}

ISR(TIMER1_CAPT_vect){  //przerwanie na PB0
    uint16_t LastCapture;

    if( !(TCCR1B & (1<<ICES1)) ){
        distance = ICR1 - LastCapture;      //dlugosc zbocza
        distance = distance/58;             //odleglosc w cm    (dla uint16_t odleglosc max 11m)
		if(distance > 50)
			PB1 = 1;
		if(distance > 100)
			PD7 = 1;
    }
    LastCapture = ICR1;

    TCCR1B ^= (1<<ICES1);   //zmiana zbocza
}

