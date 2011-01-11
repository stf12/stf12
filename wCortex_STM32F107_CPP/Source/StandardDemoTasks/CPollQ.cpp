/**
 * @file   CPollQ.cpp
 *
 * @date   Aug 17, 2010
 * @author Stefano Oliveri
 *         E-mail: software@stf12.net
 */

#include <assert.h>
#include "CPollQ.h"

#define pollqSTACK_SIZE			configMINIMAL_STACK_SIZE
#define pollqQUEUE_SIZE			( 10 )
#define pollqPRODUCER_DELAY		( ( portTickType ) 200 / portTICK_RATE_MS )
#define pollqCONSUMER_DELAY		( pollqPRODUCER_DELAY - ( portTickType ) ( 20 / portTICK_RATE_MS ) )
#define pollqNO_DELAY			( ( portTickType ) 0 )
#define pollqVALUES_TO_PRODUCE	( ( signed portBASE_TYPE ) 3 )
#define pollqINITIAL_VALUE		( ( signed portBASE_TYPE ) 0 )


// APollQ implementation
////////////////////////

APollQ::APollQ(CCheckTask *pCheckTask, CQueue *pQueue): ICommonDemoTask(pCheckTask) {
	assert(pQueue->IsValid());

	m_pQueue = pQueue;
	m_nPollingCount = pollqINITIAL_VALUE;
}

APollQ::~APollQ() {
	// TODO Auto-generated destructor stub
}

bool APollQ::IsStillRunning() {
	bool bReturn;

	/* Check both the consumer and producer poll count to check they have both
		been changed since out last trip round.  We do not need a critical section
		around the check variables as this is called from a higher priority than
		the other tasks that access the same variables. */
	if( m_nPollingCount == pollqINITIAL_VALUE )
	{
		bReturn = false;
	}
	else
	{
		bReturn = true;
	}

	/* Set the check variables back down so we know if they have been
		incremented the next time around. */
	m_nPollingCount = pollqINITIAL_VALUE;

	return bReturn;
}

const char*APollQ::GetErrorMessage() {
	return "ERROR:PollQ";
}

void APollQ::StartPolledQueueTasks(CCheckTask *pCheckTask, unsigned portBASE_TYPE nPriority ) {
	static CQueue queue;

	/* Create the queue used by the producer and consumer. */
	queue.Create(pollqQUEUE_SIZE, (unsigned portBASE_TYPE) sizeof(unsigned short));

	/* vQueueAddToRegistry() adds the queue to the queue registry, if one is
	in use.  The queue registry is provided as a means for kernel aware
	debuggers to locate queues and has no purpose if a kernel aware debugger
	is not being used.  The call to vQueueAddToRegistry() will be removed
	by the pre-processor if configQUEUE_REGISTRY_SIZE is not defined or is
	defined to be less than 1. */
	queue.AddToRegistry((signed char *) "Poll_Test_Queue");

	/* Spawn the producer and consumer. */
	static CPollQConsumer consumer(pCheckTask, &queue);
	consumer.Create("QConsNB", pollqSTACK_SIZE, nPriority);
	static CPollQProducer producer(pCheckTask, &queue);
	producer.Create("QProdNB", pollqSTACK_SIZE, nPriority);
}

// CPollQProducer implementation
////////////////////////////////

CPollQProducer::CPollQProducer(CCheckTask *pCheckTask, CQueue *pQueue): APollQ(pCheckTask, pQueue) {

}

void CPollQProducer::Run() {
	unsigned short usValue = ( unsigned short ) 0;
	signed portBASE_TYPE xError = pdFALSE, xLoop;

	for( ;; )
	{
		for( xLoop = 0; xLoop < pollqVALUES_TO_PRODUCE; xLoop++ )
		{
			/* Send an incrementing number on the queue without blocking. */
			if( m_pQueue->Send((void*)&usValue, pollqNO_DELAY ) != pdPASS )
			{
				/* We should never find the queue full so if we get here there
				has been an error. */
				xError = pdTRUE;
			}
			else
			{
				if( xError == pdFALSE )
				{
					/* If an error has ever been recorded we stop incrementing the
					check variable. */
					EnterCritical();
						m_nPollingCount++;
					ExitCritical();
				}
					/* Update the value we are going to post next time around. */
				usValue++;
			}
		}
		/* Wait before we start posting again to ensure the consumer runs and
		empties the queue. */
		Delay(pollqPRODUCER_DELAY);
	}
}


// CPollQConsumer implementation
////////////////////////////////

CPollQConsumer::CPollQConsumer(CCheckTask *pCheckTask, CQueue *pQueue): APollQ(pCheckTask, pQueue) {

}

void CPollQConsumer::Run() {
	unsigned short usData, usExpectedValue = ( unsigned short ) 0;
	signed portBASE_TYPE xError = pdFALSE;

	for( ;; )
	{
		/* Loop until the queue is empty. */
		while( m_pQueue->MessagesWaiting() )
		{
			if( m_pQueue->Receive(&usData, pollqNO_DELAY ) == pdPASS )
			{
				if( usData != usExpectedValue )
				{
					/* This is not what we expected to receive so an error has
						occurred. */
					xError = pdTRUE;

					/* Catch-up to the value we received so our next expected
						value should again be correct. */
					usExpectedValue = usData;
				}
				else
				{
					if( xError == pdFALSE )
					{
						/* Only increment the check variable if no errors have
							occurred. */
						EnterCritical();
							m_nPollingCount++;
						ExitCritical();
					}
				}

				/* Next time round we would expect the number to be one higher. */
				usExpectedValue++;
			}
		}

		/* Now the queue is empty we block, allowing the producer to place more
			items in the queue. */
		Delay(pollqCONSUMER_DELAY);
	}
}
