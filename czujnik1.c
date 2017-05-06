#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

uint16_t distance;
volatile uint16_t PulseWidth, LastCapture;

int main(void){
	DDRB |= (0<<PB0);   //ustawienie PB0 jako wejscie	(echo)
	DDRD |= (1<<PD7);	//trigger
	
	PORTB &= ~(1<<PB0);
	PORTD &= ~(1<<PD7);
	
	TCCR1B |= (1<<CS11);    //prescaler 8, 1 cykl zegara to 1 mikrosekunda (przy 1 MHz)
	TIMSK |= (1<<TICIE1);
	
	//przerwania na PB0
	TCCR1B |= (1<<ICES1);   //rosnace zbocze wyzwala przerwania
	
	sei();	// enable global interrupts

	while(1)
	{
		PORTD |= ( 1 << PD7);
		_delay_ms(10);
		PORTD &= ~( 1 << PD7);
		_delay_ms(60);
		
		distance = PulseWidth/58;             //odleglosc w cm    (dla uint16_t odleglosc max 11/2 m)
		
		if(distance < 50 && distance > 2)
			//costam
		if(distance < 100 && distance > 2)
			//costam
	}

}

ISR(TIMER1_CAPT_vect){  //przerwanie na PB0
	if( !(TCCR1B & (1<<ICES1)) ){
		PulseWidth = (ICR1 - LastCapture)/2;      //dlugosc zbocza	,przez 2 bo kwarc 16MHz i prescaler 8
	}
	LastCapture = ICR1;

	TCCR1B ^= (1<<ICES1);   //zmiana zbocza
}
