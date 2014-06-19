/**
 * @file ATimer.h
 * @class ATimer
 * @ingroup FreeRTOS_Wrapper
 *
 * The ATimer class wraps a native FreeRTOS timer handle (TimerHandle_t). The application must subclass this class and
 * implements the pure virtual method ATimer::OnExpired. The ATimer::OnExpired method is called when the timer period expires.
 * This class conforms to the iFreeRTOSProtocol protocol, so the application has to call the object ATimer::Create
 * method in order to allocate the underling FreeRTOS resource before using it.
 *
 * \sa <a href="http://www.freertos.org/RTOS-software-timer.html">Software Timer documentation</a> in the FreeRTOS web site.
 *
 * @date Jun 16, 2011
 * @author Stefano Oliveri (STF12.net)
 *         E-Mail: software@stf12.net
 */

#ifndef CTIMER_H_
#define CTIMER_H_

#include "FreeRTOS.h"
#include "timers.h"
#include "task.h"
#include "IFreeRTOSProtocol.h"

namespace freertosec {
namespace wrapper {

class ATimer: public IFreeRTOSObj {

	/**
	 * Specifies the native FreeRTOS handle managed by an instance of this class.
	 */
	TimerHandle_t m_handleTimer;

	/**
	 * Default timer callback. It gets the timer identifier, that is a pointer to the
	 * ATimer application derived object, and call the OnExpired method on that object.
	 *
	 * @param xTimer handle to the expired timer.
	 */
	static void Callback(TimerHandle_t xTimer);

public:

	/**
	 * The default constructor. It initializes the object without attaching it to a native FreeRTOS handle.
	 * Call the Create method to allocate a timer handle and attach it to the object.
	 */
	ATimer();

	/**
	 * Create a ATimer instance and attach it to a valid handle.
	 *
	 * @param handleTimer a valid timer handle.
	 */
	ATimer(TimerHandle_t handleTimer);

	/**
	 * Delete the native FreeRTOS timer.
	 */
	virtual ~ATimer();

	/**
	 * Retrieve the native FreeRTOS timer handle attached to this object.
	 *
	 * @return the native FreeRTOS timer handle attached to this object.
	 */
	inline operator TimerHandle_t() const { return m_handleTimer; }

	/**
	 * Allocate the FreeRTOS timer object. The timer identifier of the wrapped xTimerCreate function
	 * is used to pass a pointer to this object.
	 *
	 * \sa <a href="http://www.freertos.org/FreeRTOS-timers-xTimerCreate.html">xTimerCreate</a> FreeRTOS API function.
	 */
	ATimer &Create(const char *pcTimerName, TickType_t xTimerPeriod, UBaseType_t uxAutoReload);

	/**
	 * Application defined callback called when the timer expire.
	 */
	virtual void OnExpired() =0;

	/**
	 * \sa <a href="http://www.freertos.org/FreeRTOS-timers-xTimerIsTimerActive.html">xTimerIsTimerActive</a>  FreeRTOS API function.
	 */
	inline BaseType_t IsTimerActive() const;

	/**
	 * \sa <a href="http://www.freertos.org/FreeRTOS-timers-xTimerStart.html">xTimerStart</a> FreeRTOS API function.
	 */
	inline BaseType_t Start(TickType_t xBlockTime);

	/**
	 * \sa <a href="http://www.freertos.org/FreeRTOS-timers-xTimerStop.html">xTimerStop</a> FreeRTOS API function.
	 */
	inline BaseType_t Stop(TickType_t xBlockTime);

	/**
	 * \sa <a href="http://www.freertos.org/FreeRTOS-timers-xTimerChangePeriod.html">xTimerChangePeriod</a> FreeRTOS API function.
	 */
	inline BaseType_t ChangePeriod(TickType_t xNewPeriod, TickType_t xBlockTime);

	/**
	 * TODO: STF - da rivedere.
	 * \sa <a href="http://www.freertos.org/FreeRTOS-timers-xTimerDelete.html">xTimerDelete</a>  FreeRTOS API function.
	 */
	BaseType_t Delete(TickType_t xBlockTime);

	/**
	 * \sa <a href="http://www.freertos.org/FreeRTOS-timers-xTimerReset.html">xTimerReset</a> FreeRTOS API function.
	 */
	inline BaseType_t Reset(TickType_t xBlockTime);

	/**
	 * \sa <a href="http://www.freertos.org/FreeRTOS-timers-xTimerStartFromISR.html">xTimerStartFromISR</a> FreeRTOS API function.
	 */
	inline BaseType_t StartFromISR(BaseType_t *pxHigherPriorityTaskWoken);

	/**
	 * \sa <a href="http://www.freertos.org/FreeRTOS-timers-xTimerStopFromISR.html">xTimerStopFromISR</a> FreeRTOS API function.
	 */
	inline BaseType_t StopFromISR(BaseType_t *pxHigherPriorityTaskWoken);

	/**
	 * \sa <a href="http://www.freertos.org/FreeRTOS-timers-xTimerChangePeriodFromISR.html">xTimerChangePeriodFromISR</a> FreeRTOS API function.
	 */
	inline BaseType_t ChangePeriodFromISR(TickType_t xNewPeriod, BaseType_t *pxHigherPriorityTaskWoken);

	/**
	 * \sa <a href="http://www.freertos.org/FreeRTOS-timers-xTimerResetFromISR.html">xTimerResetFromISR</a> FreeRTOS API function.
	 */
	inline BaseType_t ResetFromISR(BaseType_t *pxHigherPriorityTaskWoken);

	/**
	 * \sa <a href="http://www.freertos.org/FreeRTOS-timers-pvTimerGetTimerID.html">pvTimerGetTimerID</a> FreeRTOS API function.
	 */
	inline void *GetTimerID() const;

    /**
     * \sa <a href="http://www.freertos.org/FreeRTOS-Software-Timer-API-Functions.html#xTimerGetTimerDaemonTaskHandle">xTimerGetTimerDaemonTaskHandle</a>  FreeRTOS API function.
     * \since FreeRTOS_EC v2.0.0
     */
	inline TaskHandle_t GetTimerDaemonTaskHandle();

    /**
     * \sa <a href="http://www.freertos.org/xTimerPendFunctionCall.html">xTimerPendFunctionCall</a>  FreeRTOS API function.
     * \since FreeRTOS_EC v2.0.0
     */
	inline static BaseType_t PendFunctionCall(
			PendedFunction_t xFunctionToPend,
			void *pvParameter1,
			uint32_t ulParameter2,
			TickType_t xTicksToWait);

    /**
     * \sa <a href="http://www.freertos.org/xTimerPendFunctionCallFromISR.html">xTimerPendFunctionCallFromISR</a>  FreeRTOS API function.
     * \since FreeRTOS_EC v2.0.0
     */
	inline static BaseType_t PendFunctionCallFromISR(
			PendedFunction_t xFunctionToPend,
			void *pvParameter1,
			uint32_t ulParameter2,
			BaseType_t *pxHigherPriorityTaskWoken);

	// FreeRTOS class extension.
	bool IsValid() const;
	void Attach(GenericHandle_t handle);
	GenericHandle_t Detach();
};

// inline method
////////////////

#if ( configUSE_TIMERS == 1 )

inline
BaseType_t ATimer::IsTimerActive() const {
	return xTimerIsTimerActive(m_handleTimer);
}

inline
BaseType_t ATimer::Start(TickType_t xBlockTime) {
	return xTimerStart(m_handleTimer, xBlockTime);
}

inline
BaseType_t ATimer::Stop(TickType_t xBlockTime) {
	return xTimerStop(m_handleTimer, xBlockTime);
}

inline
BaseType_t ATimer::ChangePeriod(TickType_t xNewPeriod, TickType_t xBlockTime) {
	return xTimerChangePeriod(m_handleTimer, xNewPeriod, xBlockTime);
}

inline
BaseType_t ATimer::Reset(TickType_t xBlockTime) {
	return xTimerReset(m_handleTimer, xBlockTime);
}

inline
BaseType_t ATimer::StartFromISR(BaseType_t *pxHigherPriorityTaskWoken) {
	return xTimerStartFromISR(m_handleTimer, pxHigherPriorityTaskWoken);
}

inline
BaseType_t ATimer::ChangePeriodFromISR(TickType_t xNewPeriod, BaseType_t *pxHigherPriorityTaskWoken) {
	return xTimerChangePeriodFromISR(m_handleTimer, xNewPeriod, pxHigherPriorityTaskWoken);
}

inline
BaseType_t ATimer::ResetFromISR(BaseType_t *pxHigherPriorityTaskWoken) {
	return xTimerResetFromISR(m_handleTimer, pxHigherPriorityTaskWoken);
}

inline
void *ATimer::GetTimerID() const {
	return pvTimerGetTimerID(m_handleTimer);
}

inline
bool ATimer::IsValid() const{
	return m_handleTimer != NULL;
}

inline
GenericHandle_t ATimer::Detach() {
	TimerHandle_t res = m_handleTimer;
	m_handleTimer = NULL;
	return res;
}

inline
TaskHandle_t ATimer::GetTimerDaemonTaskHandle() {
#if (INCLUDE_xTimerGetTimerDaemonTaskHandle ==1)
	return xTimerGetTimerDaemonTaskHandle();
#else
	return NULL;
#endif
}

inline
BaseType_t ATimer::PendFunctionCall(PendedFunction_t xFunctionToPend, void *pvParameter1, uint32_t ulParameter2, TickType_t xTicksToWait) {
#if (INCLUDE_xTimerPendFunctionCall == 1)
	return xTimerPendFunctionCall(xFunctionToPend, pvParameter1, ulParameter2, xTicksToWait);
#else
	return pdFALSE;
#endif
}

inline
BaseType_t ATimer::PendFunctionCallFromISR(PendedFunction_t xFunctionToPend, void *pvParameter1, uint32_t ulParameter2,	BaseType_t *pxHigherPriorityTaskWoken) {
#if (INCLUDE_xTimerPendFunctionCall == 1)
	return xTimerPendFunctionCallFromISR(xFunctionToPend, pvParameter1, ulParameter2, pxHigherPriorityTaskWoken);
#else
	return pdFALSE;
#endif
}

#endif // configUSE_TIMERS

} /* namespace wrapper */
} /* namespace freertosec */


#endif /* CTIMER_H_ */
