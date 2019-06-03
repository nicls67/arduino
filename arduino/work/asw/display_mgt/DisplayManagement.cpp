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
#include "../../lib/string/string.h"

#include "../../scheduler/scheduler.h"

#include "../../bsw/I2C/I2C.h"
#include "../../bsw/lcd/LCD.h"

#include "../TempSensor/TempSensor.h"
#include "../display_ift/DisplayInterface.h"
#include "DisplayManagement.h"

#include "../asw.h"
#include "../../main.h"

DisplayManagement* p_global_ASW_DisplayManagement;

DisplayManagement::DisplayManagement()
{
	/* Create display interface object */
	if (p_global_ASW_DisplayInterface == 0)
		p_global_ASW_DisplayInterface = new DisplayInterface(&LCD_init_cnf);

	p_display_ift = p_global_ASW_DisplayInterface;

	/* Check if temperature sensor object is already created and create one of needed */
	if(ASW_init_cnf.isTempSensorActivated && (p_global_ASW_TempSensor == 0))
		p_global_ASW_TempSensor = new TempSensor();

	p_tempSensor = p_global_ASW_TempSensor;

	/* Display welcome message on 2nd line */
	String str;
	str.appendString((uint8_t*)welcomeMessageString);
	p_display_ift->DisplayFullLine(str.getString(), str.getSize(), 1, NORMAL, CENTER);

	p_global_scheduler->addPeriodicTask((TaskPtr_t)&DisplayManagement::RemoveWelcomeMessage_Task, DISPLAY_MGT_PERIOD_WELCOME_MSG_REMOVAL);

	/* Update temperature sensor task period to match display period */
	if(p_tempSensor->getTaskPeriod() > DISPLAY_MGT_PERIOD_TASK_SENSOR)
		p_tempSensor->updateTaskPeriod(DISPLAY_MGT_PERIOD_TASK_SENSOR);

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
	DisplayInterface* displayIft_ptr;
	TempSensor* tempSensor_ptr;

	/* First get object pointer */
	displayIft_ptr = p_global_ASW_DisplayManagement->GetIftPointer();
	tempSensor_ptr = p_global_ASW_DisplayManagement->GetTempSensorPtr();

	if(tempSensor_ptr !=0)
	{
		/* Display temperature */
		String str(tempDisplayString);

		/* If sensor data are valid */
		if(tempSensor_ptr->GetValidity())
		{
			str.appendInteger((uint16_t)tempSensor_ptr->GetTempInteger(),10);
			str.appendString((uint8_t*)".");
			str.appendInteger((uint16_t)tempSensor_ptr->GetTempDecimal(),10);
			str.appendString((uint8_t*)" ");
			str.appendChar(161);
		}
		else
			str.appendString((uint8_t*)"invalide");

		displayIft_ptr->DisplayFullLine(str.getString(), str.getSize(), DISPLAY_MGT_LINE_TEMP, LINE_SHIFT);

		/* Display humidity */
		str.Clear();
		str.appendString((uint8_t*)humidityDisplayString);

		/* If sensor data are valid */
		if(tempSensor_ptr->GetValidity())
		{
			str.appendInteger((uint16_t)tempSensor_ptr->GetHumInteger(),10);
			str.appendString((uint8_t*)".");
			str.appendInteger((uint16_t)tempSensor_ptr->GetHumDecimal(),10);
			str.appendString((uint8_t*)" %");
		}
		else
			str.appendString((uint8_t*)"invalide");

		displayIft_ptr->DisplayFullLine(str.getString(), str.getSize(), DISPLAY_MGT_LINE_HUM, LINE_SHIFT);
	}
	else
		displayIft_ptr->DisplayFullLine((uint8_t*)noSensorDisplayString, sizeof(noSensorDisplayString)/sizeof(uint8_t), DISPLAY_MGT_LINE_TEMP, LINE_SHIFT);




}
