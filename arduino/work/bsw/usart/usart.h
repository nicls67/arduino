/*!
 * @file usart.h
 * @brief Header file for USART library
 *
 * @date 13 mars 2018
 * @author nicls67
 */

#ifndef WORK_BSW_USART_USART_H_
#define WORK_BSW_USART_USART_H_

/*!
 * @brief USART serial bus class
 * @details This class defines all useful functions for USART serial bus
 */
class usart
{
public:

	/*!
	 * @brief Class usart constructor
	 * @details Initializes the class and call hardware initialization function
	 * @param [in] a_BaudRate Desired Baud Rate (16 bit) - up to 57600
	 * @return Nothing.
	 */
	usart(uint16_t a_BaudRate);

	/*! @brief Send a string on USART link
	 *  @details This function writes the string object data to the serial link using usart_transmit function
	 *  @param [in] str Pointer to the string being sent
	 *  @return Nothing.
	 */
	void usart_sendString(String *str);

	/*! @brief Send a single byte on USART link
	 *  @details This function writes the given byte to the serial link using usart_transmit function
	 *  @param [in] data Data byte being sent
	 *  @return Nothing.
	 */
	void usart_sendByte(uint8_t data);

	/*!
	 * @brief Setting baud rate
	 * @details This function sets the attribute BaudRate of the class usart
	 * @param [in] a_BaudRate Desired Baud Rate (16 bit) - up to 57600
	 * @return Nothing
	 */
	void setBaudRate(uint16_t a_BaudRate);

	/*! @brief USART hardware initialization
	 *  @details This function will initialize the USART using selected baudrate.
	 *  		 User must pay attention to select one of the usually used Baud Rate (9600, 19200, 38400, 57600).
	 *  		 Note that since an uint16 is used as argument, Baud rate cannot be more than 57600.
	 *  @return Nothing.
	 */
	void usart_init();

	/*! @brief USART read function
	 *  @details This function will read reception register of USART
	 *  @return The function returns the 8 bits read from reception buffer
	 */
	uint8_t usart_read();




private:

	/*! @brief USART Transmit data
	 *  @details Nothing Special. It just wait for the transmit buffer is empty before writing it again.
	 *  @param [in] Data Desired data char to transmit
	 *  @return Nothing.
	 */
	void usart_transmit( uint8_t Data ) ;


	uint16_t BaudRate; /*!< Defines the baud rate used by driver */

};

extern usart* p_global_BSW_usart; /*!< Pointer to usart driver object */

#endif /* WORK_BSW_USART_USART_H_ */
