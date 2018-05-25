/*!
 * @file log.cpp
 *
 * @brief This file defines classes for log and debug data transmission on USART link
 *
 * @date 15 mars 2018
 * @author nicls67
 */

#include <avr/io.h>
#include <stdlib.h>

#include "log.h"



UsartDebug::UsartDebug()
{
#ifdef F_DISPLAY_TEMP
	/* Add task for temperature display */
	p_scheduler->addPeriodicTask((TaskPtr_t)(&UsartDebug::DisplayTempAndHumUsart_task), PERIOD_MS_TASK_DISPLAY_TEMP);
#endif
}

void UsartDebug::sendData(char* str)
{
	/* Call driver's transmission function to send the string str */
	BSW_cnf_struct.p_usart->usart_sendString((uint8_t*)str);
}

void UsartDebug::sendInteger(uint16_t data, uint8_t base)
{
	/* If the base in not between 2 and 36, 10 is used as default */
	if((base > 36) && (base < 2))
		base = 10;

	char* str = (char*)malloc(5* sizeof(char));
	str = itoa(data, str, base);

	BSW_cnf_struct.p_usart->usart_sendString((uint8_t*)str);

	free(str);
}

void UsartDebug::sendBool(bool data)
{
	uint8_t* str = (uint8_t*)malloc(2*sizeof(uint8_t));

	if (data == true)
		str = (uint8_t*)"1";
	else
		str = (uint8_t*)"0";

	BSW_cnf_struct.p_usart->usart_sendString(str);
}

void UsartDebug::DisplayTempAndHumUsart_task()
{
	uint16_t data;
	uint8_t data_int, data_dec;
	bool validity;

	ASW_cnf_struct.p_usartDebug->sendData((char*)"\n\nTemperature : ");

	validity = ASW_cnf_struct.p_TempSensor->getTemp(&data);

	if(validity)
	{
		data_int = (uint8_t)(data/10);
		data_dec = (uint8_t)(data%10);
		ASW_cnf_struct.p_usartDebug->sendInteger(data_int,10);
		ASW_cnf_struct.p_usartDebug->sendData((char*)".");
		ASW_cnf_struct.p_usartDebug->sendInteger(data_dec,10);
	}
	else
		ASW_cnf_struct.p_usartDebug->sendData((char*)"invalid");


	ASW_cnf_struct.p_usartDebug->sendData((char*)" degC\nHumidite : ");

	validity = ASW_cnf_struct.p_TempSensor->getHumidity(&data);

	if(validity)
	{
		data_int = (uint8_t)(data/10);
		data_dec = (uint8_t)(data%10);
		ASW_cnf_struct.p_usartDebug->sendInteger(data_int,10);
		ASW_cnf_struct.p_usartDebug->sendData((char*)".");
		ASW_cnf_struct.p_usartDebug->sendInteger(data_dec,10);
	}
	else
		ASW_cnf_struct.p_usartDebug->sendData((char*)"invalid");

	ASW_cnf_struct.p_usartDebug->sendData((char*)" %");

}
