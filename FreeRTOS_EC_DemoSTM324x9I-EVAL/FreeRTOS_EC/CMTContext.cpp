/*
 * CMTContext.cpp
 *
 *  Created on: Jun 19, 2014
 *      Author: stefano
 */

#include "CMTContext.h"
#include "AManagedTask.h"
#include "FreeRTOSConfig.h"


namespace freertosec {
namespace managed {

#if (configALLOCATE_DEFAULT_MANAGED_TASK_CONTEXT == 1)
CMTContext g_ApplicationContext;
#endif

CMTContext *CMTContext::s_pDefaultContext = NULL;

CMTContext::CMTContext() {
	m_pMTListHead = NULL;
}

CMTContext::~CMTContext() {
	// TODO Auto-generated destructor stub
}

void CMTContext::Add(AManagedTask *pTaskToAdd) {
	assert(pTaskToAdd != NULL);

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
			}
			freertosec::wrapper::CFreeRTOS::ResumeAllTasks();
		}
	}
}

bool CMTContext::DoHardwareInitStep() {
	bool bRes = true;
	for (AManagedTask *pTask=m_pMTListHead; pTask!=NULL; pTask=pTask->m_pNextManagedTask)
		bRes &= pTask->HardwareInit();

	return bRes;
}

void CMTContext::SetDefault() {
	//TODO:  Do I need to implement a switch context strategy?
	s_pDefaultContext = this;
}

CMTContext *CMTContext::GetDefaultContext() {
#if (configALLOCATE_DEFAULT_MANAGED_TASK_CONTEXT == 1)
	if (s_pDefaultContext == NULL) {
		s_pDefaultContext = &g_ApplicationContext;
	}
#endif
	return s_pDefaultContext;
}

} /* namespace managed */
} /* namespace freertosec */
