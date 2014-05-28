/*
 * LcdTask.c
 *
 *  Created on: May 23, 2014
 *      Author: oliveris
 */



#include "LcdTask.h"
#include "task.h"
#include "SplashScreen.h"

#define LCD_STACK_SIZE (configMINIMAL_STACK_SIZE * 2)

static void lcdTaskFunc(void *pvParameters);

static TaskHandle_t s_xLcdTaskHandle = NULL;

void lcdStartTask(UBaseType_t uxPriority)
{
	s_xLcdTaskHandle = NULL;
	if (pdPASS != xTaskCreate(lcdTaskFunc, "LCD", LCD_STACK_SIZE, NULL, uxPriority, &s_xLcdTaskHandle)) {
		for(;;);
	}
}

static void lcdTaskFunc(void *pvParameters)
{
	/* Just to prevent compiler warnings when the configuration options are
	set such that these static functions are not used. */
	(void) pvParameters;

	SCDisplayMessage();
	vTaskDelete(s_xLcdTaskHandle);
}
