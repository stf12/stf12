/**
 * @file   CHeapTestTask.h
 *
 * @date   Sep 13, 2010
 * @author oliveris
 *         E-mail: software@stf12.net
 */

#ifndef CHEAPTESTTASK_H_
#define CHEAPTESTTASK_H_

#include "AManagedTask.h"

class CHeapTestTask: public AManagedTask {
	enum {
		ALLOCATION_COUNT = 10
	};

	void *m_pAllocatedMemory[ALLOCATION_COUNT];

public:
	CHeapTestTask();
	virtual ~CHeapTestTask();

	void Run();
};

class CHeapTestTaskMonitor: public AManagedTask {
public:
	void Run();
};

#endif /* CHEAPTESTTASK_H_ */
