/*
 * CCheckTask.cpp
 *
 *  Created on: Aug 3, 2010
 *      Author: Stefano Oliveri
 */

#include "CCheckTask.h"
#include <stdio.h>


// ICommonDemoTask implementation
/////////////////////////////////

ICommonDemoTask::ICommonDemoTask(CCheckTask *pCheckTask) {
	pCheckTask->AddTask(this);
}

ICommonDemoTask::~ICommonDemoTask()
{

}


// CCheckTask implementation
////////////////////////////

CCheckTask::CCheckTask(TickType_t checkFrequency/*=(4000/portTICK_RATE_MS)*/) {
	m_pListCommonDemoTaskHead = NULL;
	m_checkFrequency = checkFrequency;
}

CCheckTask::~CCheckTask() {
	// TODO Auto-generated destructor stub
}

void CCheckTask::AddTask(ICommonDemoTask *pTask) {
	pTask->m_pNext = m_pListCommonDemoTaskHead;
	m_pListCommonDemoTaskHead = pTask;
}

void CCheckTask::Run() {
	const char *pcMsg;
	TickType_t xLastCheckTime;

	xLastCheckTime = GetTickCount();

	for(;;) {
		DelayUntil(&xLastCheckTime, m_checkFrequency);

		ICommonDemoTask *pTask = m_pListCommonDemoTaskHead;
		while((pTask != NULL) && (pTask->IsStillRunning()) ) {
			pTask = pTask->m_pNext;
		}
		if (pTask != NULL)
			pcMsg = pTask->GetErrorMessage();
		else
			pcMsg = "Status: PASS";

		printf("%s\n", pcMsg);
	}
}
