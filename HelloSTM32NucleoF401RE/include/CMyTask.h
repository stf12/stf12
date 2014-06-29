/*
 * CMyTask.h
 *
 *  Created on: 27/giu/2014
 *      Author: stefano
 */

#ifndef CMYTASK_H_
#define CMYTASK_H_

using freertosec::managed::AManagedTask;
using freertosec::managed::CMTContext;

class CMyTask: public freertosec::managed::AManagedTask {
public:
	CMyTask(CMTContext *pContext);
	virtual ~CMyTask();

	void Run();
	bool HardwareInit();
};

#endif /* CMYTASK_H_ */
