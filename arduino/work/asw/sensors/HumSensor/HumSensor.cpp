/*!
 * @file HumSensor.cpp
 *
 * @brief Defines function of class HumSensor
 *
 * @date 20 juin 2019
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
#include "HumSensor.h"

#define DHT22_PORT ENCODE_PORT(PORT_B, 6) /*!< DHT22 is connected to port PB6 */

HumSensor::HumSensor() : Sensor()
{
	/* Create new instance of DHT22 sensor object */
	if(p_global_BSW_dht22 == 0)
		p_global_BSW_dht22 = new dht22(DHT22_PORT);

	/* Add task to scheduler */
	p_global_scheduler->addPeriodicTask((TaskPtr_t)(&HumSensor::readHumSensor_task), task_period);
}

HumSensor::HumSensor(uint16_t val_tmo, uint16_t period) : Sensor(val_tmo, period)
{
	/* Create new instance of DHT22 sensor object */
	if(p_global_BSW_dht22 == 0)
		p_global_BSW_dht22 = new dht22(DHT22_PORT);

	/* Add task to scheduler */
	p_global_scheduler->addPeriodicTask((TaskPtr_t)(&HumSensor::readHumSensor_task), task_period);
}

void HumSensor::readHumSensor_task()
{
	HumSensor* hum_ptr = (HumSensor*)p_global_ASW_SensorManagement->getSensorObjectPtr(HUMIDITY);

	if(hum_ptr != 0)
		hum_ptr->setLastValidity(p_global_BSW_dht22->getHumidity(hum_ptr->getRawDataPtr()));
	else
		hum_ptr->setLastValidity(false);

	hum_ptr->updateValidData();
}



bool HumSensor::updateTaskPeriod(uint16_t period)
{
	task_period = period;
	return p_global_scheduler->updateTaskPeriod((TaskPtr_t)(&HumSensor::readHumSensor_task), task_period);
}


