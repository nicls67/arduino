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

/* TODO : if a line overlaps on the next one, when clearing this line, clear also the next one */

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
 * @brief Alignment mode for line display
 * @details This enumeration defines the possible alignment mode for the text displayed.
 *   		It is only used when the display mode is NORMAL or GO_TO_NEXT_LINE.
 */
typedef enum
{
	LEFT, /*!< Text is aligned left */
	CENTER, /*!< Text is centered */
	RIGHT /*!< Text is aligned right */
}
T_DisplayInterface_LineAlignment;

/*!
 * @brief Structure containing shift data
 * @details This structure contains all useful data for line shifting. These data need to be kept between each call of the periodic function.
 */
typedef struct
{
	String* str_ptr; /*!< Pointer to the start address of the string */
	uint8_t* str_cur_ptr; /*!< Pointer to the address of the first displayed character */
	uint8_t temporization; /*!< Shifting period */
}
T_Display_shift_data;

/*!
 * @brief Structure containing display data
 * @details This structure contains all data used for screen display
 */
typedef struct
{
	bool isEmpty; /*!< Flag indicating if the line is empty or not */
	T_DisplayInterface_LineDisplayMode mode; /*!< Current display mode */
	T_DisplayInterface_LineAlignment alignment; /*!< Line alignment */
	T_Display_shift_data shift_data; /*!< Shift data for the current line */
	uint8_t display_str[LCD_SIZE_NB_CHAR_PER_LINE]; /*!< Current string displayed on the screen */
}
T_display_data;


#define DISPLAY_LINE_SHIFT_PERIOD_MS 500 /*!< In "line shift" mode for line display, line is shifted every 500 ms */
#define DISPLAY_LINE_SHIFT_TEMPO_TIME 6 /*!< In "line shift" mode for line display, a temporization of 6 periods is added at the end and the beginning of the lines */


/*!
 * @brief Display interface services class
 * @details This class defines the services used for interfacing display management services and LCD screen driver
 */
class DisplayInterface {

public:
	/*!
	 * @brief Class constructor
	 * @details This function initializes all class variables and instantiates the LCD driver according to the given configuration.
	 *
	 * @param [in] LCD_init_cnf Initial configuration of the screen
	 * @return Nothing
	 */
	DisplayInterface(const T_LCD_conf_struct * LCD_init_cnf);

	/*!
	 * @brief Line display function
	 * @details This function displays the given string on the requested line. If the string is too long to be displayed entirely, the behavior is defined by the selected mode.
	 *
	 * @param [in] str Pointer to the string to display
	 * @param [in] size Size of the string to display
	 * @param [in] line Index of the line where the string shall be displayed
	 * @param [in] mode Display mode
	 * @param [in] alignment Requested alignment for the line
	 * @return True if the line has been correctly displayed, false otherwise
	 */
	bool DisplayFullLine(uint8_t* str, uint8_t size, uint8_t line, T_DisplayInterface_LineDisplayMode mode = NORMAL, T_DisplayInterface_LineAlignment alignment = LEFT);

	/*!
	 * @brief Line clearing function
	 * @details This function clears the requested line. It sets the corresponding DDRAM addresses to the ASCII value of space character.
	 * 			If it was the last line with a display shift in progress, it removes the periodic task from the scheduler.
	 *
	 * @param [in] line Line to clear
	 * @return True if the line has been cleared, false otherwise
	 */
	bool ClearLine(uint8_t line);

	/*!
	 * @brief Screen cleaning function
	 * @details This functions clears the entire display. It uses the ClearLine function on every line of the screen.
	 *
	 * @return Nothing
	 */
	void ClearFullScreen();

	/*!
	 * @brief Empty line get function
	 * @details This function answers if the line given in parameter is empty or not, according to the table isLineEmpty[]
	 *
	 * @param [in] line Requested line
	 * @return True if the line is empty, false otherwise
	 */
	bool IsLineEmpty(uint8_t line);

	/*!
	 * @brief Line shifting periodic task
	 * @details This function is called periodically by the scheduler. It shifts all the lines in line shifting mode and updates the data structures.
	 *
	 * @return Nothing
	 */
	static void shiftLine_task();

	/*!
	 * @brief Display data get function
	 * @details This function returns a pointer to the display data structure.
	 *
	 * @return Pointer to display data structure.
	 */
	inline T_display_data* getDisplayDataPtr()
	{
		return display_data;
	}

	/*!
	 * @brief Text alignment function
	 * @details This function updates the text alignment on the requested line.
	 * 			It calls the private function setLineAlignment to update data structure and then refreshes the display.
	 * 			Nothing is done if the requested alignment is the same than the current one, if the line is empty or if the line is in line shift mode.
	 *
	 * @param [in] line Requested line to update
	 * @param [in] alignment Requested alignment for the text
	 * @return Nothing
	 */
	void setLineAlignmentAndRefresh(uint8_t line, T_DisplayInterface_LineAlignment alignment);



private:

	LCD* p_lcd; /*!< Pointer to the attached LCD driver object */
	uint32_t dummy; /*!< Needed for data alignment */
	T_display_data display_data[LCD_SIZE_NB_LINES]; /*!< Screen display data */
	bool isShiftInProgress; /*!< Flag indicating if a shift is in progress on any line */

	/*!
	 * @brief Finds start address of a line.
	 * @details This function finds the address in DDRAM of the first character of a line.
	 *
	 * @param [in] line Line which address shall be found
	 * @return Address in DDRAM of the first character of the line
	 */
	uint8_t FindFirstCharAddr(uint8_t line);

	/*!
	 * @brief Line refresh function
	 * @details This function refreshes the display on the requested line. It computes the screen RAM address and writes the string to display into the screen RAM.
	 * 			It shall be called everytime the string in display data structure is updated.
	 *
	 * @param [in] line Line to refresh
	 * @return Nothing
	 */
	void RefreshLine(uint8_t line);

	/*!
	 * @brief String data clearing structure
	 * @details This function clears the string contained in the display data structure. It sets all characters to space character.
	 *
	 * @param [in] line Line to clear
	 * @return Nothing
	 */
	void ClearStringInDataStruct(uint8_t line);

	/*!
	 * @brief Text alignment setting function
	 * @details This function updates the text alignment on the requested line.
	 * 			The string in the data structure is updated with the new alignment. The alignment parameter in the data structure shall be updated before calling this function.
	 *
	 * @param [in] line Line to update
	 * @return Nothing
	 */
	void setLineAlignment(uint8_t line);

	/*!
	 * @brief Line data string update function
	 * @details This function updates the data string and refreshes the display. The string is aligned according to the given parameter.
	 *
	 * @param [in] str Pointer to the string to display
	 * @param [in] size Size of the string
	 * @param [in] line Line to update
	 *
	 * @return Nothing
	 */
	void updateLineAndRefresh(uint8_t* str, uint8_t size, uint8_t line);
};


#endif /* WORK_ASW_DISPLAY_IFT_DISPLAYINTERFACE_H_ */
