/*
 * CQueueSetTest.cpp
 *
 *  Created on: Jun 12, 2014
 *      Author: stefano
 */

#include <FreeRTOS_EC.h>
#include <CQueueSetTest.h>
#include <limits.h>
#include <stdio.h>

/* The length of each created queue. */
#define queuesetQUEUE_LENGTH	3

/* Block times used in this demo.  A block time or 0 means "don't block". */
#define queuesetSHORT_DELAY	200
#define queuesetDONT_BLOCK 0

/* Messages are sent in incrementing order from both a task and an interrupt.
The task sends values in the range 0 to 0xfffe, and the interrupt sends values
in the range of 0xffff to ULONG_MAX. */
#define queuesetINITIAL_ISR_TX_VALUE 0xffffUL

/* For test purposes the priority of the sending task is changed after every
queuesetPRIORITY_CHANGE_LOOPS number of values are sent to a queue. */
#define queuesetPRIORITY_CHANGE_LOOPS	( ( queuesetNUM_QUEUES_IN_SET * queuesetQUEUE_LENGTH ) * 2 )

/* The ISR sends to the queue every queuesetISR_TX_PERIOD ticks. */
#define queuesetISR_TX_PERIOD	( 100UL )

/* A delay inserted when the Tx task changes its priority to be above the idle
task priority to ensure the idle priority tasks get some CPU time before the
next iteration of the queue set Tx task. */
#define queuesetTX_LOOP_DELAY	( 200 / portTICK_PERIOD_MS )

/* The allowable maximum deviation between a received value and the expected
received value.  A deviation will occur when data is received from a queue
inside an ISR in between a task receiving from a queue and the task checking
the received value. */
#define queuesetALLOWABLE_RX_DEVIATION 3

/* Ignore values that are at the boundaries of allowable values to make the
testing of limits easier (don't have to deal with wrapping values). */
#define queuesetIGNORED_BOUNDARY	( queuesetALLOWABLE_RX_DEVIATION * 2 )


// CQueueSet class implementation
//*******************************

CQueueSet CQueueSetTest::s_xQueueSet;

unsigned long CQueueSetTest::s_ulNextRand = 0;

volatile unsigned long CQueueSetTest::s_ulCycleCounter = 0UL;

CQueueSetTest::CQueueSetTest() {
	// TODO Auto-generated constructor stub

}

CQueueSetTest::~CQueueSetTest() {
	// TODO Auto-generated destructor stub
}

void CQueueSetTest::StartTestTasks(CCheckTask *pCheckTask, UBaseType_t uxPriority) {
	CQSTxTask *pTx = new CQSTxTask(pCheckTask);
	CQSRxTask *pRx = new CQSRxTask(pCheckTask, pTx);

	/* Ensure the queues are created and the queue set configured before the
	sending task is unsuspended.

	First Create the queue set such that it will be able to hold a message for
	every space in every queue in the set. */
	s_xQueueSet.CreateSet(queuesetNUM_QUEUES_IN_SET * queuesetQUEUE_LENGTH);

	/* Create the tasks. */
	BaseType_t res = pdPASS;
	res &= pTx->Create("SetTx", 2*configMINIMAL_STACK_SIZE, uxPriority);
	res &= pRx->Create("SetRx", configMINIMAL_STACK_SIZE, uxPriority);

	/* It is important that the sending task does not attempt to write to a
	queue before the queue has been created.  It is therefore placed into the
	suspended state before the scheduler has started.  It is resumed by the
	receiving task after the receiving task has created the queues and added the
	queues to the queue set. */
	pTx->Suspend();
}

BaseType_t CQueueSetTest::CheckReceivedValue(unsigned long ulReceived) {
	static unsigned long ulExpectedReceivedFromTask = 0, ulExpectedReceivedFromISR = queuesetINITIAL_ISR_TX_VALUE;

	BaseType_t res = pdPASS;

	/* Values are received in tasks and interrupts.  It is likely that the
		receiving task will sometimes get preempted by the receiving interrupt
		between reading a value from the queue and calling this function.  When
		that happens, if the receiving interrupt calls this function the values
		will get passed into this function slightly out of order.  For that
		reason the value passed in is tested against a small range of expected
		values, rather than a single absolute value.  To make the range testing
		easier values in the range limits are ignored. */

	/* If the received value is equal to or greater than
		queuesetINITIAL_ISR_TX_VALUE then it was sent by an ISR. */
	if( ulReceived >= queuesetINITIAL_ISR_TX_VALUE )
	{
		/* The value was sent from the ISR. */
		if( ( ulReceived - queuesetINITIAL_ISR_TX_VALUE ) < queuesetIGNORED_BOUNDARY )
		{
			/* The value received is at the lower limit of the expected range.
				Don't test it and expect to receive one higher next time. */
		}
		else if( ( ULONG_MAX - ulReceived ) <= queuesetIGNORED_BOUNDARY )
		{
			/* The value received is at the higher limit of the expected range.
				Don't test it and expect to wrap soon. */
		}
		else
		{
			/* Check the value against its expected value range. */
			if(CQueueSetTest::CheckReceivedValueWithinExpectedRange(ulReceived, ulExpectedReceivedFromISR) != pdPASS )
			{
				res = pdFAIL;
			}
		}

		configASSERT( res );

		/* It is expected to receive an incrementing number. */
		ulExpectedReceivedFromISR++;
		if( ulExpectedReceivedFromISR == 0 )
		{
			ulExpectedReceivedFromISR = queuesetINITIAL_ISR_TX_VALUE;
		}
	}
	else
	{
		/* The value was sent from the Tx task. */
		if( ulReceived < queuesetIGNORED_BOUNDARY )
		{
			/* The value received is at the lower limit of the expected range.
				Don't test it, and expect to receive one higher next time. */
		}
		else if( ( ( queuesetINITIAL_ISR_TX_VALUE - 1 ) - ulReceived ) <= queuesetIGNORED_BOUNDARY )
		{
			/* The value received is at the higher limit of the expected range.
				Don't test it and expect to wrap soon. */
		}
		else
		{
			/* Check the value against its expected value range. */
			if( CQueueSetTest::CheckReceivedValueWithinExpectedRange(ulReceived, ulExpectedReceivedFromTask) != pdPASS )
			{
				res = pdFAIL;
			}
		}

		configASSERT( res );

		/* It is expected to receive an incrementing number. */
		ulExpectedReceivedFromTask++;
		if( ulExpectedReceivedFromTask >= queuesetINITIAL_ISR_TX_VALUE )
		{
			ulExpectedReceivedFromTask = 0;
		}
	}

	return res;
}

BaseType_t CQueueSetTest::CheckReceivedValueWithinExpectedRange(unsigned long ulReceived, unsigned long ulExpectedReceived) {
	BaseType_t xReturn = pdPASS;

	if( ulReceived > ulExpectedReceived )
	{
		configASSERT( ( ulReceived - ulExpectedReceived ) <= queuesetALLOWABLE_RX_DEVIATION );
		if( ( ulReceived - ulExpectedReceived ) > queuesetALLOWABLE_RX_DEVIATION )
		{
			xReturn = pdFALSE;
		}
	}
	else
	{
		configASSERT( ( ulExpectedReceived - ulReceived ) <= queuesetALLOWABLE_RX_DEVIATION );
		if( ( ulExpectedReceived - ulReceived ) > queuesetALLOWABLE_RX_DEVIATION )
		{
			xReturn = pdFALSE;
		}
	}

	return xReturn;
}

unsigned long CQueueSetTest::Rand()
{
	s_ulNextRand = ( s_ulNextRand * 1103515245UL ) + 12345UL;
    return ( s_ulNextRand / 65536UL ) % 32768UL;
}

void CQueueSetTest::SRand(unsigned long ulSeed)
{
	s_ulNextRand = ulSeed;
}


// QSTxTask implementation
// ***********************

CQSTxTask *CQSTxTask::s_pxTxTask = NULL;

volatile unsigned long CQSTxTask::s_ulISRTxValue = queuesetINITIAL_ISR_TX_VALUE;

CQSTxTask::CQSTxTask(CCheckTask *pCheckTask):
		AManagedTask(pCheckTask->GetContext()),
		ICommonDemoTask(pCheckTask)
{
	m_xTasksStatus = pdPASS;
	for (int i=0; i<queuesetNUM_QUEUES_IN_SET; i++) {
		m_ulQueueUsedCounter[i] = 0;
		m_strQueueNames[i] = new char[13];
	}
}

CQSTxTask::~CQSTxTask() {

}

BaseType_t  CQSTxTask::OnCreate(const portCHAR * const pcName, unsigned portSHORT usStackDepth, UBaseType_t  uxPriority) {

	/* Remove compiler warnings. */
	(void) pcName;
	(void) usStackDepth;
	(void) uxPriority;

	for( BaseType_t x = 0; x < queuesetNUM_QUEUES_IN_SET; x++ )
	{
		/* Create the queue and add it to the set.  The queue is just holding
		unsigned long value. */
		m_xQueues[x].Create(queuesetQUEUE_LENGTH, sizeof( unsigned long ));
		configASSERT( m_xQueues[x] );
		if ( CQueueSetTest::s_xQueueSet.AddToSet(m_xQueues[x]) != pdPASS )
		{
			m_xTasksStatus = pdFAIL;
		}
		else
		{
			/* The queue has now been added to the queue set and cannot be added to
			another. */
			if( CQueueSetTest::s_xQueueSet.AddToSet(m_xQueues[x]) != pdFAIL )
			{
				m_xTasksStatus = pdFAIL;
			}
		}

		// finally add the queue to the registry.
		sprintf(m_strQueueNames[x], "QueueSet_tx_%i", (int)x);
		m_xQueues[x].AddToRegistry(m_strQueueNames[x]);
	}

	return m_xTasksStatus;
}

void CQSTxTask::Run() {
	unsigned long ulTaskTxValue = 0, ulIndexOfQueueToWriteTo;

	/* Seed mini pseudo random number generator. */
	CQueueSetTest::SRand((unsigned long)&ulTaskTxValue );

	for( ;; )
	{
		/* Generate the index for the queue to which a value is to be sent. */
		ulIndexOfQueueToWriteTo = CQueueSetTest::Rand() % queuesetNUM_QUEUES_IN_SET;

		/* Note which index is being written to to ensure all the queues are
			used. */
		( m_ulQueueUsedCounter[ ulIndexOfQueueToWriteTo ] )++;

		/* Send to the queue to unblock the task that is waiting for data to
			arrive on a queue within the queue set to which this queue belongs. */
		if (m_xQueues[ulIndexOfQueueToWriteTo].SendToBack(&ulTaskTxValue, portMAX_DELAY) != pdPASS )
		{
			/* The send should always pass as an infinite block time was
				used. */
			m_xTasksStatus = pdFAIL;
		}

#if( configUSE_PREEMPTION == 0 )
		Yeld();
#endif

		ulTaskTxValue++;

		/* If the Tx value has reached the range used by the ISR then set it
			back to 0. */
		if( ulTaskTxValue == queuesetINITIAL_ISR_TX_VALUE )
		{
			ulTaskTxValue = 0;
		}

		/* Increase test coverage by occasionally change the priorities of the
			two tasks relative to each other. */
//		prvChangeRelativePriorities();
	}

}

bool CQSTxTask::IsStillRunning() {
	return m_xTasksStatus == pdPASS;
}

const char*CQSTxTask::GetErrorMessage() {
	return "CQueueSetTest:Tx:Err";
}

bool CQSTxTask::PreTest() {
	unsigned long ulValueToSend = 0;

	/* Attempt to remove a queue from a queue set it does not belong
	to (NULL being passed as the queue set in this case). */
	if( xQueueRemoveFromSet( m_xQueues[ 0 ], NULL ) != pdFAIL )
	{
		/* It is not possible to successfully remove a queue from a queue
		set it does not belong to. */
		m_xTasksStatus = pdFAIL;
		return false;
	}

	/* Attempt to remove a queue from the queue set it does belong to. */
	if( CQueueSetTest::s_xQueueSet.RemoveFromSet( m_xQueues[ 0 ]) != pdPASS )
	{
		/* It should be possible to remove the queue from the queue set it
		does belong to. */
		m_xTasksStatus = pdFAIL;
		return false;
	}

	/* Add an item to the queue before attempting to add it back into the
	set. */
	m_xQueues[0].Send(( void * ) &ulValueToSend, 0);
	if( CQueueSetTest::s_xQueueSet.AddToSet( m_xQueues[ 0 ] ) != pdFAIL )
	{
		/* Should not be able to add a non-empty queue to a set. */
		m_xTasksStatus = pdFAIL;
		return false;
	}

	/* Remove the item from the queue before adding the queue back into the
	set so the dynamic tests can begin. */
	m_xQueues[0].Receive(&ulValueToSend, 0);
	if( CQueueSetTest::s_xQueueSet.AddToSet( m_xQueues[ 0 ] ) != pdPASS )
	{
		/* If the queue was successfully removed from the queue set then it
		should be possible to add it back in again. */
		m_xTasksStatus = pdFAIL;
		return false;
	}

	/* The task that sends to the queues is not running yet, so attempting to
	read from the queue set should fail. */
	if( CQueueSetTest::s_xQueueSet.SelectFromSet( queuesetSHORT_DELAY ) != NULL )
	{
		m_xTasksStatus = pdFAIL;
		return false;
	}

	return true;
}

void CQSTxTask::ReceiveFromQueueInSetFromISR() {
	unsigned long ulReceived;
	static CQueue xActivatedQueue;
	QueueSetMemberHandle_t xActivatedQueueHandle;

	/* See if any of the queues in the set contain data. */

	xActivatedQueueHandle = CQueueSetTest::s_xQueueSet.SelectFromSetFromISR();
	if( xActivatedQueueHandle != NULL )
	{
		xActivatedQueue.Attach(xActivatedQueueHandle);
		/* Reading from the queue for test purposes only. */
		if( xActivatedQueue.ReceiveFromISR( &ulReceived, NULL ) != pdPASS )
		{
			/* Data should have been available as the handle was returned from xQueueSelectFromSetFromISR(). */
			m_xTasksStatus = pdFAIL;
		}

		/* Ensure the value received was the value expected. */
		CQueueSetTest::CheckReceivedValue( ulReceived );

		xActivatedQueue.Detach();
	}

}

void CQSTxTask::SendToQueueInSetFromISR() {
	static BaseType_t xQueueToWriteTo = 0;

	if (m_xQueues[xQueueToWriteTo].SendFromISR((void*)&s_ulISRTxValue, NULL) == pdPASS)
	{
		s_ulISRTxValue++;

		/* If the Tx value has wrapped then set it back to its initial	value. */
		if( s_ulISRTxValue == 0UL )
		{
			s_ulISRTxValue = queuesetINITIAL_ISR_TX_VALUE;
		}

		/* Use a different queue next time. */
		xQueueToWriteTo++;
		if( xQueueToWriteTo >= queuesetNUM_QUEUES_IN_SET )
		{
			xQueueToWriteTo = 0;
		}
	}
}

void CQSTxTask::AccessQueueSetFromISR() {
	static unsigned long ulCallCount = 0;

	/* xSetupComplete is set to pdTRUE when the queues have been created and
		are available for use. */
	if( s_pxTxTask != NULL )
	{
		/* It is intended that this function is called from the tick hook
			function, so each call is one tick period apart. */
		ulCallCount++;
		if( ulCallCount > queuesetISR_TX_PERIOD )
		{
			ulCallCount = 0;

			/* First attempt to read from the queue set. */
			s_pxTxTask->ReceiveFromQueueInSetFromISR();

			/* Then write to the queue set. */
			s_pxTxTask->SendToQueueInSetFromISR();
		}
	}
}

// QSRxTask implementation
// ***********************

CQSRxTask::CQSRxTask(CCheckTask *pCheckTask, CQSTxTask *pTxTask):
		AManagedTask(pCheckTask->GetContext()),
		ICommonDemoTask(pCheckTask)
{
	m_pTxTask = pTxTask;
	m_xTasksStatus = pdPASS;
}

CQSRxTask::~CQSRxTask() {

}

void CQSRxTask::Run() {

	unsigned long ulReceived;
	QueueHandle_t xActivatedQueue;

	m_pTxTask->PreTest();

	// Resume the task that writes to the queues.
	m_pTxTask->Resume();

	/* Let the ISR access the queues also. */
	m_pTxTask->s_pxTxTask = m_pTxTask;


	for( ;; )
	{
		/* Wait for a message to arrive on one of the queues in the set. */
		xActivatedQueue = CQueueSetTest::s_xQueueSet.SelectFromSet(portMAX_DELAY);
		configASSERT( xActivatedQueue );

		if( xActivatedQueue == NULL )
		{
			/* This should not happen as an infinite delay was used. */
			m_xTasksStatus = pdFAIL;
		}
		else
		{
			// Attach the handle to a quqe object
			m_xQueue.Attach(xActivatedQueue);
			/* Reading from the queue should pass with a zero block time as
			this task will only run when something has been posted to a task
			in the queue set. */
			if (m_xQueue.Receive(&ulReceived, queuesetDONT_BLOCK) != pdPASS)
			{
				m_xTasksStatus = pdFAIL;
			}

			/* Ensure the value received was the value expected.  This function
			manipulates file scope data and is also called from an ISR, hence
			the critical section. */
			EnterCritical();
			{
				m_xTasksStatus = CQueueSetTest::CheckReceivedValue(ulReceived);
			}
			ExitCritical();
			m_xQueue.Detach();
		}

		if( m_xTasksStatus == pdPASS )
		{
			CQueueSetTest::s_ulCycleCounter++;
		}
	}

}

BaseType_t  CQSRxTask::OnCreate(const portCHAR * const pcName, unsigned portSHORT usStackDepth, UBaseType_t  uxPriority) {

	/* Remove compiler warnings. */
	(void) pcName;
	(void) usStackDepth;
	(void) uxPriority;

	return pdTRUE;
}


bool CQSRxTask::IsStillRunning() {
	return m_xTasksStatus == true;
}

const char*CQSRxTask::GetErrorMessage() {
	return "CQueueSetTest:Rx:Err";
}
