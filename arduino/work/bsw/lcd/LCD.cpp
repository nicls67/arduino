/*!
 * @file LCD.cpp
 *
 * @brief LCD class source file
 *
 * @date 20 avr. 2019
 * @author nicls67
 */

#include <util/delay.h>

#include "LCD.h"

LCD::LCD()
{
	/* Screen default configuration */
	ConfigureBacklight(true);
	ConfigureLineNumber(LCD_CNF_ONE_LINE);
	ConfigureLineNumber(LCD_CNF_FONT_5_8);
	ConfigureDisplayOnOff(LCD_CNF_DISPLAY_ON);
	ConfigureCursorOnOff(LCD_CNF_CURSOR_ON);
	ConfigureCursorBlink(LCD_CNF_CURSOR_BLINK_ON);
	ConfigureEntryModeDir(LCD_CNF_ENTRY_MODE_DIRECTION_RIGHT);
	ConfigureEntryModeShift(LCD_CNF_ENTRY_MODE_DISPLAY_SHIFT_OFF);

	ConfigureScreen();

}


void LCD::write4bits(uint8_t data)
{
	uint8_t dummy;

	/* Configure backlight pin and set EN pin */
	data |= (backlight_enable << BACKLIGHT_PIN) + (1 << EN_PIN);
	dummy = BSW_cnf_struct.p_i2c->writeByte(&data);

	_delay_us(1);

	/* Clear enable pin */
	data &= ~(1 << EN_PIN);
	dummy = BSW_cnf_struct.p_i2c->writeByte(&data);

	_delay_us(50);
}

void LCD::write(uint8_t data, T_LCD_config_mode mode)
{
	uint8_t high = data & 0b11110000;
	uint8_t low = (data << 4) & 0b11110000;

	write4bits(high | (mode << RS_PIN));
	write4bits(low | (mode << RS_PIN));

}


void LCD::ConfigureScreen()
{
	uint8_t data;

	BSW_cnf_struct.p_i2c->setTxAddress(I2C_ADDR);

	/* Wait for 30ms after power on */
	_delay_us(30000);

	/*
	 * Before screen is configured in 4-bits mode, the function LCD::command cannot be used. Then we build manually the data word and send it with LCD::write4bits function
	 */

	/* Function set, first go in 8-bits mode 3 times, to avoid any screen issue */
	data = (1 << LCD_INST_FUNCTION_SET) + (1 << LCD_FCT_SET_FIELD_DL);
	write4bits(data);

	/* Wait at least 38 us */
	_delay_us(40);

	data = (1 << LCD_INST_FUNCTION_SET) + (1 << LCD_FCT_SET_FIELD_DL);
	write4bits(data);

	/* Wait at least 38 us */
	_delay_us(40);

	data = (1 << LCD_INST_FUNCTION_SET) + (1 << LCD_FCT_SET_FIELD_DL);
	write4bits(data);

	/* Wait at least 38 us */
	_delay_us(40);

	/* Function set, go in 4-bits mode */
	data = (1 << LCD_INST_FUNCTION_SET);
	write4bits(data);

	/* Wait at least 38 us */
	_delay_us(40);

	/* Configure line and matrix */
	command(LCD_CMD_FUNCTION_SET);

	/* Configure display */
	command(LCD_CMD_DISPLAY_CTRL);

	/* Wait at least 38 us */
	_delay_us(40);

	/* Clear display */
	command(LCD_CMD_CLEAR_DISPLAY);

	/* Wait at least 1.52 ms */
	_delay_us(1600);

	/* Configure cursor */
	command(LCD_CMD_ENTRY_MODE_SET);
}

void LCD::command(T_LCD_command cmd)
{
	uint8_t data = 0;

	switch(cmd)
	{
	case LCD_CMD_FUNCTION_SET:
		/* Data length bit is forced to 0, we can work only in 4-bits mode */
		data = (1 << LCD_INST_FUNCTION_SET) + (cnfLineNumber << LCD_FCT_SET_FIELD_N) + (cnfFontType << LCD_FCT_SET_FIELD_F);
		break;

	case LCD_CMD_DISPLAY_CTRL:
		data = (1 << LCD_INST_DISPLAY_CTRL) + (cnfDisplayOnOff << LCD_DISPLAY_CTRL_FIELD_D) + (cnfCursorOnOff << LCD_DISPLAY_CTRL_FIELD_C) + (cnfCursorBlink << LCD_DISPLAY_CTRL_FIELD_B);
		break;

	case LCD_CMD_CLEAR_DISPLAY:
		data = (1 << LCD_INST_CLR_DISPLAY_BIT);
		break;

	case LCD_CMD_ENTRY_MODE_SET:
		data = (1 << LCD_INST_ENTRY_MODE_SET) + (cnfEntryModeDir << LCD_CNF_SHIFT_ID) + (cnfEntryModeShift << LCD_CNF_SHIFT_SH);
		break;

	default:
		/* Do nothing */
		break;
	}

	write(data, LCD_MODE_INSTRUCTION);
}
