/*
 * CTimerTestTask.cpp
 *
 *  Created on: Jun 16, 2011
 *      Author: oliveris
 */

#include "CTimerTestTask.h"
#include "CLcdTask2.h"


CTimerTestTask::CTimerTestTask() {
	// TODO Auto-generated constructor stub

}

CTimerTestTask::~CTimerTestTask() {
	// TODO Auto-generated destructor stub
}

void CTimerTestTask::Run() {
	CMyOneShotTimer xOneShotTimer;
	xOneShotTimer.Create((signed char*)"tim1", 2000/portTICK_RATE_MS, pdFALSE);
	xOneShotTimer.Start(portMAX_DELAY);

	CMyAutoReloadTimer xAutoReloadTimer;
	xAutoReloadTimer.Create((signed char*)"tim2", 3000/portTICK_RATE_MS, pdTRUE).Start(portMAX_DELAY);

	Delete();
}

void CMyOneShotTimer::OnExpired() {
	CLcdTask2::GetSharedInstance().Println("OS timer expired!");
}

void CMyAutoReloadTimer::OnExpired() {
	CLcdTask2::GetSharedInstance().Println("AR timer expired!");
}
