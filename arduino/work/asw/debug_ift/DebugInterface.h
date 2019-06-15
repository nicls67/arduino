/*!
 * @file DebugInterface.h
 *
 * @brief Header file for debug and logging functions
 *
 * @date 15 mars 2018
 * @author nicls67
 */

#ifndef WORK_LIB_LOG_H_
#define WORK_LIB_LOG_H_



#define USART_BAUDRATE (uint16_t)9600 /*!< usart connection to PC uses a baud rate of 9600 */

/*!
 * @brief Class used for debugging on usart link
 * @details This class defines functions used for sending debug data on USART link.
 */
class DebugInterface
{
public:

	/*!
	 * @brief Class DebugInterface constructor
	 * @details Initializes the class DebugInterface. It creates a new instance of USART driver of needed.
	 * @return Nothing
	 */
	DebugInterface();

	/*!
	 * @brief Send a integer data on USART link
	 * @details This function sends the requested integer on USART link by calling driver's transmission function.
	 * 			The integer is first converted into a string and then sent
	 * @param [in] data integer data to be sent
	 * @param [in] base numerical base used to convert integer into string (between 2 and 36)
	 * @return Nothing
	 */
	void sendInteger(uint16_t data, uint8_t base);

	/*!
	* @brief Send a boolean data on USART link
	* @details 	This function sends the requested boolean on USART link by calling driver's transmission function.
	* 			The boolean data is first converted into a string and then sent. The parameter isText defines if the data is converted into a string (true/false) or an integer (1/0).
	* @param [in] data boolean data to be sent
	* @param [in] isText String conversion configuration
	* @return Nothing
	*/
	void sendBool(bool data, bool isText);

	/*!
	 * @brief Send a string on USART link
	 * @details This function sends the requested string on USART link by calling driver's transmission function
	 * @param [in] str Pointer to the string being sent
	 * @return Nothing
	 */
	void sendString(String* str);

	/*!
	 * @brief Send a chain of characters on USART link
	 * @details This function sends the requested chain of characters on USART link by calling driver's transmission function.
	 * 			The chain is first converted into a string object.
	 * @param [in] str Pointer to the chain to send.
	 * @return Nothing
	 */
	void sendString(uint8_t* str);

	/*!
	 * @brief Send a single character on USART link
	 * @details This function sends the requested character on USART link by calling driver's transmission function.
	 * @param [in] chr Character to send.
	 * @return Nothing
	 */
	void sendChar(uint8_t chr);

	/*!
	 * @brief USART read function
	 * @details This function will read the last received byte on USART link
	 * @return Received byte
	 */
	inline uint8_t read()
	{
		return usart_drv_ptr->usart_read();
	}

	/*!
	 * @brief Go to next line function
	 * @details This function goes to the next line on the console display. It sends the two characters \c \\n and \c \\r on the USART line.
	 *
	 * @return Nothing
	 */
	void nextLine();

	/*!
	 * @brief Screen clearing function
	 * @details This function clears the entire display by sending the \c \\f character on the USART line.
	 *
	 * @return Nothing
	 */
	void ClearScreen();



private:

	usart* usart_drv_ptr; /*!< Pointer to USART driver object */


};

extern DebugInterface* p_global_ASW_DebugInterface; /*!< Pointer to USART debug interface object */

#endif /* WORK_LIB_LOG_H_ */
