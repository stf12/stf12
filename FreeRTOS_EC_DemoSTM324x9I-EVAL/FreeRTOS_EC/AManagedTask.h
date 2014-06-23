/**
 * @file AManagedTask.h
 * @class AManagedTask
 *
 * This class extend the CTask in order to provide a more structured and simple to use framework for
 * FreeRTOS programming. All AManagedTask derived objects are grouped in linked list. Please note that
 * this data structure can be modified in a next release. The framework manage these task objects
 * by calling their methods during the application life cycle.
 *
 * The application extend this abstract class to create a managed task and provide its control loop.
 * \code
 * class CMyPipe: public AManagedTask
 * {
 *   // Task private variables.
 *
 * public:
 *   void Run() {  task control loop  };
 * }
 * \endcode
 *
 * @date Aug 3, 2010
 * @author Stefano Oliveri
 *         E-Mail: software@stf12.net
 */

/**
 * @defgroup FreeRTOS_Managed FreeRTOS Managed Class Extension
 * @ingroup FreeRTOS_CPP_EXT
 * The FreeRTOS Managed Class Extension are a set of class that provide a structured and
 * managed framework to develop a FreeRTOS application.
 * @{
 */

#ifndef AMANAGEDTASK_H_
#define AMANAGEDTASK_H_

#include "CFreeRTOS.h"
#include "CTask.h"

using freertosec::wrapper::CTask;

namespace freertosec {
namespace managed {

class CMTContext;

class AManagedTask: public CTask {
	friend class CMTContext;

	/**
	 * Specifies the managed task this context belong to.
	 */
	CMTContext *m_pContext;

	/**
	 * Specifies the next managed task in the list or NULL if this is the last managed task.
	 */
	AManagedTask *m_pNextManagedTask;

public:

	/**
	 * Default constructor.
	 */
	AManagedTask();

	/**
	 * Destructor.
	 */
	virtual ~AManagedTask();

	/**
	 * Create the FreeRTOS task attached to this object.
	 *
	 * @param pcName name of the task. Need only for debug purpose.
	 * @param usStackDepth size in word of the task stack.
	 * @param uxPriority task priority. Valid value ar between taskIDLE_PRIORITY and configMAX_PRIORITIES-1.
	 *
	 * @sa CTask::Create
	 */
	BaseType_t  Create(const char * const pcName, uint16_t usStackDepth, UBaseType_t  uxPriority);

	/**
	 * The framework calls this function before the scheduler start. The default implementation simply return true.
	 * Override this method to perform task specific hardware initialization that does not use FreeRTOS API.
	 *
	 * @return true if success, false otherwise.
	 */
	virtual bool HardwareInit() { return true; }

	/**
	 * Task control loop.
	 */
	virtual void Run() =0;

	/**
	 * The framework calls this function before the task is created to give a chance to a managed task to perform its
	 * own initialization code and stop the task creation process if needed.
	 *
	 * @param pcName specifies the task name.
	 * @param usStackDepth specifies the task stack depth.
	 * @param uxPriority specifies the task priority.
	 *
	 * @return pdTRUE if success, pdFALSE otherwise. If the method return pdFALSE the task creation process
	 * is stopped and no FreeRTOS resource are allocated.
	 */
	virtual BaseType_t  OnCreate(const portCHAR * const pcName, unsigned portSHORT usStackDepth, UBaseType_t  uxPriority) {
		(void)pcName;
		(void)usStackDepth;
		(void) uxPriority;

		return pdTRUE;
	}

	/**
	 * The task control function passed as first parameter to the CTask::Create method.
	 *
	 * @param pParams pointer to this object.
	 */
	static void taskControlFunc(void *pParams);
};

} /* namespace managed */
} /* namespace freertosec */

#endif /* AMANAGEDTASK_H_ */

/**
 * @}
 */

