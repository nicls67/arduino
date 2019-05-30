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
#include "../../lib/string/String.h"
#include "../../scheduler/scheduler.h"

#include "../../bsw/usart/usart.h"
#include "../../bsw/timer/timer.h"
#include "../../bsw/I2C/I2C.h"
#include "../../bsw/lcd/LCD.h"
#include "../../bsw/dio/dio.h"
#include "../../bsw/dht22/dht22.h"
#include "../../bsw/cpuLoad/CpuLoad.h"

#include "../../bsw/bsw.h"

#include "../TempSensor/TempSensor.h"
#include "../debug_ift/DebugInterface.h"
#include "../debug_mgt/DebugManagement.h"
#include "../display_ift/DisplayInterface.h"
#include "../display_mgt/DisplayManagement.h"
#include "../keepAliveLed/keepAliveLed.h"

#include "../asw.h"

#define PIT_BEFORE_INVALID 60

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
	if(BSW_cnf_struct.p_dht22 == 0)
		BSW_cnf_struct.p_dht22 = new dht22(DHT22_PORT);

	task_period = PERIOD_MS_TASK_TEMP_SENSOR;

	/* Add task to scheduler */
	p_scheduler->addPeriodicTask((TaskPtr_t)(&TempSensor::readTempSensor_task), task_period);
}

void TempSensor::readTempSensor_task()
{
	ASW_cnf_struct.p_TempSensor->setValidity(BSW_cnf_struct.p_dht22->read(ASW_cnf_struct.p_TempSensor->getHumPtr(), ASW_cnf_struct.p_TempSensor->getTempPtr()));
	ASW_cnf_struct.p_TempSensor->updateLastValidValues();
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
		valid_pit = p_scheduler->getPitNumber();
	}
	else if ((p_scheduler->getPitNumber() - valid_pit) > PIT_BEFORE_INVALID)
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
	return p_scheduler->updateTaskPeriod((TaskPtr_t)(&TempSensor::readTempSensor_task), task_period);

}
