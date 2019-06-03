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

I2C* p_global_BSW_i2c;

I2C::I2C(uint32_t l_bitrate)
{
	tx_address = 0;
	bitrate = l_bitrate;

	initializeBus();
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

	/* Load I2C address into TWDR register and start transmission of the address
	 * The address is shifted by 1 bit to the left, as bit 0 of SLA word is for RW condition
	 * Here we are in a write process, then bit 0 is set to 0 */
	TWDR = (tx_address << 1);
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

void I2C::setBitRate(uint32_t l_bitrate)
{
	bitrate = l_bitrate;
}

void I2C::initializeBus()
{
	uint8_t data = 0;

	/* Bit rate configuration */

	/* Prescaler value is fixed to 1 (TWPS1 = 0 and TWPS0 = 0), then only TWBR value shall be computed.*/
	TWSR &= 0b11111100; /* Clear TWPS bits */

	/* Compute value of register TWBR : SCL freq = F_CPU / (16 + 2*TWBR*(4^TWPS))
	 * => TWBR = ((FCPU / SCL freq) - 16)/8 */
	data = (uint8_t)(((F_CPU / bitrate) - 16) / 8);
	TWBR = data;
}
