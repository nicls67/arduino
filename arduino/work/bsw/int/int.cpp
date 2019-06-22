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

#include "../../lib/string/String.h"
#include "../../lib/LinkedList/LinkedList.h"
#include "../../scheduler/scheduler.h"

#include "../usart/usart.h"

#include "../../asw/sensors/Sensor.h"
#include "../../asw/sensors/TempSensor/TempSensor.h"
#include "../../asw/sensors/HumSensor/HumSensor.h"
#include "../../asw/debug_ift/DebugInterface.h"
#include "../../asw/debug_mgt/DebugManagement.h"

#include "../../asw/asw.h"
#include "../../main.h"


/*!
 * @brief Main software interrupt
 * @details This function handles the interrupt raised by Timer #1. It wakes up the software every 500 ms to perform applications.
 * @return Nothing
 */
ISR(TIMER1_COMPA_vect)
{
	p_global_scheduler->launchPeriodicTasks();
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
		if(p_global_ASW_DebugManagement != 0)
		{
			quit = p_global_ASW_DebugManagement->DebugModeManagement();
		}
		else if(p_global_ASW_DebugInterface->read() == 'a')
		{
			p_global_ASW_DebugManagement = new DebugManagement();
		}

		if(quit)
		{
			free(p_global_ASW_DebugManagement);
			p_global_ASW_DebugManagement = 0;
		}
	}

}
