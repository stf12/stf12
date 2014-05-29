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

class ASyncObject: public IFreeRTOSObj {
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

	virtual BaseType_t Take(TickType_t xBlockTime) =0;
	virtual BaseType_t Give() =0;
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

#endif /* ASEMAPHORE_H_ */

