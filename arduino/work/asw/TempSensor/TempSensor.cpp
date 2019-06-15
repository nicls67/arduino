/*!
 * @file TempSensor.cpp
 *
 * @brief Defines function of class TempSensor
 *
 * @date 23 mars 2018
 * @author nicls67
 */

#include <avr/io.h>

#include "../../lib/LinkedList/LinkedList.h"
#include "../../scheduler/scheduler.h"

#include "../../bsw/dio/dio.h"
#include "../../bsw/dht22/dht22.h"

#include "TempSensor.h"

#define VALIDITY_TIMEOUT_MS 30000 /*!< Sensor data are declared invalid after 30s */
#define PIT_BEFORE_INVALID (uint32_t)(VALIDITY_TIMEOUT_MS/SW_PERIOD_MS) /*!< Number of cycles after which the sensors data are declared invalid */

TempSensor* p_global_ASW_TempSensor;

TempSensor::TempSensor()
{
	read_temperature = 0;
	read_humidity = 0;
	valid_temp = 0;
	valid_hum = 0;
	validity_last_read = false;
	validity = false;
	valid_pit = 0;

	/* Create new instance of DHT22 sensor object */
	if(p_global_BSW_dht22 == 0)
		p_global_BSW_dht22 = new dht22(DHT22_PORT);

	task_period = PERIOD_MS_TASK_TEMP_SENSOR;

	/* Add task to scheduler */
	p_global_scheduler->addPeriodicTask((TaskPtr_t)(&TempSensor::readTempSensor_task), task_period);
}

void TempSensor::readTempSensor_task()
{
	p_global_ASW_TempSensor->setValidity(p_global_BSW_dht22->read(p_global_ASW_TempSensor->getHumPtr(), p_global_ASW_TempSensor->getTempPtr()));
	p_global_ASW_TempSensor->updateLastValidValues();
}

void TempSensor::setValidity(bool validity)
{
	validity_last_read = validity;
}

uint16_t* TempSensor::getHumPtr()
{
	return &read_humidity;
}

uint16_t* TempSensor::getTempPtr()
{
	return &read_temperature;
}

void TempSensor::updateLastValidValues()
{
	if (validity_last_read == true)
	{
		valid_temp = read_temperature;
		valid_hum = read_humidity;
		validity = true;
		valid_pit = p_global_scheduler->getPitNumber();
	}
	else if ((p_global_scheduler->getPitNumber() - valid_pit) > PIT_BEFORE_INVALID)
	{
		validity = false;
	}
}

bool TempSensor::getHumidity(uint16_t* hum)
{
	*hum = valid_hum;
	return validity;
}

bool TempSensor::getTemp(uint16_t* temp)
{
	*temp = valid_temp;
	return validity;
}

bool TempSensor::updateTaskPeriod(uint16_t period)
{
	task_period = period;
	return p_global_scheduler->updateTaskPeriod((TaskPtr_t)(&TempSensor::readTempSensor_task), task_period);

}
