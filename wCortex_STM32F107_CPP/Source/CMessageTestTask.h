/*
 * CMessageTestTask.h
 *
 *  Created on: Feb 24, 2011
 *      Author: oliveris
 */

#ifndef CMESASGETESTTASK_H_
#define CMESASGETESTTASK_H_

#include <CMessageTask.h>

class CMessageTestTask: public CMessageTask {
	DECLARE_MESSAGE_MAP(CMessageTestTask);
public:
	CMessageTestTask();
	virtual ~CMessageTestTask();

	void DispatchMessage(const CMessage &msg);

	void Message1Handler(unsigned long wParam);
	void Message2Handler(unsigned long wParam);
};

class CMessageProducerTask: public AManagedTask {
	CMessageTask *m_pMessageTask;
public:
	CMessageProducerTask(CMessageTask *pMessageTask);
	void Run();
};

#endif /* CMESASGETESTTASK_H_ */
