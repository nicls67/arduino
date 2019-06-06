/*!
 * @file DebugInterface.cpp
 *
 * @brief This file defines classes for log and debug data transmission on USART link
 *
 * @date 15 mars 2018
 * @author nicls67
 */


#include <avr/io.h>
#include <stdlib.h>

#include "../../lib/string/String.h"
#include "../../bsw/usart/usart.h"
#include "DebugInterface.h"


DebugInterface* p_global_ASW_DebugInterface;


DebugInterface::DebugInterface()
{
	/* Create a new instance of USART driver if needed and attach it to the class */
	if(p_global_BSW_usart == 0)
		p_global_BSW_usart = new usart(USART_BAUDRATE);

	usart_drv_ptr = p_global_BSW_usart;
}

void DebugInterface::sendString(String* str)
{
	/* Call driver's transmission function to send the string str */
	usart_drv_ptr->usart_sendString(str);
}

void DebugInterface::sendString(uint8_t* str)
{
	String strToSend;
	strToSend.appendString(str);
	sendString(&strToSend);
}

void DebugInterface::sendChar(uint8_t chr)
{
	usart_drv_ptr->usart_sendByte(chr);
}

void DebugInterface::sendInteger(uint16_t data, uint8_t base)
{
	/* If the base in not between 2 and 36, 10 is used as default */
	if((base > 36) && (base < 2))
		base = 10;

	String strToSend;

	strToSend.appendInteger(data, base);

	usart_drv_ptr->usart_sendString(&strToSend);
}

void DebugInterface::sendBool(bool data, bool isText)
{
	String str;

	str.appendBool(data, isText);

	usart_drv_ptr->usart_sendString(&str);
}

void DebugInterface::nextLine()
{
	sendChar((uint8_t)'\n');
	sendChar((uint8_t)'\r');
}

void DebugInterface::ClearScreen()
{
	sendChar((uint8_t)'\f');
}
