/*!
 * @file Bmp180.h
 *
 * @brief Bmp180 class header file
 *
 * @date 27 juil. 2019
 * @author nicls67
 */

#ifndef WORK_BSW_BMP180_BMP180_H_
#define WORK_BSW_BMP180_BMP180_H_

#define BMP180_I2C_BITRATE 100000 /*!< Bitrate used for I2C communication */
#define BMP180_I2C_ADDR 0x77 /*!< I2C address of the sensor */

#define BMP180_CHIP_ID_EEP_ADDR 0xD0 /*!< Chip ID EEPROM address */
#define BMP180_CHIP_ID_CALIB_EEP_START_ADDR 0xAA /*!< EEPROM calibration data start address */
#define BMP180_CHIP_ID_EXPECTED 0x55 /*!< Expected chip ID */

#define BMP180_CTRL_MEAS_EEP_ADDR 0xF4 /*!< Address of the the measurement control register in EEPROM */
#define BMP180_CTRL_MEAS_START_TEMP_CONV 0x2E /*!< Value of measurement control register to start a temperature conversion */
#define BMP180_CTRL_MEAS_START_PRESS_CONV_OSS0 0x34 /*!< Value of measurement control register to start a pressure conversion with OSS0 parameter */

#define BMP180_TEMP_MEAS_WAITING_TIME 6 /*!< Waiting time for a temperature conversion */
#define BMP180_PRESS_MEAS_OSS0_WAITING_TIME 15 /*!< Waiting time for a pressure conversion with parameter OSS0 */

#define BMP180_TIMER_PRESCALER_VALUE 64 /*!< Value of prescaler to use for timer */
#define BMP180_TEMP_MEAS_TIMER_CTC_VALUE ((F_CPU/BMP180_TIMER_PRESCALER_VALUE)/(1000/BMP180_TEMP_MEAS_WAITING_TIME)) /*!< Compare value for periodic timer in case of temperature conversion */
#define BMP180_PRESS_MEAS_OSS0_TIMER_CTC_VALUE ((F_CPU/BMP180_TIMER_PRESCALER_VALUE)/(1000/BMP180_PRESS_MEAS_OSS0_WAITING_TIME)) /*!< Compare value for periodic timer in case of pressure conversion with parameter OSS0 */

#define BMP180_OUT_REG_LSB_EEPROM_ADDR 0xF7 /*!< Address of LSB out register */
#define BMP180_OUT_REG_MSB_EEPROM_ADDR 0xF6 /*!< Address of MSB out register */

#define BMP180_MONITORING_DEFAULT_PERIOD 500 /*!< Monitoring period is set by default to 500ms */

/*!
 * @brief Enumeration defining the possible statuses for BMP180 sensor driver
 */
typedef enum
{
	IDLE, /*!< No conversion is in progress and communication is OK */
	TEMP_CONV_IN_PROGRESS, /*!< A temperature conversion is in progress */
	PRESSURE_CONV_IN_PROGRESS, /*!< A pressure conversion is in progress */
	COMM_FAILED, /*!< Communication is failed */
}
T_BMP180_status;

/* TODO : add monitoring of driver status : reset comm in case of failure, also for LCD ! */
/* TODO : add management of OSS for pressure */
/* TODO : check task period update between pressure and temp */

/*!
 * @brief BMP180 sensor class definition
 * @details This class manages BMP180 driver.
 */
class Bmp180 {
public:

	/*!
	 * @brief Bmp180 class constructor
	 * @details This function initializes the class Bmp180. It reads the chip ID and reads calibration data.
	 * 			If the chip ID or calibration data are incorrect, the status is set to communication failed.
	 * 			It also starts the periodic monitoring of the driver.
	 *
	 * @return Nothing
	 */
	Bmp180();

	/*!
	 * @brief Temperature reading function
	 * @details This function is used to read the temperature value from BMP180 sensor.
	 *
	 * @param [in] data Pointer the location where the temperature data shall be stored.
	 *
	 * @return True if a temperature measurement is available, false otherwise.
	 */
	bool getTemperatureValue(uint16_t* data);

	/*!
	 * @brief Pressure reading function
	 * @details This function is used to read the pressure value from BMP180 sensor.
	 *
	 * @param [in] data Pointer the location where the pressure data shall be stored.
	 *
	 * @return True if a pressure measurement is available, false otherwise.
	 */
	bool getPressureValue(uint16_t* data);

	/*!
	 * @brief End of conversion interrupt
	 * @details This function is called by the timer interrupt at the end of the conversion. It will retrieve the
	 * 			raw temperature of pressure value according to the conversion type and then calculate true value.
	 *
	 * @return Nothing
	 */
	void conversionTimerInterrupt();

	/*!
	 * @brief BMP180 periodic monitoring function
	 * @details This function is in charge of monitoring the BMP180 sensor.
	 * 			It starts new conversions of temperature and pressure values. The result of this conversion will be
	 * 			retrieved by an interrupt after 4.5 ms.
	 * 			Temperature and pressure values time stamps are monitored and availability of measures are updated.
	 * 			It also	monitors the status of the driver, if the driver is failed, it tries to restart the sensor device.
	 *
	 * @return Nothing
	 */
	static void Bmp180Monitoring_Task();

	/*!
	 * @brief Driver status get function
	 * @details This function returns the status of the driver.
	 *
	 * @return Driver status
	 */
	inline T_BMP180_status getStatus()
	{
		return status;
	}

	/*!
	 * @brief Task period get function
	 * @details This function returns the task period of the monitoring function.
	 *
	 * @return Task period
	 */
	inline uint16_t getMonitoringTaskPeriod()
	{
		return task_period;
	}

	/*!
	 * @brief Starts a new temperature conversion
	 * @details This function starts a new temperature conversion by writing 0x2E into register 0xF4 of sensor.
	 * 			It also starts a timer to retrieve sensor data after the conversion time.
	 *
	 * @return Nothing
	 */
	void startNewTemperatureConversion();

	/*!
	 * @brief Starts a new pressure conversion
	 * @details This function starts a new pressure conversion by writing 0x74 into register 0xF4 of sensor.
	 * 			It also starts a timer to retrieve sensor data after the conversion time.
	 *
	 * @return Nothing
	 */
	void startNewPressureConversion();

	/*!
	 * @brief Temperature value monitoring function
	 * @details This function monitors the temperature value.
	 * 			It the last correct measurement was done more than twice the monitoring period in the past,
	 * 			the value is declared not ready.
	 *
	 * @return Nothing
	 */
	void TemperatureMonitoring();

	/*!
	 * @brief Pressure value monitoring function
	 * @details This function monitors the pressure value.
	 * 			It the last correct measurement was done more than twice the monitoring period in the past,
	 * 			the value is declared not ready.
	 *
	 * @return Nothing
	 */
	void PressureMonitoring();

	/*!
	 * @brief Temperature conversion activation function
	 * @details This function activates or the periodic start of temperature conversion. The requested period is transmitted in parameter.
	 *
	 * @param [in] req_period Requested period for temperature conversion
	 * @return Nothing
	 */
	void ActivateTemperatureConversion(uint16_t req_period);

	/*!
	 * @brief Pressure conversion activation function
	 * @details This function activates or the periodic start of pressure and temperature conversion. The requested period is transmitted in parameter.
	 *
	 * @param [in] req_period Requested period for pressure conversion
	 * @return Nothing
	 */
	void ActivatePressureConversion(uint16_t req_period);

	/*!
	 * @brief Temperature conversion stop function
	 * @details This function stops or the periodic start of temperature conversion, only if pressure conversion is also disabled.
	 *
	 * @return Nothing
	 */
	void StopTemperatureConversion();

	/*!
	 * @brief Pressure conversion stop function
	 * @details This function stops or the periodic start of pressure conversion.
	 *
	 * @return Nothing
	 */
	void StopPressureConversion();

	/*!
	 * @brief Temperature conversion activation flag get function
	 * @details This function returns the activation status of the temperature conversion.
	 *
	 * @return True if temperature conversion is activated, false otherwise.
	 */
	inline bool isTempConversionActivated()
	{
		return isTempConvActivated;
	}

	/*!
	 * @brief Pressure conversion activation flag get function
	 * @details This function returns the activation status of the pressure conversion.
	 *
	 * @return True if pressure conversion is activated, false otherwise.
	 */
	inline bool isPressConversionActivated()
	{
		return isPressConvActivated;
	}

private:
	I2C* i2c_drv_ptr; /*!< Pointer to the I2C driver object */
	uint8_t chip_id; /*!< Sensor chip ID */
	T_BMP180_status status; /*!< Sensor status */
	uint16_t task_period; /*!< Period of the monitoring task */
	bool isTempConvActivated; /*!< Temperature conversion activation flag */
	bool isPressConvActivated; /*!< Pressure conversion activation flag */

	/*!
	 * @brief Structure defining the calibration data of BMP180 sensor
	 */
	typedef struct
	{
		int16_t AC1;
		int16_t AC2;
		int16_t AC3;
		uint16_t AC4;
		uint16_t AC5;
		uint16_t AC6;
		int16_t B1;
		int16_t B2;
		int16_t MB;
		int16_t MC;
		int16_t MD;
	}
	T_BMP180_calib_data;

	T_BMP180_calib_data calibration_data; /*!< Calibration data of the sensor */

	/*!
	 * @brief Structure defining a sensor value and its status
	 */
	typedef struct
	{
		uint16_t value; /*!< Last measurement value */
		bool ready; /*!< Measurement readiness flag */
		uint32_t ts; /*!< Time stamp of last measurement */
	}
	T_BMP180_measurement_data;

	T_BMP180_measurement_data temperature_value; /*!< Temperature data structure */
	T_BMP180_measurement_data pressure_value; /*!< Pressure data structure */

	int32_t B5_mem; /*!< Memorization of B5 coefficient (computed by temperature formula and used for pressure) */


	/*!
	 * @brief Calibration data reading function
	 * @details This function reads the pressure and temperature calibration data in BMP180 EEPROM using I2C bus.
	 * 			It also updates the driver status if the communication with the device is failed.
	 *
	 * @return Nothing.
	 */
	void readCalibData();

	/*!
	 * @brief Chip ID read function
	 * @details This function reads the ID of the sensor chip using I2C bus. It also updates the driver status
	 * 			if the communication is failed.
	 *
	 * @return Nothing
	 */
	void readChipID();

	/*!
	 * @brief Temperature calculation function
	 * @details This function calculates the true temperature from the raw value from sensor according to the BMP180 datasheet.
	 * 			The true temperature value is stored in temperature_value structure.
	 *
	 * @param [in] UT Raw temperature value
	 * @return Nothing
	 */
	void CalculateTemperature(uint16_t UT);

	/*!
	 * @brief Pressure calculation function
	 * @details This function calculates the true pressure from the raw value from sensor according to the BMP180 datasheet.
	 * 			The true pressure value is stored in pressure_value structure.
	 *
	 * @param [in] UP Raw pressure value
	 * @return Nothing
	 */
	void CalculatePressure(uint32_t UP);
};

extern Bmp180* p_global_BSW_bmp180; /*!< Pointer to BMP180 driver object */

#endif /* WORK_BSW_BMP180_BMP180_H_ */
