/**
 * AManagedApplicationDelegate.cpp
 *
 *  Created on: Jun 24, 2014
 *      Author: Stefano Oliveri
 */

#include <FreeRTOS_EC.h>

namespace freertosec {
namespace managed {

AManagedApplicationDelegate::~AManagedApplicationDelegate() {

}

AManagedApplicationDelegate::AManagedApplicationDelegate() {

}

#if ( configUSE_IDLE_HOOK == 1 )
	void AManagedApplicationDelegate::OnIdle() {

	}
#endif

#if ( configUSE_TICK_HOOK == 1 )
	void AManagedApplicationDelegate::OnTick() {

	}
#endif

#if ( configUSE_MALLOC_FAILED_HOOK == 1 )
	void AManagedApplicationDelegate::OnMallocFailed() {

	}
#endif

#if ( configCHECK_FOR_STACK_OVERFLOW > 0 )
	void AManagedApplicationDelegate::OnStackOverflow(TaskHandle_t xTask, char *pcTaskName ) {
		(void)xTask;
		(void)pcTaskName;
	}
#endif

} /* namespace managed */
} /* namespace freertosec */
