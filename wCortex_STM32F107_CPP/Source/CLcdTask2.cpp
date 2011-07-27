/**
 * @file CLcdTask2.h
 *
 * @date   Feb 28, 2011
 * @author Stefano Oliveri (STF12.net)
 *         E-mail: software@stf12.net
 */

#include "CLcdTask2.h"
#include "STM3210c_eval_lcd.h"

// Message definition
#define LCD2_PRINT_MSG APP_MSG + 1

CLcdTask2 CLcdTask2::s_sharedInstance;

BEGIN_MESSAGE_MAP(CMessageTask, CLcdTask2)
	MESSAGE_MAP_ENTRY(CLcdTask2, LCD2_PRINT_MSG, OnPrintHandler)
END_MESSAGE_MAP(CLcdTask2)

CLcdTask2::CLcdTask2() {
}

CLcdTask2::~CLcdTask2() {
}

CLcdTask2 &CLcdTask2::GetSharedInstance() {
	if (!s_sharedInstance.IsValid()) {
		// Create the task.
		s_sharedInstance.Create("LCD2", configMINIMAL_STACK_SIZE*2, 2);
	}

	return s_sharedInstance;
}

bool CLcdTask2::HardwareInit() {
	// The hardware initialization is delayed until the scheduler start,
	// due to the passive delay feature used during the initialization process.

	return true;
}

bool CLcdTask2::OnInit() {
	/* Initialize the LCD and display a startup message that includes the
	configured IP address. */
	STM3210C_LCD_Init();
	LCD_Clear(LCD_COLOR_WHITE);
	LCD_SetTextColor(LCD_COLOR_GREEN);
	LCD_DisplayStringLine( LCD_LINE_0, ( unsigned char * ) "  www.FreeRTOS.org" );
    LCD_SetTextColor(LCD_COLOR_BLUE);
	LCD_DisplayStringLine( LCD_LINE_1, ( unsigned char * ) "  Hello C++ World!");
	LCD_SetTextColor(LCD_COLOR_BLACK);

	m_nCurrentLine = LCD_LINE_3;

	return true;
}

void CLcdTask2::Println(const char *msg) {
	CMessage m;
	m.m_nId = LCD2_PRINT_MSG;
	m.wParam = (unsigned long)msg;
	PostMessage(&m);
}

void CLcdTask2::OnPrintHandler(unsigned long wParam) {
	/* Move on to the next line. */
	m_nCurrentLine += CLcdTask2::LcdLineHeight;
	if( m_nCurrentLine > LCD_LINE_9 )
	{
		// clear from Lin3 to Line9
		for (uint8_t line=LCD_LINE_3; line<=LCD_LINE_9; line+=CLcdTask2::LcdLineHeight)
			LCD_ClearLine(line);

		m_nCurrentLine = LCD_LINE_3;
	}

	/* Display the received text */
	LCD_DisplayStringLine( m_nCurrentLine, ( unsigned char * )wParam );
}
