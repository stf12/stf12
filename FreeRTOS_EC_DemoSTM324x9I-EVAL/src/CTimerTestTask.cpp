/*
 * CTimerTestTask.cpp
 *
 *  Created on: Jun 16, 2011
 *      Author: oliveris
 */

#include "CTimerTestTask.h"
#include "CLcdTask2.h"

CTimerTestTask::CTimerTestTask() {
	// TODO Auto-generated contructor stub
}

CTimerTestTask::~CTimerTestTask() {
	// TODO Auto-generated destructor stub
}

void CTimerTestTask::Run() {
	CMyOneShotTimer xOneShotTimer;
	xOneShotTimer.Create("tim1", 2000/portTICK_PERIOD_MS, pdFALSE);
	xOneShotTimer.Start(portMAX_DELAY);

	CMyAutoReloadTimer xAutoReloadTimer;
	xAutoReloadTimer.Create("tim2", 3000/portTICK_PERIOD_MS, pdTRUE).Start(portMAX_DELAY);

	Delete();
}

CMyOneShotTimer::CMyOneShotTimer() {
	m_bFlag = false;
}

void CMyOneShotTimer::OnExpired() {
	CLcdTask2::GetSharedInstance().Println(m_bFlag ? "OS timer expired!1" : "OS timer expired!0");
	m_bFlag = !m_bFlag;
}

CMyAutoReloadTimer::CMyAutoReloadTimer() {
	m_bFlag = false;
}

void CMyAutoReloadTimer::OnExpired() {
	CLcdTask2::GetSharedInstance().Println(m_bFlag ? "AR timer expired!1" : "AR timer expired!0");
	m_bFlag = !m_bFlag;
}
