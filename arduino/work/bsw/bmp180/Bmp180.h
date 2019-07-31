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

#define BMP180_DATA_VALIDITY_TMO 2 /*!< Number of PITs after which the sensor value is declared invalid */

#define BMP180_TIMER_PRESCALER_VALUE 64 /*!< Value of prescaler to use for timer */
#define BMP180_TEMP_MEAS_TIMER_CTC_VALUE ((F_CPU/PRESCALER_PERIODIC_TIMER)/(1000/10)) /**< Compare value for periodic timer */

#define BMP180_OUT_REG_LSB_EEPROM_ADDR 0xF7 /*!< Address of LSB out register */
#define BMP180_OUT_REG_MSB_EEPROM_ADDR 0xF6 /*!< Address of MSB out register */





/* TODO : add monitoring of driver status : reset comm in case of failure, also for LCD ! */

/*!
 * @brief BMP180 sensor class definition
 * @details This class manages BMP180 driver.
 */
class Bmp180 {
public:

	/*!
	 * @brief Bmp180 class constructor
	 * @details This function initializes the class Bmp180. It reads the chip ID and reads calibration data.
	 * 			If the chip ID is incorrect, the status is set to communication failed.
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

private:
	I2C* i2c_drv_ptr; /*!< Pointer to the I2C driver object */
	uint8_t chip_id; /*!< Sensor chip ID */

	/*!
	 * @brief Enumeration defining the possible statuses for BMP180 sensor driver
	 */
	typedef enum
	{
		OK,
		COMM_FAILED,
	}
	T_BMP180_status;

	T_BMP180_status status; /*!< Sensor status */

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
		uint16_t value;
		bool ready;
		uint32_t ts;
	}
	T_BMP180_measurement_data;

	T_BMP180_measurement_data temperature_value; /*!< Temperature data structure */
	T_BMP180_measurement_data pressure_value; /*!< Pressure data structure */

	/*!
	 * @brief Enumeration defining the different possible types of conversion
	 */
	typedef enum
	{
		NONE, /*!< No conversion is in progress */
		TEMPERATURE, /*!< A temperature conversion is in progress */
		PRESSURE /*!< A pressure conversion is in progress */
	}
	T_BMP180_conversion_type;

	T_BMP180_conversion_type conversionInProgress; /*<! Defines if a conversion is in progress or not, and which type */

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
	 * @brief Starts a new temperature conversion
	 * @details This function starts a new temperature conversion by writing 0x2E into register 0xF4 of sensor.
	 * 			It also starts a timer to retrieve sensor data after the conversion time.
	 *
	 * @return Nothing
	 */
	void startNewTemperatureConversion();

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
