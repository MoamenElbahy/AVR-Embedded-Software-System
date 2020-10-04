/* INCLUDES ******************************************************************/
#include "ses_timer.h"
#include "ses_motorFrequency.h"
#include "ses_led.h"
/* DEFINES & MACROS **********************************************************/

volatile static pTimerCallback pTimer1;
volatile static pTimerCallback pTimer2;
volatile static pTimer5Callback pTimer5;

//FUNCTION DEFINITION *******************************************************

void timer2_setCallback(pTimerCallback cb)
{
	pTimer2 = cb;
}

void timer2_start()
{
	TIMSK2 |= (1 << OCIE2A); //the Timer/Counter2 Compare Match A interrupt is enabled.
	TIFR2 |= (1 << OCF2A);	 //OCF2A bit is set in the Timer/Counter2 Interrupt Flag Register TIFR2

	TCCR2A &= ~(1 << WGM20); // Use Clear Timer on Compare Match (CTC) mode operation
	TCCR2A |= (1 << WGM21);	 // WGM2 : wave form generation mode timer 2
	TCCR2B &= ~(1 << WGM22); //

	TCCR2B |= (1 << CS22);	//Setting the prescalar to 64
	TCCR2B &= ~(1 << CS21); //
	TCCR2B &= ~(1 << CS20); //

	OCR2A = TIMER2_CYC_FOR_1MILLISEC;
}

void timer1_setCallback(pTimerCallback cb)
{
	pTimer1 = cb;
}

void timer1_start(uint8_t timer1Interval)
{
	TIMSK1 |= (1 << OCIE1A); //the Timer/Counter2 Compare Match A interrupt is enabled.
	TIFR1 |= (1 << OCF1A);	 //OCF1A bit is set in the Timer/Counter2 Interrupt Flag Register TIFR1

	TCCR1A &= ~(1 << WGM10); // Use Clear Timer on Compare Match (CTC) mode operation
	TCCR1A &= ~(1 << WGM11); // WGM1 : wave form generation mode timer 1
	TCCR1B |= (1 << WGM12);	 // value set to 1 0 0

	TCCR1B &= ~(1 << CS12); //Setting the prescalar to 64
	TCCR1B |= (1 << CS11);	//
	TCCR1B |= (1 << CS10);	//

	OCR1A = TIMER2_CYC_FOR_1MILLISEC * timer1Interval;
}

void timer5_setCallback(pTimer5Callback cb)
{
	pTimer5 = cb;
}


void timer5_start()
{
	TIMSK5 |= (1 << OCIE5A); //the Timer/Counter2 Compare Match A interrupt is enabled.
	TIFR5 |= (1 << OCF5A);	 //OCF5A bit is set in the Timer/Counter2 Interrupt Flag Register TIFR5

	TCCR5A &= ~(1 << WGM50); // Use Clear Timer on Compare Match (CTC) mode operation
	TCCR5A &= ~(1 << WGM51); // WGM1 : wave form generation mode timer 5
	TCCR5B |= (1 << WGM52);	 // value set to 1 0 1

	TCCR5B |= (1 << CS52); //Setting the prescalar to 1024
	TCCR5B &= ~(1 << CS51);	//
	TCCR5B |= (1 << CS50);	//

	OCR5A = ((F_CPU / PRESCALER5)) / MODIFIER ; //timer overflows MODIFIER times per second
}

void timer5_stop()
{
	TCCR5B &= ~((1<< CS50) | (1<<CS51) | (1<<CS52));
}

ISR(TIMER1_COMPA_vect)
{
	pTimer1();
}

ISR(TIMER2_COMPA_vect)
{
	pTimer2();
}

ISR(TIMER5_COMPA_vect)
{
	pTimer5();
}