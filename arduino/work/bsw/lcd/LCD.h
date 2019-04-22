/*!
 * @file LCD.h
 *
 * @brief LCD class header file
 *
 * @date 20 avr. 2019
 * @author nicls67
 */

#ifndef WORK_BSW_LCD_LCD_H_
#define WORK_BSW_LCD_LCD_H_

#define I2C_ADDR 0x27 /*!< I2C address of LCD display */

/* Dedinition of LCD pins connections */
#define EN_PIN 2 /*!< EN bit is on P2 */
#define RW_PIN 1 /*!< RW pin is on P1 */
#define RS_PIN 0 /*!< RS pin is on P0 */
#define BACKLIGHT_PIN 3 /*!< Backlight pin is on P3 */

/* Definition of LCD instruction codes bits */
#define LCD_INST_CLR_DISPLAY_BIT 0 /*!< Instruction bit for "clear display" is DB0 */
#define LCD_INST_FUNCTION_SET 5 /*!< Instruction bit for "function set" is DB5 */
#define LCD_INST_DISPLAY_CTRL 3 /*!< Instruction bit for "display control" is DB3 */
#define LCD_INST_ENTRY_MODE_SET 2 /*!< Instruction bit for "entry mode" is DB2 */

/* Definition of fields for function set command */
#define LCD_FCT_SET_FIELD_DL 4 /*!< Field DL (data length) of command "function set" is on bit DB4 */
#define LCD_FCT_SET_FIELD_N 3 /*!< Field N (number of lines) of command "function set" is on bit DB3 */
#define LCD_FCT_SET_FIELD_F 2 /*!< Field F (font type) of command "function set" is on bit DB2 */

/* Definition of fields for display control command */
#define LCD_DISPLAY_CTRL_FIELD_D 2 /*!< Field D (display on/off) of command "display control" is on bit DB2 */
#define LCD_DISPLAY_CTRL_FIELD_C 1 /*!< Field C (cursor on/off) of command "display control" is on bit DB1 */
#define LCD_DISPLAY_CTRL_FIELD_B 0 /*!< Field B (cursor blink) of command "display control" is on bit DB0 */

/* Definition of fields for entry mode set command */
#define LCD_CNF_SHIFT_ID 1 /*!< Field ID (increment or decrement) of command "entry mode set" is on bit DB1 */
#define LCD_CNF_SHIFT_SH 0 /*!< Field SH (shift of display) of command "entry mode set" is on bit DB1 */

/* Line number configuration */
#define LCD_CNF_ONE_LINE 0 /*!< One-line configuration, bit is set to 0 */
#define LCD_CNF_TWO_LINE 1 /*!< Two-line configuration, bit is set to 1 */

/* Font type configuration */
#define LCD_CNF_FONT_5_8  0 /*!< One-line configuration, bit is set to 0 */
#define LCD_CNF_FONT_5_11 1 /*!< Two-line configuration, bit is set to 1 */

/* Display configuration */
#define LCD_CNF_DISPLAY_ON  1 /*!< Display is on, bit is set to 1 */
#define LCD_CNF_DISPLAY_OFF 0 /*!< Display is off, bit is set to 0 */

/* Cursor configuration */
#define LCD_CNF_CURSOR_ON  1 /*!< Cursor is on, bit is set to 1 */
#define LCD_CNF_CURSOR_OFF 0 /*!< Cursor is off, bit is set to 0 */

/* Cursor blinking configuration */
#define LCD_CNF_CURSOR_BLINK_ON  1 /*!< Cursor blinking is on, bit is set to 1 */
#define LCD_CNF_CURSOR_BLINK_OFF 0 /*!< Cursor blinking is off, bit is set to 0 */

/* Entry mode direction configuration */
#define LCD_CNF_ENTRY_MODE_DIRECTION_RIGHT  1 /*!< Direction of shift is right, bit is set to 1 */
#define LCD_CNF_ENTRY_MODE_DIRECTION_LEFT   0 /*!< Direction of shift is left, bit is set to 0 */

/* Entry mode display shift configuration */
#define LCD_CNF_ENTRY_MODE_DISPLAY_SHIFT_ON  1 /*!< Display shift is performed, bit is set to 1 */
#define LCD_CNF_ENTRY_MODE_DISPLAY_SHIFT_OFF 0 /*!< Display shift is not performed, bit is set to 0 */



/*!
 * @brief LCD commands enumeration
 * @details This enumeration defines all command modes available for LCD configuration
 */
typedef enum
{
	LCD_CMD_FUNCTION_SET,
	LCD_CMD_CLEAR_DISPLAY,
	LCD_CMD_DISPLAY_CTRL,
	LCD_CMD_ENTRY_MODE_SET
}
T_LCD_command;

/*!
 * @brief LCD modes enumeration
 * @details This enumeration defines the possible modes for communication with LCD. Two modes are possible, DATA for writing data in RAM and INSTRUCTION for configuring the display
 */
typedef enum
{
	LCD_MODE_INSTRUCTION = 0,
	LCD_MODE_DATA = 1
}
T_LCD_config_mode;

/*!
 * @brief Class for LCD S2004A display driver
 * @details This class handles functions managing LCD display S2004a on I2C bus
 */
class LCD {
public:



	/*!
	 * @brief LCD class constructor
	 * @details This constructor function initializes the class LCD and calls screen configuration function.
	 *
	 * @return Nothing
	 */
	LCD();

	/*!
	 * @brief LCD command management function
	 * @details This function sends the requested command to the LCD screen. It builds the 8-bit command word and sends it on I2C bus.
	 *
	 * @param [in] cmd Requested command
	 * @return Nothing
	 */
	void command(T_LCD_command cmd);

	/*!
	 * @brief Backlight configuration function
	 * @details This function configures the screen backlight (enable or disable) according to the parameter enable.
	 *
	 * @param [in] enable True if backlight shall be on, False otherwise
	 * @return Nothing
	 */
	inline void ConfigureBacklight(bool enable)
	{
		backlight_enable = enable;
	}

	/*!
	 * @brief Line type configuration function
	 * @details This function configures the line number configuration of the screen (1 or 2 lines mode) according to the parameter.
	 *
	 * @param [in] param Configuration value
	 * @return Nothing
	 */
	inline void ConfigureLineNumber(bool param)
	{
		cnfLineNumber = param;
	}

	/*!
	 * @brief Font configuration function
	 * @details This function configures the font type of the screen (5*8 or 5*11 dots) according to the parameter.
	 *
	 * @param [in] param Configuration value
	 * @return Nothing
	 */
	inline void ConfigureFontType(bool param)
	{
		cnfFontType = param;
	}

	/*!
	 * @brief Display configuration function
	 * @details This function configures the display (on or off mode) according to the parameter.
	 *
	 * @param [in] param Configuration value
	 * @return Nothing
	 */
	inline void ConfigureDisplayOnOff(bool param)
	{
		cnfDisplayOnOff = param;
	}

	/*!
	 * @brief Cursor configuration function
	 * @details This function configures the cursor (on or off mode) according to the parameter.
	 *
	 * @param [in] param Configuration value
	 * @return Nothing
	 */
	inline void ConfigureCursorOnOff(bool param)
	{
		cnfCursorOnOff = param;
	}

	/*!
	 * @brief Cursor blinking configuration function
	 * @details This function configures the cursor blinking (on or off mode) according to the parameter.
	 *
	 * @param [in] param Configuration value
	 * @return Nothing
	 */
	inline void ConfigureCursorBlink(bool param)
	{
		cnfCursorBlink = param;
	}

	/*!
	 * @brief Entry mode direction configuration function
	 * @details This function configures the direction of entry mode (right or left) according to the parameter.
	 *
	 * @param [in] param Configuration value
	 * @return Nothing
	 */
	inline void ConfigureEntryModeDir(bool param)
	{
		cnfEntryModeDir = param;
	}

	/*!
	 * @brief Entry mode shift configuration function
	 * @details This function configures the display shift of entry mode (enable or disable) according to the parameter.
	 *
	 * @param [in] param Configuration value
	 * @return Nothing
	 */
	inline void ConfigureEntryModeShift(bool param)
	{
		cnfEntryModeShift = param;
	}



private:

	bool backlight_enable; /*!< Backlight enable flag */
	bool cnfLineNumber; /*!< Display line number configuration, 0 = 1-line mode, 1 = 2-line mode */
	bool cnfFontType; /*!< Font type configuration, 0 = 5*8 dots, 1 = 5*11 dots */
	bool cnfDisplayOnOff; /*!< Display configuration : 1 = display on, 0 = display off */
	bool cnfCursorOnOff; /*!< Cursor configuration : 1 = cursor on, 0 = cursor off */
	bool cnfCursorBlink; /*!< Cursor blinking configuration : 1 = cursor blink is on, 0 = cursor blink is off */
	bool cnfEntryModeDir; /*!< Entry mode direction configuration : 1 = cursor moves to right when DDRAM address is incremented, 0 = cursor moves to left when DDRAM address is incremented */
	bool cnfEntryModeShift; /*!< Entry mode configuration : 0 = no display shift is performed after a DDRAM read, 1 = a shift is performed */


	/*!
	 * @brief I2C write function for 4-bits mode
	 * @details This function sends the requested 8-bits data on the I2C bus. The backlight pin is also set/clear according to the configuration.
	 * 			The function sends the data a first time with EN pin set, then a second time with EN bit clear.
	 *
	 * 	@param [in] data 8-bit data to send
	 * 	@return Nothing
	 */
	void write4bits(uint8_t data);

	/*!
	 * @brief I2C write function
	 * @details This function writes the requested data on I2C bus. It's assumed we only perform write operation so the R/W bit is forced LOW.
	 * 			It's also assumed we work in 4-bit mode, then two calls of write4bits are performed, first with bits 4-7 of data, second with bits 0-3.
	 *
	 * @param [in] data 8-bit data for D0-7 pins of screen
	 * @param [in] mode Requested mode for LCD communication
	 *
	 * @return Nothing
	 */
	void write(uint8_t data, T_LCD_config_mode mode);

	/*!
	 * @brief Screen configuration function.
	 * @details This function configures the LCD screen. It's must be called during initialization phase, or the screen won't be usable. The configuration process is described in LCD datasheet J2004A-GFDN-DYNC
	 *
	 * @return Nothing
	 */
	void ConfigureScreen();
};

#include "../bsw.h"

#endif /* WORK_BSW_LCD_LCD_H_ */
