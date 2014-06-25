/**
 * @file   CInteger.cpp
 *
 * @date   Aug 6, 2010
 * @author Stefano Oliveri
 *         E-mail: software@stf12.net
 */

#include <FreeRTOS_EC.h>
#include "CInteger.h"

/* The constants used in the calculation. */
#define intgCONST1				( ( long ) 123 )
#define intgCONST2				( ( long ) 234567 )
#define intgCONST3				( ( long ) -3 )
#define intgCONST4				( ( long ) 7 )
#define intgEXPECTED_ANSWER		( ( ( intgCONST1 + intgCONST2 ) * intgCONST3 ) / intgCONST4 )

#define intgSTACK_SIZE			configMINIMAL_STACK_SIZE

/* As this is the minimal version, we will only create one task. */
#define intgNUMBER_OF_TASKS		( 1 )

CInteger::CInteger(CCheckTask *pCheckTask):
	AManagedTask(pCheckTask->GetContext()),
	ICommonDemoTask(pCheckTask)
{
	m_nCheck = true;
}

CInteger::~CInteger() {
	// TODO Auto-generated destructor stub
}

void CInteger::Run(){
	/* These variables are all effectively set to constants so they are volatile to
	ensure the compiler does not just get rid of them. */
	volatile long lValue;
	short sError = pdFALSE;

	/* Keep performing a calculation and checking the result against a constant. */
	for( ;; )
	{
		/* Perform the calculation.  This will store partial value in
		registers, resulting in a good test of the context switch mechanism. */
		lValue = intgCONST1;
		lValue += intgCONST2;

		/* Yield in case cooperative scheduling is being used. */
		#if configUSE_PREEMPTION == 0
		{
			Yeld();
		}
		#endif

		/* Finish off the calculation. */
		lValue *= intgCONST3;
		lValue /= intgCONST4;

		/* If the calculation is found to be incorrect we stop setting the
		m_nCheck variable so the check task can see an error has
		occurred. */
		if( lValue != intgEXPECTED_ANSWER ) /*lint !e774 volatile used to prevent this being optimised out. */
		{
			sError = pdTRUE;
		}

		if( sError == pdFALSE )
		{
			/* We have not encountered any errors, so set the flag that show
			we are still executing.  This will be periodically cleared by
			the check task. */
			EnterCritical();
				m_nCheck = true;
			ExitCritical();
		}

		/* Yield in case cooperative scheduling is being used. */
		#if configUSE_PREEMPTION == 0
		{
			Yeld();
		}
		#endif
	}
}

bool CInteger::IsStillRunning(){
	bool xReturn = true;

	/* Check the maths tasks are still running by ensuring their check variables
	are still being set to true. */
	if( !m_nCheck )
	{
		/* The check has not incremented so an error exists. */
		xReturn = false;
	}

	/* Reset the check variable so we can tell if it has been set by
	the next time around. */
	m_nCheck = false;

	return xReturn;
}

const char*CInteger::GetErrorMessage() {
	return "Error:IntMath";
}

void CInteger::StartIntegerMathTasks(CCheckTask *pCheckTask, UBaseType_t nPriority ) {
	for (int i=0; i<intgNUMBER_OF_TASKS; ++i) {
		CInteger *pTask = new CInteger(pCheckTask);
		pTask->Create("IntMath", intgSTACK_SIZE, nPriority);
	}
}

