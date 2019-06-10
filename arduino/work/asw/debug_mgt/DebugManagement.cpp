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
#include <avr/wdt.h>

#include "../../lib/string/String.h"
#include "../../lib/LinkedList/LinkedList.h"

#include "../../scheduler/scheduler.h"

#include "../../bsw/usart/usart.h"
#include "../../bsw/cpuLoad/CpuLoad.h"
#include "../../bsw/wdt/Watchdog.h"

#include "../TempSensor/TempSensor.h"
#include "../debug_ift/DebugInterface.h"
#include "DebugManagement.h"

#include "../asw.h"
#include "../../main.h"

DebugManagement* p_global_ASW_DebugManagement;

/*!
 * @brief Main menu of debug mode
 */
const uint8_t str_debug_main_menu[] =
		"Menu principal :  \n"
		"    1 : Watchdog\n"
		"\n"
		"    r : Reset du systeme\n"
		"    q : Quitter debug\n";

/*!
 * @brief Watchdog menu of debug mode
 */
const uint8_t str_debug_wdg_menu[] =
		"Menu watchdog : \n"
		"    1 : Changer timeout\n"
		"    2 : Afficher valeur actuelle du timeout\n"
		"\n"
		"    q : Retour\n";

/*!
 * @brief Watchdog timeout update selection
 */
const uint8_t str_debug_wdg_timeout_update_selection[] =
		"Selection du timeout watchdog : \n"
		"    0 : 15 ms\n"
		"    1 : 30 ms\n"
		"    2 : 60 ms\n"
		"    3 : 120 ms\n"
		"    4 : 250 ms\n"
		"    5 : 500 ms\n"
		"    6 : 1 s\n"
		"    7 : 2 s\n"
		"    8 : 4 s\n"
		"    9 : 8 s\n"
		"\n"
		"    a : Annuler\n";

/*!
 * @brief Info menu string in case a wrong selection has been performed
 */
const uint8_t str_debug_info_message_wrong_menu_selection[] = "Impossible de faire ca... !";

/*!
 * @brief Info menu string in case the watchdog timeout value has been updated
 */
const uint8_t str_debug_info_message_wdg_tmo_updated[] = "Valeur modifiee !";

/*!
 * @brief Info menu string displaying the current value of the watchdog timeout
 */
const uint8_t str_debug_info_message_wdg_tmo_value[] = "Valeur du timeout watchdog (ms) : ";

DebugManagement::DebugManagement()
{
	/* Create a new interface object if needed and attach it to the class */
	if(p_global_ASW_DebugInterface == 0)
		p_global_ASW_DebugInterface = new DebugInterface();

	debug_ift_ptr = p_global_ASW_DebugInterface;

	/* Create temperature sensor object if needed */
	if(ASW_init_cnf.isTempSensorActivated && (p_global_ASW_TempSensor == 0))
		p_global_ASW_TempSensor = new TempSensor();

	tempSensor_ptr = p_global_ASW_TempSensor;

	/* initialize menu */
	debug_state.main_state = MAIN_MENU;
	debug_state.wdg_state = WDG_MAIN;

	menu_string_ptr = (uint8_t*)str_debug_main_menu;
	info_string_ptr = new String();
	isInfoStringDisplayed = false;

	/* Display data now to avoid blank screen until the task is called by scheduler */
	DisplayData();

	/* Start display of data periodically */
	p_global_scheduler->addPeriodicTask((TaskPtr_t)(&DebugManagement::DisplayPeriodicData_task), PERIOD_MS_TASK_DISPLAY_DEBUG_DATA);
}

void DebugManagement::DisplayData()
{
	bool validity;

	/* Clear the screen */
	debug_ift_ptr->ClearScreen();

	/* First write menu */
	debug_ift_ptr->sendString(menu_string_ptr);

	/* Skip 1 line */
	debug_ift_ptr->nextLine();

	/* Write info message */
	if(info_string_ptr->getSize() != 0)
		debug_ift_ptr->sendString(info_string_ptr);

	debug_ift_ptr->sendChar((uint8_t)'\n');

	/* Skip 1 line */
	debug_ift_ptr->nextLine();

	/* Write sensor data */
	if(tempSensor_ptr != 0)
	{
		validity = tempSensor_ptr->GetValidity();

		debug_ift_ptr->sendString((uint8_t*)"Donnees capteurs :\n");
		debug_ift_ptr->sendString((uint8_t*)"    Temperature : ");

		if(validity)
		{
			debug_ift_ptr->sendInteger(tempSensor_ptr->GetTempInteger(),10);
			debug_ift_ptr->sendString((uint8_t*)".");
			debug_ift_ptr->sendInteger(tempSensor_ptr->GetTempDecimal(),10);
		}
		else
			debug_ift_ptr->sendString((uint8_t*)"invalid");

		debug_ift_ptr->sendString((uint8_t*)" degC\n    Humidite : ");

		if(validity)
		{
			debug_ift_ptr->sendInteger(tempSensor_ptr->GetHumInteger(),10);
			debug_ift_ptr->sendString((uint8_t*)".");
			debug_ift_ptr->sendInteger(tempSensor_ptr->GetHumDecimal(),10);
		}
		else
			debug_ift_ptr->sendString((uint8_t*)"invalid");

		debug_ift_ptr->sendString((uint8_t*)" %\n");
	}
	else
		debug_ift_ptr->sendString((uint8_t*)"Le capteur de temperature est desactive...\n");

	/* Skip 1 line */
	debug_ift_ptr->nextLine();

	/* Write CPU load data */
	if(p_global_BSW_cpuload !=0)
	{
		debug_ift_ptr->sendString((uint8_t*)"Charge CPU :\n");
		debug_ift_ptr->sendString((uint8_t*)"    Actuelle : ");
		debug_ift_ptr->sendInteger(p_global_BSW_cpuload->getCurrrentCPULoad(),10);
		debug_ift_ptr->sendString((uint8_t*)"\n    Moyenne : ");
		debug_ift_ptr->sendInteger(p_global_BSW_cpuload->getAverageCPULoad(),10);
		debug_ift_ptr->sendString((uint8_t*)"\n    Max : ");
		debug_ift_ptr->sendInteger(p_global_BSW_cpuload->getMaxCPULoad(),10);
	}
	else
	{
		debug_ift_ptr->sendString((uint8_t*)"Charge CPU non disponible\n");
	}

	if(isInfoStringDisplayed)
	{
		info_string_ptr->Clear();
		isInfoStringDisplayed = false;
	}
	else
		isInfoStringDisplayed = true;
}

void DebugManagement::DisplayPeriodicData_task()
{
	p_global_ASW_DebugManagement->DisplayData();
}




bool DebugManagement::DebugModeManagement()
{
	bool quit = false;

	uint8_t rcv_char = debug_ift_ptr->read();

	/* switch on menu state */
	switch(debug_state.main_state)
	{
	default:
	case MAIN_MENU:
		quit = MainMenuManagement(rcv_char);
		break;

	case WDG_MENU:
		WatchdogMenuManagement(rcv_char);
		break;
	}

	/* Force display update */
	if(!quit)
		DisplayData();

	return quit;

}

void DebugManagement::exitDebugMenu()
{
	debug_ift_ptr->sendString((uint8_t*)"\fBye !");
	p_global_scheduler->removePeriodicTask((TaskPtr_t)&DebugManagement::DisplayPeriodicData_task);
}

void DebugManagement::systemReset()
{
	debug_ift_ptr->sendString((uint8_t*)"\fReset !");
	p_global_BSW_wdg->SystemReset();
}

void DebugManagement::WatchdogMenuManagement(uint8_t rcv_char)
{
	switch(debug_state.wdg_state)
	{
	default:
	case WDG_MAIN:
		switch (rcv_char)
		{
		case '1':
			menu_string_ptr = (uint8_t*)str_debug_wdg_timeout_update_selection;
			debug_state.wdg_state = WDG_TMO_UPDATE;
			break;
		case '2':
			info_string_ptr->appendString((uint8_t*)str_debug_info_message_wdg_tmo_value);
			info_string_ptr->appendInteger(p_global_BSW_wdg->getTMOValue(), 10);
			break;
		case 'q':
			debug_state.main_state = MAIN_MENU;
			menu_string_ptr = (uint8_t*)str_debug_main_menu;
			break;
		default:
			info_string_ptr->appendString((uint8_t*)str_debug_info_message_wrong_menu_selection);
			break;
		}
		break;

	case WDG_TMO_UPDATE:
		uint8_t new_tmo = 0xFF;
		switch(rcv_char)
		{
		case 'a':
			debug_state.wdg_state = WDG_MAIN;
			menu_string_ptr = (uint8_t*)str_debug_wdg_menu;
			break;
		case '0':
			new_tmo = WDG_TMO_15MS;
			break;
		case '1':
			new_tmo = WDG_TMO_30MS;
			break;
		case '2':
			new_tmo = WDG_TMO_60MS;
			break;
		case '3':
			new_tmo = WDG_TMO_120MS;
			break;
		case '4':
			new_tmo = WDG_TMO_250MS;
			break;
		case '5':
			new_tmo = WDG_TMO_500MS;
			break;
		case '6':
			new_tmo = WDG_TMO_1S;
			break;
		case '7':
			new_tmo = WDG_TMO_2S;
			break;
		case '8':
			new_tmo = WDG_TMO_4S;
			break;
		case '9':
			new_tmo = WDG_TMO_8S;
			break;
		default:
			info_string_ptr->appendString((uint8_t*)str_debug_info_message_wrong_menu_selection);
			break;
		}

		if(new_tmo != 0xFF)
		{
			p_global_BSW_wdg->timeoutUpdate(new_tmo);
			debug_state.wdg_state = WDG_MAIN;
			menu_string_ptr = (uint8_t*)str_debug_wdg_menu;
			info_string_ptr->appendString((uint8_t*)str_debug_info_message_wdg_tmo_updated);
		}
		break;
	}

}

bool DebugManagement::MainMenuManagement(uint8_t rcv_char)
{
	bool quit = false;

	switch (rcv_char)
	{
	case '1' :
		debug_state.main_state = WDG_MENU;
		debug_state.wdg_state = WDG_MAIN;
		menu_string_ptr = (uint8_t*)str_debug_wdg_menu;
		break;
	case 'q':
		exitDebugMenu();
		quit = true;
		break;
	case 'r':
		systemReset();
		break;
	default:
		info_string_ptr->appendString((uint8_t*)str_debug_info_message_wrong_menu_selection);
		break;
	}

	return quit;
}
