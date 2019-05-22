/*!
 * @file int.cpp
 *
 * @brief Interrupt management source file
 *
 * @date 22 mai 2019
 * @author nicls67
 */

#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "../../lib/LinkedList/LinkedList.h"
#include "../../lib/string/String.h"

#include "../../scheduler/scheduler.h"

#include "../usart/usart.h"
#include "../timer/timer.h"
#include "../I2C/I2C.h"
#include "../lcd/LCD.h"
#include "../dio/dio.h"
#include "../dht22/dht22.h"
#include "../cpuLoad/CpuLoad.h"

#include "../bsw.h"

#include "../../asw/debug_ift/DebugInterface.h"
#include "../../asw/debug_mgt/DebugManagement.h"
#include "../../asw/TempSensor/TempSensor.h"
#include "../../asw/display_ift/DisplayInterface.h"
#include "../../asw/display_mgt/DisplayManagement.h"
#include "../../asw/keepAliveLed/keepAliveLed.h"

#include "../../asw/asw.h"

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
	bool quit = false;

	if(isDebugModeActivated)
	{
		/* If the debug mode is started */
		if(ASW_cnf_struct.p_DebugManagement != 0)
		{
			quit = ASW_cnf_struct.p_DebugManagement->DebugModeManagement();
		}
		else if(ASW_cnf_struct.p_DebugInterface->read() == 'a')
		{
			ASW_cnf_struct.p_DebugManagement = new DebugManagement();
		}

		if(quit)
		{
			free(ASW_cnf_struct.p_DebugManagement);
			ASW_cnf_struct.p_DebugManagement = 0;
		}
	}

}
