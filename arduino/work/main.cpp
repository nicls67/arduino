/*!
 *  @file main.cpp
 *  @brief Background task file
 *
 *  @date 12 mars 2018
 *  @author nicls67
 */



#include "main.h"


/*!
 * @brief Main software interrupt
 * @details This function handles the interrupt raised by Timer #1. It wakes up the software every 500 ms to perform applications.
 * @return Nothing
 */
ISR(TIMER1_COMPA_vect)
{
	p_scheduler->launchPeriodicTasks();
}


/*!
 * @brief USART Rx Complete interrupt
 * @details This function handles the interrupt raised when a frame has been received by USART. If debug mode mode is active, it calls debug mode management function.
 * 			If inactive, it calls debug mode activation function if the received character is 'a'
 * @return Nothing
 */
ISR(USART0_RX_vect)
{
	uint8_t data;

	if(ASW_cnf_struct.p_usartDebug->isDebugModeActive() == true)
	{
		data = BSW_cnf_struct.p_usart->usart_read();
		ASW_cnf_struct.p_usartDebug->DebugModeManagement(data);
	}
	else if(BSW_cnf_struct.p_usart->usart_read() == 'a')
	{
		ASW_cnf_struct.p_usartDebug->activateDebugMode();
	}
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


	/* Go into an infinite loop */
	while(1){}
}

