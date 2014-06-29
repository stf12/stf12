/*
 * AManagedApplicationDelegate.h
 *
 *  Created on: Jun 24, 2014
 *      Author: oliveris
 */

#ifndef AMANAGEDAPPLICATIONDELEGATE_H_
#define AMANAGEDAPPLICATIONDELEGATE_H_

#include "FreeRTOS.h"

namespace freertosec {
namespace managed {

class IManagedApplicationDelegate;

class AManagedApplicationDelegate: public IManagedApplicationDelegate {
public:
	virtual ~AManagedApplicationDelegate();
	AManagedApplicationDelegate();

#if ( configUSE_IDLE_HOOK == 1 )
	void OnIdle();
#endif

#if ( configUSE_TICK_HOOK == 1 )
	void OnTick();
#endif

#if ( configUSE_MALLOC_FAILED_HOOK == 1 )
	void OnMallocFailed();
#endif

#if ( configCHECK_FOR_STACK_OVERFLOW > 0 )
	virtual void OnStackOverflow(TaskHandle_t xTask, char *pcTaskName );
#endif
};

} /* namespace managed */
} /* namespace freertosec */

#endif /* AMANAGEDAPPLICATIONDELEGATE_H_ */
