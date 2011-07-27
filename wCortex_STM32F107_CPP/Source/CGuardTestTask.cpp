/**
 * @file   CGuardIncrementTask.cpp
 *
 * @date   Aug 23, 2010
 * @author Stefano Oliveri
 *         E-mail: software@stf12.net
 */

#include <stdio.h>

#include "CGuardTestTask.h"
#include "CLcdTask2.h"
#include "CFreeRTOS.h"


// CSharedResourceEditor implementation
//-------------------------------

void CSharedResourceEditor::StartGuardTestClass() {
	static CSharedResource<int, CMutex> basicSharedResource;
	static CSharedResource<CResource, CMutex> sharedResource;


	static CGuardIncrementTask task1;
	task1.m_nFrequecy = 500 / portTICK_RATE_MS;
	task1.m_pSharedResource = &sharedResource;
	task1.m_pBasicSharedResource = &basicSharedResource;
	task1.Create("guardInc", configMINIMAL_STACK_SIZE, 0);

	static CGuardDecrementTask task2;
	task2.m_nFrequecy = 1000 / portTICK_RATE_MS;
	task2.m_pGuardedResource = &sharedResource;
	task2.m_pBasicSharedResource = &basicSharedResource;
	// We need a bit more stack to use the snprintf function
	task2.Create("guardDec", 3*configMINIMAL_STACK_SIZE, 1);
}


// CGuardIncrementTask implementation
//-----------------------------------

CGuardIncrementTask::CGuardIncrementTask() {
	m_nFrequecy = 0;
}
CGuardIncrementTask::~CGuardIncrementTask() {
	// TODO Auto-generated destructor stub
}

void CGuardIncrementTask::Run() {
	CResource value;

	for (;;) {
		value = *m_pSharedResource;
		value.m_nValue += 1;
		*m_pSharedResource = value;

		*m_pBasicSharedResource = *m_pBasicSharedResource + 1;

		 Delay(m_nFrequecy);
	}
}


// CGuardDecrementTask implementation
//-----------------------------------

CGuardDecrementTask::CGuardDecrementTask() {
	m_nFrequecy = 0;
}
CGuardDecrementTask::~CGuardDecrementTask() {
	// TODO Auto-generated destructor stub
}

void CGuardDecrementTask::Run() {
	CResource value;
	char msg[20];

	CSharedResource<int, CMutex, 10> n;
	n = 0;

	for (;;) {
		 value = m_pGuardedResource->GetResource();
		 value.m_nValue -= 2;
		 m_pGuardedResource->SetResource(value);

		 *m_pBasicSharedResource = *m_pBasicSharedResource - 2;

		 n = n+1;

		 int m = n;

//		 CFreeRTOS::SuspendAllTasks();
		 snprintf(msg, 20, "sr=%d bsr=%d", value.m_nValue, (int)*m_pBasicSharedResource);
//		 CFreeRTOS::ResumeAllTasks();

		 CLcdTask2::GetSharedInstance().Println(msg);

		 Delay(m_nFrequecy);
	}
}
