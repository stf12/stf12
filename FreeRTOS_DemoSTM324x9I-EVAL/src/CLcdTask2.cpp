/**
 * @file CLcdTask2.h
 *
 * @date   Feb 28, 2011
 * @author Stefano Oliveri (STF12.net)
 *         E-mail: software@stf12.net
 */

#include "CLcdTask2.h"
#include "stm324x9i_eval.h"
#include "stm324x9i_eval_lcd.h"

// Message definition
#define LCD2_PRINT_MSG APP_MSG + 1

/* Private macro */
#define MESSAGE1   "          STM32F439X          "
#define MESSAGE2   "      Device running on       "
#define MESSAGE3   "        STM324x9I-EVAL        "
#define MESSAGE4   "                              "
#define MESSAGE5   "      program built with      "
#define MESSAGE6   "   Open Source Technologies   "

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
	/* Hardware initialization */
	BSP_LCD_Init();

	/* LCD Initialization */
	BSP_LCD_LayerDefaultInit(0, LCD_FB_START_ADDRESS);
	BSP_LCD_LayerDefaultInit(1, LCD_FB_START_ADDRESS+(BSP_LCD_GetXSize()*BSP_LCD_GetYSize()*4));

	/* Enable the LCD */
	BSP_LCD_DisplayOn();

	/* Select the LCD Background Layer  */
	BSP_LCD_SelectLayer(0);

	/* Clear the Background Layer */
	BSP_LCD_Clear(LCD_COLOR_BLUE);

	/* Select the LCD Foreground Layer  */
	BSP_LCD_SelectLayer(1);

	/* Clear the Foreground Layer */
	BSP_LCD_Clear(LCD_COLOR_BLUE);

	/* Configure the transparency for foreground and background : Increase the transparency */
	BSP_LCD_SetTransparency(0, 0);
	BSP_LCD_SetTransparency(1, 100);

	// Display a startup message
	BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);

    BSP_LCD_DisplayStringAtLine(1, (uint8_t*)MESSAGE1);
    BSP_LCD_DisplayStringAtLine(2, (uint8_t*)MESSAGE2);
    BSP_LCD_DisplayStringAtLine(3, (uint8_t*)MESSAGE3);
    BSP_LCD_DisplayStringAtLine(4, (uint8_t*)MESSAGE4);
    BSP_LCD_DisplayStringAtLine(5, (uint8_t*)MESSAGE5);
    BSP_LCD_DisplayStringAtLine(6, (uint8_t*)MESSAGE6);

	m_nCurrentLine = FIRST_LINE;

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
	if( m_nCurrentLine > LAST_LINE )
	{
		m_nCurrentLine = FIRST_LINE;
	}

	/* Display the received text */
	BSP_LCD_ClearStringLine(m_nCurrentLine);
	BSP_LCD_DisplayStringAtLine(m_nCurrentLine, (uint8_t*)wParam);
	++m_nCurrentLine;
}
