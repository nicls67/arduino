/*!
 * @file Sensor.cpp
 *
 * @brief Sensor class source code file
 *
 * @date 20 juin 2019
 * @author nicls67
 */

#include <stdlib.h>
#include <avr/io.h>

#include "../../lib/LinkedList/LinkedList.h"
#include "../../scheduler/scheduler.h"

#include "Sensor.h"

#define TASK_PERIOD_DEFAULT 1000 /*!< Default sensor task period : 1s */
#define VALIDITY_TIMEOUT_MS_DEFAULT 30000 /*!< Sensor data are declared invalid after 30s */


Sensor::Sensor()
{
	/* Initialize class members */
	raw_data = 0;
	valid_value = 0;

	validity = false;
	validity_last_read = false;

	valid_pit = 0;
	validity_tmo = VALIDITY_TIMEOUT_MS_DEFAULT/SW_PERIOD_MS;

	task_period = TASK_PERIOD_DEFAULT;

}

Sensor::Sensor(uint16_t val_tmo, uint16_t period)
{
	/* Initialize class members */
	raw_data = 0;
	valid_value = 0;

	validity = false;
	validity_last_read = false;

	valid_pit = 0;
	validity_tmo = val_tmo/SW_PERIOD_MS;

	task_period = period;
}

void Sensor::updateValidData()
{
	if (validity_last_read == true)
	{
		valid_value = raw_data;
		validity = true;
		valid_pit = p_global_scheduler->getPitNumber();
	}
	else if ((p_global_scheduler->getPitNumber() - valid_pit) > validity_tmo)
	{
		validity = false;
	}
}
