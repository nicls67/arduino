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
#include "../../scheduler/scheduler.h"
#include "../../bsw/usart/usart.h"
#include "../../bsw/dio/dio.h"
#include "../../bsw/timer/timer.h"
#include "../../bsw/dht22/dht22.h"
#include "../../bsw/bsw.h"

#include "../log/log.h"
#include "../keepAliveLed/keepAliveLed.h"
#include "../asw.h"

TempSensor::TempSensor()
{
	/* Add task to scheduler */
	p_scheduler->addPeriodicTask((TaskPtr_t)(&TempSensor::readTempSensor_task), PERIOD_MS_TASK_TEMP_SENSOR);
}

void TempSensor::readTempSensor_task()
{
	bool validity;

	validity = BSW_cnf_struct.p_dht22->read();

	ASW_cnf_struct.p_usartDebug->sendData((char*)"\nValidity ");
	ASW_cnf_struct.p_usartDebug->sendBool(validity);
}
