/*!
 * @file TempSensor.h
 *
 * @brief Class TempSensor header file
 *
 * @date 23 mars 2018
 * @author nicls67
 */

#ifndef WORK_ASW_TEMPSENSOR_TEMPSENSOR_H_
#define WORK_ASW_TEMPSENSOR_TEMPSENSOR_H_

#define PERIOD_MS_TASK_TEMP_SENSOR 5000 /*!< Period for reading temperature data */

class TempSensor
{
public:

	/*!
	 * @brief Class constructor
	 * @details This function initializes all data of the class TempSensor
	 * @return Nothing
	 */
	TempSensor();

	/*!
	 * @brief Task for reading temperature and humidity values
	 * @details This task reads temperature and humidity data using DHT22 driver. It is called every 5 seconds.
	 * @return Nothing
	 */
	static void readTempSensor_task();
};

#include "../../scheduler/scheduler.h"
#include "../../bsw/bsw.h"
#include "../asw.h"

#endif /* WORK_ASW_TEMPSENSOR_TEMPSENSOR_H_ */
