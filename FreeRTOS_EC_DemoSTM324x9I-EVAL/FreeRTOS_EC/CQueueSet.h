/**
 * @file CQueueSet.h
 * @class CQueueSet
 * @ingroup FreeRTOS_Wrapper
 *
 * The CQueueSet class wraps a native FreeRTOS queue-set handle (QueueSetHandle_t).
 * To create a queue instance an object of this class and then call its CQueueSet::CreateSet method
 * like showed in the following sample:
 * \code
 * TODO: STF - add a code sample...
 * \endcode
 *
 *
 * \sa <a href="http://www.freertos.org/Pend-on-multiple-rtos-objects.html">queue set documentation</a> in the FreeRTOS web site.
 *
 * @date June 03, 2014
 * @author Stefano Oliveri (STF12.net)
 *         E-Mail: software@stf12.net
 */

#ifndef CQUEUESET_H_
#define CQUEUESET_H_

#include <assert.h>

#include "FreeRTOS.h"
#include "queue.h"
#include "IFreeRTOSProtocol.h"
#include "IQueueSetMember.h"

class CQueueSet: public IFreeRTOSObj {
	/**
	 * Specifies the native FreeRTOS handle managed by an instance of this class.
	 */
	QueueSetHandle_t m_handleQueueSet;

public:

	/**
	 * The default constructor. It initializes the object without attaching it to a native FreeRTOS handle.
	 * Call the Create method to allocate a queue handle and attach it to the object.
	 */
	CQueueSet();

	/**
	 * Create a CQueue instance and attach it to a valid handle.
	 *
	 * @param handleQueueSet a valid queue set handle.
	 */
	CQueueSet(QueueSetHandle_t handleQueueSet);

	/**
	 * Delete the native FreeRTOS queue.
	 */
	virtual ~CQueueSet();

	/**
	 * Retrieve the native FreeRTOS queue handle attached to this object.
	 *
	 * @return the native FreeRTOS queue handle attached to this object.
	 */
	inline operator QueueSetHandle_t() const { return m_handleQueueSet; }

	inline bool IsValid() const;
	void Attach(GenericHandle_t handle);
	inline GenericHandle_t Detach();

	// FreeRTOS API Wrappers

    /**
     * \sa <a href="http://www.freertos.org/xQueueCreateSet.html">xQueueCreateSet</a>  FreeRTOS API function.
     * \since FreeRTOS_EC v2.0.0
     */
	CQueueSet &CreateSet(const UBaseType_t uxEventQueueLength);

    /**
     * \sa <a href="http://www.freertos.org/xQueueAddToSet.html">xQueueAddToSet</a>  FreeRTOS API function.
     * \since FreeRTOS_EC v2.0.0
     */
	inline BaseType_t AddToSet(IQueueSetMember &xQueueOrSemaphore);

    /**
     * \sa <a href="http://www.freertos.org/xQueueRemoveFromSet.html">xQueueRemoveFromSet</a>  FreeRTOS API function.
     * \since FreeRTOS_EC v2.0.0
     */
	inline BaseType_t RemoveFromSet(IQueueSetMember &xQueueOrSemaphore);

    /**
     * \sa <a href="http://www.freertos.org/xQueueSelectFromSet.html">xQueueSelectFromSet</a>  FreeRTOS API function.
     * \since FreeRTOS_EC v2.0.0
     */
	inline QueueSetMemberHandle_t SelectFromSet(const TickType_t xTicksToWait);

    /**
     * \sa <a href="http://www.freertos.org/xQueueSelectFromSetFromISR.html">xQueueSelectFromSetFromISR</a>  FreeRTOS API function.
     * \since FreeRTOS_EC v2.0.0
     */
	inline QueueSetMemberHandle_t SelectFromSetFromISR();
};

// inline method implementation

inline
bool CQueueSet::IsValid() const{
	return m_handleQueueSet != NULL;
}

inline
GenericHandle_t CQueueSet::Detach() {
	QueueSetHandle_t res = m_handleQueueSet;
	m_handleQueueSet = NULL;
	return res;
}

inline
BaseType_t CQueueSet::AddToSet(IQueueSetMember &xQueueOrSemaphore) {
#if (configUSE_QUEUE_SETS == 1)
	return xQueueAddToSet(xQueueOrSemaphore, m_handleQueueSet);
#else
	return pdFAIL;
#endif
}

inline
BaseType_t CQueueSet::RemoveFromSet(IQueueSetMember &xQueueOrSemaphore) {
#if (configUSE_QUEUE_SETS == 1)
	return xQueueRemoveFromSet(xQueueOrSemaphore, m_handleQueueSet);
#else
	return pdFAIL;
#endif
}

inline
QueueSetMemberHandle_t CQueueSet::SelectFromSet(const TickType_t xTicksToWait) {
#if (configUSE_QUEUE_SETS == 1)
	return xQueueSelectFromSet(m_handleQueueSet, xTicksToWait);
#else
	return NULL;
#endif
}

inline
QueueSetMemberHandle_t CQueueSet::SelectFromSetFromISR() {
#if (configUSE_QUEUE_SETS == 1)
	return xQueueSelectFromSetFromISR(m_handleQueueSet);
#else
	return NULL;
#endif
}

#endif /* CQUEUESET_H_ */
