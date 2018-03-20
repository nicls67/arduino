/*!
 *  @file main.cpp
 *  @brief Background task file
 *
 *  @date 12 mars 2018
 *  @author nicls67
 */



#include "main.h"

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>

#include "lib/operators.h"

#include "bsw/usart/usart.h"
#include "bsw/dio/dio.h"
#include "bsw/timer/timer.h"
#include "bsw/bsw.h"
#include "asw/log/log.h"
#include "asw/keepAliveLed/keepAliveLed.h"
#include "asw/asw.h"
#include "scheduler/scheduler.h"


/*!
 * @brief Main software interrupt
 * @details This function handles the interrupt raised by Timer #1. It wakes up the software every 500 ms to perform applications.
 * @return Nothing
 */
ISR(TIMER1_COMPA_vect)
{
#ifdef DEBUG_FLAG
	ASW_cnf_struct.p_usartDebug->sendData((char*)"IT started\n");
#endif
	p_scheduler->launchPeriodicTasks();
}

/*!
 * @brief Background task of program
 * @details This function initializes all the software and then goes into an infinite loop.
 *          Periodic interrupt will wake up the software to perform application
 */
int main( void )
{

	/* Initialize scheduler */
	p_scheduler = new scheduler();

	/* Initialize all classes */
	bsw_init();
	asw_init();

	/* Enable interrupts */
	sei();

	/* Configure and start main timer for periodic interrupt at 500ms */
	p_scheduler->startScheduling();

#ifdef DEBUG_FLAG
	ASW_cnf_struct.p_usartDebug->sendData((char*)"Initialization done\n");
#endif

	/* Go into an infinite loop */
	while(1){}
}

