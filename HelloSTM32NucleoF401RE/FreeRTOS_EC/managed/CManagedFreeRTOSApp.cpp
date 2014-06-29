/*
 * CManagedFreeRTOSApp.cpp
 *
 *  Created on: Jun 20, 2014
 *      Author: stefano
 */

#include <FreeRTOS_EC.h>
#include <assert.h>

#if ( configUSE_FREERTOS_EC_MANAGED_EXTENSION == 1 )

using freertosec::wrapper::CFreeRTOS;
using freertosec::wrapper::CTask;

namespace freertosec {
namespace managed {

// The only one instance of the Managed Appliation.
static CManagedFreeRTOSApp *g_pTheApp = NULL;

CManagedFreeRTOSApp::CManagedFreeRTOSApp(CMTContext *pApplicationContext) {
	m_pApplicationContext = pApplicationContext;
}

CManagedFreeRTOSApp::~CManagedFreeRTOSApp() {
	// TODO Auto-generated destructor stub
}

void CManagedFreeRTOSApp::OnIdle() {
#if ( configUSE_IDLE_HOOK == 1 )
	IManagedApplicationDelegate *pAppDelegate = m_pApplicationContext->GetActiveContext()->GetApplicationDelegate();
	if (pAppDelegate != NULL) {
		pAppDelegate->OnIdle();
	}
#endif
}

void CManagedFreeRTOSApp::OnTick() {
#if ( configUSE_TICK_HOOK == 1 )
	IManagedApplicationDelegate *pAppDelegate = m_pApplicationContext->GetActiveContext()->GetApplicationDelegate();
	if (pAppDelegate != NULL) {
		pAppDelegate->OnTick();
	}
#endif
}

void CManagedFreeRTOSApp::OnMallocFailed() {
#if ( configUSE_MALLOC_FAILED_HOOK == 1)
	IManagedApplicationDelegate *pAppDelegate = m_pApplicationContext->GetActiveContext()->GetApplicationDelegate();
	if (pAppDelegate != NULL) {
		pAppDelegate->OnMallocFailed();
	}
#endif
}

void CManagedFreeRTOSApp::OnStackOverflow(TaskHandle_t xTask, char *pcTaskName ) {
#if ( configCHECK_FOR_STACK_OVERFLOW > 0 )
	IManagedApplicationDelegate *pAppDelegate = m_pApplicationContext->GetActiveContext()->GetApplicationDelegate();
	if (pAppDelegate != NULL) {
		pAppDelegate->OnStackOverflow(xTask, pcTaskName);
	}
#else
	(void)xTask;
	(void)pcTaskName;
#endif
}

void CManagedFreeRTOSApp::StartManagedApplication(CMTContext *pApplicationContext) {

	if (pApplicationContext == NULL) {
		pApplicationContext = CMTContext::GetActiveContext();
		assert(pApplicationContext != NULL);
	}

	if (g_pTheApp == NULL) {
		g_pTheApp = new CManagedFreeRTOSApp(pApplicationContext);
	}

	if ( pApplicationContext->DoHardwareInitStep() ) {
		CFreeRTOS::StartScheduler();
	}
	else {
		CTask::DisableInterrupt();
		for (;;);
	}
}

extern "C"
void vApplicationTickHook( void )
{
	if (g_pTheApp != NULL) {
		g_pTheApp->OnTick();
	}
}

extern "C"
void vApplicationMallocFailedHook( void ) {
	if (g_pTheApp != NULL) {
		g_pTheApp->OnMallocFailed();
	}
}

extern "C"
void vApplicationStackOverflowHook( TaskHandle_t xTask, char *pcTaskName )
{
	if (g_pTheApp != NULL) {
		g_pTheApp->OnStackOverflow(xTask, pcTaskName);
	}
}

extern "C"
void vApplicationIdleHook( void ) {
	if (g_pTheApp != NULL) {
		g_pTheApp->OnTick();
	}
}

} /* namespace managed */
} /* namespace freertosec */

#endif
