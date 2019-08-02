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

#define BMP180_TIMER_PRESCALER_VALUE 64 /*!< Value of prescaler to use for timer */
#define BMP180_TEMP_MEAS_TIMER_CTC_VALUE ((F_CPU/PRESCALER_PERIODIC_TIMER)/(1000/10)) /**< Compare value for periodic timer */

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
/* TODO : add manual activation of temp and pressure reading */

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
	 * 			If a temperature measurement is available, this value is returned directly.
	 * 			A new conversion is started after the read operation. The result of this conversion will be
	 * 			retrieved by an interrupt after 4.5 ms.
	 * 			If the driver status is not OK, the function behaves as if no measurement is available.
	 *
	 * @param [in] data Pointer the location where the temperature data shall be stored.
	 *
	 * @return True if a temperature measurement is available, false otherwise.
	 */
	bool getTemperatureValue(uint16_t* data);

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
	 * 			It starts new conversions of temperature and pressure values.
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
	 * @brief Temperature value monitoring function
	 * @details This function monitors the temperature value.
	 * 			It the last correct measurement was done more than twice the monitoring period in the past,
	 * 			the value is declared not ready.
	 *
	 * @return Nothing
	 */
	void TemperatureMonitoring();

	/*!
	 * @brief Temperature conversion activation function
	 * @details This function activates or the periodic start of temperature conversion. The requested period is transmitted in parameter.
	 *
	 * @param [in] req_period Requested period for temperature conversion
	 * @return Nothing
	 */
	void ActivateTemperatureConversion(uint16_t req_period);

	/*!
	 * @brief Temperature conversion stop function
	 * @details This function stops or the periodic start of temperature conversion.
	 *
	 * @return Nothing
	 */
	void StopTemperatureConversion();

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

private:
	I2C* i2c_drv_ptr; /*!< Pointer to the I2C driver object */
	uint8_t chip_id; /*!< Sensor chip ID */
	T_BMP180_status status; /*!< Sensor status */
	uint16_t task_period; /*!< Period of the monitoring task */ /*TODO : add update of period */
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
};

extern Bmp180* p_global_BSW_bmp180; /*!< Pointer to BMP180 driver object */

#endif /* WORK_BSW_BMP180_BMP180_H_ */
