/**
 * @file   CInteger.h
 * @class  CInteger
 * @ingroup FreeRTOS_STANDARD_DEMO
 *
 * This version of integer. c is for use on systems that have limited stack
 * space and no display facilities.  The complete version can be found in
 * the Demo/Common/Full directory.
 *
 * As with the full version, the tasks created in this file are a good test
 * of the scheduler context switch mechanism.  The processor has to access
 * 32bit variables in two or four chunks (depending on the processor).  The low
 * priority of these tasks means there is a high probability that a context
 * switch will occur mid calculation.  See flop. c documentation for
 * more information.
 *
 * @date   Aug 6, 2010
 * @author Stefano Oliveri
 *         E-mail: software@stf12.net
 */

#ifndef CINTEGER_H_
#define CINTEGER_H_

//#include "AManagedTask.h"
#include "CCheckTask.h"

class CInteger: public AManagedTask, public ICommonDemoTask {

	/**
	 * Variables that are set to true within the calculation task to indicate
	 * that the task is still executing.  The check task sets the variable back to
	 * false, flagging an error if the variable is still false the next time it
	 * is called.
	 */
	BaseType_t m_nCheck;

public:
	CInteger(CCheckTask *pCheckTask);
	virtual ~CInteger();

	void Run();

	bool IsStillRunning();
	const char*GetErrorMessage();

	static void StartIntegerMathTasks(CCheckTask *pCheckTask, UBaseType_t nPriority );
};

#endif /* CINTEGER_H_ */
