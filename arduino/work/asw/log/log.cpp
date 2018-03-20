/*!
 * @file log.cpp
 *
 * @brief This file defines classes for log and debug data transmission on USART link
 *
 * @date 15 mars 2018
 * @author nicls67
 */

#include <avr/io.h>

#include "log.h"
#include "../../bsw/usart/usart.h"
#include "../../bsw/dio/dio.h"
#include "../../bsw/timer/timer.h"
#include "../../bsw/bsw.h"

UsartDebug::UsartDebug()
{
	/* The function does nothing yet */
}

void UsartDebug::sendData(char* str)
{
	/* Call driver's transmission function to send the string str */
	BSW_cnf_struct.p_usart->usart_sendString((uint8_t*)str);
}



