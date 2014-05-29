/**
 * @file   CCheckTask.h
 *
 * @date   Aug 3, 2010
 * @author Stefano Oliveri
 */

/**
 * @defgroup FreeRTOS_STANDARD_DEMO Standard Demo Tasks
 * These classes are a porting of the <a href="http://www.freertos.org/a00102.html">standard FreeRTOS demo tasks</a> in the
 * C++ framework. For each FreeRTOS standard demo file there is a .cpp file and defined classes implement the FreeRTOS demo tasks.
 * All classes in this group inherit from AManagedTask to simplify the task implementation and to show how to use the
 * Managed Class Extension. Moreover each demo class implements the ICommonDemoTask interface to be able to exchange information with
 * the CCheckTask class.
 * @{
 */

#ifndef CCHECKTASK_H_
#define CCHECKTASK_H_

#include "AManagedTask.h"

class CCheckTask;

/**
 * Base interface for each Common Demo Class. It declares the methods needed by the CCheckTask
 * to implemet the software watchdog feature.
 */
class ICommonDemoTask {
	friend class CCheckTask;

	/**
	 * Specifies the pointer to the next ICommonDemoTask object.
	 */
	ICommonDemoTask *m_pNext;

public:

	/**
	 * Default constructor.
	 *
	 * @param pCheckTask specifies a pointer to a CCheckTask object the manage this instance.
	 * Usually there is just one instance of CCheckTask in the applicatkion.
	 */
	ICommonDemoTask(CCheckTask *pCheckTask);

	/**
	 * Destructor
	 */
	virtual ~ICommonDemoTask();

	/**
	 * Used by the CCheckTask instance to check if the task is running with no error.
	 *
	 * @return true if the the task is running with no error, false otherwise.
	 */
	virtual bool IsStillRunning()=0;

	/**
	 * If the task is not running properly, return an error message.
	 *
	 * @return an error message.
	 */
	virtual const char*GetErrorMessage()=0;
};

/**
 * Implement the software watchdog features. This task runs at highest priority. It is a periodic
 * task runs with a low frequency and checks if all registered task are still running.
 */
class CCheckTask: public AManagedTask {
	/**
	 * Specifies the head of the linked list for the registered tasks.
	 */
	ICommonDemoTask *m_pListCommonDemoTaskHead;

	/**
	 * Specifies the frequency of the task. The default value is 4s.
	 */
	portTickType m_checkFrequency;

public:

	/**
	 * Default constructor.
	 *
	 * @param checkFrequency specifies the task frequency.
	 */
	CCheckTask(portTickType checkFrequency=(4000/portTICK_RATE_MS));

	/**
	 * Destructor.
	 */
	virtual ~CCheckTask();

	/**
	 * Add a task object to the list of the controlled tasks.
	 * @param pTask specifies a pointer to the task to add.
	 */
	void AddTask(ICommonDemoTask *pTask);

	/**
	 * Task control loop.
	 */
	void Run();
};

#endif /* CCHECKTASK_H_ */

/**
 * @}
 */
