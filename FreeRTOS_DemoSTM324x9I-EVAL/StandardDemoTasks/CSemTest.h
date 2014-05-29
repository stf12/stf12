/**
 * @file   CSemTest.h
 *
 * Creates two sets of two tasks.  The tasks within a set share a variable, access
 * to which is guarded by a semaphore.
 *
 * Each task starts by attempting to obtain the semaphore.  On obtaining a
 * semaphore a task checks to ensure that the guarded variable has an expected
 * value.  It then clears the variable to zero before counting it back up to the
 * expected value in increments of 1.  After each increment the variable is checked
 * to ensure it contains the value to which it was just set. When the starting
 * value is again reached the task releases the semaphore giving the other task in
 * the set a chance to do exactly the same thing.  The starting value is high
 * enough to ensure that a tick is likely to occur during the incrementing loop.
 *
 * An error is flagged if at any time during the process a shared variable is
 * found to have a value other than that expected.  Such an occurrence would
 * suggest an error in the mutual exclusion mechanism by which access to the
 * variable is restricted.
 *
 * The first set of two tasks poll their semaphore.  The second set use blocking
 * calls.
 *
 * @date   Aug 19, 2010
 * @author Stefano Oliveri
 *         E-mail: software@stf12.net
 */

#ifndef CSEMTEST_H_
#define CSEMTEST_H_

#include "AManagedTask.h"
#include "CCheckTask.h"
#include "CBinarySemaphore.h"

class CSemTest: public AManagedTask, public ICommonDemoTask {
	unsigned long *m_pSharedVariable;
	portTickType m_nBlockTime;
	CBinarySemaphore *m_pSemaphore;

	/**
	 * Variable used to check that the task is still running without errors.
	 */
	short m_nCheckVariable;
	short m_nLastCheckVariable;

public:
	CSemTest(CCheckTask *pCheckTask);
	virtual ~CSemTest();

	void Run();

	bool IsStillRunning();
	const char*GetErrorMessage() { return "ERR:SEM"; }

	static void StartSemTestTasks(CCheckTask *pCheckTask, unsigned portBASE_TYPE nPriority );
};

#endif /* CSEMTEST_H_ */
