/**
 * AManagedTask.cpp
 *
 *  Created on: Aug 3, 2010
 *      Author: Stefano Oliveri
 */


#include <FreeRTOS_EC.h>
#include <assert.h>

#if ( configUSE_FREERTOS_EC_MANAGED_EXTENSION == 1 )
//#include "CFreeRTOS.h"
//#include "AManagedTask.h"
//#include "CMTContext.h"


namespace freertosec {
namespace managed {


AManagedTask::AManagedTask() {
	assert(CMTContext::GetActiveContext() != NULL);

	m_pNextManagedTask = NULL;
	m_pContext = CMTContext::GetActiveContext();
	m_pContext->Add(this);
}

AManagedTask::AManagedTask(CMTContext *pContext) {
	assert(pContext != NULL);

	m_pNextManagedTask = NULL;
	m_pContext = pContext;
	m_pContext->Add(this);
}

AManagedTask::~AManagedTask() {
	m_pContext->Remove(this);
}

BaseType_t  AManagedTask::Create(const char * const pcName, uint16_t usStackDepth, UBaseType_t  uxPriority) {
	return OnCreate(pcName, usStackDepth, uxPriority) &&
			CTask::Create(AManagedTask::taskControlFunc, pcName, usStackDepth, this, uxPriority);
}

void AManagedTask::taskControlFunc(void *pParams) {
	static_cast<AManagedTask *>(pParams)->Run();
}

} /* namespace managed */
} /* namespace freertosec */

#endif
