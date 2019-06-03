/*!
 * @file CpuLoad.h
 *
 * @brief CpuLoad class header file
 *
 * @date 21 mars 2019
 * @author nicls67
 */

#ifndef WORK_BSW_CPULOAD_CPULOAD_H_
#define WORK_BSW_CPULOAD_CPULOAD_H_

#define NB_OF_SAMPLES 50

/*!
 * @brief Class defining CPU load libraries
 * @details This class defines tools to compute and monitor CPU load.
 */
class CpuLoad
{
public:
	/*!
	 * @brief CpuLoad class constructor
	 * @details This function initializes class CpuLoad. It also creates a new object of Timer class in case it is still not created.
	 * 			Normally the CpuLoad class is used by the scheduler object, which should create the Timer object.
	 * 			Thus the initialization of Timer object in CpuLoad class should not be needed. We still do the check here to avoid any issue with null pointer.
	 *
	 * @return Nothing
	 */
	CpuLoad();

	/*!
	 * @brief Computes current CPU load
	 * @details This function computes the current CPU load using value of the timer used by the scheduler at the end of the periodic cycle.
	 * 			This value is divided by the PIT period to obtain CPU load;
	 *
	 * @return Nothing
	 */
	void ComputeCPULoad();

	/*!
	 * @brief Get current CPU load value
	 * @details This function returns the current CPU load value
	 *
	 * @return Current CPU load value
	 */
	inline uint8_t getCurrrentCPULoad()
	{
		return current_load;
	}

	/*!
	 * @brief Get average CPU load value
	 * @details This function returns the average CPU load value
	 *
	 * @return Average CPU load value
	 */
	inline uint8_t getAverageCPULoad()
	{
		return avg_load;
	}

	/*!
	 * @brief Get maximum CPU load value
	 * @details This function returns the maximum CPU load value
	 *
	 * @return Maximum CPU load value
	 */
	inline uint8_t getMaxCPULoad()
	{
		return max_load;
	}

private:
	uint8_t current_load; /*!< Current CPU load (load of last cycle) */
	uint8_t avg_load; /*!< Average CPU load based on the last 50 cycles */
	uint8_t max_load; /*!< Maximum CPU load since power on */
	uint8_t sample_cnt; /*!< Number of samples used to compute average load */
	uint8_t sample_mem[NB_OF_SAMPLES]; /*!< Memorization of the last NB_OF_SAMPLES measures */
	uint8_t sample_idx; /*!< Current measurement index (used to memorize the current measure at the correct location in table) */
	uint16_t last_sum_value; /*!< Value of the last computed sum (it will reduce the number of samples to sum and speed up execution time) */
};

extern CpuLoad* p_global_BSW_cpuload; /*!< Pointer to cpu load library object */

#endif /* WORK_BSW_CPULOAD_CPULOAD_H_ */
