/*!
 * @file log.cpp
 *
 * @brief This file defines classes for log and debug data transmission on USART link
 *
 * @date 15 mars 2018
 * @author nicls67
 */

#include "log.h"

UsartDebug::UsartDebug()
{
	/* The function does nothing yet */
}

void UsartDebug::sendData(uint8_t* str)
{
	/* Call driver's transmission function to send the string str */
	BSW_cnf_struct.p_usart->usart_sendString(str);
}



