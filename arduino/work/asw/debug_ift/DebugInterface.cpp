/*!
 * @file debug.cpp
 *
 * @brief This file defines classes for log and debug data transmission on USART link
 *
 * @date 15 mars 2018
 * @author nicls67
 */


#include <avr/io.h>
#include <stdlib.h>

#include "../../lib/LinkedList/LinkedList.h"
#include "../../lib/string/String.h"
#include "../../scheduler/scheduler.h"

#include "../../bsw/usart/usart.h"
#include "../../bsw/timer/timer.h"
#include "../../bsw/I2C/I2C.h"
#include "../../bsw/lcd/LCD.h"
#include "../../bsw/dio/dio.h"
#include "../../bsw/dht22/dht22.h"
#include "../../bsw/cpuLoad/CpuLoad.h"

#include "../../bsw/bsw.h"

#include "../debug_ift/DebugInterface.h"
#include "../debug_mgt/DebugManagement.h"
#include "../TempSensor/TempSensor.h"
#include "../display_ift/DisplayInterface.h"
#include "../display_mgt/DisplayManagement.h"
#include "../keepAliveLed/keepAliveLed.h"

#include "../asw.h"







DebugInterface::DebugInterface()
{
	/* Create a new instance of USART driver if needed and attach it to the class */
	if(BSW_cnf_struct.p_usart == 0)
		BSW_cnf_struct.p_usart = new usart(USART_BAUDRATE);

	usart_drv_ptr = BSW_cnf_struct.p_usart;
}

void DebugInterface::sendString(String* str)
{
	/* Call driver's transmission function to send the string str */
	BSW_cnf_struct.p_usart->usart_sendString(str);
}

void DebugInterface::sendString(uint8_t* str)
{
	String strToSend;
	strToSend.appendString(str);
	sendString(&strToSend);
}

void DebugInterface::sendChar(uint8_t chr)
{
	BSW_cnf_struct.p_usart->usart_sendByte(chr);
}

void DebugInterface::sendInteger(uint16_t data, uint8_t base)
{
	/* If the base in not between 2 and 36, 10 is used as default */
	if((base > 36) && (base < 2))
		base = 10;

	String strToSend;

	strToSend.appendInteger(data, base);

	BSW_cnf_struct.p_usart->usart_sendString(&strToSend);
}

void DebugInterface::sendBool(bool data, bool isText)
{
	String str;

	str.appendBool(data, isText);

	BSW_cnf_struct.p_usart->usart_sendString(&str);
}


