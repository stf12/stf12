/*
 * CINDataTask.h
 *
 *  Created on: Nov 9, 2010
 *      Author: oliveris
 */

#ifndef CINDATATASK_H_
#define CINDATATASK_H_

#include "AManagedTask.h"
#include "CBinarySemaphore.h"
#include "iNEMO_lib.h"

class CINDataTask: public AManagedTask {
	CBinarySemaphore &m_timSemaphore;
	iNEMO_DATA *m_pSharedData;

public:
	CINDataTask(CBinarySemaphore &timSemaphore, iNEMO_DATA *pSharedData);
	virtual ~CINDataTask();

	bool HardwareInit();
	portBASE_TYPE OnCreate(const portCHAR * const pcName, unsigned portSHORT usStackDepth, unsigned portBASE_TYPE uxPriority);
	void Run();

private:
	void TimerConfig();
};

#endif /* CINDATATASK_H_ */
