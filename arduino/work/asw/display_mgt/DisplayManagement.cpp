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

#include "../sensors/Sensor.h"
#include "../sensors/TempSensor/TempSensor.h"
#include "../sensors/HumSensor/HumSensor.h"
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

	/* Check if humidity sensor object is already created and create one of needed */
	if(ASW_init_cnf.isHumSensorActivated && (p_global_ASW_HumSensor == 0))
		p_global_ASW_HumSensor = new HumSensor();

	p_tempSensor = p_global_ASW_TempSensor;
	p_humSensor = p_global_ASW_HumSensor;

	/* Display welcome message on 2nd line */
	String str;
	str.appendString((uint8_t*)welcomeMessageString);
	p_display_ift->DisplayFullLine(str.getString(), str.getSize(), 1, NORMAL, CENTER);

	p_global_scheduler->addPeriodicTask((TaskPtr_t)&DisplayManagement::RemoveWelcomeMessage_Task, DISPLAY_MGT_PERIOD_WELCOME_MSG_REMOVAL);

	/* Update temperature and humidity sensor task period to match display period */
	if(p_tempSensor->getTaskPeriod() > DISPLAY_MGT_PERIOD_TASK_SENSOR)
		p_tempSensor->updateTaskPeriod(DISPLAY_MGT_PERIOD_TASK_SENSOR);

	if(p_humSensor->getTaskPeriod() > DISPLAY_MGT_PERIOD_TASK_SENSOR)
		p_humSensor->updateTaskPeriod(DISPLAY_MGT_PERIOD_TASK_SENSOR);

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
	HumSensor* humSensor_ptr;

	/* First get object pointer */
	displayIft_ptr = p_global_ASW_DisplayManagement->GetIftPointer();
	tempSensor_ptr = p_global_ASW_DisplayManagement->GetTempSensorPtr();
	humSensor_ptr = p_global_ASW_DisplayManagement->GetHumSensorPtr();

	if(tempSensor_ptr !=0)
	{
		/* Display temperature */
		String str(tempDisplayString);

		/* If sensor data are valid */
		if(tempSensor_ptr->getValidity())
		{
			str.appendInteger((uint16_t)tempSensor_ptr->getValueInteger(),10);
			str.appendString((uint8_t*)".");
			str.appendInteger((uint16_t)tempSensor_ptr->getValueDecimal(),10);
			str.appendString((uint8_t*)" ");
			str.appendChar(161);
		}
		else
			str.appendString((uint8_t*)"invalide");

		displayIft_ptr->DisplayFullLine(str.getString(), str.getSize(), DISPLAY_MGT_LINE_TEMP, LINE_SHIFT);

	}
	else
		displayIft_ptr->DisplayFullLine((uint8_t*)noTempSensorDisplayString, sizeof(noTempSensorDisplayString)/sizeof(uint8_t), DISPLAY_MGT_LINE_TEMP, LINE_SHIFT);


	if(humSensor_ptr !=0)
	{
		/* Display humidity */
		String str(humidityDisplayString);

		/* If sensor data are valid */
		if(humSensor_ptr->getValidity())
		{
			str.appendInteger((uint16_t)humSensor_ptr->getValueInteger(),10);
			str.appendString((uint8_t*)".");
			str.appendInteger((uint16_t)humSensor_ptr->getValueDecimal(),10);
			str.appendString((uint8_t*)" %");
		}
		else
			str.appendString((uint8_t*)"invalide");

		displayIft_ptr->DisplayFullLine(str.getString(), str.getSize(), DISPLAY_MGT_LINE_HUM, LINE_SHIFT);
	}
	else
		displayIft_ptr->DisplayFullLine((uint8_t*)noHumSensorDisplayString, sizeof(noHumSensorDisplayString)/sizeof(uint8_t), DISPLAY_MGT_LINE_TEMP, LINE_SHIFT);


}
