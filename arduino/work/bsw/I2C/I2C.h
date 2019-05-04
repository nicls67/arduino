/*!
 * @file I2C.h
 *
 * @brief I2C class header file
 *
 * @date 19 avr. 2019
 * @author nicls67
 */

#ifndef WORK_BSW_I2C_I2C_H_
#define WORK_BSW_I2C_I2C_H_

#define START 0x08 /*!< TWSR status code : START condition transmitted */
#define SLA_ACK 0x18 /*!< TWSR status code : SLA has been transmitted and ACK has been received */
#define DATA_ACK 0x28 /*!< TWSR status code : DATA has been transmitted and ACK has been received */



/*!
 * @brief Two-wire serial interface (I2C) class definition
 * @details This class manages I2C driver.
 */
class I2C {
public:
	/*!
	 * @brief I2C class constructor
	 * @details This function initializes the I2C class and calls bus initialization function
	 *
	 * @param [in] l_bitrate Requested bitrate for I2C bus (in Hz)
	 * @return Nothing
	 */
	I2C(uint32_t l_bitrate);

	/*!
	 * @brief Byte sending function
	 * @details This function sends one byte on I2C bus
	 *
	 * @param [in] data Pointer to the data to send
	 * @return True if transmission is completed, False if an error has occurred
	 */
	bool writeByte(uint8_t* data);

	/*!
	 * @brief Setting function for Tx I2C address
	 * @details This function sets the given Tx I2C address in the internal class variable.
	 *
	 * @param [in] address Requested Tx address
	 * @return Nothing
	 */
	void setTxAddress(uint8_t address);

	/*!
	 * @brief Variable bitrate setting function
	 * @details This function sets the class variable bitrate as requested in parameter.
	 *
	 * @param [in] l_bitrate Requested bitrate (in Hz)
	 * @return Nothing
	 */
	void setBitRate(uint32_t l_bitrate);

private:
	uint8_t tx_address;
	uint32_t bitrate;

	/*!
	 * @brief I2C bus initialization
	 * @details This function initializes the I2C bus, it resets the bus and configure the bitrate as requested.
	 * 			Bitrate is configured according to formula in the ATMEGA2560 datasheet : SCL freq = F_CPU / (16 + 2*TWBR*(4^TWPS)).
	 * 			Prescaler value is fixed to 1 (TWPS1 = 0 and TWPS0 = 0), then only TWBR value shall be computed.
	 *
	 * @return Nothing
	 */
	void initializeBus();
};


#endif /* WORK_BSW_I2C_I2C_H_ */
