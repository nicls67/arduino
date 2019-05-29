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

/*!
 * @brief Main menu of debug mode
 */
const uint8_t str_debug_main_menu[] =
		"Menu principal :  \n"
		"    s : Quitter debug\n";

/*!
 * @brief Info menu empty string
 */
const uint8_t str_debug_info_message_empty[] = "\n";

/*!
 * @brief Info menu string in case a wrong selection has been performed
 */
const uint8_t str_debug_info_message_wrong_selection[] = "Impossible de faire ca... !\n";


DebugManagement::DebugManagement()
{
	/* Create a new interface object if needed and attach it to the class */
	if(debug_ift_ptr == 0)
		debug_ift_ptr = new DebugInterface();

	ASW_cnf_struct.p_DebugInterface = debug_ift_ptr;

	/* initialize menu */
	menu_state = MAIN_MENU;
	menu_string_ptr = (uint8_t*)str_debug_main_menu;
	info_string_ptr = (uint8_t*)str_debug_info_message_empty;

	/* Display data now to avoid blank screen until the task is called by scheduler */
	DisplayData();

	/* Start display of data periodically */
	p_scheduler->addPeriodicTask((TaskPtr_t)(&DebugManagement::DisplayPeriodicData_task), PERIOD_MS_TASK_DISPLAY_DEBUG_DATA);
}

void DebugManagement::DisplayData()
{
	bool validity;

	/* Clear the screen */
	debug_ift_ptr->sendChar('\f');

	/* First write menu */
	debug_ift_ptr->sendString(menu_string_ptr);

	/* Skip 1 line */
	debug_ift_ptr->nextLine();

	/* Write info message */
	debug_ift_ptr->sendString(info_string_ptr);

	/* Skip 1 line */
	debug_ift_ptr->nextLine();

	/* Write sensor data */
	validity = ASW_cnf_struct.p_TempSensor->GetValidity();

	debug_ift_ptr->sendString((uint8_t*)"Donnees capteurs :\n");
	debug_ift_ptr->sendString((uint8_t*)"    Temperature : ");

	if(validity)
	{
		debug_ift_ptr->sendInteger(ASW_cnf_struct.p_TempSensor->GetTempInteger(),10);
		debug_ift_ptr->sendString((uint8_t*)".");
		debug_ift_ptr->sendInteger(ASW_cnf_struct.p_TempSensor->GetTempDecimal(),10);
	}
	else
		debug_ift_ptr->sendString((uint8_t*)"invalid");

	debug_ift_ptr->sendString((uint8_t*)" degC\n    Humidite : ");

	if(validity)
	{
		debug_ift_ptr->sendInteger(ASW_cnf_struct.p_TempSensor->GetHumInteger(),10);
		debug_ift_ptr->sendString((uint8_t*)".");
		debug_ift_ptr->sendInteger(ASW_cnf_struct.p_TempSensor->GetHumDecimal(),10);
	}
	else
		debug_ift_ptr->sendString((uint8_t*)"invalid");

	debug_ift_ptr->sendString((uint8_t*)" %\n");

	/* Skip 1 line */
	debug_ift_ptr->nextLine();

	/* Write CPU load data */
	if(BSW_cnf_struct.p_cpuload !=0)
	{
		debug_ift_ptr->sendString((uint8_t*)"Charge CPU :\n");
		debug_ift_ptr->sendString((uint8_t*)"    Actuelle : ");
		debug_ift_ptr->sendInteger(BSW_cnf_struct.p_cpuload->getCurrrentCPULoad(),10);
		debug_ift_ptr->sendString((uint8_t*)"\n    Moyenne : ");
		debug_ift_ptr->sendInteger(BSW_cnf_struct.p_cpuload->getAverageCPULoad(),10);
		debug_ift_ptr->sendString((uint8_t*)"\n    Max : ");
		debug_ift_ptr->sendInteger(BSW_cnf_struct.p_cpuload->getMaxCPULoad(),10);
	}
	else
	{
		debug_ift_ptr->sendString((uint8_t*)"Charge CPU non disponible\n");
	}

	info_string_ptr = (uint8_t*)str_debug_info_message_empty;
}

void DebugManagement::DisplayPeriodicData_task()
{
	ASW_cnf_struct.p_DebugManagement->DisplayData();
}




bool DebugManagement::DebugModeManagement()
{
	bool quit = false;

	uint8_t rcv_char = debug_ift_ptr->read();

	/* switch on menu state */
	switch(menu_state)
	{
	case MAIN_MENU:
		switch (rcv_char)
		{
		case 's':
			debug_ift_ptr->sendString((uint8_t*)"\fBye !");
			p_scheduler->removePeriodicTask((TaskPtr_t)&DebugManagement::DisplayPeriodicData_task);
			quit = true;
			break;

		default:
			info_string_ptr = (uint8_t*)str_debug_info_message_wrong_selection;
			break;
		}
		break;

	}

	/* Force display update */
	if(!quit)
		DebugManagement::DisplayPeriodicData_task();

	return quit;

}
