/**
 * @file CMessageTask.h
 * @class CMessageTask
 *
 *  Created on: Sep 20, 2010
 *      Author: Stefano Oliveri
 */

#ifndef CMESSAGETASK_H_
#define CMESSAGETASK_H_

#include "AManagedTask.h"
#include "MessageMacro.h"
#include "Message.h"
#include "IMessageTaskDelegate.h"
#include "CQueue.h"


class CMessageTask: public AManagedTask {
public:

	enum {
		MESSAGE_QUEUE_SIZE = 5,
		TASK_STACK_SIZE = configMINIMAL_STACK_SIZE * 2,
		TASK_PRIORITY = tskIDLE_PRIORITY
	};

private:

	DECLARE_MESSAGE_MAP(CMessageTask);

	/**
	 * Specifies the synchronization queue used to serialize the access at the LCD.
	 */
	CQueue m_queue;

	/**
	 * Specifies the task delegate object. It is the object that receive the notification
	 * message from the task.
	 */
	IMessageTaskDelegate *m_pDelegate;

public:

	CMessageTask();
	virtual ~CMessageTask();


	/**
	 * Create the FreeRTOS task. The application should not call this method directly.
	 *
	 * @param pcName default value is LCD
	 * @param usStackDepth default value is configMINIMAL_STACK_SIZE*2
	 * @param uxPriority 2
	 *
	 */
	portBASE_TYPE OnCreate(const portCHAR * const pcName, unsigned portSHORT usStackDepth, unsigned portBASE_TYPE uxPriority);

	/**
	 * Task control function.
	 */
	void Run();

	inline void SetDelegate(IMessageTaskDelegate *pDelegate) { m_pDelegate = pDelegate; }

	void OnIdle();

	void PostMessage(CMessage *pMsg);
	void PostMessageFromISR(CMessage *pMsg);
	virtual void DispatchMessage(const CMessage &msg);

};

#endif /* CMESSAGETASK_H_ */
