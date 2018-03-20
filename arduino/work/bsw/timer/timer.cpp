/*!
 * @file timer.cpp
 *
 * @brief Defines function for class timer.
 *
 * @date 15 mars 2018
 * @author nicls67
 */

#include <avr/io.h>
#include "timer.h"

timer::timer()
{
	prescaler = (uint8_t)0b100;
}

void timer::configureTimer1(uint16_t a_prescaler, uint16_t a_ctcValue)
{
	/* Configure the Timer/Counter1 Control Register A */
	TCCR1A = 0 ;
	/* Configure the Timer/Counter1 Control Register B */
	TCCR1B = (1 << WGM12) ; // Configure timer 1 for CTC mode

	/* Enable CTC interrupt */
	TIMSK1 = (1 << OCIE1A);

	/* Set CTC compare value */
	OCR1A = a_ctcValue;

	/* memorize prescaler value */
	switch(a_prescaler)
	{
	case 1:
		prescaler = (uint8_t)0b001;
		break;
	case 8:
		prescaler = (uint8_t)0b010;
		break;
	case 64:
		prescaler = (uint8_t)0b011;
		break;
	case 256:
		prescaler = (uint8_t)0b100;
		break;
	case 1024:
		prescaler = (uint8_t)0b101;
		break;
	default:
		/* Keep default value */
		break;
	}
}


void timer::startTimer1()
{
	uint8_t mask = (uint8_t)0b111;

	/* Reset bits 0-2 of TCCR1 */
	TCCR1B &= (~mask);

	/* Set bits 0-2 of TCCR1 to prescaler value to start the timer */
	TCCR1B |= prescaler ;
}

void timer::stopTimer1()
{
	/* Reset bits 0-2 of TCCR1 */
	uint8_t mask = (uint8_t)0b111;
	TCCR1B &= (~mask);
}
