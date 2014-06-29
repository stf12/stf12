/**
 * @file CQueue.h
 * @class CQueue
 * @ingroup FreeRTOS_Wrapper
 *
 * The CQueue class wraps a native FreeRTOS queue handle (QueueHandle_t).
 * To create a queue instance an object of this class and then call its CQueue::Create method
 * like showed in the following sample:
 * \code
 * CQueue g_aQueue // global task declaration.
 *
 * void taskControlFunc(void *pParams)
 * {
 * 	unsigned char *Message;
 * 	g_aQueue.Create(nQueueSize, sizeof(queueItemT));
 *
 * 	for (;;) {
 * 		g_aQueue.Receive(&Message, portMAX_DELAY ) != pdTRUE)
 * 		// do something
 * }
 * \endcode
 *
 * See the CLcdTask for an example.
 *
 * \sa <a href="http://www.freertos.org/Inter-Task-Communication.html">queue documentation</a> in the FreeRTOS web site.
 *
 * @date Jul 12, 2010
 * @author Stefano Oliveri (STF12.net)
 *         E-Mail: software@stf12.net
 */


#ifndef CQUEUE_H_
#define CQUEUE_H_

#include "FreeRTOS.h"
#include "queue.h"
//#include "IFreeRTOSProtocol.h"
//#include "IQueueSetMember.h"

#include <assert.h>

namespace freertosec {
namespace wrapper {

class CQueue: public IFreeRTOSObj, public IQueueSetMember {
	/**
	 * Specifies the native FreeRTOS handle managed by an instance of this class.
	 */
	QueueHandle_t m_handleQueue;

public:
	/**
	 * The default constructor. It initializes the object without attaching it to a native FreeRTOS handle.
	 * Call the Create method to allocate a queue handle and attach it to the object.
	 */
	CQueue();

	/**
	 * Create a CQueue instance and attach it to a valid handle.
	 *
	 * @param handleQueue a valid queue handle.
	 */
	CQueue(QueueHandle_t handleQueue);

	/**
	 * Delete the native FreeRTOS queue.
	 */
	virtual ~CQueue();

	/**
	 * Retrieve the native FreeRTOS queue handle attached to this object.
	 *
	 * @return the native FreeRTOS queue handle attached to this object.
	 */
	inline operator QueueHandle_t() const { return m_handleQueue; }

	inline bool IsValid() const;
	void Attach(GenericHandle_t handle);
	inline GenericHandle_t Detach();

	// FreeRTOS API Wrappers

	/**
	 * \sa <a href="http://www.freertos.org/a00116.html">xQueueCreate</a> FreeRTOS API function.
	 */
	CQueue &Create(UBaseType_t uxQueueLength, UBaseType_t uxItemSize);

	/**
	 * \sa <a href="http://www.freertos.org/a00018.html#vQueueDelete">vQueueDelete</a> FreeRTOS API function.
	 */
	void Delete();

	/**
	 * \sa <a href="http://www.freertos.org/a00018.html#ucQueueMessagesWaiting">uxQueueMessagesWaiting</a> FreeRTOS API function.
	 */
	inline UBaseType_t MessagesWaiting();

	/**
	 * \sa <a href="http://www.freertos.org/a00117.html">xQueueSend</a> FreeRTOS API function.
	 */
	inline BaseType_t Send(const void * const pvItemToQueue, TickType_t xTicksToWait);

	/**
	 * \sa <a href="http://www.freertos.org/xQueueSendToBack.html">SendToBack</a> FreeRTOS API function.
	 */
	inline BaseType_t SendToBack(const void * const pvItemToQueue, TickType_t xTicksToWait);

	/**
	 * \sa <a href="http://www.freertos.org/xQueueSendToFront.html">xQueueSendToFront</a> FreeRTOS API function.
	 */
	inline BaseType_t SendToFront(const void * const pvItemToQueue, TickType_t xTicksToWait);

	/**
	 * \sa <a href="http://www.freertos.org/a00118.html">xQueueReceive</a> FreeRTOS API function.
	 */
	inline BaseType_t Receive(void * const pvBuffer, TickType_t xTicksToWait);

	/**
	 * \sa <a href="http://www.freertos.org/xQueuePeek.html">xQueuePeek</a> FreeRTOS API function.
	 */
	inline BaseType_t Peek(void * const pvBuffer, TickType_t xTicksToWait);

	/**
	 * \sa <a href="http://www.freertos.org/a00119.html">xQueueSendFromISR</a> FreeRTOS API function.
	 */
	inline BaseType_t SendFromISR(const void * const pvItemToQueue, BaseType_t *pxHigherPriorityTaskWoken);

	/**
	 * \sa <a href="http://www.freertos.org/xQueueSendToBackFromISR.html">xQueueSendToBackFromISR</a> FreeRTOS API function.
	 */
	inline BaseType_t SendToBackFromISR(const void * const pvItemToQueue, BaseType_t *pxHigherPriorityTaskWoken);

	/**
	 * \sa <a href="http://www.freertos.org/xQueueSendToFrontFromISR.html">xQueueSendToFrontFromISR</a> FreeRTOS API function.
	 */
	inline BaseType_t SendToFrontFromISR(const void * const pvItemToQueue, BaseType_t *pxHigherPriorityTaskWoken);

	/**
	 * \sa <a href="http://www.freertos.org/a00120.html">xQueueReceiveFromISR</a> FreeRTOS API function.
	 */
	inline BaseType_t ReceiveFromISR(void * const pvBuffer, BaseType_t *pxTaskWoken);

	/**
	 * \sa <a href="http://www.freertos.org/vQueueAddToRegistry.html">vQueueAddToRegistry</a> FreeRTOS API function.
	 */
	inline void AddToRegistry(const char *pcQueueName);

	/* Since FreeRTOS v8 and FreeRTOS_EC v2.0.0*/

    /**
     * \sa <a href="http://www.freertos.org/a00018.html#ucQueueMessagesWaitingFromISR">uxQueueMessagesWaitingFromISR</a>  FreeRTOS API function.
     * \since FreeRTOS_EC v2.0.0 (FreeRTOS vX.X.X todo: to check FreeRTOS version)
     */
	inline UBaseType_t MessagesWaitingFromISR();


    /**
     * \sa <a href="http://www.freertos.org/a00018.html#uxQueueSpacesAvailable">uxQueueSpacesAvailable</a>  FreeRTOS API function.
     * \since FreeRTOS_EC v2.0.0 (FreeRTOS vX.X.X todo: to check FreeRTOS version)
     */
	inline UBaseType_t SpacesAvailable() const;

    /**
     * \sa <a href="http://www.freertos.org/a00018.html#xQueueReset">xQueueReset</a>  FreeRTOS API function.
     * \since FreeRTOS_EC v2.0.0 (FreeRTOS vX.X.X todo: to check FreeRTOS version)
     */
	inline BaseType_t Reset();

    /**
     * \sa <a href="http://www.freertos.org/xQueueOverwrite.html">xQueueOverwrite</a>  FreeRTOS API function.
     * \since FreeRTOS_EC v2.0.0 (FreeRTOS vX.X.X todo: to check FreeRTOS version)
     */
	inline BaseType_t Overwrite(const void * pvItemToQueue);

    /**
     * \sa <a href="http://www.freertos.org/xQueueOverwriteFromISR.html">xQueueOverwriteFromISR</a>  FreeRTOS API function.
     * \since FreeRTOS_EC v2.0.0 (FreeRTOS vX.X.X todo: to check FreeRTOS version)
     */
	inline BaseType_t OverwriteFromISR(const void * pvItemToQueue, BaseType_t *pxHigherPriorityTaskWoken);

    /**
     * \sa <a href="http://www.freertos.org/xQueuePeekFromISR.html">xQueuePeekFromISR</a>  FreeRTOS API function.
     * \since FreeRTOS_EC v2.0.0 (FreeRTOS vX.X.X todo: to check FreeRTOS version)
     */
	inline BaseType_t PeekFromISR(void *pvBuffer);

    /**
     * \sa <a href="http://www.freertos.org/vQueueUnregisterQueue.html">vQueueUnregisterQueue</a>  FreeRTOS API function.
     * \since FreeRTOS_EC v2.0.0 (FreeRTOS vX.X.X todo: to check FreeRTOS version)
     */
	inline void UnregisterQueue();

    /**
     * \sa <a href="http://www.freertos.org/a00018.html#xQueueIsQueueFullFromISR">xQueueIsQueueFullFromISR</a>  FreeRTOS API function.
     * \since FreeRTOS_EC v2.0.0 (FreeRTOS vX.X.X todo: to check FreeRTOS version)
     */
	inline BaseType_t IsQueueFullFromISR() const;

    /**
     * \sa <a href="http://www.freertos.org/a00018.html#xQueueIsQueueEmptyFromISR">xQueueIsQueueEmptyFromISR</a>  FreeRTOS API function.
     * \since FreeRTOS_EC v2.0.0 (FreeRTOS vX.X.X todo: to check FreeRTOS version)
     */
	inline BaseType_t QueueEmptyFromISR() const;

	inline BaseType_t Read(TickType_t xTicksToWait, void * const pvBuffer);

};

// inline method implementation

inline
BaseType_t CQueue::Send(const void * const pvItemToQueue, TickType_t xTicksToWait) {
	return xQueueSend(m_handleQueue, pvItemToQueue, xTicksToWait);
}

inline
BaseType_t CQueue::SendFromISR(const void * const pvItemToQueue, BaseType_t *pxHigherPriorityTaskWoken) {
	return xQueueSendFromISR(m_handleQueue, pvItemToQueue, pxHigherPriorityTaskWoken);
}

inline
UBaseType_t CQueue::MessagesWaiting() {
	assert(IsValid());

	return uxQueueMessagesWaiting(m_handleQueue);
}

inline
BaseType_t CQueue::SendToBack(const void * const pvItemToQueue, TickType_t xTicksToWait) {
	assert(IsValid());

	return xQueueSendToBack(m_handleQueue, pvItemToQueue, xTicksToWait);
}

inline
BaseType_t CQueue::SendToFront(const void * const pvItemToQueue, TickType_t xTicksToWait) {
	assert(IsValid());

	return xQueueSendToFront(m_handleQueue, pvItemToQueue, xTicksToWait);
}

inline
BaseType_t CQueue::Receive(void * const pvBuffer, TickType_t xTicksToWait) {
	assert(IsValid());

	return xQueueReceive(m_handleQueue, pvBuffer, xTicksToWait);
}

inline
BaseType_t CQueue::Peek(void * const pvBuffer, TickType_t xTicksToWait) {
	assert(IsValid());

	return xQueuePeek(m_handleQueue, pvBuffer, xTicksToWait);
}

inline
BaseType_t CQueue::SendToBackFromISR(const void * const pvItemToQueue, BaseType_t *pxHigherPriorityTaskWoken) {
	assert(IsValid());

	return xQueueSendToBackFromISR(m_handleQueue, pvItemToQueue, pxHigherPriorityTaskWoken);
}

inline
BaseType_t CQueue::SendToFrontFromISR(const void * const pvItemToQueue, BaseType_t *pxHigherPriorityTaskWoken) {
	assert(IsValid());

	return xQueueSendToFrontFromISR(m_handleQueue, pvItemToQueue, pxHigherPriorityTaskWoken);
}

inline
BaseType_t CQueue::ReceiveFromISR(void * const pvBuffer, BaseType_t *pxTaskWoken) {
	assert(IsValid());

	return xQueueReceiveFromISR(m_handleQueue, pvBuffer, pxTaskWoken);
}

inline
void CQueue::AddToRegistry(const char *pcQueueName) {
#if (configQUEUE_REGISTRY_SIZE > 0)
	assert(IsValid());

	vQueueAddToRegistry(m_handleQueue, pcQueueName);
#endif
}

inline
bool CQueue::IsValid() const{
	return m_handleQueue != NULL;
}

inline
GenericHandle_t CQueue::Detach() {
	QueueHandle_t res = m_handleQueue;
	m_handleQueue = NULL;
	return res;
}

inline
UBaseType_t CQueue::MessagesWaitingFromISR() {
	return uxQueueMessagesWaitingFromISR(m_handleQueue);
}

inline
UBaseType_t CQueue::SpacesAvailable() const {
	return uxQueueSpacesAvailable(m_handleQueue);
}

inline
BaseType_t CQueue::Reset() {
	return xQueueReset(m_handleQueue);
}

inline
BaseType_t CQueue::Overwrite(const void * pvItemToQueue) {
	return xQueueOverwrite(m_handleQueue, pvItemToQueue);
}

inline
BaseType_t CQueue::OverwriteFromISR(const void * pvItemToQueue, BaseType_t *pxHigherPriorityTaskWoken) {
	return xQueueOverwriteFromISR(m_handleQueue, pvItemToQueue, pxHigherPriorityTaskWoken);
}

inline
BaseType_t CQueue::PeekFromISR(void *pvBuffer) {
	return xQueuePeekFromISR(m_handleQueue, pvBuffer);
}

inline
void CQueue::UnregisterQueue() {
	vQueueUnregisterQueue(m_handleQueue);
}

inline
BaseType_t CQueue::IsQueueFullFromISR() const {
	return xQueueIsQueueFullFromISR(m_handleQueue);
}

inline
BaseType_t CQueue::QueueEmptyFromISR() const {
	return xQueueIsQueueEmptyFromISR(m_handleQueue);
}

inline
BaseType_t CQueue::Read(TickType_t xTicksToWait, void * const pvBuffer) {
	return Receive(pvBuffer, xTicksToWait);
}

} /* namespace wrapper */
} /* namespace freertosec */

#endif /* CQUEUE_H_ */

