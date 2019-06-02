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
#include "../bsw/timer/timer.h"
#include "../bsw/I2C/I2C.h"
#include "../bsw/lcd/LCD.h"
#include "../bsw/dio/dio.h"
#include "../bsw/dht22/dht22.h"
#include "../bsw/cpuLoad/CpuLoad.h"

#include "../bsw/bsw.h"

#include "TempSensor/TempSensor.h"
#include "debug_ift/DebugInterface.h"
#include "debug_mgt/DebugManagement.h"
#include "display_ift/DisplayInterface.h"
#include "display_mgt/DisplayManagement.h"
#include "keepAliveLed/keepAliveLed.h"

#include "asw.h"

#include "../main.h"


T_ASW_cnf_struct ASW_cnf_struct;

/* TODO : check what happens if sensor value is invalid */

void asw_init()
{
	/* Temperature sensor activation */
	if(ASW_init_cnf.isTempSensorActivated)
	{
		if(ASW_cnf_struct.p_TempSensor == 0)
			ASW_cnf_struct.p_TempSensor = new TempSensor();
	}
	else
		ASW_cnf_struct.p_TempSensor = 0;

	/* Creation of debug services */
	if(isDebugModeActivated)
	{
		if(ASW_cnf_struct.p_DebugInterface == 0)
			ASW_cnf_struct.p_DebugInterface = new DebugInterface();

		ASW_cnf_struct.p_DebugInterface->sendString((uint8_t*)"\fMode debug actif !\n");
	}
	else
		ASW_cnf_struct.p_DebugInterface = 0;

	/* Debug management object is created on user request by USART interrupt */
	ASW_cnf_struct.p_DebugManagement = 0;

	/* LED activation */
	if(ASW_init_cnf.isLEDActivated)
		ASW_cnf_struct.p_keepAliveLed = new keepAliveLed();
	else
		ASW_cnf_struct.p_keepAliveLed = 0;

	/* Display interface is created by Display management class */
	ASW_cnf_struct.p_DisplayInterface = 0;

	/* Display activation */
	if(ASW_init_cnf.isDisplayActivated)
	{
		if(ASW_cnf_struct.p_DisplayManagement == 0)
			ASW_cnf_struct.p_DisplayManagement = new DisplayManagement();
	}
	else
		ASW_cnf_struct.p_DisplayManagement = 0;

}
