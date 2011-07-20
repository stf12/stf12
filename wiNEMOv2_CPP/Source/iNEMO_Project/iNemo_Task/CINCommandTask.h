/*
 * CINCommandTask.h
 *
 *  Created on: Nov 9, 2010
 *      Author: oliveris
 */

#ifndef CINCOMMANDTASK_H_
#define CINCOMMANDTASK_H_

#include "AManagedTask.h"
#include "CQueue.h"
#include "iNEMO_lib.h"

class CINCommandTask: public AManagedTask {
	CQueue &m_usbQueue;
	iNEMO_DATA *m_pSharedData;

public:
	CINCommandTask(CQueue &usbQueue, iNEMO_DATA *pSharedData);
	virtual ~CINCommandTask();

	bool HardwareInit();
	portBASE_TYPE OnCreate(const portCHAR * const pcName, unsigned portSHORT usStackDepth, unsigned portBASE_TYPE uxPriority);
	void Run();
};

#endif /* CINCOMMANDTASK_H_ */
