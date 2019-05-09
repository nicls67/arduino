/*!
 * @file DebugManagement.cpp
 *
 * @brief Debug management class source file
 *
 * @date 8 mai 2019
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

/* TODO : clear display before sending a string in the menu */

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


DebugManagement::DebugManagement()
{
	/* Create a new interface object if needed and attach it to the class */
	if(debug_ift_ptr == 0)
		debug_ift_ptr = new DebugInterface();

	ASW_cnf_struct.p_DebugInterface = debug_ift_ptr;

	/* initialize debug menu */
	debug_state = INIT;

	/* Send main menu string */
	debug_ift_ptr->sendString((uint8_t*)str_debug_main_menu);
}

void DebugManagement::DisplaySensors_task()
{
	bool validity;
	DebugInterface* ift_ptr = ASW_cnf_struct.p_DebugManagement->getIftPtr();

	validity = ASW_cnf_struct.p_TempSensor->GetValidity();

	ift_ptr->sendString((uint8_t*)"\n\nTemperature : ");

	if(validity)
	{
		ift_ptr->sendInteger(ASW_cnf_struct.p_TempSensor->GetTempInteger(),10);
		ift_ptr->sendString((uint8_t*)".");
		ift_ptr->sendInteger(ASW_cnf_struct.p_TempSensor->GetTempDecimal(),10);
	}
	else
		ift_ptr->sendString((uint8_t*)"invalid");

	ift_ptr->sendString((uint8_t*)" degC\nHumidite : ");

	if(validity)
	{
		ift_ptr->sendInteger(ASW_cnf_struct.p_TempSensor->GetHumInteger(),10);
		ift_ptr->sendString((uint8_t*)".");
		ift_ptr->sendInteger(ASW_cnf_struct.p_TempSensor->GetHumDecimal(),10);
	}
	else
		ift_ptr->sendString((uint8_t*)"invalid");

	ift_ptr->sendString((uint8_t*)" %");

}

void DebugManagement::DisplayCPULoad_task()
{
	DebugInterface* ift_ptr = ASW_cnf_struct.p_DebugManagement->getIftPtr();

	ift_ptr->sendString((uint8_t*)"\n\nCharge CPU :\n");
	ift_ptr->sendString((uint8_t*)"    Actuelle : ");
	ift_ptr->sendInteger(BSW_cnf_struct.p_cpuload->getCurrrentCPULoad(),10);
	ift_ptr->sendString((uint8_t*)"\n    Moyenne : ");
	ift_ptr->sendInteger(BSW_cnf_struct.p_cpuload->getAverageCPULoad(),10);
	ift_ptr->sendString((uint8_t*)"\n    Max : ");
	ift_ptr->sendInteger(BSW_cnf_struct.p_cpuload->getMaxCPULoad(),10);
}


bool DebugManagement::DebugModeManagement()
{
	bool quit = false;

	uint8_t rcv_char = debug_ift_ptr->read();

	/* switch on debug state */
	switch(debug_state)
	{
	case INIT:
		switch (rcv_char)
		{
		case 's':
			debug_ift_ptr->sendString((uint8_t*)"\nBye !\n");
			quit = true;
			break;

		case '1':
			debug_state = DISPLAY_DATA;
			debug_ift_ptr->sendString((uint8_t*)"\nOk, appuyer sur s pour arreter !\n");
			p_scheduler->addPeriodicTask((TaskPtr_t)(&DebugManagement::DisplaySensors_task), PERIOD_MS_TASK_DISPLAY_SENSORS);
			break;

		case '2':
			debug_state = DISPLAY_CPU_LOAD;
			debug_ift_ptr->sendString((uint8_t*)"\nOk, appuyer sur s pour arreter !\n");
			p_scheduler->addPeriodicTask((TaskPtr_t)(&DebugManagement::DisplayCPULoad_task), PERIOD_MS_TASK_DISPLAY_CPU_LOAD);
			break;

		default:
			debug_ift_ptr->sendString((uint8_t*)"\nImpossible de faire ca... !\n");
			break;
		}
		break;

	case DISPLAY_DATA:
		if (rcv_char == 's')
		{
			if (p_scheduler->removePeriodicTask(&DebugManagement::DisplaySensors_task) == false)
				debug_ift_ptr->sendString((uint8_t*)"Impossible de supprimer la tache...");

			debug_state = INIT;
			debug_ift_ptr->sendString((uint8_t*)str_debug_main_menu);
		}
		break;

	case DISPLAY_CPU_LOAD:
		if (rcv_char == 's')
		{
			if (p_scheduler->removePeriodicTask(&DebugManagement::DisplayCPULoad_task) == false)
				debug_ift_ptr->sendString((uint8_t*)"Impossible de supprimer la tache...");

			debug_state = INIT;
			debug_ift_ptr->sendString((uint8_t*)str_debug_main_menu);
		}
		break;

	}

	return quit;

}
