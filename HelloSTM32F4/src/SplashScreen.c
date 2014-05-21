/*
 * SplashScreen.c
 *
 *  Created on: May 20, 2014
 *      Author: oliveris
 */

#include "SplashScreen.h"

static int s_bLcdInitialized = 0;

void SCLcdConfig() {
	/* LCD Initialization */
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

	/* Configure the transparency for foreground and background :
	     Increase the transparency */
	BSP_LCD_SetTransparency(0, 0);
	BSP_LCD_SetTransparency(1, 100);

	s_bLcdInitialized = 1;

}

void SCDisplayMessage() {
	if (!s_bLcdInitialized) {
		SCLcdConfig();
	}

	BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);

    BSP_LCD_DisplayStringAtLine(3, (uint8_t*)MESSAGE1);
    BSP_LCD_DisplayStringAtLine(4, (uint8_t*)MESSAGE2);
    BSP_LCD_DisplayStringAtLine(5, (uint8_t*)MESSAGE3);
    BSP_LCD_DisplayStringAtLine(6, (uint8_t*)MESSAGE4);
    BSP_LCD_DisplayStringAtLine(7, (uint8_t*)MESSAGE5);
    BSP_LCD_DisplayStringAtLine(8, (uint8_t*)MESSAGE6);
}

