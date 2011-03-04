/**
 * \file CLcdTask.h
 *
 * This class was replaced by the CLcdTask2 class that show how to use the CMessageTask
 * design pattern. This class is usefull to compare the two programming tecnique.
 *
 * A simple interface to display text message on the screen of the STM32C-EVAL board.
 * It implements the singleton design pattern that match the underling gate-keeper concept
 * used by FreeRTOS.
 * This example show how to use this class:
 * \code
 * {
 *   //task code...
 *   CLcdTask::SharedInstance().Println("Hello World!");
 *   // ...
 * }
 * \endcode
 *
 *  Created on: Jul 28, 2010
 *      Author: Stefano Oliveri
 */

#ifndef CLCDTASK_H_
#define CLCDTASK_H_

#include "AManagedTask.h"
#include "CQueue.h"

class CLcdTask: public AManagedTask {
public:
	enum {
		LcdQueueSize = 3,
		LcdLineHeight = 24
	};
private:

	/**
	 * Specify the shared instance of the class.
	 */
	static CLcdTask s_sharedInstance;

	/**
	 * Specify the synchronization queue used to serialize the access at the LCD.
	 */
	CQueue m_queue;

	/**
	 * Specifies the current line for displaying text.
	 */
	unsigned char m_nCurrentLine;

private:
	/**
	 * Private constructor. This class implement the singleton design pattern.
	 * To access the only one instance use the method \code CLcdTask::GetSharedInstance() \endcode
	 *
	 */
	CLcdTask();

public:
	virtual ~CLcdTask();

	/**
	 * Start the LCD task and get a reference to the shared instance.
	 *
	 * @return a reference to the shared instance.
	 */
	static CLcdTask &GetSharedInstance();

	/**
	 * Create the FreeRTOS task. The application should not call this method directly.
	 *
	 * @param pcName default value is LCD
	 * @param usStackDepth default value is configMINIMAL_STACK_SIZE*2
	 * @param uxPriority 2
	 *
	 */
	portBASE_TYPE Create(const portCHAR * const pcName, unsigned portSHORT usStackDepth, unsigned portBASE_TYPE uxPriority);

	/**
	 * It do nothing. The hardware initializations deferred until the scheduler start due to the usage of vTaskDaly function.
	 * @return true
	 */
	bool HardwareInit();

	/**
	 * Display a text message on the screen. If the message size is bigger that 20 characters,
	 * the string will be trunked due to the display resolution.
	 *
	 * @param msg specifies the string to display.
	 */
	void Println(const char *msg);

	/**
	 * Task control function.
	 */
	void Run();
};

#endif /* CLCDTASK_H_ */
