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

#include <avr/io.h>
#include "../../bsw/bsw.h"

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
	 * @brief Send a debug data on USART link
	 * @details This functions sends the requested string on USART link by calling driver's transmission function
	 * @param [in] str Pointer to the string being sent
	 * @return Nothing
	 */
	void sendData(uint8_t* str);

private:
};



#endif /* WORK_LIB_LOG_H_ */
