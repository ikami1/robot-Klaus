////////////////////////////////////
//
// 	Title: "cny.c"
//  Created on: Apr 21, 2017
//  Author: Kyrpat
//  Version: 0.02 beta
//
////////////////////////////////////
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

// :))))
#define bool char
#define true 1
#define false 0

#define isNigga >=900

#define maxSpeed 220

////////////////////////////////////
// setBit(Reg, Bit1, ...)
// resetBit(Reg, Bit1, ...)
////////////////////////////////////
#define setBit(a,b) a|=(1<<b)
#define setBit2(a,b,c) a|=(1<<b)|(1<<c)
#define setBit3(a,b,c,d) a|=(1<<b)|(1<<c)|(1<<d)
#define setBit4(a,b,c,d,e) a|=(1<<b)|(1<<c)|(1<<d)|(1<<e)
#define resetBit(a,b) a&=~(1<<b)
#define resetBit2(a,b,c) a&=~((1<<b)|(1<<c))
#define resetBit3(a,b,c,d) a&=~((1<<b)|(1<<c)|(1<<d))
#define resetBit4(a,b,c,d,e) a&=~((1<<b)|(1<<c)|(1<<d)|(1<<e))
#define adcValue ADC


inline void setCurrADC(char n)
{
	setBit(ADMUX, REFS0);
	if((n & 4) != 0) setBit(ADMUX, MUX2); else resetBit(ADMUX, MUX2);
	if((n & 2) != 0) setBit(ADMUX, MUX1); else resetBit(ADMUX, MUX1);
	if((n & 1) != 0) setBit(ADMUX, MUX0); else resetBit(ADMUX, MUX0);
}

inline void readResult()
{
	setBit(ADCSRA, ADSC);
    while(ADCSRA & (1<<ADSC));
}

inline void init()
{
	setBit(TCCR2, COM21);
	setBit2(TCCR2, WGM21, WGM20);
	setBit(TCCR2, CS21);

	// Fast PWM 8bit
	setBit(TCCR1A, WGM10);
	setBit(TCCR1B, WGM12);

	setBit2(TCCR1A, COM1A1, COM1B1);
	setBit2(TCCR1B, CS10, CS11);

	setBit2(ADCSRA, ADEN, ADPS2);
	setBit(ADMUX, REFS0);

	setBit(DDRB,PB1);
	setBit(DDRB,PB2);

	DDRC = 0;

	OCR2 = 255;
}

inline short readADC(char n)
{
	setCurrADC(n);
	readResult();
	return adcValue;
}


int main(void)
{
	int i;
	unsigned int cny[6];

	_delay_ms(1000);

	int dev = 0;

	init();

	unsigned short black[6];
	for(i=0;i<6;i++) { black[i] = readADC(i)*0.80; _delay_ms(50); };

	//Czarek
	float e=0;
    int setpoint = 0;   //wart zadana
    float v = 0;          //predkosc silnika
    int vb = 180;       //predkosc bazowa
    float waga = 0;
    int kp = 30;        //wzmocnienie p
    int ile_czujnikow = 0;
    int flaga = 0;

    while(1)
    {
        ile_czujnikow = 0;
        waga = 0;

		//for(i=2; i<6; i++) { cny[i]=readADC(i); }
		if(readADC(2) >= black[2]){
			flaga = 1;
            ile_czujnikow++;
            waga += 5;
		}
		if(readADC(5) >= black[5]){
			flaga = -1;
            ile_czujnikow++;
            waga += -5;
		}
		if(readADC(3) >= black[3]){
					flaga = 0;
		            ile_czujnikow++;
		            waga += 1;
		}
		if(readADC(4) >= black[4]){
			flaga = 0;
			ile_czujnikow++;
			waga += -1;
		}

		waga /= ile_czujnikow;
        e = setpoint - waga;
        if(e < 0)
            v = vb - (kp*e);
        else
            v = vb + (kp*e);

        if(v > 250)
        	v = 250;

        if(e == 0){
        	if(flaga == 0){
				OCR1A = v;
				OCR1B = v;
        	}
        	else{
        		if(flaga < -1){
        			OCR1A = v;
        			OCR1B = 0;
        		}
        		else{
        			OCR1A = 0;
        			OCR1B = v;
        		}
        	}
        }
        if(e > 0){
            OCR1A = v;
            if(waga == -1)
                OCR1B = v/4;
            else
                OCR1B = 0;
        }
        if(e < 0){
            if(waga == 1)
                OCR1A = v/4;
            else
                OCR1A = 0;
            OCR1B = v;
        }

		_delay_ms(20);
    }
}

