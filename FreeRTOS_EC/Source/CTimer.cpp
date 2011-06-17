/**
 * CTimer.cpp
 *
 * @date Jun 16, 2011
 * @author Stefano Oliveri (STF12.net)
 *         E-Mail: software@stf12.net
 */

#include "CTimer.h"
#include <assert.h>

#if ( configUSE_TIMERS == 1 )

CTimer::CTimer() {
	m_handleTimer = NULL;

}

CTimer::CTimer(xTimerHandle handleTimer) {
	Attach(handleTimer);
}

CTimer::~CTimer() {
	if ( IsValid() )
		Delete(portMAX_DELAY);
}


//void CTimer::Callback(CTimer &timer) {
//
//}

void CTimer::Callback(xTimerHandle xTimer) {

	static_cast<CTimer *>(pvTimerGetTimerID(xTimer))->OnExpired();
}

CTimer &CTimer::Create(const signed char *pcTimerName, portTickType xTimerPeriod, unsigned portBASE_TYPE uxAutoReload){//, void * pvTimerID, tmrTIMER_CALLBACK pxCallbackFunction) {
	assert(!IsValid());

	xTimerHandle handle;

//	handle = xTimerCreate(pcTimerName, xTimerPeriod, uxAutoReload, pvTimerID, pxCallbackFunction);
	handle = xTimerCreate(pcTimerName, xTimerPeriod, uxAutoReload, this, CTimer::Callback);
	if (handle != NULL)
		Attach(handle);

	return *this;
}

void CTimer::Attach(xGenericHandle handle) {
	assert(handle != NULL);

	if (IsValid()) {
		Delete(portMAX_DELAY);
	}
	m_handleTimer = handle;
}

portBASE_TYPE CTimer::Delete(portTickType xBlockTime) {
	portBASE_TYPE res = pdTRUE;
	if (m_handleTimer != NULL) {
		res = xTimerDelete(m_handleTimer, xBlockTime);
		if (res != pdTRUE)
			m_handleTimer = NULL;
	}

	return res;
}

#endif // configUSE_TIMERS
