/*!
 * @file keepAliveLed.cpp
 *
 * @brief Definition of function for class keepAliveLed
 *
 * @date 17 mars 2018
 * @author nicls67
 */

#include <avr/io.h>

#include "keepAliveLed.h"
#include "../../scheduler/scheduler.h"
#include "../../bsw/usart/usart.h"
#include "../../bsw/dio/dio.h"
#include "../../bsw/timer/timer.h"
#include "../../bsw/dht22/dht22.h"
#include "../../bsw/bsw.h"
#include "../log/log.h"
#include "../keepAliveLed/keepAliveLed.h"
#include "../TempSensor/TempSensor.h"
#include "../../asw/asw.h"
#include "../../main.h"

keepAliveLed::keepAliveLed()
{
	/* Add task blinLed_task to scheduler with period of 500ms */
	p_scheduler->addPeriodicTask((TaskPtr_t)(&keepAliveLed::blinkLed_task), PERIOD_MS_TASK_LED);
}

void keepAliveLed::blinkLed_task()
{
	/* Call DIO driver to invert LED pin state */
	BSW_cnf_struct.p_dio->dio_invertPortB(PB7);
}
