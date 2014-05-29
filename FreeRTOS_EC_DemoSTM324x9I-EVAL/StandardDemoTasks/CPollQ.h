/**
 * @file   CPollQ.h
 * @class  APollQ
 * @ingroup FreeRTOS_STANDARD_DEMO
 *
 * Creates two tasks that communicate over a single queue.  One task acts as a
 * producer, the other a consumer.
 *
 * The producer loops for three iteration, posting an incrementing number onto the
 * queue each cycle.  It then delays for a fixed period before doing exactly the
 * same again.
 *
 * The consumer loops emptying the queue.  Each item removed from the queue is
 * checked to ensure it contains the expected value.  When the queue is empty it
 * blocks for a fixed period, then does the same again.
 *
 * All queue access is performed without blocking.  The consumer completely empties
 * the queue each time it runs so the producer should never find the queue full.
 *
 * An error is flagged if the consumer obtains an unexpected value or the producer
 * find the queue is full.

 *
 * @date   Aug 17, 2010
 * @author Stefano Oliveri
 *         E-mail: software@stf12.net
 */

#ifndef CPOLLQ_H_
#define CPOLLQ_H_

#include "AManagedTask.h"
#include "CCheckTask.h"
#include "CQueue.h"

class APollQ: public AManagedTask, public ICommonDemoTask {
protected:
	/**
	 * The queue to be used by the task.
	 */
	CQueue *m_pQueue;

	/**
	 * Variable that are used to check that the task is still running with no errors.
	 */
	BaseType_t m_nPollingCount;

public:
	APollQ(CCheckTask *pCheckTask, CQueue *pQueue);
	virtual ~APollQ();

	bool IsStillRunning();
	const char*GetErrorMessage();

	static void StartPolledQueueTasks(CCheckTask *pCheckTask, UBaseType_t nPriority );
};

class CPollQProducer : public APollQ {

public:
	CPollQProducer(CCheckTask *pCheckTask, CQueue *pQueue);
	void Run();
};

class CPollQConsumer : public APollQ {

public:
	CPollQConsumer(CCheckTask *pCheckTask, CQueue *pQueue);
	void Run();
};

#endif /* CPOLLQ_H_ */
