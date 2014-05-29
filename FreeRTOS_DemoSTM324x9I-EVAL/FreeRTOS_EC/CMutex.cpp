/**
 * CMutex.cpp
 *
 *  Created on: Jul 14, 2010
 *      Author: Stefano Oliveri (STF12.net)
 *      E-Mail: software@stf12.net
 */

#include "CMutex.h"

CMutex::CMutex() {
	// TODO Auto-generated constructor stub

}

CMutex::~CMutex() {
	// TODO Auto-generated destructor stub
}

CMutex::CMutex(SemaphoreHandle_t handleSemaphore) {
	Attach(handleSemaphore);
}

bool CMutex::Create() {

#if ( configUSE_MUTEXES == 1 )

	SemaphoreHandle_t handle;

	handle = xSemaphoreCreateMutex();

	if (handle != NULL)
		Attach(handle);
#endif

	return IsValid();
}

