/**
 * @file   CHeapTestTask.cpp
 *
 * @date   Sep 13, 2010
 * @author oliveris
 *         E-mail: software@stf12.net
 */

#include <stdlib.h>
#include <stdio.h>
#include "CHeapTestTask.h"
#include "CLcdTask.h"
//#include "CFreeRTOS.h"

CHeapTestTask::CHeapTestTask() {
	// TODO Auto-generated constructor stub

}

CHeapTestTask::~CHeapTestTask() {
	// TODO Auto-generated destructor stub
}

void CHeapTestTask::Run() {

	int const nMaxSize = configTOTAL_HEAP_SIZE-(2*1024);
	int const nHalfMaxSize = nMaxSize<<1;

	for (;;) {
		for (int i=0; i<ALLOCATION_COUNT; ++i) {
			int size = rand();
			if (size >= nMaxSize)
				size = size%nHalfMaxSize;
			m_pAllocatedMemory[i] = pvPortMalloc(size);
		}

		//Delay(500/portTICK_RATE_MS);

		for (int i=0; i<ALLOCATION_COUNT; ++i)
			vPortFree(m_pAllocatedMemory[i]);

		Delay(1000/portTICK_RATE_MS);
	}
}

// CHeapTestTaskMonitor implementation
// -----------------------------------

void CHeapTestTaskMonitor::Run() {
	unsigned long nFreeHeap = 0;
	char msg[20];

	for (;;) {
		//EnterCritical();
			nFreeHeap = xPortGetFreeHeapSize();
		//ExitCritical();

		snprintf(msg, 20, "HEAP F=%ld", nFreeHeap);
		CLcdTask::GetSharedInstance().Println(msg);
		Delay(4000/portTICK_RATE_MS);
	}
}
