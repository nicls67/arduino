/*!
 * @file scheduler.h
 *
 * @brief Scheduler class header file
 *
 * @date 16 mars 2018
 * @author nicls67
 */

#ifndef WORK_SCHEDULER_SCHEDULER_H_
#define WORK_SCHEDULER_SCHEDULER_H_

#include "../lib/LinkedList/LinkedList.h"


#define SW_PERIOD_MS 500 /*!< Software period, used to define periodic timer interrupt */
#define PRESCALER_PERIODIC_TIMER 256 /*!< Value of prescaler to use for periodic timer */
#define TIMER_CTC_VALUE ((F_CPU/PRESCALER_PERIODIC_TIMER)/(1000/SW_PERIOD_MS)) /**< Compare value for periodic timer */

/*!
* @brief Type defining a pointer to function
*/
typedef void (*TaskPtr_t)(void);

/*!
 * @brief Scheduler class
 * @details This class defines the scheduler of the system. \n
 * 			It is called by the main interrupt and calls successively all applicative functions according to their recurrence time. \n
 * 			All tasks called by the scheduler shall have the following prototype : static void task();
 */
class scheduler
{
public:



	/*!
	 * @brief scheduler class constructor
	 * @details This function initializes the class scheduler
	 *
	 * @return Nothing
	 */
	scheduler();

	/*!
	 * @brief Main scheduler function
	 * @details This function launches the scheduled tasks according to current software time and task configuration
	 *
	 * @return Nothing
	 */
	void launchPeriodicTasks();

	/*!
	 * @brief Starts the tasks scheduling
	 * @details This function starts the timer which will trigger an interrupt every software period.
	 * 			When the interrupt is raised the scheduler will launch applications
	 *
	 * 	@return Nothing
	 */
	void startScheduling();

	/*!
	 * @brief Add a task into the scheduler
	 * @details This function create a new task in the scheduler linked to the function task_ptr with a period a_period and an ID a_task_id
	 *
	 * @param [in] task_ptr Pointer to the task which will be added
	 * @param [in] a_period Period of the new task
	 * @return Nothing
	 */
	void addPeriodicTask(TaskPtr_t task_ptr, uint16_t a_period);

	/*!
	 * @brief Remove a task from the scheduler
	 * @details This function finds the task defined by task_ptr in the scheduler and removes it.
	 * @param [in] task_ptr address of the task to remove from scheduler
	 * @return TRUE if the task has been removed, FALSE if the task does not exist in the scheduler
	 */
	bool removePeriodicTask(TaskPtr_t task_ptr);

	/*!
	 * @brief Get function for PIT number
	 * @details This function returns the PIT number
	 * @return PIT number
	 */
	uint32_t getPitNumber();

	/*!
	 * @brief Linked list comparison function
	 * @details This function is called by the linked list class to compare one element of the list to a given element.
	 * 			In the class scheduler, the LLElement is a task pointer (containing a function pointer and a period), and the compareElement a function pointer.
	 * 			The comparison will be done between the two function pointer.
	 *
	 * @param [in] LLElement Pointer to the linked list element
	 * @param [in] CompareElement Pointer to the element to the compare
	 * @return True if both elements are identical, false otherwise
	 */
	static bool LLElementCompare(void* LLElement, void* CompareElement);

private:



	/*!
	 * @brief Type defining a task structure
	 * @details This structure defines a task.
	 * 			A task is defined by a function to call (defined by its pointer) and a period.
	 */
	typedef struct Task_t
	{
		TaskPtr_t TaskPtr; /*!< Pointer to the task */
		uint16_t period; /*!< Period of the task */
	}
	Task_t;

	LinkedList* TasksLL_ptr; /*!< Pointer to the linked list object containing the tasks */

	uint32_t pit_number; /*!< Counter of periodic interrupts */

};


extern scheduler* p_scheduler;

#endif /* WORK_SCHEDULER_SCHEDULER_H_ */
