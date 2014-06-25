/**
 * @file CBinarySemaphore.h
 * @class CBinarySemaphore
 * @ingroup FreeRTOS_Wrapper
 *
 * The CBinarySemaphore class wraps a native FreeRTOS binary semaphore handle (SemaphoreHandle_t).
 * To create a binary semaphore instance an object of this class and then call its CBinarySemaphore::Create method
 * like showed in the following sample:
 * \code
 * CBinarySemaphore g_aSemaphore // global semaphore declaration.
 *
 * void taskControlFunc(void *pParams) // a task control loop
 * {
 * 	g_aSemaphore.Create();
 *
 * 	for (;;) {
 * 		while (g_aSemaphore.Take(portMAX_DELAY) != pdTRUE) {
 * 		// do something
 * 		}
 * }
 * \endcode
 *
 * \sa <a href="http://www.freertos.org/Inter-Task-Communication.html#Binary">binary semaphore documentation</a> in the FreeRTOS web site.

 *
 * @date   Jul 13, 2010
 * @author Stefano Oliveri (STF12.net)
 *         E-Mail: software@stf12.net
 */


#ifndef CBINARYSEMAPHORE_H_
#define CBINARYSEMAPHORE_H_

#include "FreeRTOS.h"
#include "semphr.h"
//#include "ASyncObject.h"
#include <assert.h>

namespace freertosec {
namespace wrapper {

class CBinarySemaphore: public ASyncObject {
public:

	/**
	 * Default constructor. It initializes the object without attaching it to a native FreeRTOS handle.
	 * Call the Create method to allocate a binary semaphore handle and attach it to the object.
	 */
	CBinarySemaphore();

	/**
	 * Create a binary semaphore instance and attach it to a valid handle.
	 *
	 * @param handleSemaphore a valid binary semaphore handle.
	 */
	CBinarySemaphore(SemaphoreHandle_t handleSemaphore);

	/**
	 * Destructor. It deletes the native FreeRTOS counting semaphore.
	 */
	virtual ~CBinarySemaphore();

	/**
	 * Create a FreeRTOS binary semaphore and attach its handle to the receiver object.
	 *
	 * @return true if the counting semaphore is created with success, false otherwise.
	 *
	 * \sa <a href="http://www.freertos.org/a00121.html">vSemaphoreCreateBinary</a> FreeRTOS API function.
	 */
	bool Create();

	/**
	 * \sa <a href="http://www.freertos.org/a00122.html">xSemaphoreTake</a> FreeRTOS API function.
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
BaseType_t CBinarySemaphore::Take(TickType_t xBlockTime) {
	assert(IsValid());

	return xSemaphoreTake(m_handleSemaphore, xBlockTime);
}

inline
BaseType_t CBinarySemaphore::Give() {
	assert(IsValid());

	return xSemaphoreGive(m_handleSemaphore);
}

inline
BaseType_t CBinarySemaphore::GiveFromISR(BaseType_t *pxHigherPriorityTaskWoken) {
	assert(IsValid());

	return xSemaphoreGiveFromISR(m_handleSemaphore, pxHigherPriorityTaskWoken);
}

inline
BaseType_t CBinarySemaphore::TakeFromISR(BaseType_t *pxHigherPriorityTaskWoken) {
	assert(IsValid());

	return xSemaphoreTakeFromISR(m_handleSemaphore, pxHigherPriorityTaskWoken);
}

} /* namespace wrapper */
} /* namespace freertosec */


#endif /* CBINARYSEMAPHORE_H_ */

