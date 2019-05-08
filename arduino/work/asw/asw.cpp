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


#include "../bsw/LCD/LCD.h"

#include "debug_ift/DebugInterface.h"
#include "TempSensor/TempSensor.h"
#include "display_ift/DisplayInterface.h"
#include "display_mgt/DisplayManagement.h"
#include "keepAliveLed/keepAliveLed.h"
#include "asw.h"




T_ASW_cnf_struct ASW_cnf_struct;


void asw_init()
{
	/* TODO : class initialization should be done in the user class, not every time in ASW */
	ASW_cnf_struct.p_DebugInterface = new DebugInterface();
	ASW_cnf_struct.p_keepAliveLed = new keepAliveLed();
	ASW_cnf_struct.p_TempSensor = new TempSensor();
	ASW_cnf_struct.p_DisplayInterface = 0;
	ASW_cnf_struct.p_DisplayManagement = new DisplayManagement();
}
