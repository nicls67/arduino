/*!
 * @file LCD.cpp
 *
 * @brief LCD class source file
 *
 * @date 20 avr. 2019
 * @author nicls67
 */

#include <util/delay.h>
#include <avr/io.h>

#include "../I2C/I2C.h"
#include "LCD.h"

LCD* p_global_BSW_lcd;

LCD::LCD(const T_LCD_conf_struct* init_conf)
{
	/* Initialize class variables */
	ddram_addr = 0;

	/* Create new instance of I2C driver if needed */
	if(p_global_BSW_i2c == 0)
		p_global_BSW_i2c = new I2C(init_conf->i2c_bitrate);

	i2c_drv_ptr = p_global_BSW_i2c;

	/* Screen default configuration */
	ConfigureBacklight(init_conf->backlight_en);
	ConfigureLineNumber(init_conf->lineNumber_cnf);
	ConfigureFontType(init_conf->fontType_cnf);
	ConfigureDisplayOnOff(init_conf->display_en);
	ConfigureCursorOnOff(init_conf->cursor_en);
	ConfigureCursorBlink(init_conf->cursorBlink_en);
	ConfigureEntryModeDir(init_conf->entryModeDir);
	ConfigureEntryModeShift(init_conf->entryModeShift);
	ConfigureI2CAddr(init_conf->i2c_addr);

	InitializeScreen();

}


void LCD::write4bits(uint8_t data)
{
	uint8_t dummy;

	/* Configure backlight pin and set EN pin */
	data |= (backlight_enable << BACKLIGHT_PIN) + (1 << EN_PIN);
	dummy = i2c_drv_ptr->writeByte(&data);

	_delay_us(1);

	/* Clear enable pin */
	data &= ~(1 << EN_PIN);
	dummy = i2c_drv_ptr->writeByte(&data);

	_delay_us(50);
}

void LCD::write(uint8_t data, T_LCD_config_mode mode)
{
	uint8_t high = data & 0b11110000;
	uint8_t low = (data << 4) & 0b11110000;

	write4bits(high | (mode << RS_PIN));
	write4bits(low | (mode << RS_PIN));

}


void LCD::InitializeScreen()
{
	uint8_t data;

	i2c_drv_ptr->setTxAddress(cnfI2C_addr);

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

	/* Clear display */
	command(LCD_CMD_CLEAR_DISPLAY);

	/* Configure cursor */
	command(LCD_CMD_ENTRY_MODE_SET);
}

void LCD::command(T_LCD_command cmd)
{
	uint8_t data = 0;
	uint32_t wait_time = 0;

	switch(cmd)
	{
	case LCD_CMD_FUNCTION_SET:
		/* Data length bit is forced to 0, we can work only in 4-bits mode */
		data = (1 << LCD_INST_FUNCTION_SET) + (cnfLineNumber << LCD_FCT_SET_FIELD_N) + (cnfFontType << LCD_FCT_SET_FIELD_F);
		wait_time = LCD_WAIT_OTHER_MODES;
		break;

	case LCD_CMD_DISPLAY_CTRL:
		data = (1 << LCD_INST_DISPLAY_CTRL) + (cnfDisplayOnOff << LCD_DISPLAY_CTRL_FIELD_D) + (cnfCursorOnOff << LCD_DISPLAY_CTRL_FIELD_C) + (cnfCursorBlink << LCD_DISPLAY_CTRL_FIELD_B);
		wait_time = LCD_WAIT_OTHER_MODES;
		break;

	case LCD_CMD_CLEAR_DISPLAY:
		data = (1 << LCD_INST_CLR_DISPLAY_BIT);
		ddram_addr = 0;
		wait_time = LCD_WAIT_CLR_RETURN;
		break;

	case LCD_CMD_ENTRY_MODE_SET:
		data = (1 << LCD_INST_ENTRY_MODE_SET) + (cnfEntryModeDir << LCD_CNF_SHIFT_ID) + (cnfEntryModeShift << LCD_CNF_SHIFT_SH);
		wait_time = LCD_WAIT_OTHER_MODES;
		break;

	case LCD_CMD_SET_DDRAM_ADDR:
		data = (1 << LCD_INST_SET_DDRAM_ADDR) + ddram_addr;
		wait_time = LCD_WAIT_OTHER_MODES;
		break;

	default:
		/* Do nothing */
		break;
	}

	write(data, LCD_MODE_INSTRUCTION);
	_delay_us(wait_time);
}

void LCD::SetDDRAMAddress(uint8_t addr)
{
	/* If 1-line mode, address shall be between 0 and 0x4F */
	if (cnfLineNumber == LCD_CNF_ONE_LINE)
	{
		if (addr > LCD_RAM_1_LINE_MAX)
			addr = LCD_RAM_1_LINE_MAX;
	}
	/* If 2-lines mode, address shall be between 0 and 0x27 or between 0x40 and 0x67 */
	else if (cnfLineNumber == LCD_CNF_TWO_LINE)
	{
		if ((addr > LCD_RAM_2_LINES_MAX_1) && (addr < LCD_RAM_2_LINES_MIN_2))
			addr = LCD_RAM_2_LINES_MAX_1;
		else if (addr > LCD_RAM_2_LINES_MAX_2)
			addr = LCD_RAM_2_LINES_MAX_2;
	}

	/* Now set the internal variable and update screen */
	ddram_addr = addr;
	command(LCD_CMD_SET_DDRAM_ADDR);
}

void LCD::WriteInRam(uint8_t a_char, T_LCD_ram_area area)
{

	/* Only DDRAM is implemented for now */
	if (area == LCD_DATA_DDRAM)
	{
		/* Increment RAM address */
		if (cnfLineNumber == LCD_CNF_ONE_LINE)
			ddram_addr = (ddram_addr + 1) % (LCD_RAM_1_LINE_MAX + 1);

		else if (cnfLineNumber == LCD_CNF_TWO_LINE)
		{
			ddram_addr++;

			if (ddram_addr == (LCD_RAM_2_LINES_MAX_1 + 1))
				ddram_addr = LCD_RAM_2_LINES_MIN_2;

			else if (ddram_addr == (LCD_RAM_2_LINES_MAX_2 + 1))
				ddram_addr = LCD_RAM_2_LINES_MIN_1;
		}
	}

	write(a_char, LCD_MODE_DATA);
	_delay_us(LCD_WAIT_OTHER_MODES);
}
