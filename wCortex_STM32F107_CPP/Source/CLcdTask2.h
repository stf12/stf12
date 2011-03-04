/**
 * @file CLcdTask2.h
 * @class CLcdTask2
 *
 * A simple interface to display text message on the screen of the STM32C-EVAL board.
 * It implements the singleton and the CMessageTask design pattern to match the
 * underling gate-keeper concept used by FreeRTOS.
 * This example show how to use this class:
 * \code
 * {
 *   //task code...
 *   CLcdTask2::SharedInstance().Println("Hello World!");
 *   // ...
 * }
 * \endcode
 *
 * @date   Feb 28, 2011
 * @author Stefano Oliveri (STF12.net)
 *         E-mail: software@stf12.net
 */

#ifndef CLCDTASK2_H_
#define CLCDTASK2_H_

#include <CMessageTask.h>

class CLcdTask2: public CMessageTask {
public:
	enum {
		LcdLineHeight = 24
	};

private:

	DECLARE_MESSAGE_MAP(CLcdTask2);

	/**
	 * Specify the shared instance of the class.
	 */
	static CLcdTask2 s_sharedInstance;

	/**
	 * Specifies the current line for displaying text.
	 */
	unsigned char m_nCurrentLine;

	/**
	 * Private constructor. This class implement the singleton design pattern.
	 * To access the only one instance use the method \code CLcdTask::GetSharedInstance() \endcode
	 */
	CLcdTask2();

	/**
	 * Handle the LCD2_PRINT_MSG message.
	 *
	 * @param wParam specifies the string to display.
	 */
	void OnPrintHandler(unsigned long wParam);

public:

	virtual ~CLcdTask2();

	/**
	 * Start the LCD task and get a reference to the shared instance.
	 *
	 * @return a reference to the shared instance.
	 */
	static CLcdTask2 &GetSharedInstance();

	/**
	 * It do nothing. The hardware initializations deferred until the scheduler start due to the usage of vTaskDaly function.
	 * @return <code>true</code>
	 */
	bool HardwareInit();

	/**
	 * Perform runtime initialization. Called by the framework before starting the message loop.
	 *
	 * @return <code>true</code>
	 */
	bool OnInit();

	/**
	 * Display a text message on the screen. If the message size is bigger that 20 characters,
	 * the string will be trunked due to the display resolution.
	 *
	 * @param msg specifies the string to display.
	 */
	void Println(const char *msg);

};

#endif /* CLCDTASK2_H_ */
