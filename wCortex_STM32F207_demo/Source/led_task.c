/**
 * @file led_task.c
 *
 * @date   Mar 9, 2011
 * @author Stefano Oliveri (STF12.net)
 *         E-mail: software@stf12.net
 */


#include "led_task.h"
#include "stm32f2xx.h"
#include "stm32_eval.h"
#include "task.h"

static void LedTaskFunc(void *pParams);

void LedHardwareInit(void *pParam) {
	STM_EVAL_LEDInit(LED1);
}

void LedStartTask(unsigned short nStackDepth, unsigned portBASE_TYPE nPriority, void *pParams) {
	xTaskCreate(LedTaskFunc, "LED", nStackDepth, pParams, nPriority, NULL);
}

void LedTaskFunc(void *pParams) {

	const portTickType nFlashRate = 1000 / portTICK_RATE_MS;

	for (;;) {
		vTaskDelay(nFlashRate);
		STM_EVAL_LEDToggle(LED1);
	}
}
