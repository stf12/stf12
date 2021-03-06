/**
 * ASyncObject.cpp
 *
 *  Created on: Jul 13, 2010
 *      Author: Stefano Oliveri (STF12.net)
 *      E-Mail: software@stf12.net
 */

#include <FreeRTOS_EC.h>

//#include "ASyncObject.h"
#include <assert.h>

namespace freertosec {
namespace wrapper {


ASyncObject::ASyncObject() {
	m_handleSemaphore = NULL;
}

ASyncObject::~ASyncObject() {
	if (IsValid())
		Delete();
}

void ASyncObject::Delete() {
	if (m_handleSemaphore != NULL) {
		vQueueDelete(m_handleSemaphore);
		m_handleSemaphore = NULL;
	}
}

void ASyncObject::Attach(GenericHandle_t handle) {
	assert(handle != NULL);

	if (IsValid()) {
		Delete();
	}
	m_handleSemaphore = handle;
}

} /* namespace wrapper */
} /* namespace freertosec */

