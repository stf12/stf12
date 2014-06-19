/**
 * @file CIrqDrivenTask.h
 * @class CIrqDrivenTask
 *
 * 	This is the task used as an example of how to synchronize a task with
 * 	an interrupt.  Each time the button interrupt gives the semaphore, this task
 * 	will unblock, increment its execution counter, then return to block again.
 *
 * @date May 30, 2014
 * @author Stefano Oliveri
 *         E-Mail: software@stf12.net
 */

#ifndef CIRQDRIVENTASK_H_
#define CIRQDRIVENTASK_H_

#include <AManagedTask.h>
#include <CBinarySemaphore.h>

using namespace freertosec::managed;
using namespace freertosec::wrapper;


class CIrqDrivenTask: public AManagedTask {
public:
	/**
	 * Specifies the semaphore that is used to demonstrate a task being
	 * synchronized with an interrupt.
	 */
	static CBinarySemaphore s_semButton;

private:
	/**
	 * Count how many time the user press the Key button.
	 */
	int m_nButtonPressedCount;

public:
	CIrqDrivenTask();
	virtual ~CIrqDrivenTask();

	/**
	 * Initialize the target button to be connected to EXTI line with interrupt generation capability.
	 *
	 * @return <code>true</code> if success, <code>false</code> otherwise
	 */
	bool HardwareInit();

	/**
	 * Allocate the software resource needed by the task. It creates the synchronization object.
	 */
	BaseType_t  OnCreate(const portCHAR * const pcName, unsigned portSHORT usStackDepth, UBaseType_t  uxPriority);

	/**
	 * Task control loop.
	 */
	void Run();
};

#endif /* CIRQDRIVENTASK_H_ */
