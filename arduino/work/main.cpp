/*!
 *  @file main.cpp
 *  @brief Background task file
 *
 *  @date 12 mars 2018
 *  @author nicls67
 */


#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>


#include "bsw/bsw.h"
#include "asw/asw.h"


/*!
 * @brief Main software interrupt
 * @details This function handles the interrupt raised by Timer #1. It wakes up the software every 500 ms to perform applications.
 * @return Nothing
 */
ISR(TIMER1_COMPA_vect)
{
	ASW_cnf_struct.p_usartDebug->sendData((uint8_t*)("hello\n"));

}

/*!
 * @brief Background task of program
 * @details This function initializes all the software and then goes into an infinite loop.
 *          Periodic interrupt will wake up the software to perform application
 */
int main( void )
{

	/* Initialize all classes */
	bsw_init();
	asw_init();



	/* Enable interrupts */
	sei();

	/* Configure and start main timer for periodic interrupt at 500ms */
	BSW_cnf_struct.p_timer->configureTimer1(256,31250);
	BSW_cnf_struct.p_timer->startTimer1();

	/* Go into an infinite loop */
	while(1){}
}

