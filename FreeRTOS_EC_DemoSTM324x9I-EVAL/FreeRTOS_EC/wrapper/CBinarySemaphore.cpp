/**
 * CBinarySemaphore.cpp
 *
 *  Created on: Jul 13, 2010
 *      Author: Stefano Oliveri (STF12.net)
 *      E-Mail: software@stf12.net
 */

#include <FreeRTOS_EC.h>

//#include "CBinarySemaphore.h"

namespace freertosec {
namespace wrapper {

CBinarySemaphore::CBinarySemaphore() {
	// TODO Auto-generated constructor stub

}

CBinarySemaphore::~CBinarySemaphore() {
	// TODO Auto-generated destructor stub
}

CBinarySemaphore::CBinarySemaphore(SemaphoreHandle_t handleSemaphore) {
	Attach(handleSemaphore);
}

bool CBinarySemaphore::Create() {
	SemaphoreHandle_t handle = NULL;
	handle = xSemaphoreCreateBinary();

	if (handle != NULL)
		Attach(handle);

	return IsValid();
}

} /* namespace wrapper */
} /* namespace freertosec */
