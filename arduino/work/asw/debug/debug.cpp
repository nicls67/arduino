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

#include "debug.h"

/*!
 * @brief Main menu of debug mode
 */
const char str_debug_main_menu[] =
		"\n\n"
		"Menu principal :  \n"
		"1 : Afficher donnees capteurs\n"
		"2 : Afficher charge CPU\n"
		"\n"
		"s : Quitter debug\n";




UsartDebug::UsartDebug()
{
	debugModeActive_F = false;
	debug_state = INIT;
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

void UsartDebug::DisplaySensors_task()
{
	uint16_t data;
	uint8_t data_int, data_dec;
	bool validity;

	ASW_cnf_struct.p_usartDebug->sendData((char*)"\n\nTemperature : ");

	validity = ASW_cnf_struct.p_TempSensor->getTemp(&data);

	/* TODO : add function for data formatting */

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

void UsartDebug::DisplayCPULoad_task()
{
	ASW_cnf_struct.p_usartDebug->sendData((char*)"\n\nCharge CPU :\n");
	ASW_cnf_struct.p_usartDebug->sendData((char*)"    Actuelle : ");
	ASW_cnf_struct.p_usartDebug->sendInteger(BSW_cnf_struct.p_cpuload->getCurrrentCPULoad(),10);
	ASW_cnf_struct.p_usartDebug->sendData((char*)"\n    Moyenne : ");
	ASW_cnf_struct.p_usartDebug->sendInteger(BSW_cnf_struct.p_cpuload->getAverageCPULoad(),10);
	ASW_cnf_struct.p_usartDebug->sendData((char*)"\n    Max : ");
	ASW_cnf_struct.p_usartDebug->sendInteger(BSW_cnf_struct.p_cpuload->getMaxCPULoad(),10);
}

bool UsartDebug::isDebugModeActive()
{
	return debugModeActive_F;
}

void UsartDebug::activateDebugMode()
{
	debugModeActive_F = true;

	ASW_cnf_struct.p_usartDebug->sendData((char*)"Debug actif!\n");
	ASW_cnf_struct.p_usartDebug->DebugModeManagement((uint8_t)0);
}

void UsartDebug::DebugModeManagement(uint8_t rcv_char)
{
	/* switch on debug state */
	switch(debug_state)
	{
	case INIT:
		ASW_cnf_struct.p_usartDebug->sendData((char*)str_debug_main_menu);
		debug_state = WAIT_INIT;
		break;

	case WAIT_INIT:
		switch (rcv_char)
		{
		case 's':
			ASW_cnf_struct.p_usartDebug->sendData((char*)"\nBye !\n");
			debugModeActive_F = false;
			debug_state = INIT;
			break;

		case '1':
			debug_state = DISPLAY_DATA;
			ASW_cnf_struct.p_usartDebug->sendData((char*)"\nOk, appuyer sur s pour arreter !\n");
			p_scheduler->addPeriodicTask((TaskPtr_t)(&UsartDebug::DisplaySensors_task), PERIOD_MS_TASK_DISPLAY_SENSORS);
			break;

		case '2':
			debug_state = DISPLAY_CPU_LOAD;
			ASW_cnf_struct.p_usartDebug->sendData((char*)"\nOk, appuyer sur s pour arreter !\n");
			p_scheduler->addPeriodicTask((TaskPtr_t)(&UsartDebug::DisplayCPULoad_task), PERIOD_MS_TASK_DISPLAY_CPU_LOAD);
			break;

		default:
			ASW_cnf_struct.p_usartDebug->sendData((char*)"\nImpossible de faire ca... !\n");
			break;
		}
		break;

	case DISPLAY_DATA:
		if (rcv_char == 's')
		{
			if (p_scheduler->removePeriodicTask(&UsartDebug::DisplaySensors_task) == false)
				ASW_cnf_struct.p_usartDebug->sendData((char*)"Impossible de supprimer la tache...");

			debug_state = WAIT_INIT;
			ASW_cnf_struct.p_usartDebug->sendData((char*)str_debug_main_menu);
		}
		break;

	case DISPLAY_CPU_LOAD:
		if (rcv_char == 's')
		{
			if (p_scheduler->removePeriodicTask(&UsartDebug::DisplayCPULoad_task) == false)
				ASW_cnf_struct.p_usartDebug->sendData((char*)"Impossible de supprimer la tache...");

			debug_state = WAIT_INIT;
			ASW_cnf_struct.p_usartDebug->sendData((char*)str_debug_main_menu);
		}
		break;

	}

}
