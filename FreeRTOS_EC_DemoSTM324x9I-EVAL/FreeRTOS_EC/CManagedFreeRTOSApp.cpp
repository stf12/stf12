/*
 * CManagedFreeRTOSApp.cpp
 *
 *  Created on: Jun 20, 2014
 *      Author: stefano
 */

#include "CFreeRTOS.h"
#include "CTask.h"
#include "CManagedFreeRTOSApp.h"
#include "CMTContext.h"

#include <assert.h>

using freertosec::wrapper::CFreeRTOS;
using freertosec::wrapper::CTask;

namespace freertosec {
namespace managed {

CManagedFreeRTOSApp::CManagedFreeRTOSApp() {
	// TODO Auto-generated constructor stub

}

CManagedFreeRTOSApp::~CManagedFreeRTOSApp() {
	// TODO Auto-generated destructor stub
}

void CManagedFreeRTOSApp::StartManagedApplication(CMTContext *pApplicationContext) {
	if (pApplicationContext == NULL) {
		pApplicationContext = CMTContext::GetDefaultContext();
		assert(pApplicationContext != NULL);
	}

	if ( pApplicationContext->DoHardwareInitStep() ) {
		CFreeRTOS::StartScheduler();
	}
	else {
		CTask::DisableInterrupt();
		for (;;);
	}
}

} /* namespace managed */
} /* namespace freertosec */
