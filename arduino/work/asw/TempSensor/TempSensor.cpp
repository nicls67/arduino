/*!
 * @file TempSensor.cpp
 *
 * @brief Defines function of class TempSensor
 *
 * @date 23 mars 2018
 * @author nicls67
 */

#include <avr/io.h>

#include "TempSensor.h"



TempSensor::TempSensor()
{
	/* Add task to scheduler */
	p_scheduler->addPeriodicTask((TaskPtr_t)(&TempSensor::readTempSensor_task), PERIOD_MS_TASK_TEMP_SENSOR);
}

void TempSensor::readTempSensor_task()
{
	bool validity;

	validity = BSW_cnf_struct.p_dht22->read();
}
