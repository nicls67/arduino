/*!
 * @file TempSensor.cpp
 *
 * @brief Defines function of class TempSensor
 *
 * @date 23 mars 2018
 * @author nicls67
 */

#include <avr/io.h>
#include <stdlib.h>

#include "../../../lib/LinkedList/LinkedList.h"
#include "../../../lib/String/String.h"
#include "../../../scheduler/scheduler.h"

#include "../../../bsw/dio/dio.h"
#include "../../../bsw/dht22/dht22.h"

#include "../../sensors_mgt/SensorManagement.h"
#include "../Sensor.h"
#include "TempSensor.h"

#define DHT22_PORT ENCODE_PORT(PORT_B, 6) /*!< DHT22 is connected to port PB6 */

TempSensor::TempSensor() : Sensor()
{
	/* Create new instance of DHT22 sensor object */
	if(p_global_BSW_dht22 == 0)
		p_global_BSW_dht22 = new dht22(DHT22_PORT);

	/* Add task to scheduler */
	p_global_scheduler->addPeriodicTask((TaskPtr_t)(&TempSensor::readTempSensor_task), task_period);
}

TempSensor::TempSensor(uint16_t val_tmo, uint16_t period) : Sensor(val_tmo, period)
{
	/* Create new instance of DHT22 sensor object */
	if(p_global_BSW_dht22 == 0)
		p_global_BSW_dht22 = new dht22(DHT22_PORT);

	/* Add task to scheduler */
	p_global_scheduler->addPeriodicTask((TaskPtr_t)(&TempSensor::readTempSensor_task), task_period);
}

void TempSensor::readTempSensor_task()
{
	TempSensor* temp_ptr = (TempSensor*)p_global_ASW_SensorManagement->getSensorObjectPtr(TEMPERATURE);

	if(temp_ptr != 0)
		temp_ptr->setLastValidity(p_global_BSW_dht22->getTemperature(temp_ptr->getRawDataPtr()));
	else
		temp_ptr->setLastValidity(false);

	temp_ptr->updateValidData();
}



bool TempSensor::updateTaskPeriod(uint16_t period)
{
	task_period = period;
	return p_global_scheduler->updateTaskPeriod((TaskPtr_t)(&TempSensor::readTempSensor_task), task_period);
}
