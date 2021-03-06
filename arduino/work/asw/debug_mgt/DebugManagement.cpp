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

#include "../sensors/Sensor.h"
#include "../sensors/TempSensor/TempSensor.h"
#include "../sensors/HumSensor/HumSensor.h"
#include "../sensors_mgt/SensorManagement.h"
#include "../debug_ift/DebugInterface.h"
#include "DebugManagement.h"

#include "../asw.h"
#include "../../main.h"

DebugManagement* p_global_ASW_DebugManagement;

/* TODO : display current timeout value in timeout update menu  -> impact on string class */

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
		"    3 : Activer/desactiver watchdog\n"
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

/*!
 * @brief Info menu string displayed when the watchdog has been disabled
 */
const uint8_t str_debug_info_message_wdg_disabled[] = "Watchdog inactif !";

/*!
 * @brief Info menu string displayed when the watchdog has been enabled
 */
const uint8_t str_debug_info_message_wdg_enabled[] = "Watchdog actif !";



DebugManagement::DebugManagement()
{
	/* Create a new interface object if needed and attach it to the class */
	if(p_global_ASW_DebugInterface == 0)
		p_global_ASW_DebugInterface = new DebugInterface();

	debug_ift_ptr = p_global_ASW_DebugInterface;

	/* Initialize sensor management pointer */
	if(p_global_ASW_SensorManagement != 0)
		sensorMgt_ptr = p_global_ASW_SensorManagement;
	else
		sensorMgt_ptr = 0;

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
	if(sensorMgt_ptr != 0)
	{
		for(uint8_t i=0; i<sensorMgt_ptr->getSensorCount(); i++)
		{
			String str;
			sensorMgt_ptr->getFullStringFormattedValue(i, &str);
			debug_ift_ptr->sendString(&str);
			debug_ift_ptr->nextLine();
		}
	}
	else
		debug_ift_ptr->sendString((uint8_t*)"Les capteurs sont desactives...\n");


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
	bool status;

	switch(debug_state.wdg_state)
	{
	default:
	/* Main page of the watchdog menu */
	case WDG_MAIN:
		switch (rcv_char)
		{
		/* User choice : update timeout
		 * Go to the next menu and display current timeout value
		 */
		case '1':
			menu_string_ptr = (uint8_t*)str_debug_wdg_timeout_update_selection;
			debug_state.wdg_state = WDG_TMO_UPDATE;
			break;
		/* User choice : display TMO value
		 * Get the timeout value from watchdog class and displays it in info string
		 */
		case '2':
			info_string_ptr->appendString((uint8_t*)str_debug_info_message_wdg_tmo_value);
			info_string_ptr->appendInteger(p_global_BSW_wdg->getTMOValue(), 10);
			break;
		/* User choice : enable/disable watchdog
		 * Switch the state of the watchdog
		 * Display the new status in the info string.
		 */
		case '3':
			status = p_global_BSW_wdg->SwitchWdg();
			if(status)
				info_string_ptr->appendString((uint8_t*)str_debug_info_message_wdg_enabled);
			else
				info_string_ptr->appendString((uint8_t*)str_debug_info_message_wdg_disabled);
			break;
		/* User choice : go back to main menu */
		case 'q':
			debug_state.main_state = MAIN_MENU;
			menu_string_ptr = (uint8_t*)str_debug_main_menu;
			break;
		default:
			info_string_ptr->appendString((uint8_t*)str_debug_info_message_wrong_menu_selection);
			break;
		}
		break;

	/* Watchdog timeout update:
	 * Select the new timeout value, update the watchdog with this value and display a message in info string
	 */
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

	/* Main menu page */
	switch (rcv_char)
	{
	/* User choice : go to watchdog menu
	 * Display the menu and the watchdog status in the info string
	 */
	case '1' :
		debug_state.main_state = WDG_MENU;
		debug_state.wdg_state = WDG_MAIN;
		menu_string_ptr = (uint8_t*)str_debug_wdg_menu;
		if(p_global_BSW_wdg->isEnabled())
			info_string_ptr->appendString((uint8_t*)str_debug_info_message_wdg_enabled);
		else
			info_string_ptr->appendString((uint8_t*)str_debug_info_message_wdg_disabled);
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
