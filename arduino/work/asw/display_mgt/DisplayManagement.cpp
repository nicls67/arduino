/*!
 * @file DisplayManagement.cpp
 *
 * @brief Display management source file
 *
 * @date 1 mai 2019
 * @author nicls67
 */

#include <stdlib.h>
#include <avr/io.h>

#include "../asw.h"
#include "../../scheduler/scheduler.h"
#include "DisplayManagement.h"

DisplayManagement::DisplayManagement() {

	/* Create display interface object */
	if (ASW_cnf_struct.p_DisplayInterface == 0)
		ASW_cnf_struct.p_DisplayInterface = new DisplayInterface();

	p_display_ift = ASW_cnf_struct.p_DisplayInterface;

	/* Add periodic task in scheduler */
	p_scheduler->addPeriodicTask((TaskPtr_t)&DisplayManagement::DisplaySensorData_Task, DISPLAY_MGT_PERIOD_TASK_SENSOR);

}

void DisplayManagement::DisplaySensorData_Task()
{
	DisplayInterface* displayIft_ptr;

	/* First get interface object pointer */
	displayIft_ptr = ASW_cnf_struct.p_DisplayManagement->GetIftPointer();

	/* Display temperature */

}
