/*!
 * @file String.h
 *
 * @brief String class header file
 *
 * @date 2 mai 2019
 * @author nicls67
 */

#ifndef WORK_LIB_STRING_STRING_H_
#define WORK_LIB_STRING_STRING_H_

/*!
 * @brief String management class
 * @details This class defines string object. It implements some functions to manage chains of characters.
 * 			The string is limited to 255 characters. It must finish by the character '\0'.
 */
class String {

public:

	/*!
	 * @brief Class constructor
	 * @details This function initializes the class. The string is initialized with the data given in parameter.
	 *
	 * @param [in] str Pointer to initialization string
	 * @return Nothing
	 */
	String(const uint8_t* str);

	/*!
	 * @brief Class constructor
	 * @details This function initializes the class with an empty string. The size is set to 0.
	 *
	 * @return Nothing
	 */
	String();

	/*!
	 * @brief Class destructor
	 * @details This function frees the memory used to contain the string when the object is deleted
	 *
	 * @return Nothing
	 */
	~String();

	/*!
	 * @brief String pointer get function
	 * @details This function returns the pointer to the beginning of the string.
	 *
	 * @return String pointer
	 */
	inline uint8_t* getString()
	{
		return string;
	}

	/*!
	 * @brief Size get function
	 * @details This function returns the size of the string.
	 *
	 * @return Size of the string
	 */
	inline uint8_t getSize()
	{
		return size;
	}

	/*!
	 * @brief String adding function
	 * @details This functions adds the given string at the end of the main string. The string size is updated accordingly.
	 *
	 * @param [in] str New string to add
	 * @return Nothing
	 */
	void appendString(uint8_t* str);

	/*!
	 * @brief Integer adding function
	 * @details This functions adds the given integer at the end of the main string. The string size is updated accordingly.
	 * 			The integer parameter is first converted into a chain of character according to the base and then added to the string.
	 *
	 * @param [in] value Integer to add
	 * @param [in] base Base of computation of the integer (between 2 and 36)
	 * @return Nothing
	 */
	void appendInteger(uint16_t value, uint8_t base);

	/*!
	 * @brief Boolean adding function
	 * @details This functions adds the given boolean data at the end of the main string. The string size is updated accordingly.
	 * 			According to the input parameter isText, the boolean parameter is converted into a string (true/false) or an integer (0/1).
	 * @param [in] data Boolean data to add
	 * @param [in] isText Defines the conversion mode : text or integer
	 *
	 * @return Nothing
	 */
	void appendBool(bool data, bool isText);

	/*!
	 * @brief String clear function
	 * @details This function clears the string. Size is set to 0 and the memory is freed.
	 *
	 * @return Nothing
	 */
	void Clear();


private:

	uint8_t* string; /*!< Pointer to the start of the string */
	uint8_t size; /*!< Size of the string (the '\0' at the end of the string is not taken into account */

	/*!
	 * @brief String size computation function
	 * @details This function computes the sizes of the given string. It counts the number of character between the start of the string given in parameter and the next \0 character.
	 *
	 * @param [in] str Pointer to the beginning of the string
	 * @return Number of character of the string (the \0 is excluded)
	 */
	uint8_t ComputeStringSize(uint8_t* str);
};

#endif /* WORK_LIB_STRING_STRING_H_ */
