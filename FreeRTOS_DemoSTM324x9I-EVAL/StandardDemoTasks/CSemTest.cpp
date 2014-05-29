/**
 * @file   CSemTest.cpp
 *
 * @date   Aug 19, 2010
 * @author oliveris
 *         E-mail: software@stf12.net
 */

#include "CSemTest.h"

/* The value to which the shared variables are counted. */
#define semtstBLOCKING_EXPECTED_VALUE		( ( unsigned long ) 0xfff )
#define semtstNON_BLOCKING_EXPECTED_VALUE	( ( unsigned long ) 0xff  )

#define semtstSTACK_SIZE			configMINIMAL_STACK_SIZE
#define semtstNUM_TASKS				( 4 )
#define semtstDELAY_FACTOR			( ( portTickType ) 10 )

CSemTest::CSemTest(CCheckTask *pCheckTask): ICommonDemoTask(pCheckTask) {
	m_pSharedVariable = NULL;
	m_nBlockTime = 0;
	m_pSemaphore = NULL;
	m_nCheckVariable = m_nLastCheckVariable = 0;
}

CSemTest::~CSemTest() {
	// TODO Auto-generated destructor stub
}

void CSemTest::StartSemTestTasks(CCheckTask *pCheckTask, unsigned portBASE_TYPE nPriority ) {
	const portTickType xBlockTime = ( portTickType ) 100;


	/* Create the semaphore and the first two tasks. */
	static CBinarySemaphore semaphore1;
	static CSemTest polSem1(pCheckTask);
	static CSemTest polSem2(pCheckTask);

	if ( semaphore1.Create() ) {
		polSem1.m_pSemaphore = &semaphore1;
		polSem2.m_pSemaphore = &semaphore1;
		//Create the variable which is to be shared by the first two tasks.
		polSem1.m_pSharedVariable = (unsigned long *) pvPortMalloc(sizeof(unsigned long));
		polSem2.m_pSharedVariable = polSem1.m_pSharedVariable;
		//Initialize the share variable to the value the tasks expect.
		*polSem1.m_pSharedVariable = semtstNON_BLOCKING_EXPECTED_VALUE;
		//The first two tasks do not block on semaphore calls.
		polSem1.m_nBlockTime = 0;
		polSem2.m_nBlockTime = 0;

		polSem1.Create("PolSEM1", semtstSTACK_SIZE, tskIDLE_PRIORITY);
		polSem2.Create("PolSEM2", semtstSTACK_SIZE, tskIDLE_PRIORITY);
	}

	// Do exactly the same to create the second set of tasks, only this time
	// provide a block time for the semaphore calls.
	static CBinarySemaphore semaphore2;
	static CSemTest polSem3(pCheckTask);
	static CSemTest polSem4(pCheckTask);

	if (semaphore2.Create()) {
		polSem3.m_pSemaphore = &semaphore2;
		polSem4.m_pSemaphore = &semaphore2;
		polSem3.m_pSharedVariable = (unsigned long *) pvPortMalloc(sizeof(unsigned long));
		polSem4.m_pSharedVariable = polSem3.m_pSharedVariable;
		//Initialize the share variable to the value the tasks expect.
		*polSem3.m_pSharedVariable = semtstBLOCKING_EXPECTED_VALUE;
		polSem3.m_nBlockTime = xBlockTime / portTICK_RATE_MS;
		polSem4.m_nBlockTime = xBlockTime / portTICK_RATE_MS;

		polSem3.Create("BlkSEM1", semtstSTACK_SIZE, nPriority);
		polSem4.Create("BlkSEM2", semtstSTACK_SIZE, nPriority);
	}

	// Use the standard FreeRTOS API to add the semaphore to the register queue.
	vQueueAddToRegistry( semaphore1, "Counting_Sem_1" );
	vQueueAddToRegistry( semaphore2, "Counting_Sem_2" );
}

void CSemTest::Run() {

	volatile unsigned long ulExpectedValue;
	unsigned long ulCounter;
	short sError = pdFALSE;


	/* If we are blocking we use a much higher count to ensure loads of context
		switches occur during the count. */
	if( m_nBlockTime > ( portTickType ) 0 )
	{
		ulExpectedValue = semtstBLOCKING_EXPECTED_VALUE;
	}
	else
	{
		ulExpectedValue = semtstNON_BLOCKING_EXPECTED_VALUE;
	}

	for( ;; )
	{
		/* Try to obtain the semaphore. */
		if( m_pSemaphore->Take(m_nBlockTime) == pdPASS )
		{
			/* We have the semaphore and so expect any other tasks using the
				shared variable to have left it in the state we expect to find
				it. */
			if( *m_pSharedVariable != ulExpectedValue )
			{
				sError = pdTRUE;
			}

			/* Clear the variable, then count it back up to the expected value
				before releasing the semaphore.  Would expect a context switch or
				two during this time. */
			for( ulCounter = ( unsigned long ) 0; ulCounter <= ulExpectedValue; ulCounter++ )
			{
				*m_pSharedVariable = ulCounter;
				if( *m_pSharedVariable != ulCounter )
				{
					sError = pdTRUE;
				}
			}

			/* Release the semaphore, and if no errors have occurred increment the check
				variable. */
			if(	m_pSemaphore->Give() == pdFALSE )
			{
				sError = pdTRUE;
			}

			if( sError == pdFALSE )
			{
				++m_nCheckVariable;
			}

			/* If we have a block time then we are running at a priority higher
				than the idle priority.  This task takes a long time to complete
				a cycle	(deliberately so to test the guarding) so will be starving
				out lower priority tasks.  Block for some time to allow give lower
				priority tasks some processor time. */
			Delay( m_nBlockTime * semtstDELAY_FACTOR );
		}
		else
		{
			if( m_nBlockTime == ( portTickType ) 0 )
			{
				/* We have not got the semaphore yet, so no point using the
					processor.  We are not blocking when attempting to obtain the
					semaphore. */
				Yeld();
			}
		}
	}

}

bool CSemTest::IsStillRunning() {
	bool bRes = true;

	if( m_nLastCheckVariable == m_nCheckVariable )
	{
		bRes = pdFALSE;
	}
	m_nLastCheckVariable = m_nCheckVariable;

	return bRes;
}
