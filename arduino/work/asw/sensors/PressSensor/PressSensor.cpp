/*!
 * @file PressSensor.cpp
 *
 * @brief Defines function of class PressSensor
 *
 * @date 6 aout 2019
 * @author nicls67
 */

#include <avr/io.h>
#include <stdlib.h>

#include "../../../lib/LinkedList/LinkedList.h"
#include "../../../lib/String/String.h"
#include "../../../scheduler/scheduler.h"

#include "../../../bsw/I2C/I2C.h"
#include "../../../bsw/bmp180/Bmp180.h"

#include "../../sensors_mgt/SensorManagement.h"
#include "../Sensor.h"
#include "PressSensor.h"

/* TODO : pressure value with 2 digits ? */

PressSensor::PressSensor() : Sensor()
{
	/* Create new instance of BMP180 sensor object */
	if(p_global_BSW_bmp180 == 0)
		p_global_BSW_bmp180 = new Bmp180();

	p_global_BSW_bmp180->ActivatePressureConversion(task_period);

	/* Add task to scheduler */
	p_global_scheduler->addPeriodicTask((TaskPtr_t)(&PressSensor::readPressSensor_task), task_period);
}

PressSensor::PressSensor(uint16_t val_tmo, uint16_t period) : Sensor(val_tmo, period)
{
	/* Create new instance of BMP180 sensor object */
	if(p_global_BSW_bmp180 == 0)
		p_global_BSW_bmp180 = new Bmp180();

	p_global_BSW_bmp180->ActivatePressureConversion(task_period);

	/* Add task to scheduler */
	p_global_scheduler->addPeriodicTask((TaskPtr_t)(&PressSensor::readPressSensor_task), task_period);
}

void PressSensor::readPressSensor_task()
{
	PressSensor* temp_ptr = (PressSensor*)p_global_ASW_SensorManagement->getSensorObjectPtr(PRESSURE);

	if(temp_ptr != 0)
		temp_ptr->setLastValidity(p_global_BSW_bmp180->getPressureValue(temp_ptr->getRawDataPtr()));
	else
		temp_ptr->setLastValidity(false);

	temp_ptr->updateValidData();

}

/* TODO : also update BMP180 period */
bool PressSensor::updateTaskPeriod(uint16_t period)
{
	task_period = period;
	return p_global_scheduler->updateTaskPeriod((TaskPtr_t)(&PressSensor::readPressSensor_task), task_period);
}
