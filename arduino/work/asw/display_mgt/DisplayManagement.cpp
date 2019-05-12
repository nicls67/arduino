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

#include "../../bsw/usart/usart.h"
#include "../../bsw/timer/timer.h"
#include "../../bsw/I2C/I2C.h"
#include "../../bsw/lcd/LCD.h"
#include "../../bsw/dio/dio.h"
#include "../../bsw/dht22/dht22.h"
#include "../../bsw/cpuLoad/CpuLoad.h"

#include "../../bsw/bsw.h"

#include "../debug_ift/DebugInterface.h"
#include "../debug_mgt/DebugManagement.h"
#include "../display_ift/DisplayInterface.h"
#include "../keepAliveLed/keepAliveLed.h"
#include "../TempSensor/TempSensor.h"
#include "DisplayManagement.h"

#include "../asw.h"

/* TODO : center welcome message */


DisplayManagement::DisplayManagement()
{
	/* Create display interface object */
	if (ASW_cnf_struct.p_DisplayInterface == 0)
		ASW_cnf_struct.p_DisplayInterface = new DisplayInterface(&LCD_init_cnf);

	p_display_ift = ASW_cnf_struct.p_DisplayInterface;

	p_tempSensor = ASW_cnf_struct.p_TempSensor;

	/* Display welcome message on 2nd line */
	String str;
	str.appendString((uint8_t*)welcomeMessageString);
	p_display_ift->DisplayFullLine(str.getString(), str.getSize(), 1, NORMAL);

	p_scheduler->addPeriodicTask((TaskPtr_t)&DisplayManagement::RemoveWelcomeMessage_Task, DISPLAY_MGT_PERIOD_WELCOME_MSG_REMOVAL);

}

void DisplayManagement::RemoveWelcomeMessage_Task()
{
	DisplayInterface * ift_ptr = ASW_cnf_struct.p_DisplayManagement->GetIftPointer();

	/* Clear the screen */
	ift_ptr->ClearFullScreen();

	/* Remove itself from scheduler */
	p_scheduler->removePeriodicTask((TaskPtr_t)&DisplayManagement::RemoveWelcomeMessage_Task);

	/* Add periodic task in scheduler */
	p_scheduler->addPeriodicTask((TaskPtr_t)&DisplayManagement::DisplaySensorData_Task, DISPLAY_MGT_PERIOD_TASK_SENSOR);
}

void DisplayManagement::DisplaySensorData_Task()
{
	DisplayInterface* displayIft_ptr;
	TempSensor* tempSensor_ptr;

	/* First get object pointer */
	displayIft_ptr = ASW_cnf_struct.p_DisplayManagement->GetIftPointer();
	tempSensor_ptr = ASW_cnf_struct.p_DisplayManagement->GetTempSensorPtr();

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
