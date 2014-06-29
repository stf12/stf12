/*
 * CMTContext.cpp
 *
 *  Created on: Jun 19, 2014
 *      Author: stefano
 */

#include <FreeRTOS_EC.h>

//#include "CMTContext.h"
//#include "AManagedTask.h"
//#include "FreeRTOSConfig.h"

#if ( configUSE_FREERTOS_EC_MANAGED_EXTENSION == 1 )


namespace freertosec {
namespace managed {

#if (configALLOCATE_DEFAULT_MANAGED_TASK_CONTEXT == 1)
CMTContext g_SystemApplicationContext;
#endif

CMTContext *CMTContext::s_pActiveContext = NULL;

CMTContext::CMTContext() {
	m_pMTListHead = NULL;
	m_pDelegate = NULL;
}

CMTContext::CMTContext(IManagedApplicationDelegate *pApplicationDelegate) {
	m_pMTListHead = NULL;
	m_pDelegate = pApplicationDelegate;
}

CMTContext::~CMTContext() {
	// TODO Auto-generated destructor stub
}

void CMTContext::Add(AManagedTask *pTaskToAdd) {
	assert(pTaskToAdd != NULL);

	if (pTaskToAdd->m_pContext != NULL) {
		// A managed task can belong to only one context, so first remove from the previous context.
		pTaskToAdd->m_pContext->Remove(pTaskToAdd);
	}

	if (pTaskToAdd != NULL) {
		pTaskToAdd->EnterCritical();
		// is the first element?
		if (m_pMTListHead == NULL) {
			m_pMTListHead = pTaskToAdd;
		}
		else {
			// add the task to the head of the managed task list.
			pTaskToAdd->m_pNextManagedTask = m_pMTListHead;
			m_pMTListHead = pTaskToAdd;
		}
		pTaskToAdd->ExitCritical();
	}
}

void CMTContext::Remove(AManagedTask *pTaskToRemove) {
	assert(pTaskToRemove != NULL);

	if (pTaskToRemove != NULL) {
		// search for the task to remove in the managed task list
		// is it the first element?
		pTaskToRemove->EnterCritical();
		if (pTaskToRemove == m_pMTListHead) {
			m_pMTListHead = pTaskToRemove->m_pNextManagedTask;
			pTaskToRemove->m_pNextManagedTask = NULL;
			pTaskToRemove->m_pContext = NULL;
			pTaskToRemove->ExitCritical();
		}
		else {
			pTaskToRemove->ExitCritical();
			freertosec::wrapper::CFreeRTOS::SuspendAllTasks();
			AManagedTask *pTask = m_pMTListHead;
			while (pTask != NULL && pTask->m_pNextManagedTask != pTaskToRemove) {
				pTask = pTask->m_pNextManagedTask;
			}
			if (pTask != NULL) { // Check to be sure that the task is in the list
				// remove the task from the list
				pTask->m_pNextManagedTask = pTaskToRemove->m_pNextManagedTask;
				pTaskToRemove->m_pNextManagedTask = NULL;
				pTaskToRemove->m_pContext = NULL;
			}
			freertosec::wrapper::CFreeRTOS::ResumeAllTasks();
		}
	}
}

bool CMTContext::Find(AManagedTask *pTaskToCheck) {
	if (pTaskToCheck == NULL)
		return false;

	bool bFound = false;
	AManagedTask *pTask = m_pMTListHead;
	freertosec::wrapper::CFreeRTOS::SuspendAllTasks();
	while (pTask != NULL && !bFound) {
		if (pTask == pTaskToCheck)
			bFound = true;
		pTask = pTask->m_pNextManagedTask;
	}
	freertosec::wrapper::CFreeRTOS::ResumeAllTasks();

	return bFound;
}

bool CMTContext::DoHardwareInitStep() {
	bool bRes = true;
	for (AManagedTask *pTask=m_pMTListHead; pTask!=NULL; pTask=pTask->m_pNextManagedTask)
		bRes &= pTask->HardwareInit();

	return bRes;
}

void CMTContext::Activate() {
	//TODO:  Do I need to implement a switch context strategy?
	s_pActiveContext = this;
}

CMTContext *CMTContext::GetActiveContext() {
#if (configALLOCATE_DEFAULT_MANAGED_TASK_CONTEXT == 1)
	if (s_pActiveContext == NULL) {
		s_pActiveContext = &g_SystemApplicationContext;
	}
#endif
	return s_pActiveContext;
}

IManagedApplicationDelegate *CMTContext::GetApplicationDelegate() {
	return m_pDelegate;
}

} /* namespace managed */
} /* namespace freertosec */

#endif

