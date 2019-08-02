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

#include "../../../bsw/usart/usart.h"
#include "../../../bsw/dio/dio.h"
#include "../../../bsw/dht22/dht22.h"
#include "../../../bsw/I2C/I2C.h"
#include "../../../bsw/bmp180/Bmp180.h"

#include "../../debug_ift/DebugInterface.h"
#include "../../sensors_mgt/SensorManagement.h"
#include "../Sensor.h"
#include "TempSensor.h"

#define DHT22_PORT ENCODE_PORT(PORT_B, 6) /*!< DHT22 is connected to port PB6 */
#define TEMP_SENSOR_VALUE_ACCEPTABLE_DIFFERENCE 10 /*!< Sensors value can have a difference of 1 degree before becoming invalid */

TempSensor::TempSensor() : Sensor()
{
	/* Create new instance of DHT22 sensor object */
	if(p_global_BSW_dht22 == 0)
		p_global_BSW_dht22 = new dht22(DHT22_PORT);

	/* Create new instance of BMP180 sensor object */
	if(p_global_BSW_bmp180 == 0)
		p_global_BSW_bmp180 = new Bmp180();

	p_global_BSW_bmp180->ActivateTemperatureConversion(task_period);

	/* Add task to scheduler */
	p_global_scheduler->addPeriodicTask((TaskPtr_t)(&TempSensor::readTempSensor_task), task_period);

}

TempSensor::TempSensor(uint16_t val_tmo, uint16_t period) : Sensor(val_tmo, period)
{
	/* Create new instance of DHT22 sensor object */
	if(p_global_BSW_dht22 == 0)
		p_global_BSW_dht22 = new dht22(DHT22_PORT);

	/* Create new instance of BMP180 sensor object */
	if(p_global_BSW_bmp180 == 0)
		p_global_BSW_bmp180 = new Bmp180();

	p_global_BSW_bmp180->ActivateTemperatureConversion(task_period);

	/* Add task to scheduler */
	p_global_scheduler->addPeriodicTask((TaskPtr_t)(&TempSensor::readTempSensor_task), task_period);
}

void TempSensor::readTempSensor_task()
{
	TempSensor* temp_ptr = (TempSensor*)p_global_ASW_SensorManagement->getSensorObjectPtr(TEMPERATURE);

	if(temp_ptr != 0)
	{
		/* Get temperature values from both sensors */
		uint16_t temperature1, temperature2;
		bool val1, val2;

		val1 = p_global_BSW_dht22->getTemperature(&temperature1);
		val2 = p_global_BSW_bmp180->getTemperatureValue(&temperature2);

		/* If both sensors are valid, check if the values are similar */
		if(val1 && val2)
		{
			if(((temperature1 > temperature2) && ((temperature1 - temperature2) < TEMP_SENSOR_VALUE_ACCEPTABLE_DIFFERENCE))
				|| ((temperature1 < temperature2) && ((temperature2 - temperature1) < TEMP_SENSOR_VALUE_ACCEPTABLE_DIFFERENCE)))
			{
				/* Sensors return similar values, set validity to true and update sensor value */
				temp_ptr->setLastValidity(true);
				*(temp_ptr->getRawDataPtr()) = (temperature1 + temperature2)/2;
			}
			else
			{
				/* Else set validity to false */
				temp_ptr->setLastValidity(false);
			}
		}
		/* If only one sensor is valid, use this value */
		else if(val1)
		{
			temp_ptr->setLastValidity(true);
			*(temp_ptr->getRawDataPtr()) = temperature1;
		}
		else if(val2)
		{
			temp_ptr->setLastValidity(true);
			*(temp_ptr->getRawDataPtr()) = temperature2;
		}
		/* If no sensor is valid, validity is set to false */
		else
			temp_ptr->setLastValidity(false);

		/* Update validity data */
		temp_ptr->updateValidData();

		p_global_ASW_DebugInterface->sendInteger(temperature1, 10);
		p_global_ASW_DebugInterface->sendInteger(temperature2, 10);
		p_global_ASW_DebugInterface->nextLine();

	}

}



bool TempSensor::updateTaskPeriod(uint16_t period)
{
	task_period = period;
	return p_global_scheduler->updateTaskPeriod((TaskPtr_t)(&TempSensor::readTempSensor_task), task_period);
}
