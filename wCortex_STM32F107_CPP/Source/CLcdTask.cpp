/*
 * CLcdTask.cpp
 *
 *  Created on: Jul 28, 2010
 *      Author: oliveris
 */

#include "CLcdTask.h"

//#include <stdio.h>
#include "STM3210c_eval_lcd.h"
#include "CFreeRTOS.h"


CLcdTask CLcdTask::s_sharedInstance;

CLcdTask::CLcdTask() {
	m_nCurrentLine = LCD_LINE_3;
}

CLcdTask::~CLcdTask() {
	// TODO Auto-generated destructor stub
}

CLcdTask &CLcdTask::GetSharedInstance() {
	if (!s_sharedInstance.IsValid()) {
		// Create the task.
		s_sharedInstance.Create("LCD", configMINIMAL_STACK_SIZE*2, 2);
	}

	return s_sharedInstance;
}

portBASE_TYPE CLcdTask::Create(const portCHAR * const pcName, unsigned portSHORT usStackDepth, unsigned portBASE_TYPE uxPriority) {
	portBASE_TYPE res;
	if ( pdTRUE == (res = AManagedTask::Create(pcName, usStackDepth, uxPriority)) ) {
		res = m_queue.Create(CLcdTask::LcdQueueSize, sizeof(char[20])).IsValid();
	}

	return res;
}

bool CLcdTask::HardwareInit() {
	// The hardware initialization is delayed until the scheduler start,
	// due to the passive delay feature used during the initialization process.

	return true;
}

void CLcdTask::Println(const char *msg) {
	m_queue.Send(&msg, 0);
}

void CLcdTask::Run() {
	unsigned char *pucMessage;

	/* Initialize the LCD and display a startup message that includes the
	configured IP address. */
	STM3210C_LCD_Init();
	LCD_Clear(LCD_COLOR_WHITE);
	LCD_SetTextColor(LCD_COLOR_GREEN);
	LCD_DisplayStringLine( LCD_LINE_0, ( unsigned char * ) "  www.FreeRTOS.org" );
    LCD_SetTextColor(LCD_COLOR_BLUE);
	LCD_DisplayStringLine( LCD_LINE_1, ( unsigned char * ) "  Hello C++ World!");
	LCD_SetTextColor(LCD_COLOR_BLACK);

	for( ;; )
	{
		/* Wait for a message to arrive to be displayed. */
		m_queue.Receive(&pucMessage, portMAX_DELAY );

		/* Move on to the next line. */
		m_nCurrentLine += CLcdTask::LcdLineHeight;
		if( m_nCurrentLine > LCD_LINE_9 )
		{
			// clear from Lin3 to Line9
			for (uint8_t line=LCD_LINE_3; line<=LCD_LINE_9; line+=CLcdTask::LcdLineHeight)
				LCD_ClearLine(line);

			m_nCurrentLine = LCD_LINE_3;
		}

		/* Display the received text */
		LCD_DisplayStringLine( m_nCurrentLine, ( unsigned char * )pucMessage );
	}

}
