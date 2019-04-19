/*!
 * @file I2C.cpp
 *
 * @brief Two-wire interface (I2C) source file
 *
 * @date 19 avr. 2019
 * @author nicls67
 */

#include <avr/io.h>

#include "I2C.h"


I2C::I2C()
{
	tx_address = 0;
}

bool I2C::writeByte(uint8_t* data)
{
	/* Send START condition */
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);

	/* Wait until TWINT flag is set */
	while (!(TWCR & (1<<TWINT)));

	/* If an error has occurred, stop the transmission */
	if ((TWSR & 0xF8) != START)
		return false;

	/* Load I2C address into TWDR register and start transmission of the address */
	TWDR = tx_address;
	TWCR = (1<<TWINT) | (1<<TWEN);

	/* Wait until TWINT flag is set */
	while (!(TWCR & (1<<TWINT)));

	/* If an error has occurred, stop the transmission */
	if ((TWSR & 0xF8) != SLA_ACK)
		return false;

	/* Load data into TWDR and start transmission */
	TWDR = *data;
	TWCR = (1<<TWINT) | (1<<TWEN);

	/* Wait until TWINT flag is set */
	while (!(TWCR & (1<<TWINT)));

	/* If an error has occurred, stop the transmission */
	if ((TWSR & 0xF8) != DATA_ACK)
		return false;

	/* Send STOP condition */
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);

	return true;
}

void I2C::setTxAddress(uint8_t address)
{
	tx_address = address;
}
