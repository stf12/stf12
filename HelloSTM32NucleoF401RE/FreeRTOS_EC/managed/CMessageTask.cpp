/*
 * CMessageTask.cpp
 *
 *  Created on: Sep 20, 2010
 *      Author: oliveris
 */

#include <FreeRTOS_EC.h>

#include <stdio.h>
//#include "CMessageTask.h"

#if ( configUSE_FREERTOS_EC_MANAGED_EXTENSION == 1 )

namespace freertosec {
namespace managed {

const CMessageTask::message_map_entry_t CMessageTask::s_message_map[] = {
		{ NULL_MSG, NULL }
};

CMessageTask::CMessageTask()
{
	m_pDelegate = NULL;
	m_nTimeOut = portMAX_DELAY;
}

CMessageTask::~CMessageTask() {
	// TODO Auto-generated destructor stub
}

BaseType_t CMessageTask::OnCreate(const char * const pcName, uint16_t usStackDepth, UBaseType_t uxPriority) {
	(void)pcName;
	(void)usStackDepth;
	(void)uxPriority;

	BaseType_t res;
	res = m_queue.Create(CMessageTask::MESSAGE_QUEUE_SIZE, sizeof(char[20])).IsValid();

	return res;
}

void CMessageTask::Run() {

	// call task specific initialization code.
	 if ( !OnInit() ) {
#if INCLUDE_vTaskDelete == 1
		 Delete();
		 return;
#else
	 // The task must not start due to initialization problem, but the FreeRTOS
	 // vTaskDelete function is not defined by configuration file. Suspend the task.
		 Suspend();
#endif
	 }

	CMessage msg;
	for (;;) {

		if ( m_queue.Receive(&msg, m_nTimeOut) == pdTRUE ) {
			// Message Handling routine

			// Call the delegate, if one, before try to dispatch the event
			if (m_pDelegate) m_pDelegate->OnHandleEvent(msg);
			DispatchMessage(msg);
			// Call the delegate, if one, after tried to dispatch the event
			if (m_pDelegate) m_pDelegate->DidHandleEvent(msg);
		}
		else {
			// TODO: STF - timeout expired.
			OnTimeOut();
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

bool CMessageTask::PostMessage(CMessage *pMsg, TickType_t nTicksToWait/*=portMAX_DELAY*/) {
	return (bool)m_queue.Send(pMsg, nTicksToWait);
}

bool CMessageTask::PostMessageFromISR(CMessage *pMsg, BaseType_t *pxHigherPriorityTaskWoken) {
	return (bool)m_queue.SendFromISR(pMsg, pxHigherPriorityTaskWoken);
}

bool CMessageTask::SendMessage(CMessage *pMsg, TickType_t nTicksToWait/*=portMAX_DELAY*/) {
	return (bool)m_queue.SendToFront(pMsg, nTicksToWait);
}

bool CMessageTask::SenfFromISR(CMessage *pMsg, BaseType_t * pxHigherPriorityTaskWoken) {
	return (bool)m_queue.SendToFrontFromISR(pMsg, pxHigherPriorityTaskWoken);
}

} /* namespace managed */
} /* namespace freertosec */

#endif
