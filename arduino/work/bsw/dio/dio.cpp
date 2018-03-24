/*!
 * @file dio.cpp
 * @brief DIO library
 *
 * @date 13 mars 2018
 * @author nicls67
 */


#include <avr/io.h>

#include "dio.h"

/*!
 * @brief Defines the configuration of DDRB register
 * @details This constant defines the direction of IO pins of PORT B. It will configure register DDRB.\n
 *          PB0 : N/A\n
 *          PB1 : N/A\n
 *          PB2 : N/A\n
 *          PB3 : N/A\n
 *          PB4 : N/A\n
 *          PB5 : N/A\n
 *          PB6 : OUT\n
 *          PB7 : OUT\n
 */
#define PORTB_CNF_DDRB (uint8_t)0b11000000

/*!
 * @brief Defines the configuration of PORTB register
 * @details This constant defines the initial value of IO pins for PORT B. It will configure register PORTB.
 * 			Pins configured as input shall not be configured here.\n
 *          PB0 : N/A\n
 *          PB1 : N/A\n
 *          PB2 : N/A\n
 *          PB3 : N/A\n
 *          PB4 : N/A\n
 *          PB5 : N/A\n
 *          PB6 : HIGH\n
 *          PB7 : HIGH\n
 */
#define PORTB_CNF_PORTB (uint8_t)0b11000000

void dio::dio_init()
{
	/* Initialize ports as input or output */
	DDRB = PORTB_CNF_DDRB;

	/* Set initial value for output pins */
	PORTB = PORTB_CNF_PORTB;
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


bool dio::dio_getPortB(uint8_t pin)
{
	bool res;

	res = (PINB >> pin) & 0x1;

	return res;
}

void dio::dio_changePortBPinCnf(uint8_t pin, uint8_t cnf)
{
	uint8_t mask = ~(1 << pin);

	/* reset selected pin */
	DDRB &= mask;

	/* If cnf is different from 0 or 1, set it by default to 0 (input) */
	if ((cnf != PORT_CNF_OUT) && (cnf != PORT_CNF_IN))
		cnf = PORT_CNF_IN;

	/* set selected pin to cnf */
	DDRB |= (cnf << pin);
}
