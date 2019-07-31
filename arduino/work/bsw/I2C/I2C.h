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
#define REPEATED_START 0x10 /*!< TWSR status code : REPEATED START condition transmitted */
#define SLAW_ACK 0x18 /*!< TWSR status code : SLA+W has been transmitted and ACK has been received */
#define DATA_ACK 0x28 /*!< TWSR status code : DATA has been transmitted and ACK has been received */
#define SLAR_ACK 0x40 /*!< TWSR status code : SLA+R has been transmitted and ACK has been received */



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
	 * @details This function sends one byte to the I2C device with the given address.
	 * 			It only calls "write" function with size equal to 1. Kept for compatibility with LCD driver.
	 *
	 * @param [in] data Data to send
	 * @param [in] tx_address I2C address of the device
	 * @param [in] sendStopCond Defines if the stop condition shall be sent or not
	 * @return True if transmission is completed, False if an error has occurred
	 */
	bool writeByte(uint8_t data, uint8_t tx_address, bool sendStopCond);

	/*!
	 * @brief I2C write function
	 * @details This function sends the requested number of bytes to the I2C device with the given address
	 *
	 * @param [in] data Pointer to the data to send
	 * @param [in] tx_address I2C address of the device
	 * @param [in] size Number of bytes to send
	 * @param [in] sendStopCond Defines if the stop condition shall be sent or not
	 * @return True if transmission is completed, False if an error has occurred
	 */
	bool write(uint8_t* data, uint8_t tx_address, uint8_t size, bool sendStopCond);

	/*!
	 * @brief I2C read function
	 * @details This function performs a read operation on the I2C. The requested number of bytes is read on the bus
	 * 			and the received data are copied in the reception buffer. Calling function has to check that enough
	 * 			space is allocated for the reception buffer.
	 *
	 * @param [in] i2c_address I2C address of the device
	 * @param [in] size Number of bytes to read
	 * @param [out] buf_ptr Pointer to the start of the reception buffer.
	 * @return True if the receive process has succeeded, false otherwise
	 */
	bool read(uint8_t i2c_address, uint8_t size, uint8_t* buf_ptr);

	/*!
	 * @brief Variable bitrate setting function
	 * @details This function sets the class variable bitrate as requested in parameter.
	 *
	 * @param [in] l_bitrate Requested bitrate (in Hz)
	 * @return Nothing
	 */
	void setBitRate(uint32_t l_bitrate);

private:
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

extern I2C* p_global_BSW_i2c; /*!< Pointer to I2C driver object */

#endif /* WORK_BSW_I2C_I2C_H_ */
