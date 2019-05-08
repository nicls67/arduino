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

#include "../../bsw/LCD/LCD.h"

#include "../debug_ift/DebugInterface.h"
#include "../display_ift/DisplayInterface.h"
#include "../keepAliveLed/keepAliveLed.h"
#include "../TempSensor/TempSensor.h"
#include "DisplayManagement.h"

#include "../asw.h"




DisplayManagement::DisplayManagement() {

	/* Create display interface object */
	if (ASW_cnf_struct.p_DisplayInterface == 0)
		ASW_cnf_struct.p_DisplayInterface = new DisplayInterface(&LCD_init_cnf);

	p_display_ift = ASW_cnf_struct.p_DisplayInterface;
	p_tempSensor = ASW_cnf_struct.p_TempSensor; /* TODO : create the class here, not in ASW main function */

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
