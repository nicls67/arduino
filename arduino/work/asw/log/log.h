/*!
 * @file log.h
 *
 * @brief Header file for debug and logging functions
 *
 * @date 15 mars 2018
 * @author nicls67
 */

#ifndef WORK_LIB_LOG_H_
#define WORK_LIB_LOG_H_


/*!
 * @brief
 * @details This class defines functions used for sending debug data on USART link.
 */
class UsartDebug
{
public:

	/*!
	 * @brief Class UsartDebug constructor
	 * @details Initializes the class UsartDebug
	 * @return Nothing
	 */
	UsartDebug();

	/*!
	 * @brief Send a string on USART link
	 * @details This functions sends the requested string on USART link by calling driver's transmission function
	 * @param [in] str Pointer to the string being sent
	 * @return Nothing
	 */
	void sendData(char* str);

	/*!
	 * @brief Send a integer data on USART link
	 * @details This functions sends the requested integer on USART link by calling driver's transmission function.
	 * 			The integer is first converted into a string and then sent
	 * @param [in] data integer data to be sent
	 * @param [in] base numerical base used to convert integer into string (between 2 and 36)
	 * @return Nothing
	 */
	void sendInteger(uint16_t data, uint8_t base);

private:
};



#endif /* WORK_LIB_LOG_H_ */
