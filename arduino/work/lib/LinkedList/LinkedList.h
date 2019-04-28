/*!
 * @file LinkedList.h
 *
 * @brief Linked List library header file
 *
 * @date 27 avr. 2019
 * @author nicls67
 */

#ifndef WORK_LIB_LINKEDLIST_H_
#define WORK_LIB_LINKEDLIST_H_


typedef bool (*CompareFctPtr_t)(void* LLElement, void* CompareElement); /*!< Type defining a pointer to the comparison function */

/*!
 * @brief Linked list class
 * @details This class defines a linked list and the associated services
 */
class LinkedList {

public:
	/*!
	 * @brief Class constructor
	 * @details This constructor initializes a linked list
	 *
	 * @return Nothing
	 */
	LinkedList();

	/*!
	 * @brief Add an new element to the list
	 * @details This function adds a new element at the end of the list. The data pointer to attach to the element is given in parameter
	 *
	 * @param [in] data_ptr Pointer to the data element
	 * @return Nothing
	 */
	void AttachNewElement(void* data_ptr);

	/*!
	 * @brief Removes an element from the chain
	 * @details This function removes an element from the chain. To know which element shall be removed, we use the comparison function given in parameter.
	 * 			This function is called with two parameters : the data pointer from the chain and the reference pointer given as parameter.
	 *
	 * 	@param [in] comparisonFct_ptr Pointer to the comparison function to use
	 * 	@param [in] reference_ptr Pointer to the reference data used for comparison
	 * 	@return True if the element has been correctly removed from the chain, false otherwise
	 */
	bool RemoveElement(CompareFctPtr_t comparisonFct_ptr, void* reference_ptr);

	/*!
	 * @brief Current element get function
	 * @details This function returns a pointer to the current pointed data in the chain.
	 *
	 * @return Pointer to the current data
	 */
	inline void* getCurrentElement()
	{
		return curElement_ptr->data_ptr;
	}

	/*!
	 * @brief Move to next element function
	 * @details This function moves the element pointer tot he next element of the chain.
	 *
	 * @return True if the next element exists, false if there is no next element
	 */
	bool MoveToNextElement();

	/*!
	 * @brief Resets element pointer
	 * @details This function sets the element pointer to the first element of the chain.
	 *
	 * @return Nothing
	 */
	inline void ResetElementPtr()
	{
		curElement_ptr = firstElement;
	}

private:
	/*!
	 * @brief Type defining a linked list element
	 * @details This structure defines a linked list element. An element is defined by a pointer to the attached data and a pointer to the next element.
	 */
	typedef struct T_LL_element
	{
		void* data_ptr;
		T_LL_element* nextElement;
	}
	T_LL_element;

	T_LL_element* firstElement;
	T_LL_element* curElement_ptr;
};



#endif /* WORK_LIB_LINKEDLIST_H_ */
