/*
 * IManagedApplicationDelegate.h
 *
 *  Created on: Jun 23, 2014
 *      Author: oliveris
 */

#ifndef IMANAGEDAPPLICATIONDELEGATE_H_
#define IMANAGEDAPPLICATIONDELEGATE_H_

#include "FreeRTOS.h"

namespace freertosec {
namespace managed {

class IManagedApplicationDelegate {
public:
	virtual ~IManagedApplicationDelegate() {}

#if ( configUSE_IDLE_HOOK == 1 )
	/**
	 * This method will only be called if configUSE_IDLE_HOOK is set
	 * to 1 in FreeRTOSConfig.h.  It will be called on each iteration of the idle
	 * task.  It is essential that code added to this hook function never attempts
	 * to block in any way (for example, call xQueueReceive() with a block time
	 * specified, or call vTaskDelay()).  If the application makes use of the
	 * vTaskDelete() API function, then it is also
	 * important that it is permitted to return to its calling
	 * function, because it is the responsibility of the idle task to clean up
	 * memory allocated by the kernel to any task that has since been deleted.
	 */
	virtual void OnIdle() =0;
#endif

#if ( configUSE_TICK_HOOK == 1 )
	virtual void OnTick() =0;
#endif

#if ( configUSE_MALLOC_FAILED_HOOK == 1 )
   /**
	* This method will only be called if configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h.
	* It is a callback function that will get called if a call to pvPortMalloc() fails.
	* pvPortMalloc() is called internally by the kernel whenever a task, queue,
	* timer or semaphore is created.  It is also called by various parts of the
	* demo application.  If heap_1.c or heap_2.c are used, then the size of the
	* heap available to pvPortMalloc() is defined by configTOTAL_HEAP_SIZE in
	* FreeRTOSConfig.h, and the xPortGetFreeHeapSize() API function can be used
	* to query the size of free heap space that remains (although it does not
	* provide information on how the remaining heap might be fragmented).
	*/
	virtual void OnMallocFailed() =0;
#endif

#if ( configCHECK_FOR_STACK_OVERFLOW > 0 )
	/**
	 * Run time stack overflow checking is performed if
	 * configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This callback
	 * function is called if a stack overflow is detected.
	 *
	 * @param pxTask specifies the handle of the task that generated the overflow
	 * @param pcTaskName specifies the name of the task.
	 */
	virtual void OnStackOverflow(TaskHandle_t xTask, char *pcTaskName) =0;
#endif

};

} /* namespace managed */
} /* namespace freertosec */

#endif /* IMANAGEDAPPLICATIONDELEGATE_H_ */
