/*!
 * @file dio.cpp
 * @brief DIO library
 *
 * @date 13 mars 2018
 * @author nicls67
 */



#include "dio.h"


void dio::dio_init()
{
	/* Initialize ports as input or output */

	DDRB |= (1 << DDB7) ; /* PB7 as output port */
}

dio::dio()
{
	dio_init();
}

void dio::dio_setPortB(uint8_t pin, bool state)
{
	uint8_t mask;

	mask = ~(1 << pin);

	PORTB &= mask;

	PORTB |= (state << pin);
}

void dio::dio_invertPortB(uint8_t pin)
{
	PORTB ^= (1 << pin);
}

