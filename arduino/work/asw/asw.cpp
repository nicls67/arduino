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

#include "debug_ift/DebugInterface.h"
#include "debug_mgt/DebugManagement.h"
#include "TempSensor/TempSensor.h"
#include "display_ift/DisplayInterface.h"
#include "display_mgt/DisplayManagement.h"
#include "keepAliveLed/keepAliveLed.h"

#include "../main.h"

#include "asw.h"




T_ASW_cnf_struct ASW_cnf_struct;



void asw_init()
{
	/* TODO : create a configuration to manage which service should be launched or not */

	if(isDebugModeActivated)
	{
		if(ASW_cnf_struct.p_DebugInterface == 0)
			ASW_cnf_struct.p_DebugInterface = new DebugInterface();

		ASW_cnf_struct.p_DebugInterface->sendString((uint8_t*)"\nMode debug actif !\n");
	}
	else
		ASW_cnf_struct.p_DebugInterface = 0;


	ASW_cnf_struct.p_DebugManagement = 0;

	ASW_cnf_struct.p_keepAliveLed = new keepAliveLed();

	if(ASW_cnf_struct.p_TempSensor == 0)
		ASW_cnf_struct.p_TempSensor = new TempSensor();

	ASW_cnf_struct.p_DisplayInterface = 0;

	if(ASW_cnf_struct.p_DisplayManagement == 0)
		ASW_cnf_struct.p_DisplayManagement = new DisplayManagement();

}
