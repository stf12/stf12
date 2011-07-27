/*
 * CTimerTestTask.h
 *
 *  Created on: Jun 16, 2011
 *      Author: oliveris
 */

#ifndef CTIMERTESTTASK_H_
#define CTIMERTESTTASK_H_

#include <AManagedTask.h>
#include <ATimer.h>

/**
 * One Shot timer class.
 */
class CMyOneShotTimer: public ATimer {
public:
	void OnExpired();
};

/**
 * Auto Reload timer class
 */
class CMyAutoReloadTimer: public ATimer {
public:
	void OnExpired();
};

class CTimerTestTask: public AManagedTask {
public:
	CTimerTestTask();
	virtual ~CTimerTestTask();

	/**
	 * Task control function.
	 */
	void Run();
};

#endif /* CTIMERTESTTASK_H_ */
