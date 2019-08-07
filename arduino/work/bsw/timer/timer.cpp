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

timer* p_global_BSW_timer;

timer::timer()
{
	prescaler1 = (uint8_t)0b100;
	prescaler3 = (uint8_t)0b100;
	prescaler4 = (uint8_t)0b100;
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
		prescaler1 = (uint8_t)0b001;
		break;
	case 8:
		prescaler1 = (uint8_t)0b010;
		break;
	case 64:
		prescaler1 = (uint8_t)0b011;
		break;
	case 256:
		prescaler1 = (uint8_t)0b100;
		break;
	case 1024:
		prescaler1 = (uint8_t)0b101;
		break;
	default:
		/* Keep default value */
		break;
	}
}

void timer::configureTimer3(uint16_t a_prescaler, uint16_t a_ctcValue)
{
	/* Configure the Timer/Counter1 Control Register A */
	TCCR3A = 0 ;
	/* Configure the Timer/Counter1 Control Register B */
	TCCR3B = (1 << WGM12) ; // Configure timer 1 for CTC mode

	/* Enable CTC interrupt */
	TIMSK3 = (1 << OCIE3A);

	/* Set CTC compare value */
	OCR3A = a_ctcValue;

	/* memorize prescaler value */
	switch(a_prescaler)
	{
	case 1:
		prescaler3 = (uint8_t)0b001;
		break;
	case 8:
		prescaler3 = (uint8_t)0b010;
		break;
	case 64:
		prescaler3 = (uint8_t)0b011;
		break;
	case 256:
		prescaler3 = (uint8_t)0b100;
		break;
	case 1024:
		prescaler3 = (uint8_t)0b101;
		break;
	default:
		/* Keep default value */
		break;
	}
}

void timer::configureTimer4(uint16_t a_prescaler, uint16_t a_ctcValue)
{
	/* Configure the Timer/Counter4 Control Register A */
	TCCR4A = 0 ;
	/* Configure the Timer/Counter4 Control Register B */
	TCCR4B = (1 << WGM12) ; // Configure timer 1 for CTC mode

	/* Enable CTC interrupt */
	TIMSK4 = (1 << OCIE4A);

	/* Set CTC compare value */
	OCR4A = a_ctcValue;

	/* memorize prescaler value */
	switch(a_prescaler)
	{
	case 1:
		prescaler4 = (uint8_t)0b001;
		break;
	case 8:
		prescaler4 = (uint8_t)0b010;
		break;
	case 64:
		prescaler4 = (uint8_t)0b011;
		break;
	case 256:
		prescaler4 = (uint8_t)0b100;
		break;
	case 1024:
		prescaler4 = (uint8_t)0b101;
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
	TCCR1B |= prescaler1 ;
}

void timer::startTimer3()
{
	uint8_t mask = (uint8_t)0b111;

	/* Reset bits 0-2 of TCCR3 */
	TCCR3B &= (~mask);

	/* Set bits 0-2 of TCCR3 to prescaler value to start the timer */
	TCCR3B |= prescaler3 ;
}

void timer::startTimer4()
{
	uint8_t mask = (uint8_t)0b111;

	/* Reset bits 0-2 of TCCR4 */
	TCCR4B &= (~mask);

	/* Set bits 0-2 of TCCR4 to prescaler value to start the timer */
	TCCR4B |= prescaler4 ;
}

void timer::stopTimer1()
{
	/* Reset bits 0-2 of TCCR1 */
	uint8_t mask = (uint8_t)0b111;
	TCCR1B &= (~mask);
}

void timer::stopTimer3()
{
	/* Reset bits 0-2 of TCCR3 */
	uint8_t mask = (uint8_t)0b111;
	TCCR3B &= (~mask);
}

void timer::stopTimer4()
{
	/* Reset bits 0-2 of TCCR4 */
	uint8_t mask = (uint8_t)0b111;
	TCCR4B &= (~mask);
}


