/**
 * @file CBlockQ.h
 * @class ABlockQ
 * @ingroup FreeRTOS_STANDARD_DEMO
 *
 * Creates six tasks that operate on three queues as follows:
 *
 * The first two tasks send and receive an incrementing number to/from a queue.
 * One task acts as a producer and the other as the consumer.  The consumer is a
 * higher priority than the producer and is set to block on queue reads.  The queue
 * only has space for one item - as soon as the producer posts a message on the
 * queue the consumer will unblock, pre-empt the producer, and remove the item.
 *
 * The second two tasks work the other way around.  Again the queue used only has
 * enough space for one item.  This time the consumer has a lower priority than the
 * producer.  The producer will try to post on the queue blocking when the queue is
 * full.  When the consumer wakes it will remove the item from the queue, causing
 * the producer to unblock, pre-empt the consumer, and immediately re-fill the
 * queue.
 *
 * The last two tasks use the same queue producer and consumer functions.  This time the queue has
 * enough space for lots of items and the tasks operate at the same priority.  The
 * producer will execute, placing items into the queue.  The consumer will start
 * executing when either the queue becomes full (causing the producer to block) or
 * a context switch occurs (tasks of the same priority will time slice).
 *
 *
 *  @date Aug 4, 2010
 *  @author Stefano Oliveri
 */


#ifndef CBLOCKQ_H_
#define CBLOCKQ_H_

#include "AManagedTask.h"
#include "CCheckTask.h"
#include "CQueue.h"

using namespace freertosec::wrapper;


/**
 * Abstract base class.
 */
class ABlockQ: public AManagedTask, public ICommonDemoTask {
protected:
	/**
	 * The queue to be used by the task.
	 */
	CQueue *m_pQueue;

	/**
	 * The block time to use on queue reads/writes
	 */
	TickType_t m_xBlockTime;

	/**
	 * Incremented on each successful cycle to check the task is still running.
	 */
	short *m_psCheckVariable;

	/**
	 * Used to check the task is still running
	 */
	short m_nLastBlockingCount;

public:
	ABlockQ(CCheckTask *pCheckTask, CQueue *pQueue, short *psCheckVariable, TickType_t xBlockTime);
	virtual ~ABlockQ();

	bool IsStillRunning();
	const char*GetErrorMessage();

	static void StartBlockingQueueTasks(CCheckTask *pCheckTask, UBaseType_t nPriority );
};

class CBlockQProducer: public ABlockQ {
public:
	CBlockQProducer(CCheckTask *pCheckTask, CQueue *pQueue, short *psCheckVariable, TickType_t xBlockTime);
	void Run();
};

class CBlockQConsumer: public ABlockQ {
public:
	CBlockQConsumer(CCheckTask *pCheckTask, CQueue *pQueue, short *psCheckVariable, TickType_t xBlockTime);
	void Run();
};

#endif /* CBLOCKQ_H_ */
