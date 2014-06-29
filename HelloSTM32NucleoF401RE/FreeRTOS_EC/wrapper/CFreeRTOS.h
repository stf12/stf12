/**
 * @file CFreeRTOS.h
 * @class CFreeRTOS
 *
 * This class provide an interface for the <a href="http://www.freertos.org/a00020.html">Kernek Control FreeRTOS API</a>.
 * Moreover provide a method used by the FreeRTOS Extension Layer to initialize the hardware resources of a managed task.
 * All methods are static and inline.
 *
 * @date Jul 14, 2010
 * @author Stefano Oliveri (STF12.net)
 *         E-Mail: software@stf12.net
 */

/**
 * @defgroup FreeRTOS_CPP_EXT FreeRTOS Class Extension
 * This set of C++ class aim to provide an Object Oriented framework to program a FreeRTOS application.
 * @{
 */

/**
 * @defgroup FreeRTOS_Wrapper FreeRTOS Wrapper Class
 * @ingroup FreeRTOS_CPP_EXT
 * The FreeRTOS Wrapper Layer provides a set of class that are able to attach to and manage a FreeRTOS
 * native handle.
 * @{
 */

#ifndef CFREERTOS_H_
#define CFREERTOS_H_

#include "FreeRTOS.h"
#include "task.h"

namespace freertosec {
namespace wrapper {

class CFreeRTOS {
public:
	/**
	 * Default constructor. Not used.
	 */
	CFreeRTOS();

	/**
	 * Destructor. Not used.
	 */
	~CFreeRTOS();

	/**
	 * \sa <a href="http://www.freertos.org/a00132.html">vTaskStartScheduler</a> FreeRTOS API function.
	 */
	static void StartScheduler() { vTaskStartScheduler(); }

	/**
	 * \sa <a href="http://www.freertos.org/a00133.html">vTaskEndScheduler</a> FreeRTOS API function.
	 */
	static void EndScheduler() { vTaskEndScheduler(); }

	/**
	 * \sa <a href="http://www.freertos.org/a00134.html">vTaskSuspendAll</a> FreeRTOS API function.
	 */
	static void SuspendAllTasks() { vTaskSuspendAll(); }

	/**
	 * \sa <a href="http://www.freertos.org/a00135.html">xTaskResumeAll</a> FreeRTOS API function.
	 */
	static BaseType_t ResumeAllTasks() { return xTaskResumeAll(); }

	/**
	 * \sa <a href="http://www.freertos.org/vTaskStepTick.html">vTaskStepTick</a> FreeRTOS API function.
	 */
	static void StepTick(TickType_t xTicksToJump) {
#if ( configUSE_TICKLESS_IDLE == 1 )
		vTaskStepTick(xTicksToJump);
#else
		(void)xTicksToJump;
#endif
	}
};

} /* namespace wrapper */
} /* namespace freertosec */

#endif /* CFREERTOS_H_ */

/**
 * @}
 */

/**
 * @}
 */
