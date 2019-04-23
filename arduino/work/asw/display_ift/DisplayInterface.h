/*!
 * @file DisplayInterface.h
 *
 * @brief DisplayInterface class header file
 *
 * @date 23 avr. 2019
 * @author nicls67
 */

#ifndef WORK_ASW_DISPLAY_IFT_DISPLAYINTERFACE_H_
#define WORK_ASW_DISPLAY_IFT_DISPLAYINTERFACE_H_

#include "../../bsw/lcd/lcd.h"

const T_LCD_conf_struct LCD_init_cnf = {
	LCD_CNF_BACKLIGHT_ON,
	LCD_CNF_TWO_LINE,
	LCD_CNF_FONT_5_8,
	LCD_CNF_DISPLAY_ON,
	LCD_CNF_CURSOR_ON,
	LCD_CNF_CURSOR_BLINK_ON,
	LCD_CNF_ENTRY_MODE_DIRECTION_RIGHT,
	LCD_CNF_ENTRY_MODE_DISPLAY_SHIFT_OFF
};

/*!
 * @brief Display interface services class
 * @details This class defines the services used for interfacing display management services and LCD screen driver
 */
class DisplayInterface {

public:
	/*!
	 * @brief Class constructor
	 * @details This function initializes all class variables and instantiates the LCD driver according to the defined configuration.
	 */
	DisplayInterface();


private:

	LCD* p_lcd; /*!< Pointer to the attached LCD driver object */
};

#include "../../bsw/bsw.h"
#include "../asw.h"

#endif /* WORK_ASW_DISPLAY_IFT_DISPLAYINTERFACE_H_ */
