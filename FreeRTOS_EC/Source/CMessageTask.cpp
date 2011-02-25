/*
 * CMessageTask.cpp
 *
 *  Created on: Sep 20, 2010
 *      Author: oliveris
 */

#include <stdio.h>
#include "CMessageTask.h"



BEGIN_MESSAGE_MAP(CMessageTask)
//	MESSAGE_MAP_ENTRY(CMessageTask, KEY_MSG, OnKey)
//	MESSAGE_MAP_ENTRY(CMessageTask, WND_UPDATE_MSG, OnUpdate)
//	MESSAGE_MAP_ENTRY(CMessageTask, WND_LOCK_ALL_CONTROL_MSG, OnLockAllControl)
//	MESSAGE_MAP_ENTRY(CMessageTask, WND_UNLOCK_ALL_CONTROL_MSG, OnUnLockAllControl)
//	MESSAGE_MAP_ENTRY(CMessageTask, WND_DISPLAY_INFO_MSG, OnDisplayInfo)
END_MESSAGE_MAP(CMessageTask)

CMessageTask::CMessageTask()
{
	m_pDelegate = NULL;
}

CMessageTask::~CMessageTask() {
	// TODO Auto-generated destructor stub
}

portBASE_TYPE CMessageTask::OnCreate(const portCHAR * const pcName, unsigned portSHORT usStackDepth, unsigned portBASE_TYPE uxPriority) {
	portBASE_TYPE res;
	res = m_queue.Create(CMessageTask::MESSAGE_QUEUE_SIZE, sizeof(char[20])).IsValid();

	return res;
}

void CMessageTask::Run() {

	const portTickType nRefreshRate = 300 / portTICK_RATE_MS;

	CMessage msg;
	for (;;) {

		if ( m_queue.Receive(&msg, nRefreshRate) == pdTRUE ) {
			// Message Handling routine
			DispatchMessage(msg);
		}
		else {
			// Idle time processing.
		}
	}
}

void CMessageTask::DispatchMessage(const CMessage &msg) {
	for (int i=0; s_message_map[i].id!=NULL_MSG; ++i) {
		if (s_message_map[i].id == msg.m_nId) {
			(this->*s_message_map[i].MessageHandler)(msg.wParam);
			break;
		}
	}
}

void CMessageTask::OnIdle() {

//		if ( m_display.IsInvalid() ) {
//		// post a WND_UPDATE_MSG message
//		CMessage msg;
//		msg.m_nId = WND_UPDATE_MSG;
//
//		PostMessageFromISR(&msg);
//	}

}

void CMessageTask::PostMessage(CMessage *pMsg) {
	m_queue.Send(pMsg, portMAX_DELAY);
}

void CMessageTask::PostMessageFromISR(CMessage *pMsg) {
	portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
	m_queue.SendFromISR(pMsg, &xHigherPriorityTaskWoken);

	portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
}
