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

#include "../../lib/LinkedList/LinkedList.h"
#include "../../lib/string/String.h"

#include "../../scheduler/scheduler.h"

#include "../../bsw/I2C/I2C.h"
#include "../../bsw/lcd/LCD.h"

#include "../sensors_mgt/SensorManagement.h"
#include "../display_ift/DisplayInterface.h"
#include "DisplayManagement.h"

DisplayManagement* p_global_ASW_DisplayManagement;

const uint8_t welcomeMessageString[] = "Bienvenue !"; /*!< String displayed on the screen at startup */
const uint8_t noSensorsDisplayString[] = "Capteurs desactives"; /*!< String used in case sensors are deactivated */

DisplayManagement::DisplayManagement()
{
	/* Create display interface object */
	if (p_global_ASW_DisplayInterface == 0)
		p_global_ASW_DisplayInterface = new DisplayInterface(&LCD_init_cnf);

	p_display_ift = p_global_ASW_DisplayInterface;

	/* Initialize sensor management pointer */
	if(p_global_ASW_SensorManagement != 0)
		p_SensorMgt = p_global_ASW_SensorManagement;
	else
		p_SensorMgt = 0;

	/* Display welcome message on 2nd line */
	String str;
	str.appendString((uint8_t*)welcomeMessageString);
	p_display_ift->DisplayFullLine(str.getString(), str.getSize(), 1, NORMAL, CENTER);

	p_global_scheduler->addPeriodicTask((TaskPtr_t)&DisplayManagement::RemoveWelcomeMessage_Task, DISPLAY_MGT_PERIOD_WELCOME_MSG_REMOVAL);

	/* Update temperature and humidity sensor task period to match display period */
	/* TODO : check how to manage display period synchro */

}

void DisplayManagement::RemoveWelcomeMessage_Task()
{
	DisplayInterface * ift_ptr = p_global_ASW_DisplayManagement->GetIftPointer();

	/* Remove itself from scheduler */
	p_global_scheduler->removePeriodicTask((TaskPtr_t)&DisplayManagement::RemoveWelcomeMessage_Task);

	/* Add periodic task in scheduler */
	p_global_scheduler->addPeriodicTask((TaskPtr_t)&DisplayManagement::DisplaySensorData_Task, DISPLAY_MGT_PERIOD_TASK_SENSOR);

	/* Clear the screen */
	ift_ptr->ClearFullScreen();

	/* Call sensor data task manually to avoid a blank screen until the task is called by the scheduler */
	DisplayManagement::DisplaySensorData_Task();
}

void DisplayManagement::DisplaySensorData_Task()
{
	/* TODO : possible to use string class into DisplayFullLine? */
	DisplayInterface* displayIft_ptr = p_global_ASW_DisplayManagement->GetIftPointer();
	SensorManagement* sensor_ptr = p_global_ASW_DisplayManagement->GetSensorMgtPtr();

	if(sensor_ptr !=0)
	{
		for(uint8_t i=0; i<sensor_ptr->getSensorCount(); i++)
		{
			String str = sensor_ptr->getFullStringFormattedValue(i);
			displayIft_ptr->DisplayFullLine(str.getString(), str.getSize(), DISPLAY_MGT_FIRST_LINE_SENSORS + i, LINE_SHIFT);
		}
	}
	else
		displayIft_ptr->DisplayFullLine((uint8_t*)noSensorsDisplayString, sizeof(noSensorsDisplayString)/sizeof(uint8_t) - 1, DISPLAY_MGT_FIRST_LINE_SENSORS, GO_TO_NEXT_LINE);


}
