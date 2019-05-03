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

#include "asw.h"


T_ASW_cnf_struct ASW_cnf_struct;


void asw_init()
{
	/* TODO : class initialization should be done in the user class, not everytime in ASW */
	ASW_cnf_struct.p_usartDebug = new UsartDebug();
	ASW_cnf_struct.p_keepAliveLed = new keepAliveLed();
	ASW_cnf_struct.p_TempSensor = new TempSensor();
	ASW_cnf_struct.p_DisplayInterface = 0;
	ASW_cnf_struct.p_DisplayManagement = new DisplayManagement();
}
