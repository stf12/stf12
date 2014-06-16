/**
 * CQueueSetTest.h
 *
 * Tests the use of queue sets.
 *
 * A receive task creates a number of queues and adds them to a queue set before
 * blocking on the queue set receive.  A transmit task and (optionally) an
 * interrupt repeatedly unblocks the receive task by sending messages to the
 * queues in a pseudo random order.  The receive task removes the messages from
 * the queues and flags an error if the received message does not match that
 * expected.  The task sends values in the range 0 to
 * queuesetINITIAL_ISR_TX_VALUE, and the ISR sends value in the range
 * queuesetINITIAL_ISR_TX_VALUE to ULONG_MAX.

 *
 *  Created on: Jun 12, 2014
 *      Author: stefano
 */

#ifndef CQUEUESETTEST_H_
#define CQUEUESETTEST_H_

#include "AManagedTask.h"
#include "CCheckTask.h"
#include "CQueueSet.h"
#include "CQueue.h"

/* The number of queues that are created and added to the queue set. */
#define queuesetNUM_QUEUES_IN_SET 3


class CQueueSetTest {
	friend class CQSRxTask;
	friend class CQSTxTask;

private:
	/* The handle of the queue set to which the queues are added. */
	static CQueueSet s_xQueueSet;
	//static QueueSetHandle_t xQueueSet;

	/* Used by the pseudo random number generator. */
	static unsigned long s_ulNextRand;

	/* If the prvQueueSetReceivingTask() task has not detected any errors then
	it increments ulCycleCounter on each iteration.
	xAreQueueSetTasksStillRunning() returns pdPASS if the value of
	ulCycleCounter has changed between consecutive calls, and pdFALSE if
	ulCycleCounter has stopped incrementing (indicating an error condition). */
	static volatile unsigned long s_ulCycleCounter;

	/* Just a flag to let the function that writes to a queue from an ISR know that
	the queues are setup and can be used. */
	static volatile BaseType_t s_xSetupComplete;


	static unsigned long Rand();
	static void SRand(unsigned long ulSeed);

public:
	CQueueSetTest();
	virtual ~CQueueSetTest();

	static void StartTestTasks(CCheckTask *pCheckTask, UBaseType_t uxPriority);

private:
	static BaseType_t CheckReceivedValue(unsigned long ulReceived);
	static BaseType_t CheckReceivedValueWithinExpectedRange(unsigned long ulReceived, unsigned long ulExpectedReceived);

};

class CQSTxTask: public AManagedTask, public ICommonDemoTask {
private:
	BaseType_t m_xTasksStatus;

	/* The queues that are added to the set. */
	CQueue m_xQueues[queuesetNUM_QUEUES_IN_SET];

	/**
	 * Name of the queue for the System Queue Register.
	 */
	char *m_strQueueNames[queuesetNUM_QUEUES_IN_SET];

	/* Counts how many times each queue in the set is used to ensure all the
	queues are used. */
	unsigned long m_ulQueueUsedCounter[queuesetNUM_QUEUES_IN_SET];

public:
	CQSTxTask(CCheckTask *pCheckTask);
	virtual ~CQSTxTask();

	void Run();
	BaseType_t  OnCreate(const portCHAR * const pcName, unsigned portSHORT usStackDepth, UBaseType_t  uxPriority);

	bool IsStillRunning();
	const char*GetErrorMessage();

	/**
	 * Perform some API test on the CQueueSet object using the CQueue array.
	 * It is called by the CQSRxTask before starting resuming the CQSTxTask
	 * and start its main loop.
	 *
	 * @return <code>true</code> if success, <code>false</code> otherwise.
	 */
	bool PreTest();
};

class CQSRxTask: public AManagedTask, public ICommonDemoTask {
private:
	CQSTxTask *m_pTxTask;
	CQueue m_xQueue;
	BaseType_t m_xTasksStatus;

public:
	CQSRxTask(CCheckTask *pCheckTask, CQSTxTask *pTxTask);
	virtual ~CQSRxTask();

	void Run();
	BaseType_t  OnCreate(const portCHAR * const pcName, unsigned portSHORT usStackDepth, UBaseType_t  uxPriority);

	bool IsStillRunning();
	const char*GetErrorMessage();
};

#endif /* CQUEUESETTEST_H_ */
