/*
 * CMyTask.cpp
 *
 *  Created on: 27/giu/2014
 *      Author: stefano
 */

#include <FreeRTOS_EC.h>
#include "CMyTask.h"
#include "diag/Trace.h"

CMyTask::CMyTask(CMTContext *pContext): AManagedTask(pContext) {
	// TODO Auto-generated constructor stub

}

CMyTask::~CMyTask() {
	// TODO Auto-generated destructor stub
}

void CMyTask::Run() {
	int n = 1;

	for(;;) {
		trace_printf("My first task: %i\n", n);
		Delay(2000 / portTICK_PERIOD_MS);
		n++;
	}
}
