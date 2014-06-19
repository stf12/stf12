/**
 * @file CCountingSemaphore.h
 * @class CCountingSemaphore
 * @ingroup FreeRTOS_Wrapper
 *
 * The CCountingSemaphore class wraps a native FreeRTOS counting semaphore handle (SemaphoreHandle_t).
 * To create a counting semaphore instance an object of this class and then call its CCountingSemaphore::Create method
 * like showed in the following sample:
 * \code
 * CCountingSemaphore g_aSemaphore // global semaphore declaration.
 *
 * void taskControlFunc(void *pParams) // a task control loop
 * {
 * 	g_aSemaphore.Create(10, 0);
 *
 * 	for (;;) {
 * 		while (g_aSemaphore.Take(portMAX_DELAY) != pdTRUE) {
 * 		// do something
 * 		}
 * }
 * \endcode
 *
 * \sa <a href="http://www.freertos.org/Inter-Task-Communication.html#Counting">counting semaphore documentation</a> in the FreeRTOS web site.
 *
 * @date Jul 13, 2010
 * @date Stefano Oliveri (STF12.net)
 *       E-Mail: software@stf12.net
 */


#ifndef CCOUNTINGSEMAPHORE_H_
#define CCOUNTINGSEMAPHORE_H_

#include <assert.h>

#include "ASyncObject.h"

namespace freertosec {
namespace wrapper {

class CCountingSemaphore: public ASyncObject {
public:

	/**
	 * Default constructor. It initializes the object without attaching it to a native FreeRTOS handle.
	 * Call the Create method to allocate a counting semaphore handle and attach it to the object.
	 */
	CCountingSemaphore();

	/**
	 * Create a counting semaphore instance and attach it to a valid handle.
	 *
	 * @param handleSemaphore a valid counting semaphore handle.
	 */
	CCountingSemaphore(SemaphoreHandle_t handleSemaphore);

	/**
	 * Destructor. It deletes the native FreeRTOS counting semaphore.
	 */
	virtual ~CCountingSemaphore();

	/**
	 * Create a FreeRTOS counting semaphore and attach its handle to the receiver object.
	 *
	 * @return true if the mutex is created with success, false otherwise.
	 *
	 * \sa <a href="http://www.freertos.org/CreateCounting.html">xSemaphoreCreateCounting</a> FreeRTOS API function.
	 */
	bool Create(UBaseType_t uxMaxCount, UBaseType_t uxInitialCount);

	/**
	 *  \sa <a href="http://www.freertos.org/a00122.html">xSemaphoreTake</a> FreeRTOS API function.
	 */
	inline BaseType_t Take(TickType_t xBlockTime);

	/**
	 * \sa <a href="http://www.freertos.org/a00123.html">xSemaphoreGive</a> FreeRTOS API function.
	 */
	inline BaseType_t Give();

	/**
	 * \sa <a href="http://www.freertos.org/a00124.html">xSemaphoreGiveFromISR</a> FreeRTOS API function.
	 */
	inline BaseType_t GiveFromISR(BaseType_t *pxHigherPriorityTaskWoken);

	/**
	 * \sa <a href="http://www.freertos.org/xSemaphoreTakeFromISR.html">xSemaphoreTakeFromISR</a> FreeRTOS API function.
	 */
	inline BaseType_t TakeFromISR(BaseType_t *pxHigherPriorityTaskWoken);
};


// inline methods implementation

inline
BaseType_t CCountingSemaphore::Take(TickType_t xBlockTime) {
	assert(IsValid());

	return xSemaphoreTake(m_handleSemaphore, xBlockTime);
}

inline
BaseType_t CCountingSemaphore::Give() {
	assert(IsValid());

	return xSemaphoreGive(m_handleSemaphore);
}

inline
BaseType_t CCountingSemaphore::GiveFromISR(BaseType_t *pxHigherPriorityTaskWoken) {
	assert(IsValid());

	return xSemaphoreGiveFromISR(m_handleSemaphore, pxHigherPriorityTaskWoken);
}

inline
BaseType_t CCountingSemaphore::TakeFromISR(BaseType_t *pxHigherPriorityTaskWoken) {
	assert(IsValid());

	return xSemaphoreTakeFromISR(m_handleSemaphore, pxHigherPriorityTaskWoken);
}

} /* namespace wrapper */
} /* namespace freertosec */


#endif /* CCOUNTINGSEMAPHORE_H_ */
