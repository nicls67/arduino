/*!
 * @file asw.cpp
 *
 * @brief ASW main file
 *
 * @date 15 mars 2018
 * @author nicls67
 */

#include <stdlib.h>
#include <avr/io.h>

#include "../lib/LinkedList/LinkedList.h"
#include "../lib/string/String.h"

#include "../bsw/usart/usart.h"
#include "../bsw/I2C/I2C.h"
#include "../bsw/lcd/LCD.h"
#include "../bsw/dio/dio.h"
#include "../bsw/dht22/dht22.h"

#include "../bsw/bsw.h"

#include "sensors_mgt/SensorManagement.h"
#include "debug_ift/DebugInterface.h"
#include "debug_mgt/DebugManagement.h"
#include "display_ift/DisplayInterface.h"
#include "display_mgt/DisplayManagement.h"
#include "keepAliveLed/keepAliveLed.h"
#include "time_mgt/TimeManagement.h"

#include "asw.h"

#include "../main.h"

void asw_init()
{
	/* Creation of debug services */
	if(isDebugModeActivated)
	{
		if(p_global_ASW_DebugInterface == 0)
			p_global_ASW_DebugInterface = new DebugInterface();

		p_global_ASW_DebugInterface->sendString((uint8_t*)"\fMode debug actif !\n");
	}
	else
		p_global_ASW_DebugInterface = 0;


	/* Debug management object is created on user request by USART interrupt */
	p_global_ASW_DebugManagement = 0;


	/* Time management activation */
	if(ASW_init_cnf.isTimeMgtActivated)
	{
		if(p_global_ASW_TimeManagement == 0)
			p_global_ASW_TimeManagement = new TimeManagement();
	}
	else
		p_global_ASW_TimeManagement = 0;

	/* Sensors activation */
	if(ASW_init_cnf.isSensorMgtActivated)
	{
		if(p_global_ASW_SensorManagement == 0)
			p_global_ASW_SensorManagement = new SensorManagement();
	}
	else
		p_global_ASW_SensorManagement = 0;


	/* LED activation */
	if(ASW_init_cnf.isLEDActivated)
	{
		if(p_global_ASW_keepAliveLed == 0)
			p_global_ASW_keepAliveLed = new keepAliveLed();
	}
	else
		p_global_ASW_keepAliveLed = 0;


	/* Display interface is created by Display management class */
	p_global_ASW_DisplayInterface = 0;


	/* Display activation */
	if(ASW_init_cnf.isDisplayActivated)
	{
		if(p_global_ASW_DisplayManagement == 0)
			p_global_ASW_DisplayManagement = new DisplayManagement();
	}
	else
		p_global_ASW_DisplayManagement = 0;

}
