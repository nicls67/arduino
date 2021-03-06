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
 * @details This class defines a linked list and the associated services. \n
 * 			All classes using a linked list with this interface shall implement a comparison function used to find the list element to remove. This function shall have the following prototype :
 * 			static bool LLElementCompare(void* LLElement, void* CompareElement);
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
	 * @brief Class destructor
	 * @details This function deletes the linked list
	 *
	 * @return Nothing
	 */
	~LinkedList();

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
	 * @details This function moves the element pointer to the next element of the chain.
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

	/*!
	 * @brief Empty linked list
	 * @details This function checks whether the linked list is empty or not (pointer to first element is equal to 0 or not).
	 *
	 * @return True if the list is empty, false otherwise
	 */
	bool IsLLEmpty();

	/*!
	 * @brief Element finding function
	 * @details This function finds the given element reference_ptr inside the chain. The comparison between the elements of the chain and the reference element is done using the given comparison function.
	 *
	 * @param [in] comparisonFct_ptr Pointer to the comparison function
	 * @param [in] reference_ptr Pointer to the element to find in the chain
	 * @param [out] chainElement_ptr Pointer to pointer to the found element
	 *
	 * @return True if the element has been found in the chain, false otherwise
	 */
	bool FindElement(CompareFctPtr_t comparisonFct_ptr, void* reference_ptr, void** chainElement_ptr);


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

	T_LL_element* firstElement; /*!< Pointer to the first element of the list */
	T_LL_element* curElement_ptr; /*!< Pointer to the current element of the list */
};



#endif /* WORK_LIB_LINKEDLIST_H_ */
