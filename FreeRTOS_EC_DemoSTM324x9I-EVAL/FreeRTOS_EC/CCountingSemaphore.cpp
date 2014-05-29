/**
 * CCountingSemaphore.cpp
 *
 *  Created on: Jul 13, 2010
 *      Author: Stefano Oliveri (STF12.net)
 *      E-Mail: software@stf12.net
 */

#include "CCountingSemaphore.h"
#include <assert.h>

CCountingSemaphore::CCountingSemaphore() {
	// TODO Auto-generated constructor stub

}

CCountingSemaphore::~CCountingSemaphore() {
	// TODO Auto-generated destructor stub
}

CCountingSemaphore::CCountingSemaphore(SemaphoreHandle_t handleSemaphore) {
	Attach(handleSemaphore);
}

bool CCountingSemaphore::Create(UBaseType_t uxMaxCount, UBaseType_t uxInitialCount) {

#if (configUSE_COUNTING_SEMAPHORES == 1)
	SemaphoreHandle_t handle = NULL;
	handle = xSemaphoreCreateCounting(uxMaxCount, uxInitialCount);

	if (handle != NULL)
		Attach(handle);
#endif

	return IsValid();
}
