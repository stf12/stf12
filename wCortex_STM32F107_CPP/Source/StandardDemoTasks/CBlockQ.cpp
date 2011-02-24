/**
 *  @file    CBlockQ.cpp
 *
 *  @date   Aug 4, 2010
 *  @author Stefano Oliveri
 */

#include <assert.h>
#include "CBlockQ.h"

#define blckqSTACK_SIZE		(configMINIMAL_STACK_SIZE)
#define blckqNUM_TASK_SETS	( 3 )


/**
 *  Variables which are incremented each time an item is removed from a queue, and found to be the expected value.
 *  These are used to check that the tasks are still running.
 */
static short s_sBlockingConsumerCount[ blckqNUM_TASK_SETS ] = { ( unsigned short ) 0, ( unsigned short ) 0, ( unsigned short ) 0 };

/**
 *  Variable which are incremented each time an item is posted on a queue.
 *  These are used to check that the tasks are still running.
 */
static short s_sBlockingProducerCount[ blckqNUM_TASK_SETS ] = { ( unsigned short ) 0, ( unsigned short ) 0, ( unsigned short ) 0 };



// ABlockQ implementation.
//////////////////////////

ABlockQ::ABlockQ(CCheckTask *pCheckTask, CQueue *pQueue, short *psCheckVariable,  portTickType xBlockTime): ICommonDemoTask(pCheckTask) {
	assert(pQueue->IsValid());

	m_pQueue = pQueue;
	m_psCheckVariable = psCheckVariable;
	m_xBlockTime = xBlockTime;
	m_nLastBlockingCount = 0;
}

ABlockQ::~ABlockQ() {
	// TODO Auto-generated destructor stub
}

void ABlockQ::StartBlockingQueueTasks(CCheckTask *pCheckTask, unsigned portBASE_TYPE nPriority) {
	const unsigned portBASE_TYPE uxQueueSize1 = 1, uxQueueSize5 = 5;
	const portTickType xBlockTime = ( portTickType ) 1000 / portTICK_RATE_MS;
	const portTickType xDontBlock = ( portTickType ) 0;

	// Create the first two tasks as described at the top of the file.

	// Create the queue used by the first two tasks to pass the incrementing number.
	static CQueue queue1;
	queue1.Create(uxQueueSize1, (unsigned portBASE_TYPE) sizeof(unsigned short));
	static CBlockQConsumer consumer1Task(pCheckTask, &queue1, &s_sBlockingConsumerCount[0], xBlockTime);
	static CBlockQProducer producer1Task(pCheckTask, &queue1, &s_sBlockingProducerCount[0], xDontBlock);

	//Note the producer has a lower priority than the consumer when the tasks are spawned.
	consumer1Task.Create("QConsB1", blckqSTACK_SIZE, nPriority);
	producer1Task.Create("QProdB2", blckqSTACK_SIZE, tskIDLE_PRIORITY);

	// Create the second two tasks as described at the top of the file. This uses
	// the same mechanism but reverses the task priorities.
	static CQueue queue2;
	queue2.Create(uxQueueSize1, (unsigned portBASE_TYPE) sizeof(unsigned short));
	static CBlockQConsumer consumer2Task(pCheckTask, &queue2, &s_sBlockingConsumerCount[1], xDontBlock);
	static CBlockQProducer producer2Task(pCheckTask, &queue2, &s_sBlockingProducerCount[1], xBlockTime);
	consumer2Task.Create("QProdB3", blckqSTACK_SIZE, tskIDLE_PRIORITY);
	producer2Task.Create("QConsB4", blckqSTACK_SIZE, nPriority);

	// Create the last two tasks as described above.  The mechanism is again just
	// the same.  This time both parameter structures are given a block time.
	static CQueue queue3;
	queue3.Create(uxQueueSize5, (unsigned portBASE_TYPE) sizeof(unsigned short));
	static CBlockQProducer producer3Task(pCheckTask, &queue3, &s_sBlockingProducerCount[2], xBlockTime);
	static CBlockQConsumer consumer3Task(pCheckTask, &queue3, &s_sBlockingConsumerCount[2], xBlockTime);
	producer3Task.Create("QProdB5", blckqSTACK_SIZE, tskIDLE_PRIORITY);
	consumer3Task.Create("QConsB6", blckqSTACK_SIZE, tskIDLE_PRIORITY);
}

bool ABlockQ::IsStillRunning() {
	bool bRes = true;

	// Not too worried about mutual exclusion on these variables as they are 16
	// bits and we are only reading them. We also only care to see if they have
	// changed or not.

	// return false if any are found not to have changed since the last call.

	if (*m_psCheckVariable == m_nLastBlockingCount)
		bRes = false;
	m_nLastBlockingCount = *m_psCheckVariable;

	return bRes;
}

const char*ABlockQ::GetErrorMessage() {
	return "ERROR:BlockQ";
}

// CBlockQProducer implementation.
//////////////////////////////////

CBlockQProducer::CBlockQProducer(CCheckTask *pCheckTask, CQueue *pQueue, short *psCheckVariable, portTickType xBlockTime):
		ABlockQ(pCheckTask, pQueue, psCheckVariable, xBlockTime) {
}

void CBlockQProducer::Run() {
	unsigned short usValue = 0;
	short sErrorEverOccurred = pdFALSE;

	for( ;; )
	{
		if( m_pQueue->Send((void *)&usValue, m_xBlockTime) != pdPASS )
		{
			sErrorEverOccurred = pdTRUE;
		}
		else
		{
			//We have successfully posted a message, so increment the variable
			//used to check we are still running.
			if( sErrorEverOccurred == pdFALSE )
			{
				(*m_psCheckVariable)++;
			}

			/* Increment the variable we are going to post next time round.  The
			consumer will expect the numbers to	follow in numerical order. */
			++usValue;
		}
	}
}


// CBlockQConsumer implementation.
//////////////////////////////////

CBlockQConsumer::CBlockQConsumer(CCheckTask *pCheckTask, CQueue *pQueue, short *psCheckVariable, portTickType xBlockTime):
		ABlockQ(pCheckTask, pQueue, psCheckVariable, xBlockTime) {

}

void CBlockQConsumer::Run() {
	unsigned short usData, usExpectedValue = 0;
	short sErrorEverOccurred = pdFALSE;

	for( ;; )
	{
		if( m_pQueue->Receive(&usData, m_xBlockTime) == pdPASS )
		{
			if( usData != usExpectedValue )
			{
				// Catch-up.
				usExpectedValue = usData;

				sErrorEverOccurred = pdTRUE;
			}
			else
			{
				// We have successfully received a message, so increment the
				// variable used to check we are still running.
				if( sErrorEverOccurred == pdFALSE )
				{
					(*m_psCheckVariable)++;
				}

				/* Increment the value we expect to remove from the queue next time
				round. */
				++usExpectedValue;
			}
		}
	}
}


