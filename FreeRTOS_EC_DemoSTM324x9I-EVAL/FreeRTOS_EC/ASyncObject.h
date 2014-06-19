/**
 * @file   ASyncObject.h
 * @class  ASyncObject
 * @ingroup FreeRTOS_Wrapper
 *
 * Abstract base class for all task synchronization object.
 *
 * @date   Jul 13, 2010
 * @author Stefano Oliveri (STF12.net)
 *         E-Mail: software@stf12.net
 */

#ifndef ASEMAPHORE_H_
#define ASEMAPHORE_H_

#include "FreeRTOS.h"
#include "semphr.h"
#include "IFreeRTOSProtocol.h"
#include "IQueueSetMember.h"

namespace freertosec {
namespace wrapper {

class ASyncObject: public IFreeRTOSObj, IQueueSetMember {
protected:
    SemaphoreHandle_t m_handleSemaphore;

public:
	ASyncObject();
	virtual ~ASyncObject();

	inline operator SemaphoreHandle_t() const { return m_handleSemaphore; }

	void Delete();
	inline bool IsValid()const;
	void Attach(GenericHandle_t handle);
	inline GenericHandle_t Detach();

    /**
     * \sa <a href="http://www.freertos.org/xSemaphoreGetMutexHolder.html">xSemaphoreGetMutexHolder</a>  FreeRTOS API function.
     * \since FreeRTOS_EC v2.0.0
     */
	inline void* GetMutexHolder();

	virtual BaseType_t Take(TickType_t xBlockTime) =0;
	virtual BaseType_t Give() =0;

	inline BaseType_t Read(TickType_t xTicksToWait, void * const pvBuffer);
};

// inline method

inline
bool ASyncObject::IsValid() const {
	return m_handleSemaphore != NULL;
}

inline
GenericHandle_t ASyncObject::Detach() {
	SemaphoreHandle_t res = m_handleSemaphore;
	m_handleSemaphore = NULL;
	return res;
}

inline
void* ASyncObject::GetMutexHolder() {
#if ( configUSE_MUTEXES == 1 ) && ( INCLUDE_xSemaphoreGetMutexHolder == 1 )
	return xSemaphoreGetMutexHolder(m_handleSemaphore);
#else
	return NULL;
#endif
}

inline
BaseType_t ASyncObject::Read(TickType_t xTicksToWait, void * const pvBuffer) {
	return Take(xTicksToWait);
}

} /* namespace wrapper */
} /* namespace freertosec */

#endif /* ASEMAPHORE_H_ */

