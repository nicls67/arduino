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

/*!
 * @brief LCD configuration structure
 * @details This structure defines the initial configuration of the LCD screen.
 */
const T_LCD_conf_struct LCD_init_cnf = {
	0x27,
	LCD_CNF_BACKLIGHT_ON,
	LCD_CNF_TWO_LINE,
	LCD_CNF_FONT_5_8,
	LCD_CNF_DISPLAY_ON,
	LCD_CNF_CURSOR_OFF,
	LCD_CNF_CURSOR_BLINK_OFF,
	LCD_CNF_ENTRY_MODE_DIRECTION_RIGHT,
	LCD_CNF_ENTRY_MODE_DISPLAY_SHIFT_OFF
};

/*!
 * @brief Modes for line display
 * @details This enumeration defines the available modes for the line display functionality :\n
 * 				1- Normal mode : if the string is too long to be displayed entirely, the end of the string is cut.\n
 * 				2- Line shift mode : the display is moving to display all the string.\n
 * 				3- Next line mode : the remaining characters are displayed on the next lines.\n
 */
typedef enum
{
	NORMAL,
	LINE_SHIFT,
	GO_TO_NEXT_LINE
}
T_DisplayInterface_LineDisplayMode;

/*!
 * @brief Structure containing shift data
 * @details This structure contains all useful data for line shifting. These data need to be kept between each call of the periodic function.
 */
typedef struct
{
	uint8_t* str_start_ptr;
	uint8_t* str_cur_ptr;
	uint8_t size;
	uint8_t line;
	uint8_t temporization;
}
T_Display_shift_data;

#define DISPLAY_LINE_SHIFT_PERIOD_MS 500 /*!< In "line shift" mode for line display, line is shifted every 500 ms */
#define DISPLAY_LINE_SHIFT_TEMPO_TIME 6 /*!< In "line shift" mode for line display, a temporization of 3 periods is added at the end and the beginning of the lines */


/*!
 * @brief Display interface services class
 * @details This class defines the services used for interfacing display management services and LCD screen driver
 */
class DisplayInterface {

public:
	/*!
	 * @brief Class constructor
	 * @details This function initializes all class variables and instantiates the LCD driver according to the defined configuration.
	 *
	 * @return Nothing
	 */
	DisplayInterface();

	/*!
	 * @brief Line display function
	 * @details This function displays the given string on the requested line. If the string is too long to be displayed entirely, the behavior is defined by the selected mode.
	 *
	 * @param [in] str Pointer to the string to display
	 * @param [in] size Size of the string to display
	 * @param [in] line Index of the line where the string shall be displayed
	 * @param [in] mode Display mode
	 * @return True if the line has been correctly displayed, false otherwise
	 */
	bool DisplayFullLine(uint8_t* str, uint8_t size, uint8_t line, T_DisplayInterface_LineDisplayMode mode);

	/*!
	 * @brief Line clearing function
	 * @details This function clears the requested line. It sets the corresponding DDRAM addresses to the ASCII value of space character
	 *
	 * @param [in] line Line to clear
	 * @return True if the line has been cleared, false otherwise
	 */
	bool ClearLine(uint8_t line);

	/*!
	 * @brief Empty line get function
	 * @details This function answers if the line given in parameter is empty or not, according to the table isLineEmpty[]
	 *
	 * @param [in] line Requested line
	 * @return True if the line is empty, false otherwise
	 */
	bool IsLineEmpty(uint8_t line);

	/*!
	 * @brief Linked list comparison function
	 * @details This function is called by the linked list class to compare one element of the list to a given element.
	 * 			In the class DisplayInterface, the LLElement is a shift data pointer (containing a line number inside it), and the compareElement a line number.
	 * 			The comparison will be done between the two function pointer.
	 *
	 * @param [in] LLElement Pointer to the linked list element
	 * @param [in] CompareElement Pointer to the element to the compare
	 * @return True if both elements are identical, false otherwise
	 */
	static bool LLElementCompare(void* LLElement, void* CompareElement);

	/*!
	 * @brief Line shifting periodic task
	 * @details This function is called periodically by the scheduler. It shifts all the lines in line shifting mode and updates the data structures.
	 *
	 * @return Nothing
	 */
	static void shiftLine_task();

	/*!
	 * @brief Linked list shift data get function
	 * @details This function returns the pointer to the shift data linked list.
	 *
	 * @return Pointer to linked list
	 */
	inline LinkedList* getLLShiftDataPtr()
	{
		return LL_shift_data_ptr;
	}

private:

	LCD* p_lcd; /*!< Pointer to the attached LCD driver object */
	uint32_t dummy; /*!< Needed for data alignment */
	bool lineEmptyTab[LCD_SIZE_NB_LINES]; /*!< Table indicating whether a line is empty or not (true = line empty, false = line not empty */
	LinkedList* LL_shift_data_ptr; /*!< Linked list containing data for line shifting, each element of the list corresponds to a line of the screen */

	/*!
	 * @brief Finds start address of a line.
	 * @details This function finds the address in DDRAM of the first character of a line.
	 *
	 * @param [in] line Line which address shall be found
	 * @return Address in DDRAM of the first character of the line
	 */
	uint8_t FindFirstCharAddr(uint8_t line);
};

#include "../../bsw/bsw.h"
#include "../asw.h"

#endif /* WORK_ASW_DISPLAY_IFT_DISPLAYINTERFACE_H_ */
