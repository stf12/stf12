/**
 * ATimer.cpp
 *
 * @date Jun 16, 2011
 * @author Stefano Oliveri (STF12.net)
 *         E-Mail: software@stf12.net
 */

#include "ATimer.h"
#include <assert.h>

#if ( configUSE_TIMERS == 1 )

ATimer::ATimer() {
	m_handleTimer = NULL;

}

ATimer::ATimer(TimerHandle_t handleTimer) {
	Attach(handleTimer);
}

ATimer::~ATimer() {
	if ( IsValid() )
		Delete(portMAX_DELAY);
}

void ATimer::Callback(TimerHandle_t xTimer) {

	static_cast<ATimer *>(pvTimerGetTimerID(xTimer))->OnExpired();
}

ATimer &ATimer::Create(const char *pcTimerName, TickType_t xTimerPeriod, UBaseType_t uxAutoReload){//, void * pvTimerID, tmrTIMER_CALLBACK pxCallbackFunction) {
	assert(!IsValid());

	TimerHandle_t handle;
	handle = xTimerCreate(pcTimerName, xTimerPeriod, uxAutoReload, this, ATimer::Callback);
	if (handle != NULL)
		Attach(handle);

	return *this;
}

void ATimer::Attach(GenericHandle_t handle) {
	assert(handle != NULL);

	if (IsValid()) {
		Delete(portMAX_DELAY);
	}
	m_handleTimer = handle;
}

BaseType_t ATimer::Delete(TickType_t xBlockTime) {
	BaseType_t res = pdTRUE;
	if (m_handleTimer != NULL) {
		res = xTimerDelete(m_handleTimer, xBlockTime);
		if (res != pdTRUE)
			m_handleTimer = NULL;
	}

	return res;
}

#endif // configUSE_TIMERS
