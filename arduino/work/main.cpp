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
#include "lib/string/String.h"

#include "bsw/usart/usart.h"
#include "bsw/timer/timer.h"
#include "bsw/I2C/I2C.h"
#include "bsw/lcd/LCD.h"
#include "bsw/dio/dio.h"
#include "bsw/dht22/dht22.h"
#include "bsw/cpuLoad/CpuLoad.h"

#include "bsw/bsw.h"

#include "scheduler/scheduler.h"

#include "asw/TempSensor/TempSensor.h"
#include "asw/debug_ift/DebugInterface.h"
#include "asw/debug_mgt/DebugManagement.h"
#include "asw/display_ift/DisplayInterface.h"
#include "asw/display_mgt/DisplayManagement.h"
#include "asw/keepAliveLed/keepAliveLed.h"
#include "asw/asw.h"

#include "main.h"

#define DEBUG_ACTIVE_PORT ENCODE_PORT(PORT_B, 4) /*!< Debug activation pin is port PB6 */

bool isDebugModeActivated;

const T_ASW_init_cnf ASW_init_cnf =
{
	true, 	/* Debug services*/
	true, 	/* LED */
	true, 	/* Temperature sensor */
	true  	/* Display */
};

/* TODO : Remove ASW and BSW cnf structures to avoid using too many includes */
/* TODO : add CPU load activation in ASW init structure */

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
	if((BSW_cnf_struct.p_dio->dio_getPort(DEBUG_ACTIVE_PORT) == true) && ASW_init_cnf.isDebugActivated)
		isDebugModeActivated = true;
	else
		isDebugModeActivated = false;

	/* Initialize scheduler */
	p_scheduler = new scheduler();

	/* Initialize ASW */
	asw_init();

	/* Enable interrupts */
	sei();

	/* Configure and start main timer for periodic interrupt at 500ms */
	p_scheduler->startScheduling();


	/* Go into an infinite loop */
	while(1){}
}

