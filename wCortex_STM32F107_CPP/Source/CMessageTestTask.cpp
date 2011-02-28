/*
 * CMessageTestTask.cpp
 *
 *  Created on: Feb 24, 2011
 *      Author: oliveris
 */

#include "CMessageTestTask.h"

#define MSG_1 APP_MSG + 1
#define MSG_2 APP_MSG + 2

BEGIN_MESSAGE_MAP(CMessageTask, CMessageTestTask)
	MESSAGE_MAP_ENTRY(CMessageTestTask, MSG_1, Message1Handler)
	MESSAGE_MAP_ENTRY(CMessageTestTask, MSG_2, Message2Handler)
END_MESSAGE_MAP(CMessageTestTask)

CMessageTestTask::CMessageTestTask() {
	// TODO Auto-generated constructor stub

}

CMessageTestTask::~CMessageTestTask() {
	// TODO Auto-generated destructor stub
}

void CMessageTestTask::Message1Handler(unsigned long wParam) {
	int pippo = 1;
}

void CMessageTestTask::Message2Handler(unsigned long wParam) {
	int pippo = 2;
}

// CMessageProducerTask class implementation.
// ******************************************
CMessageProducerTask::CMessageProducerTask(CMessageTask *pMessageTask) {
	m_pMessageTask = pMessageTask;
}

void CMessageProducerTask::Run() {
	CMessage msg;
	msg.wParam = 0;

	for (;;) {
		Delay(500 / portTICK_RATE_MS);
		msg.m_nId = MSG_1;
		m_pMessageTask->PostMessage(&msg);

		Delay(500 / portTICK_RATE_MS);
		msg.m_nId = MSG_2;
		m_pMessageTask->PostMessage(&msg);

		Delay(500 / portTICK_RATE_MS);
		msg.m_nId = MSG_2+1;
		m_pMessageTask->PostMessage(&msg);

	}
}
