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
#include "../../bsw/lcd/LCD.h"
#include "../../bsw/I2C/I2C.h"
#include "../../bsw/dio/dio.h"
#include "../../bsw/dht22/dht22.h"
#include "../../bsw/cpuLoad/CpuLoad.h"

#include "../../bsw/bsw.h"

#include "../debug/debug.h"
#include "../TempSensor/TempSensor.h"
#include "../display_ift/DisplayInterface.h"
#include "../display_mgt/DisplayManagement.h"
#include "../keepAliveLed/keepAliveLed.h"

#include "../asw.h"

#include "debug.h"

/*!
 * @brief Main menu of debug mode
 */
const uint8_t str_debug_main_menu[] =
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

void UsartDebug::sendString(String* str)
{
	/* Call driver's transmission function to send the string str */
	BSW_cnf_struct.p_usart->usart_sendString(str);
}

void UsartDebug::sendString(uint8_t* str)
{
	String strToSend;
	strToSend.appendString(str);
	sendString(&strToSend);
}

void UsartDebug::sendInteger(uint16_t data, uint8_t base)
{
	/* If the base in not between 2 and 36, 10 is used as default */
	if((base > 36) && (base < 2))
		base = 10;

	String strToSend;

	strToSend.appendInteger(data, base);

	BSW_cnf_struct.p_usart->usart_sendString(&strToSend);
}

void UsartDebug::sendBool(bool data, bool isText)
{
	String str;

	str.appendBool(data, isText);

	BSW_cnf_struct.p_usart->usart_sendString(&str);
}

void UsartDebug::DisplaySensors_task()
{
	bool validity;

	validity = ASW_cnf_struct.p_TempSensor->GetValidity();

	ASW_cnf_struct.p_usartDebug->sendString((uint8_t*)"\n\nTemperature : ");

	if(validity)
	{
		ASW_cnf_struct.p_usartDebug->sendInteger(ASW_cnf_struct.p_TempSensor->GetTempInteger(),10);
		ASW_cnf_struct.p_usartDebug->sendString((uint8_t*)".");
		ASW_cnf_struct.p_usartDebug->sendInteger(ASW_cnf_struct.p_TempSensor->GetTempDecimal(),10);
	}
	else
		ASW_cnf_struct.p_usartDebug->sendString((uint8_t*)"invalid");

	ASW_cnf_struct.p_usartDebug->sendString((uint8_t*)" degC\nHumidite : ");

	if(validity)
	{
		ASW_cnf_struct.p_usartDebug->sendInteger(ASW_cnf_struct.p_TempSensor->GetHumInteger(),10);
		ASW_cnf_struct.p_usartDebug->sendString((uint8_t*)".");
		ASW_cnf_struct.p_usartDebug->sendInteger(ASW_cnf_struct.p_TempSensor->GetHumDecimal(),10);
	}
	else
		ASW_cnf_struct.p_usartDebug->sendString((uint8_t*)"invalid");

	ASW_cnf_struct.p_usartDebug->sendString((uint8_t*)" %");

}

void UsartDebug::DisplayCPULoad_task()
{
	ASW_cnf_struct.p_usartDebug->sendString((uint8_t*)"\n\nCharge CPU :\n");
	ASW_cnf_struct.p_usartDebug->sendString((uint8_t*)"    Actuelle : ");
	ASW_cnf_struct.p_usartDebug->sendInteger(BSW_cnf_struct.p_cpuload->getCurrrentCPULoad(),10);
	ASW_cnf_struct.p_usartDebug->sendString((uint8_t*)"\n    Moyenne : ");
	ASW_cnf_struct.p_usartDebug->sendInteger(BSW_cnf_struct.p_cpuload->getAverageCPULoad(),10);
	ASW_cnf_struct.p_usartDebug->sendString((uint8_t*)"\n    Max : ");
	ASW_cnf_struct.p_usartDebug->sendInteger(BSW_cnf_struct.p_cpuload->getMaxCPULoad(),10);
}

bool UsartDebug::isDebugModeActive()
{
	return debugModeActive_F;
}

void UsartDebug::activateDebugMode()
{
	debugModeActive_F = true;

	ASW_cnf_struct.p_usartDebug->sendString((uint8_t*)"Debug actif!\n");
	ASW_cnf_struct.p_usartDebug->DebugModeManagement((uint8_t)0);
}

void UsartDebug::DebugModeManagement(uint8_t rcv_char)
{
	/* switch on debug state */
	switch(debug_state)
	{
	case INIT:
		ASW_cnf_struct.p_usartDebug->sendString((uint8_t*)str_debug_main_menu);
		debug_state = WAIT_INIT;
		break;

	case WAIT_INIT:
		switch (rcv_char)
		{
		case 's':
			ASW_cnf_struct.p_usartDebug->sendString((uint8_t*)"\nBye !\n");
			debugModeActive_F = false;
			debug_state = INIT;
			break;

		case '1':
			debug_state = DISPLAY_DATA;
			ASW_cnf_struct.p_usartDebug->sendString((uint8_t*)"\nOk, appuyer sur s pour arreter !\n");
			p_scheduler->addPeriodicTask((TaskPtr_t)(&UsartDebug::DisplaySensors_task), PERIOD_MS_TASK_DISPLAY_SENSORS);
			break;

		case '2':
			debug_state = DISPLAY_CPU_LOAD;
			ASW_cnf_struct.p_usartDebug->sendString((uint8_t*)"\nOk, appuyer sur s pour arreter !\n");
			p_scheduler->addPeriodicTask((TaskPtr_t)(&UsartDebug::DisplayCPULoad_task), PERIOD_MS_TASK_DISPLAY_CPU_LOAD);
			break;

		default:
			ASW_cnf_struct.p_usartDebug->sendString((uint8_t*)"\nImpossible de faire ca... !\n");
			break;
		}
		break;

	case DISPLAY_DATA:
		if (rcv_char == 's')
		{
			if (p_scheduler->removePeriodicTask(&UsartDebug::DisplaySensors_task) == false)
				ASW_cnf_struct.p_usartDebug->sendString((uint8_t*)"Impossible de supprimer la tache...");

			debug_state = WAIT_INIT;
			ASW_cnf_struct.p_usartDebug->sendString((uint8_t*)str_debug_main_menu);
		}
		break;

	case DISPLAY_CPU_LOAD:
		if (rcv_char == 's')
		{
			if (p_scheduler->removePeriodicTask(&UsartDebug::DisplayCPULoad_task) == false)
				ASW_cnf_struct.p_usartDebug->sendString((uint8_t*)"Impossible de supprimer la tache...");

			debug_state = WAIT_INIT;
			ASW_cnf_struct.p_usartDebug->sendString((uint8_t*)str_debug_main_menu);
		}
		break;

	}

}
