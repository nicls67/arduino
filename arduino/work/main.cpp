/*!
 *  @file main.cpp
 *  @brief Background task file
 *
 *  @date 12 mars 2018
 *  @author nicls67
 */

#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "lib/LinkedList/LinkedList.h"

#include "bsw/dio/dio.h"

#include "bsw/bsw.h"

#include "scheduler/scheduler.h"

#include "asw/asw.h"

#include "main.h"

#define DEBUG_ACTIVE_PORT ENCODE_PORT(PORT_B, 4) /*!< Debug activation pin is port PB6 */

bool isDebugModeActivated;

const T_ASW_init_cnf ASW_init_cnf =
{
	true, 	/* Debug services*/
	true, 	/* LED */
	true, 	/* Temperature sensor */
	true,	/* Humidity sensor */
	true  	/* Display */
};

/* TODO : add the possibility to activate/deactivate ASW functions dynamically in debug menu */

/*!
 * @brief Background task of program
 * @details This function initializes all the software and then goes into an infinite loop.
 *          Periodic interrupt will wake up the software to perform application
 */
int main( void )
{
	/* Initialize BSW */
	bsw_init();

	/* Debug interface is activated only if the port is set to level HIGH and debug services are needed */
	if((p_global_BSW_dio->dio_getPort(DEBUG_ACTIVE_PORT) == true) && ASW_init_cnf.isDebugActivated)
		isDebugModeActivated = true;
	else
		isDebugModeActivated = false;

	/* Initialize scheduler */
	p_global_scheduler = new scheduler();

	/* Initialize ASW */
	asw_init();

	/* Enable interrupts */
	sei();

	/* Configure and start main timer for periodic interrupt at 500ms */
	p_global_scheduler->startScheduling();


	/* Go into an infinite loop */
	while(1){}
}

